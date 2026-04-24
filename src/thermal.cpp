// ============================================================================
// Component Tester PRO v3.0 — Sonda Térmica DS18B20 (Implementação)
// ============================================================================

#include "thermal.h"
#include "globals.h"
#include "config.h"
#include "buzzer.h"
#include "leds.h"
#include "graphics.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// ============================================================================
// INSTÂNCIAS
// ============================================================================
static OneWire oneWire(PIN_ONEWIRE);
static DallasTemperature sensors(&oneWire);
static DeviceAddress sensorAddress;

// Estados
static float lastTempRead = 25.0f;
static bool sensorFound = false;
static unsigned long lastReadMs = 0;
static bool alertActive = false;
static unsigned long alertBeepMs = 0;

// ============================================================================
// INICIALIZAÇÃO
// ============================================================================
void thermal_init() {
    DBG_PRINTLN("[THERM] Procurando sensores DS18B20...");

    sensors.begin();
    sensorFound = sensors.getAddress(sensorAddress, 0);

    if (!sensorFound) {
        DBG_PRINTLN("[THERM] Sensor não encontrado!");
        return;
    }

    sensors.setResolution(sensorAddress, 12); // 12-bit = 0.0625°C
    sensors.setWaitForConversion(false);      // Não-bloqueante

    DBG_PRINT("[THERM] Sensor encontrado: ");
    for (int i = 0; i < 8; i++) {
        DBG_PRINTF("%02X", sensorAddress[i]);
    }
    DBG_PRINTLN("");

    // Primeira conversão
    sensors.requestTemperatures();
    lastReadMs = millis();

    DBG_PRINTLN("[THERM] Sonda térmica pronta");
}

// ============================================================================
// LEITURAS
// ============================================================================
float thermal_read() {
    if (!sensorFound) return -999.0f;

    sensors.requestTemperatures();
    float temp = sensors.getTempC(sensorAddress);
    lastTempRead = temp;
    lastReadMs = millis();

    return temp;
}

float thermal_read_async() {
    if (!sensorFound) return lastTempRead;

    unsigned long now = millis();
    if (now - lastReadMs > 750) { // DS18B20 precisa de ~750ms para 12-bit
        float temp = sensors.getTempC(sensorAddress);
        if (temp != 85.0f && temp != -127.0f) { // Erros conhecidos do DS18B20
            lastTempRead = temp;
        }
        sensors.requestTemperatures();
        lastReadMs = now;
    }

    return lastTempRead;
}

float thermal_get_last() {
    return lastTempRead;
}

// ============================================================================
// ALERTAS
// ============================================================================
bool thermal_has_alert() {
    return thermal_is_warning();
}

bool thermal_is_warning() {
    return lastTempRead > TEMP_NORMAL_MAX;
}

bool thermal_is_critical() {
    return lastTempRead > TEMP_WARNING;
}

// ============================================================================
// UI — HANDLE
// ============================================================================
void thermal_handle() {
    float temp = thermal_read_async();

    // Atualiza backlight
    lastActivityMs = millis();

    // Desenha atualização
    tft.fillRect(STATUS_BAR_HEIGHT, STATUS_BAR_HEIGHT + 30, 280, 160,
                   UI_COLORS::BACKGROUND);

    // Gauge de temperatura
    int cx = 160, cy = 120, r = 60;
    float pct = (temp - 20.0f) / 80.0f; // 20°C a 100°C
    pct = constrain(pct, 0.0f, 1.0f);

    uint16_t gaugeColor = (temp > TEMP_WARNING) ?
                             UI_COLORS::DANGER :
                             (temp > TEMP_NORMAL_MAX) ?
                             UI_COLORS::WARNING :
                             UI_COLORS::SUCCESS;

    draw_gauge(cx, cy, r, temp, 20.0f, 100.0f, "C", gaugeColor,
                 UI_COLORS::STATUS_BG);

    // Texto de status
    const char* statusText;
    uint16_t statusColor;
    if (temp > TEMP_WARNING) {
        statusText = "CRITICO!";
        statusColor = UI_COLORS::DANGER;
    } else if (temp > TEMP_NORMAL_MAX) {
        statusText = "QUENTE";
        statusColor = UI_COLORS::WARNING;
    } else {
        statusText = "NORMAL";
        statusColor = UI_COLORS::SUCCESS;
    }

    tft.setTextColor(statusColor);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(cx, cy + 75);
    tft.print(statusText);
    tft.setTextDatum(TL_DATUM);

    // Alerta sonoro periódico
    unsigned long now = millis();
    if (temp > TEMP_WARNING && now - alertBeepMs > 3000) {
        buzzer_alert();
        alertBeepMs = now;
    }
}

void thermal_alert_beep() {
    buzzer_alert();
}

void thermal_draw() {
    tft.fillScreen(UI_COLORS::BACKGROUND);
    draw_status_bar();
    thermal_handle();
    draw_footer();
}

void thermal_set_resolution(uint8_t resolution) {
    if (sensorFound) {
        sensors.setResolution(sensorAddress, resolution);
    }
}