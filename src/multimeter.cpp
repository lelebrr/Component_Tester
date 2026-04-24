// ============================================================================
// Component Tester PRO v3.0 — Multímetro AC/DC (Implementação)
// Descrição: ZMPT101B para AC, INA219 para DC
// ============================================================================

#include "multimeter.h"
#include "globals.h"
#include "config.h"
#include "buzzer.h"
#include "leds.h"
#include "graphics.h"
#include <INA219.h>
#include <Wire.h>

// ============================================================================
// INSTÂNCIAS
// ============================================================================
static INA219 ina219;
static bool ina219Initialized = false;

// ============================================================================
// ESTADO DO MULTÍMETRO
// ============================================================================
static MultimeterMode currentMode = MMODE_DC_VOLTAGE;
static MultimeterReading currentReading;

// Buffer circular para amostras RMS
static float acSamples[ZMPT_SAMPLES];
static int acSampleIndex = 0;
static unsigned long lastAcReadMs = 0;

// ============================================================================
// INICIALIZAÇÃO
// ============================================================================
void multimeter_init() {
    DBG_PRINTLN("[MULTI] Inicializando multímetro...");

    // INA219 para DC
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    ina219.begin();
    ina219Initialized = ina219.begin();

    if (ina219Initialized) {
        ina219.configure(INA219_BRNG_32V, INA219_PG_320mA,
                           INA219_ADC_12BIT, INA219_ADC_12BIT, 1);
        ina219.calibrate(INA219_SHUNT_R * 1000.0f, 320.0f,
                           INA219_SHUNT_R, 3.3f);
        DBG_PRINTLN("[MULTI] INA219 OK");
    } else {
        DBG_PRINTLN("[MULTI] INA219 não encontrado");
    }

    currentMode = MMODE_DC_VOLTAGE;
    DBG_PRINTLN("[MULTI] Multímetro pronto");
}

// ============================================================================
// LEITURAS DC (INA219)
// ============================================================================
float multimeter_read_dc_voltage() {
    if (!ina219Initialized) return 0;
    return ina219.getBusVoltage_V();
}

float multimeter_read_dc_current() {
    if (!ina219Initialized) return 0;
    return ina219.getCurrent_mA();
}

// ============================================================================
// LEITURAS AC (ZMPT101B)
// Usa amostragem rápida e cálculo de True RMS
// ============================================================================
float multimeter_read_ac_voltage() {
    unsigned long startUs = micros();
    int count = 0;
    float sumSq = 0;
    int raw;
    float v;

    // Coleta amostras até o timeout
    while (count < ZMPT_SAMPLES) {
        raw = analogRead(PIN_ZMPT_AC);
        v = ((float)raw - ZMPT_ZERO_POINT) * ADC_VOLTS_PER_BIT;
        sumSq += v * v;
        acSamples[count] = v;
        count++;

        // Taxa de amostragem
        while ((micros() - startUs) < (ZMPT_SAMPLE_PERIOD / ZMPT_SAMPLES)) {
            // Busy-wait para timing preciso
        }
        startUs = micros();
    }

    // RMS = sqrt(soma(V²) / N)
    float rms = sqrt(sumSq / count);

    // Escala para tensão de linha (220V RMS → 311V pico)
    float vrms = rms * ZMPT_SCALE_FACTOR / ZMPT_ZERO_POINT;
    return vrms;
}

// ============================================================================
// TRUE RMS CALCULATION
// ============================================================================
float calculate_true_rms(float* samples, int count) {
    if (count <= 0) return 0;
    float sumSq = 0;
    for (int i = 0; i < count; i++) {
        sumSq += samples[i] * samples[i];
    }
    return sqrt(sumSq / count);
}

// ============================================================================
// LEITURA COMPLETA
// ============================================================================
MultimeterReading multimeter_read() {
    MultimeterReading r;
    r.mode = currentMode;
    r.timestamp = millis();
    r.valid = true;

    switch (currentMode) {
        case MMODE_DC_VOLTAGE:
            r.value = multimeter_read_dc_voltage();
            r.unit = "V";
            r.statusColor = (r.value > 30) ?
                              UI_COLORS::DANGER : UI_COLORS::SUCCESS;
            break;

        case MMODE_AC_VOLTAGE:
            r.value = multimeter_read_ac_voltage();
            r.unit = "V";
            r.statusColor = (r.value > 250) ?
                              UI_COLORS::WARNING : UI_COLORS::SUCCESS;
            break;

        case MMODE_DC_CURRENT:
            r.value = multimeter_read_dc_current();
            r.unit = "mA";
            r.statusColor = UI_COLORS::SUCCESS;
            break;

        default:
            r.value = 0;
            r.unit = "-";
            r.statusColor = UI_COLORS::UNKNOWN;
            r.valid = false;
            break;
    }

    return r;
}

// ============================================================================
// CALIBRAÇÃO
// ============================================================================
void multimeter_calibrate_zmpt(float realVoltage) {
    // Corrige o fator de escala baseado na tensão real medida
    // com um multímetro de referência
    // realVoltage tipicamente = 220V ou 110V
    float currentReading = multimeter_read_ac_voltage();
    if (currentReading > 1) {
        deviceSettings.zmptScaleFactor = realVoltage / currentReading *
                                           ZMPT_SCALE_FACTOR;
        saveSettings();
        DBG_PRINTF("[MULTI] ZMPT calibrado: %.2f\n",
                   deviceSettings.zmptScaleFactor);
    }
}

// ============================================================================
// MODO
// ============================================================================
void multimeter_set_mode(MultimeterMode mode) {
    currentMode = mode;
    DBG_PRINTF("[MULTI] Modo: %d\n", mode);
}

MultimeterMode multimeter_get_mode() {
    return currentMode;
}

// ============================================================================
// UI
// ============================================================================
void multimeter_draw(const MultimeterReading* reading) {
    tft.fillScreen(UI_COLORS::BACKGROUND);
    draw_status_bar();

    if (!reading) return;

    // Card grande no centro
    int cx = 160, cy = 110;
    tft.fillRoundRect(cx - 130, cy - 70, 260, 140, 12,
                       UI_COLORS::CARD_BG);
    tft.drawRoundRect(cx - 130, cy - 70, 260, 140, 12,
                      reading->statusColor);

    // Modo
    const char* modeStr;
    switch (reading->mode) {
        case MMODE_DC_VOLTAGE: modeStr = "DC VOLTAGE"; break;
        case MMODE_AC_VOLTAGE:  modeStr = "AC VOLTAGE"; break;
        case MMODE_DC_CURRENT: modeStr = "DC CURRENT"; break;
        case MMODE_AC_CURRENT: modeStr = "AC CURRENT"; break;
        default: modeStr = "MULTIMETER"; break;
    }

    tft.setTextColor(UI_COLORS::TEXT_DIM);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(cx, cy - 55);
    tft.print(modeStr);

    // Valor
    char valStr[32];
    if (reading->valid) {
        if (reading->value >= 100) {
            snprintf(valStr, sizeof(valStr), "%.1f", reading->value);
        } else if (reading->value >= 1) {
            snprintf(valStr, sizeof(valStr), "%.2f", reading->value);
        } else {
            snprintf(valStr, sizeof(valStr), "%.3f", reading->value);
        }
    } else {
        snprintf(valStr, sizeof(valStr), "---");
    }

    tft.setTextColor(UI_COLORS::TEXT);
    tft.setTextSize(5);
    tft.setCursor(cx, cy - 20);
    tft.print(valStr);
    tft.setTextSize(2);
    tft.setCursor(cx + 10, cy + 20);
    tft.print(reading->unit);

    tft.setTextDatum(TL_DATUM);
    draw_footer();
}

void multimeter_handle() {
    lastActivityMs = millis();

    // Atualiza LED RGB
    static int phase = 0;
    static unsigned long lastToggle = 0;
    unsigned long now = millis();
    if (now - lastToggle > 500) {
        phase = !phase;
        led_set_rgb(0, phase ? 20 : 5, 0);
        lastToggle = now;
    }

    // Leitura
    currentReading = multimeter_read();
    multimeter_draw(&currentReading);

    // Navegação por botões
    if (btn_just_pressed(BTN_LEFT)) {
        int m = (int)currentMode;
        if (m > 0) {
            currentMode = (MultimeterMode)(m - 1);
            led_flash_rgb(0, 30, 0, 100, 0);
        }
    } else if (btn_just_pressed(BTN_RIGHT)) {
        int m = (int)currentMode;
        if (m < MMODE_CONTINUITY) {
            currentMode = (MultimeterMode)(m + 1);
            led_flash_rgb(0, 30, 0, 100, 0);
        }
    } else if (btn_just_pressed(BTN_BACK)) {
        currentAppState = STATE_MENU;
    }

    delay(100);
}