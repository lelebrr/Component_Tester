// ============================================================================
// Component Tester PRO v3.0 — Main (CYD Edition)
// Plataforma: ESP32-2432S028R (Cheap Yellow Display)
// Descrição: Estrutura principal com FreeRTOS
// Displays: Adafruit ILI9341 + Adafruit GFX
// Touch: XPT2046
// ============================================================================

#include "config.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
#include <SD.h>
#include <SPI.h>
#include <Preferences.h>

// ============================================================================
// INCLUDES DOS MÓDULOS DO PROJETO
// ============================================================================
#include "globals.h"
#include "buttons.h"
#include "buzzer.h"
#include "leds.h"
#include "menu.h"
#include "measurements.h"
#include "multimeter.h"
#include "thermal.h"
#include "database.h"
#include "logger.h"
#include "utils.h"
#include "graphics.h"

// ============================================================================
// INSTÂNCIAS GLOBAIS DE HARDWARE
// ============================================================================
// Display TFT ILI9341 — usa HSPI (MOSI=23, MISO=19, SCLK=18)
// Construtor: Adafruit_ILI9341(CS, DC, RST=-1)
Adafruit_ILI9341 tft(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);

// Touchscreen XPT2046 — usa SPI software (bitbang)
XPT2046_Touchscreen ts(PIN_TOUCH_CS, PIN_TOUCH_IRQ);
#define TS_CS PIN_TOUCH_CS

// NVS (Non-Volatile Storage — flash interna)
Preferences nvs;

// ============================================================================
// FILAS DO FREE RTOS (comunicação entre tasks)
// ============================================================================
QueueHandle_t measurementQueue;
QueueHandle_t displayQueue;
QueueHandle_t sdQueue;

// ============================================================================
//Handles das tasks
// ============================================================================
TaskHandle_t xMeasurementTaskHandle = NULL;
TaskHandle_t xThermalTaskHandle = NULL;
TaskHandle_t xSDTaskHandle = NULL;

// ============================================================================
// VARIÁVEIS GLOBAIS DO SISTEMA
// ============================================================================
// Estado atual da máquina de estados
volatile AppState currentAppState = STATE_SPLASH;
volatile AppState previousAppState = STATE_SPLASH;

// Controle de tempo
unsigned long lastActivityMs = 0;      // Última interação do usuário
unsigned long backlightOffTimeMs = 0;  // Quando desligar o backlight
bool backlightOn = true;

// Medições atuais
float lastResistance = 0.0f;
float lastCapacitance = 0.0f;
float lastInductance = 0.0f;
float lastVoltage = 0.0f;
float lastCurrent = 0.0f;
float lastTemperature = 25.0f;

// Estatísticas
uint32_t totalMeasurements = 0;
uint32_t goodMeasurements = 0;
uint32_t badMeasurements = 0;

// Calibração dos probes
float probeOffsetResistance = 0.0f;
float probeOffsetCapacitance = 0.0f;

// Banco de dados na RAM
ComponentDatabase componentDB;

// SD Card
bool sdCardPresent = false;
bool sdCardError = false;

// ============================================================================
// PROTÓTIPOS
// ============================================================================
void splashScreen();
void initHardware();
void initFreeRTOS();
void powerManagement();
void enterSleep();
void wakeUp();

// ============================================================================
// FUNÇÕES AUXILIARES
// ============================================================================

// Reinicia o ESP32 de forma controlada
void systemReset() {
    DBG_PRINTLN("[SYS] Reiniciando...");
    delay(100);
    ESP.restart();
}

// Reinicia apenas a UI (sem reset completo)
void softReset() {
    DBG_PRINTLN("[SYS] Soft Reset da UI...");
    currentAppState = STATE_MENU;
    menuRefresh();
}

// ============================================================================
// BACKLIGHT (controle PWM via LEDC)
// ============================================================================

void backlight_init() {
    // Configura canal LEDC para o backlight
    ledcSetup(LEDC_CH_BACKLIGHT, LEDC_FREQ_BL, LEDC_TIMER_BIT);
    ledcAttachPin(PIN_TFT_BL, LEDC_CH_BACKLIGHT);

    // Define duty cycle inicial
    uint8_t defaultBrightness = (uint8_t)((float)255 * deviceSettings.backlight / 255.0f);
    ledcWrite(LEDC_CH_BACKLIGHT, defaultBrightness);

    DBG_PRINTF("[BL] Inicializado com brilho: %d%%\n",
               (int)(deviceSettings.backlight * 100 / 255));
}

void backlight_set(uint8_t level) {
    // level: 0-255
    ledcWrite(LEDC_CH_BACKLIGHT, level);
    lastActivityMs = millis();
    backlightOn = (level > 10);
}

void backlight_on() {
    backlight_set(deviceSettings.backlight);
}

void backlight_off() {
    backlight_set(0);
    backlightOn = false;
}

void backlight_toggle() {
    if (backlightOn) {
        backlight_off();
    } else {
        backlight_on();
    }
}

void backlight_fadeTo(uint8_t targetLevel, uint16_t durationMs) {
    uint8_t currentLevel = ledcRead(LEDC_CH_BACKLIGHT);
    if (currentLevel == targetLevel) return;

    unsigned long startMs = millis();
    unsigned long steps = 30;
    unsigned long stepDelay = durationMs / steps;

    for (unsigned long i = 1; i <= steps; i++) {
        uint8_t newLevel = map(i, 1, steps, currentLevel, targetLevel);
        ledcWrite(LEDC_CH_BACKLIGHT, newLevel);
        delay(stepDelay);
    }
    ledcWrite(LEDC_CH_BACKLIGHT, targetLevel);
    lastActivityMs = millis();
    backlightOn = (targetLevel > 10);
}

// ============================================================================
// PERSISTÊNCIA (NVS)
// ============================================================================

void loadSettings() {
    nvs.begin("ct_settings", true); // read-only

    deviceSettings.backlight          = nvs.getUChar("backlight", LEDC_DUTY_DEFAULT);
    deviceSettings.darkMode           = nvs.getBool("darkMode", false);
    deviceSettings.silentMode          = nvs.getBool("silentMode", false);
    deviceSettings.autoSleep           = nvs.getBool("autoSleep", true);
    deviceSettings.autoSleepMs         = nvs.getULong("autoSleepMs", AUTO_SLEEP_MS);
    deviceSettings.soundEnabled        = nvs.getBool("sound", true);
    deviceSettings.calibrated          = nvs.getBool("calibrated", false);

    probeOffsetResistance = nvs.getFloat("offsetRes", 0.0f);
    probeOffsetCapacitance = nvs.getFloat("offsetCap", 0.0f);

    totalMeasurements = nvs.getULong("totalMeas", 0);
    goodMeasurements = nvs.getULong("goodMeas", 0);
    badMeasurements = nvs.getULong("badMeas", 0);

    nvs.end();

    DBG_PRINTLN("[NVS] Configurações carregadas");
}

void saveSettings() {
    nvs.begin("ct_settings", false); // read-write

    nvs.putUChar("backlight", deviceSettings.backlight);
    nvs.putBool("darkMode", deviceSettings.darkMode);
    nvs.putBool("silentMode", deviceSettings.silentMode);
    nvs.putBool("autoSleep", deviceSettings.autoSleep);
    nvs.putULong("autoSleepMs", deviceSettings.autoSleepMs);
    nvs.putBool("sound", deviceSettings.soundEnabled);
    nvs.putBool("calibrated", deviceSettings.calibrated);

    nvs.putFloat("offsetRes", probeOffsetResistance);
    nvs.putFloat("offsetCap", probeOffsetCapacitance);

    nvs.putULong("totalMeas", totalMeasurements);
    nvs.putULong("goodMeas", goodMeasurements);
    nvs.putULong("badMeas", badMeasurements);

    nvs.end();

    DBG_PRINTLN("[NVS] Configurações salvas");
}

// ============================================================================
// SD CARD
// ============================================================================

bool sd_init() {
    SPI.begin(PIN_SD_SCK, PIN_SD_MISO, PIN_SD_MOSI, PIN_SD_CS);

    if (!SD.begin(PIN_SD_CS, SPI, SD_SPI_SPEED)) {
        DBG_PRINTLN("[SD] Falha ao inicializar cartão SD");
        sdCardPresent = false;
        return false;
    }

    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        DBG_PRINTLN("[SD] Sem cartão SD detectado");
        sdCardPresent = false;
        return false;
    }

    sdCardPresent = true;
    DBG_PRINTF("[SD] Cartão SD presente (tipo: %d)\n", cardType);
    return true;
}

bool sd_writeLog(const char* line) {
    if (!sdCardPresent) return false;

    File f = SD.open(LOG_FILE_PATH, FILE_APPEND);
    if (!f) {
        DBG_PRINTLN("[SD] Erro ao abrir arquivo de log");
        return false;
    }

    size_t written = f.println(line);
    f.close();
    return written > 0;
}

bool sd_loadDatabase() {
    if (!sdCardPresent) return false;

    File f = SD.open(DB_COMPONENTS_PATH, FILE_READ);
    if (!f) {
        DBG_PRINTLN("[SD] Banco de dados não encontrado no SD");
        return false;
    }

    int count = 0;
    while (f.available() && count < MAX_DB_COMPONENTS) {
        String line = f.readStringUntil('\n');
        line.trim();
        if (line.length() == 0) continue;

        int p1 = line.indexOf(',');
        int p2 = line.indexOf(',', p1 + 1);
        int p3 = line.indexOf(',', p2 + 1);
        int p4 = line.indexOf(',', p3 + 1);

        if (p1 > 0 && p2 > p1 && p3 > p2 && p4 > p3) {
            componentDB.items[count].type = line.substring(0, p1).toInt();
            strncpy(componentDB.items[count].name,
                    line.substring(p1 + 1, p2).c_str(), 19);
            componentDB.items[count].name[19] = '\0';
            strncpy(componentDB.items[count].unit,
                    line.substring(p2 + 1, p3).c_str(), 7);
            componentDB.items[count].unit[7] = '\0';
            componentDB.items[count].minGood = line.substring(p3 + 1, p4).toFloat();
            componentDB.items[count].maxGood = line.substring(p4 + 1).toFloat();
            count++;
        }
    }
    f.close();
    componentDB.count = count;
    DBG_PRINTF("[SD] Carregados %d componentes\n", count);
    return true;
}

// ============================================================================
// INICIALIZAÇÃO DO HARDWARE
// ============================================================================

void initHardware() {
    DBG_PRINTLN("");
    DBG_PRINTLN(F("=========================================="));
    DBG_PRINTLN(F("  CT PRO v3.0 — CYD Edition"));
    DBG_PRINTLN(F("  " FW_BOARD));
    DBG_PRINTLN(F("  Build: " __DATE__ " " __TIME__));
    DBG_PRINTLN(F("=========================================="));
    DBG_PRINTLN("");

    // --- LEDs ---
    pinMode(PIN_RGB_LED, OUTPUT);
    digitalWrite(PIN_RGB_LED, LOW);

    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, LOW);

    // --- Buzzer ---
    buzzer_init();

    // --- Backlight ---
    backlight_init();

    // --- Display TFT (usa HSPI: MOSI=23, MISO=19, SCLK=18) ---
    SPI.begin(PIN_TFT_SCK, PIN_TFT_MISO, PIN_TFT_MOSI, PIN_TFT_CS);
    tft.begin();
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(UI_COLORS::BACKGROUND);

    DBG_PRINTLN("[TFT] Display ILI9341 inicializado");

    // --- Touchscreen ---
    ts.begin();
    ts.setRotation(1); // paisagem
    DBG_PRINTLN("[TOUCH] XPT2046 inicializado");

    // --- SD Card ---
    sd_init();
    sd_loadDatabase();

    // --- Load Settings ---
    loadSettings();

    // --- Sensores ---
    thermal_init();
    multimeter_init();

    // --- LED RGB — sinal de boot OK ---
    led_set_rgb(0, 50, 0); // verde
    delay(300);
    led_set_rgb(0, 0, 0);
}

// ============================================================================
// FREE RTOS — TASKS
// ============================================================================

void task_measurement(void* pvParameters) {
    (void)pvParameters;
    MeasurementResult result;

    DBG_PRINTLN("[TASK] Task de medição iniciada");

    for (;;) {
        if (xQueueReceive(measurementQueue, &result, portMAX_DELAY) == pdTRUE) {
            lastResistance = result.resistance;
            lastCapacitance = result.capacitance;
            lastInductance = result.inductance;
        }
        taskYIELD();
    }
}

void task_thermal(void* pvParameters) {
    (void)pvParameters;
    float temperature;

    DBG_PRINTLN("[TASK] Task térmica iniciada");

    for (;;) {
        temperature = thermal_read();
        lastTemperature = temperature;

        if (temperature > TEMP_WARNING) {
            // Disparar alerta
            DisplayMessage msg;
            msg.type = MSG_TEMPERATURE;
            msg.value = temperature;
            xQueueSend(displayQueue, &msg, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void task_sd_logging(void* pvParameters) {
    (void)pvParameters;
    LogEntry entry;

    DBG_PRINTLN("[TASK] Task de logging SD iniciada");

    for (;;) {
        if (xQueueReceive(sdQueue, &entry, portMAX_DELAY) == pdTRUE) {
            char line[128];
            snprintf(line, sizeof(line), "%lu,%s,%.6f,%s",
                     entry.timestamp,
                     entry.componentName,
                     entry.value,
                     entry.status);
            sd_writeLog(line);
        }
        taskYIELD();
    }
}

void initFreeRTOS() {
    // Criar filas
    measurementQueue = xQueueCreate(4, sizeof(MeasurementResult));
    displayQueue = xQueueCreate(8, sizeof(DisplayMessage));
    sdQueue = xQueueCreate(16, sizeof(LogEntry));

    if (measurementQueue == NULL || displayQueue == NULL || sdQueue == NULL) {
        DBG_PRINTLN("[RTOS] ERRO ao criar filas");
        return;
    }

    DBG_PRINTLN("[RTOS] Filas criadas com sucesso");

    // Tarefa de medição (prioridade média, Core 1)
    xTaskCreatePinnedToCore(
        task_measurement,
        "MeasTask",
        TASK_STACK_MEASURE,
        NULL,
        TASK_PRIORITY_MEDIUM,
        &xMeasurementTaskHandle,
        1);

    // Tarefa térmica (prioridade baixa, Core 1)
    xTaskCreatePinnedToCore(
        task_thermal,
        "ThermTask",
        TASK_STACK_THERMAL,
        NULL,
        TASK_PRIORITY_LOW,
        &xThermalTaskHandle,
        1);

    // Tarefa de logging SD (prioridade baixa, Core 0)
    xTaskCreatePinnedToCore(
        task_sd_logging,
        "SDTask",
        TASK_STACK_SD,
        NULL,
        TASK_PRIORITY_LOW,
        &xSDTaskHandle,
        0);

    DBG_PRINTLN("[RTOS] Tasks iniciadas");
}

// ============================================================================
// SPLASH SCREEN
// ============================================================================

void splashScreen() {
    tft.fillScreen(UI_COLORS::SPLASH_BG);

    // --- Barra superior decorativa ---
    tft.fillRect(0, 0, 320, 4, UI_COLORS::PRIMARY);

    // --- Logo "CT PRO" com moldura ---
    // Moldura externa (drawRect arredondado simula uma card)
    tft.drawRoundRect(70, 55, 180, 65, 12, UI_COLORS::PRIMARY);
    tft.drawRoundRect(72, 57, 176, 61, 10, UI_COLORS::SPLASH_BG);

    // Texto principal
    tft.setTextColor(UI_COLORS::PRIMARY);
    tft.setTextSize(3);
    tft.setCursor(95, 75);
    tft.print(F("CT PRO"));

    // Versão
    tft.setTextColor(UI_COLORS::TEXT);
    tft.setTextSize(2);
    tft.setCursor(110, 100);
    tft.print(F("v" FW_VERSION));

    // --- Subtítulo ---
    tft.setTextSize(1);
    tft.setTextColor(UI_COLORS::TEXT_DIM);
    tft.setCursor(88, 138);
    tft.print(FW_BOARD);

    tft.setCursor(105, 152);
    tft.print(F("CYD Edition"));

    // --- Informações do sistema ---
    tft.setTextColor(UI_COLORS::TEXT_HINT);
    tft.setCursor(88, 172);
    tft.print(F("240MHz | 520KB RAM"));

    tft.setCursor(105, 185);
    tft.print(F("ILI9341 2.8\""));

    // --- Barra de progresso animada ---
    int barW = 220;
    int barH = 10;
    int barX = (320 - barW) / 2;
    int barY = 210;

    tft.drawRoundRect(barX - 1, barY - 1, barW + 2, barH + 2, 4,
                      UI_COLORS::BORDER);

    for (int i = 0; i <= 100; i += 3) {
        int filledW = (barW * i) / 100;
        tft.fillRoundRect(barX, barY, filledW, barH, 3, UI_COLORS::PRIMARY);

        tft.setTextColor(UI_COLORS::TEXT_HINT);
        tft.setTextSize(1);
        tft.setCursor(barX + barW / 2 - 10, barY + barH + 4);
        char pct[8];
        snprintf(pct, sizeof(pct), "%d%%", i);
        tft.print(pct);

        // Etapas de inicialização
        if (i == 10) printInitStatus("Iniciando display...", UI_COLORS::TEXT_HINT);
        if (i == 25) printInitStatus("Carregando SD...", UI_COLORS::TEXT_HINT);
        if (i == 40) printInitStatus("Configurando sensores...", UI_COLORS::TEXT_HINT);
        if (i == 55) printInitStatus("Carregando database...", UI_COLORS::TEXT_HINT);
        if (i == 70) printInitStatus("Iniciando FreeRTOS...", UI_COLORS::TEXT_HINT);
        if (i == 85) printInitStatus("Quase pronto...", UI_COLORS::TEXT_HINT);
        if (i == 100) printInitStatus("Sistema pronto!", UI_COLORS::SUCCESS);

        delay(20);
    }

    delay(600);
    buzzer_beep(200);
}

void printInitStatus(const char* text, uint16_t color) {
    static int lastY = 205;
    tft.setTextColor(UI_COLORS::SPLASH_BG);
    tft.setTextSize(1);
    tft.setCursor(85, lastY);
    tft.print(F("                                    "));
    tft.setTextColor(color);
    tft.setCursor(85, lastY);
    tft.print(text);
    lastY += 14;
}

// ============================================================================
// GERENCIAMENTO DE ENERGIA
// ============================================================================

void powerManagement() {
    unsigned long now = millis();

    if (!deviceSettings.autoSleep) {
        lastActivityMs = now;
        return;
    }

    // Detecta inatividade (tecla, toque)
    if (now - lastActivityMs > deviceSettings.autoSleepMs) {
        if (backlightOn) {
            backlight_off();
        }
    }

    // Desliga completamente após mais tempo
    if (now - lastActivityMs > deviceSettings.autoSleepMs * 2) {
        enterSleep();
    }
}

void enterSleep() {
    DBG_PRINTLN("[PWR] Entrando em modo sleep...");

    backlight_off();
    measurements_stop();

    esp_sleep_enable_ext1_wakeup(
        GPIO_SEL_0 | GPIO_SEL_35 | GPIO_SEL_36, // botões + touch
        EXT1_WAKEUP_ANY_HIGH);

    esp_deep_sleep_start();
}

void wakeUp() {
    DBG_PRINTLN("[PWR] Acordando...");
    backlight_on();
}

// ============================================================================
// SETUP PRINCIPAL
// ============================================================================

void setup() {
    // Inicializa serial
    Serial.begin(SERIAL_BAUD);

    // Inicializa todo o hardware
    initHardware();

    // Tela de splash
    splashScreen();

    // Inicializa FreeRTOS
    initFreeRTOS();

    // Inicializa меню
    menu_init();

    // Transição suave para o menu
    backlight_on();
    currentAppState = STATE_MENU;

    // Inicializa lastActivity
    lastActivityMs = millis();

    DBG_PRINTLN("");
    DBG_PRINTLN(F("[SYS] ====== Sistema Inicializado ======"));
    DBG_PRINTLN(F("[SYS] Menu principal pronto"));
    DBG_PRINTLN("");
}

// ============================================================================
// LOOP PRINCIPAL (não-bloqueante)
// ============================================================================

void loop() {
    buttons_update();
    buzzer_update();
    powerManagement();

    switch (currentAppState) {
        case STATE_SPLASH:
            break;

        case STATE_MENU:
            menu_handle();
            break;

        case STATE_MEASURE_RESISTOR:
        case STATE_MEASURE_CAPACITOR:
        case STATE_MEASURE_DIODE:
        case STATE_MEASURE_TRANSISTOR:
        case STATE_MEASURE_INDUCTOR:
        case STATE_MEASURE_IC:
            measurements_handle();
            break;

        case STATE_MULTIMETER:
            multimeter_handle();
            break;

        case STATE_THERMAL_PROBE:
            thermal_handle();
            break;

        case STATE_CALIBRATION:
            calibration_handle();
            break;

        case STATE_SETTINGS:
            settings_handle();
            break;

        case STATE_ABOUT:
            about_handle();
            break;

        case STATE_HISTORY:
            history_handle();
            break;

        case STATE_STATS:
            stats_handle();
            break;

        case STATE_SCANNER:
            scanner_handle();
            break;
    }
}