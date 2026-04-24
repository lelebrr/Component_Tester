// ============================================================================
// Component Tester PRO v3.0 — Multímetro AC/DC com True RMS
// Descrição: Implementação completa do sistema de medição elétrica
// Versão: CYD Edition para ESP32-2432S028R
// ============================================================================

#include "multimeter.h"
#include "config.h"
#include "globals.h"
#include "buzzer.h"
#include "leds.h"

#include "display_globals.h"

#include <Arduino.h>
#include <Preferences.h>

// ============================================================================
// VARIÁVEIS GLOBAIS DO MÓDULO
// ============================================================================

static MultimeterMode currentMode = MMODE_DC_VOLTAGE;
static MultimeterState currentState = MSTATE_IDLE;
static MeasurementRange currentRange = RANGE_AUTO;

static int16_t adcSamples[ZMPT_SAMPLES_PER_CYCLE * 2];
static uint16_t sampleIndex = 0;
static bool sampling = false;

static MultimeterReading lastReading;
static unsigned long lastReadingTime = 0;
static bool readingValid = false;

static float filteredValue = 0.0f;
static float movingAverageBuffer[32];
static uint8_t movingAvgIndex = 0;
static uint8_t movingAvgCount = 0;

static float zmptCalibrationFactor = 1.0f;
static float ina219CalibrationFactor = 1.0f;
static float shuntResistance = INA219_SHUNT_OHMS;

static MeasurementHistory history;
static unsigned long lastHistoryAdd = 0;

static MultimeterConfig config;
static MultimeterStatus status;

static TaskHandle_t acVoltageTaskHandle = NULL;
static bool core1TaskRunning = false;

static Preferences nvs;

// ============================================================================
// INICIALIZAÇÃO
// ============================================================================

void multimeter_init(bool calibrate) {
    Serial.println(F("[MULTI] Multimeter init..."));
    
    config.zmptCalibration = 1.0f;
    config.ina219Calibration = 1.0f;
    config.shuntResistance = INA219_SHUNT_OHMS;
    config.filterSamples = 8;
    config.filterAlpha = 0.3f;
    config.updateIntervalMs = 200;
    config.highVoltageAlert = true;
    config.shortCircuitAlert = true;
    config.soundEnabled = true;
    config.currentRange = RANGE_AUTO;
    
    status.zmptConnected = false;
    status.ina219Connected = false;
    status.errors = 0;
    status.uptimeMs = 0;
    
    multimeter_adc_init();
    
    if (!calibrate) {
        multimeter_load_calibration();
    }
    
    history.count = 0;
    history.index = 0;
    
    sampleIndex = 0;
    sampling = false;
    filteredValue = 0.0f;
    movingAvgCount = 0;
    movingAvgIndex = 0;
    
    currentState = MSTATE_IDLE;
    currentMode = MMODE_DC_VOLTAGE;
    
    Serial.println(F("[MULTI] Multimeter ready"));
}

void multimeter_adc_init() {
    // Configura ADC para 12-bit
    analogReadResolution(12);
    analogSetWidth(12);
    
    // Configura pino como entrada
    pinMode(MULTIMETER_ADC_PIN, INPUT);
    
    status.zmptConnected = true;
    Serial.println(F("[MULTI] ADC configured"));
}

bool multimeter_ina219_init() {
    #ifdef USE_INA219
    Wire.begin(32, 33);
    Wire.beginTransmission(INA219_I2C_ADDR);
    uint8_t error = Wire.endTransmission();
    if (error == 0) return true;
    #endif
    return false;
}

void multimeter_shutdown() {
    if (acVoltageTaskHandle != NULL) {
        vTaskDelete(acVoltageTaskHandle);
        acVoltageTaskHandle = NULL;
        core1TaskRunning = false;
    }
    
    set_green_led(false);
    set_red_led(false);
    
    currentState = MSTATE_IDLE;
}

// ============================================================================
// LEITURAS
// ============================================================================

float multimeter_read_dc_voltage() {
    uint16_t adcValue = analogRead(MULTIMETER_ADC_PIN);
    float voltage = (adcValue * 3.3f / 4095.0f) * 6.0f;
    voltage *= config.zmptCalibration;
    return voltage;
}

float multimeter_read_ac_voltage() {
    if (!status.zmptConnected) {
        lastReading.value = 0.0f;
        lastReading.valid = false;
        lastReading.state = MSTATE_ERROR;
        return 0.0f;
    }
    
    const uint16_t numSamples = ZMPT_SAMPLES_PER_CYCLE;
    int16_t samples[ZMPT_SAMPLES_PER_CYCLE];
    
    for (uint16_t i = 0; i < numSamples; i++) {
        uint16_t adcValue = analogRead(MULTIMETER_ADC_PIN);
        samples[i] = (int16_t)adcValue - ZMPT_ZERO_VOLTAGE;
        delayMicroseconds(ZMPT_SAMPLE_RATE_US);
    }
    
    float rms = multimeter_calculate_rms(samples, numSamples);
    float voltage = (rms / 4095.0f) * 3.3f * ZMPT_VOLTS_PER_COUNT;
    voltage *= zmptCalibrationFactor;
    voltage = multimeter_apply_filters(voltage);
    
    lastReading.value = voltage;
    lastReading.valid = (voltage > 1.0f && voltage < AC_VOLTAGE_MAX);
    lastReading.mode = MMODE_AC_VOLTAGE;
    lastReading.timestamp = millis();
    
    if (voltage > HIGH_VOLTAGE_THRESHOLD) {
        lastReading.state = MSTATE_HIGH_VOLTAGE;
        multimeter_check_high_voltage(voltage);
    } else {
        lastReading.state = MSTATE_MEASURING;
    }
    
    return voltage;
}

float multimeter_calculate_rms(const int16_t* samples, uint16_t count) {
    if (count == 0) return 0.0f;
    
    float sumSquares = 0.0f;
    for (uint16_t i = 0; i < count; i++) {
        float sample = (float)samples[i];
        sumSquares += (sample * sample);
    }
    
    return sqrt(sumSquares / (float)count);
}

float multimeter_read_dc_current() {
    if (!status.ina219Connected) {
        float voltage = multimeter_read_dc_voltage();
        float current = voltage / config.shuntResistance;
        return current;
    }
    
    #ifdef USE_INA219
    Wire.requestFrom(INA219_I2C_ADDR, 2);
    uint16_t shuntVoltage = (Wire.read() << 8) | Wire.read();
    float current = ((float)shuntVoltage / INA219_CALIBRATION) * INA219_MAX_AMPS;
    current *= ina219CalibrationFactor;
    return current;
    #else
    return 0.0f;
    #endif
}

float multimeter_read_resistance() {
    uint16_t adcValue = analogRead(MULTIMETER_ADC_PIN);
    float vMeasured = (float)adcValue * 3.3f / 4095.0f;
    float rKnown = 10000.0f;
    
    float resistance = 0.0f;
    if (vMeasured > 0.01f) {
        resistance = rKnown * (3.3f - vMeasured) / vMeasured;
    }
    
    resistance = multimeter_apply_filters(resistance);
    
    if (resistance < SHORT_CIRCUIT_OHMS) {
        currentState = MSTATE_SHORT;
        multimeter_check_short_circuit(resistance);
    } else {
        currentState = MSTATE_MEASURING;
    }
    
    return resistance;
}

bool multimeter_test_short() {
    float resistance = multimeter_read_resistance();
    if (resistance < SHORT_CIRCUIT_OHMS) {
        multimeter_check_short_circuit(resistance);
        return true;
    }
    return false;
}

// ============================================================================
// LEITURA COMPLETA
// ============================================================================

MultimeterReading multimeter_read() {
    MultimeterReading reading;
    reading.mode = currentMode;
    reading.range = currentRange;
    reading.timestamp = millis();
    
    switch (currentMode) {
        case MMODE_DC_VOLTAGE:
            reading.value = multimeter_read_dc_voltage();
            reading.unit = "V";
            reading.unitAbbrev = "V";
            reading.state = (reading.value > 0.1f) ? MSTATE_MEASURING : MSTATE_IDLE;
            break;
            
        case MMODE_AC_VOLTAGE:
            reading.value = multimeter_read_ac_voltage();
            reading.unit = "V~";
            reading.unitAbbrev = "VAC";
            break;
            
        case MMODE_DC_CURRENT:
            reading.value = multimeter_read_dc_current();
            reading.unit = "A";
            reading.unitAbbrev = "A";
            break;
            
        case MMODE_RESISTANCE:
            reading.value = multimeter_read_resistance();
            reading.unit = "Ohm";
            reading.unitAbbrev = "Ohm";
            break;
            
        case MMODE_CONTINUITY:
            reading.value = multimeter_read_resistance();
            reading.unit = "Ohm";
            reading.unitAbbrev = "Ohm";
            reading.state = (reading.value < SHORT_CIRCUIT_OHMS) ? MSTATE_SHORT : MSTATE_MEASURING;
            break;
            
        case MMODE_POWER:
            {
                float v = multimeter_read_dc_voltage();
                float i = multimeter_read_dc_current();
                reading.value = v * i;
                reading.unit = "W";
                reading.unitAbbrev = "W";
            }
            break;
    }
    
    reading.value = multimeter_apply_filters(reading.value);
    reading.valid = (reading.value >= 0.0f);
    
    if (!reading.valid) {
        reading.statusColor = C_RED;
    } else if (currentState == MSTATE_SHORT || currentState == MSTATE_HIGH_VOLTAGE) {
        reading.statusColor = C_ORANGE;
    } else {
        reading.statusColor = C_GREEN;
    }
    
    lastReading = reading;
    readingValid = true;
    lastReadingTime = millis();
    
    return reading;
}

void multimeter_read_async_start() {
    if (currentMode == MMODE_AC_VOLTAGE && !core1TaskRunning) {
        core1TaskRunning = true;
        xTaskCreatePinnedToCore(
            [](void* param) { multimeter_read_ac_voltage(); },
            "AC_Voltage",
            4096,
            NULL,
            1,
            &acVoltageTaskHandle,
            1
        );
    }
}

MultimeterReading multimeter_get_last_reading() {
    return lastReading;
}

// ============================================================================
// MODO E RANGE
// ============================================================================

void multimeter_set_mode(MultimeterMode mode) {
    currentMode = mode;
    currentState = MSTATE_IDLE;
    movingAvgCount = 0;
    filteredValue = 0.0f;
}

MultimeterMode multimeter_get_mode() {
    return currentMode;
}

void multimeter_set_range(MeasurementRange range) {
    currentRange = range;
    config.currentRange = range;
}

MeasurementRange multimeter_get_range() {
    return currentRange;
}

void multimeter_auto_range() {
    MeasurementRange suggested = multimeter_suggest_range(lastReading.value);
    if (suggested != currentRange) {
        currentRange = suggested;
        config.currentRange = suggested;
    }
}

MeasurementRange multimeter_suggest_range(float value) {
    if (value < 0.0f) return RANGE_LOW;
    
    switch (currentMode) {
        case MMODE_DC_VOLTAGE:
            if (value < DC_VOLTAGE_RANGE_MED) return RANGE_MED;
            return RANGE_HIGH;
            
        case MMODE_AC_VOLTAGE:
            if (value < 150.0f) return RANGE_LOW;
            return RANGE_HIGH;
            
        case MMODE_DC_CURRENT:
            if (value < CURRENT_RANGE_MA) return RANGE_LOW;
            return RANGE_HIGH;
            
        default:
            return RANGE_AUTO;
    }
}

// ============================================================================
// CALIBRAÇÃO
// ============================================================================

void multimeter_calibrate_zmpt(float realVoltage) {
    uint16_t adcValue = analogRead(MULTIMETER_ADC_PIN);
    zmptCalibrationFactor = realVoltage / ((float)adcValue * ZMPT_VOLTS_PER_COUNT / 1000.0f);
    config.zmptCalibration = zmptCalibrationFactor;
    Serial.print(F("[MULTI] ZMPT calibrated: "));
    Serial.println(zmptCalibrationFactor);
}

void multimeter_calibrate_ina219(float realVoltage, float realCurrent) {
    float calculatedCurrent = multimeter_read_dc_current();
    ina219CalibrationFactor = realCurrent / calculatedCurrent;
    config.ina219Calibration = ina219CalibrationFactor;
    Serial.print(F("[MULTI] INA219 calibrated: "));
    Serial.println(ina219CalibrationFactor);
}

void multimeter_save_calibration() {
    nvs.begin("multimeter", false);
    nvs.putFloat("zmptCal", config.zmptCalibration);
    nvs.putFloat("inaCal", config.ina219Calibration);
    nvs.putFloat("shuntR", config.shuntResistance);
    nvs.end();
    Serial.println(F("[MULTI] Calibration saved"));
}

void multimeter_load_calibration() {
    nvs.begin("multimeter", true);
    
    if (nvs.isKey("zmptCal")) {
        config.zmptCalibration = nvs.getFloat("zmptCal", 1.0f);
        zmptCalibrationFactor = config.zmptCalibration;
    }
    
    if (nvs.isKey("inaCal")) {
        config.ina219Calibration = nvs.getFloat("inaCal", 1.0f);
        ina219CalibrationFactor = config.ina219Calibration;
    }
    
    if (nvs.isKey("shuntR")) {
        config.shuntResistance = nvs.getFloat("shuntR", INA219_SHUNT_OHMS);
        shuntResistance = config.shuntResistance;
    }
    
    nvs.end();
    Serial.println(F("[MULTI] Calibration loaded"));
}

void multimeter_reset_calibration() {
    config.zmptCalibration = 1.0f;
    config.ina219Calibration = 1.0f;
    config.shuntResistance = INA219_SHUNT_OHMS;
    
    zmptCalibrationFactor = 1.0f;
    ina219CalibrationFactor = 1.0f;
    shuntResistance = INA219_SHUNT_OHMS;
    
    multimeter_save_calibration();
    Serial.println(F("[MULTI] Calibration reset"));
}

// ============================================================================
// FILTROS
// ============================================================================

float multimeter_filter_moving_average(float newValue) {
    movingAverageBuffer[movingAvgIndex] = newValue;
    
    movingAvgIndex++;
    if (movingAvgIndex >= 32) movingAvgIndex = 0;
    
    if (movingAvgCount < 32) movingAvgCount++;
    
    float sum = 0.0f;
    for (uint8_t i = 0; i < movingAvgCount; i++) {
        sum += movingAverageBuffer[i];
    }
    
    return sum / movingAvgCount;
}

float multimeter_filter_exponential(float newValue) {
    if (movingAvgCount == 0) {
        filteredValue = newValue;
    } else {
        filteredValue = config.filterAlpha * newValue + (1.0f - config.filterAlpha) * filteredValue;
    }
    
    movingAvgCount++;
    return filteredValue;
}

float multimeter_apply_filters(float rawValue) {
    float filtered = multimeter_filter_moving_average(rawValue);
    filtered = multimeter_filter_exponential(filtered);
    return filtered;
}

// ============================================================================
// UI / DISPLAY
// ============================================================================

void multimeter_handle() {
    status.uptimeMs = millis();
    
    unsigned long now = millis();
    if (now - lastReadingTime >= config.updateIntervalMs) {
        multimeter_read();
        lastReadingTime = now;
    }
    
    if (now - lastReadingTime < 500) {
        multimeter_update_display();
    }
}

void multimeter_update_display() {
    static unsigned long lastUpdate = 0;
    unsigned long now = millis();
    
    if (now - lastUpdate >= config.updateIntervalMs) {
        multimeter_draw(&lastReading);
        lastUpdate = now;
    }
}

void multimeter_draw(const MultimeterReading* reading) {
    extern TFT_eSPI tft;
    
    tft.fillRect(0, STATUS_BAR_H, SCREEN_W, SCREEN_H - STATUS_BAR_H - FOOTER_H, C_BLACK);
    
    if (!reading->valid) {
        tft.setTextColor(C_GREY);
        tft.setTextSize(2);
        tft.setCursor(80, 100);
        tft.print(F("NO READING"));
        return;
    }
    
    char valueStr[32];
    multimeter_format_value(reading->value, valueStr, sizeof(valueStr));
    
    uint16_t valueColor;
    switch (reading->state) {
        case MSTATE_SHORT:
        case MSTATE_HIGH_VOLTAGE:
            valueColor = C_ORANGE;
            break;
        case MSTATE_ERROR:
        case MSTATE_OVERLOAD:
            valueColor = C_RED;
            break;
        default:
            valueColor = C_YELLOW;
    }
    
    tft.setTextColor(valueColor);
    tft.setTextSize(4);
    tft.setCursor(40, 60);
    tft.print(valueStr);
    
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setCursor(40 + strlen(valueStr) * 28, 80);
    tft.print(reading->unit);
    
    multimeter_draw_indicators(reading->mode, reading->range);
    
    if (reading->state == MSTATE_SHORT) {
        tft.fillRect(50, 130, 220, 30, C_ORANGE);
        tft.setTextColor(C_BLACK);
        tft.setTextSize(1);
        tft.setCursor(160, 145);
        tft.print(F("SHORT CIRCUIT!"));
        set_red_led(true);
    } else if (reading->state == MSTATE_HIGH_VOLTAGE) {
        tft.fillRect(50, 130, 220, 30, C_RED);
        tft.setTextColor(C_WHITE);
        tft.setTextSize(1);
        tft.setCursor(150, 145);
        tft.print(F("DANGER! HIGH VOLTAGE!"));
        set_red_led(true);
    }
    
    if (millis() - lastHistoryAdd > 2000) {
        multimeter_history_add(reading->value, reading->mode);
        lastHistoryAdd = millis();
    }
}

void multimeter_draw_value(float value, const char* unit, uint16_t color) {
    extern TFT_eSPI tft;
    
    char buffer[32];
    multimeter_format_value(value, buffer, sizeof(buffer));
    
    tft.setTextColor(color);
    tft.setTextSize(5);
    tft.setCursor(30, 80);
    tft.print(buffer);
    
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setCursor(30 + strlen(buffer) * 24, 120);
    tft.print(unit);
}

void multimeter_draw_indicators(MultimeterMode mode, MeasurementRange range) {
    extern TFT_eSPI tft;
    
    const char* modeStr;
    uint16_t modeColor;
    
    switch (mode) {
        case MMODE_DC_VOLTAGE: modeStr = "VDC"; modeColor = C_CYAN; break;
        case MMODE_AC_VOLTAGE: modeStr = "VAC"; modeColor = C_YELLOW; break;
        case MMODE_DC_CURRENT: modeStr = "A"; modeColor = C_GREEN; break;
        case MMODE_RESISTANCE: modeStr = "Ohm"; modeColor = C_RED; break;
        case MMODE_CONTINUITY: modeStr = "CONT"; modeColor = C_ORANGE; break;
        case MMODE_POWER: modeStr = "W"; modeColor = C_PURPLE; break;
        default: modeStr = ""; modeColor = C_WHITE;
    }
    
    tft.setTextColor(modeColor);
    tft.setTextSize(1);
    tft.setCursor(250, 30);
    tft.print(modeStr);
    
    const char* rangeStr;
    switch (range) {
        case RANGE_LOW: rangeStr = "LOW"; break;
        case RANGE_MED: rangeStr = "MED"; break;
        case RANGE_HIGH: rangeStr = "HIGH"; break;
        default: rangeStr = "AUTO";
    }
    
    tft.setTextColor(C_GREY);
    tft.setCursor(250, 45);
    tft.print(rangeStr);
}

void multimeter_draw_alert(const char* message, uint16_t color) {
    extern TFT_eSPI tft;
    
    tft.drawRect(20, 100, 280, 40, C_RED);
    tft.drawRect(21, 101, 278, 38, C_RED);
    
    tft.setTextColor(color);
    tft.setTextSize(2);
    int x = 160 - (strlen(message) * 12) / 2;
    tft.setCursor(x, 115);
    tft.print(message);
}

// ============================================================================
// PROTEÇÃO / SEGURANÇA
// ============================================================================

void multimeter_check_high_voltage(float voltage) {
    if (!config.highVoltageAlert) return;
    
    if (voltage > HIGH_VOLTAGE_THRESHOLD) {
        currentState = MSTATE_HIGH_VOLTAGE;
        set_red_led(true);
        status.errors++;
    } else {
        set_red_led(false);
    }
}

void multimeter_check_short_circuit(float resistance) {
    if (!config.shortCircuitAlert) return;
    
    if (resistance < SHORT_CIRCUIT_OHMS) {
        currentState = MSTATE_SHORT;
        set_red_led(true);
        status.errors++;
    } else {
        set_red_led(false);
    }
}

void multimeter_alert_sound(uint8_t type) {
    if (!config.soundEnabled) return;
    buzzer_alert();
}

void multimeter_alert_led(uint8_t type) {
    flash_alert();
}

void multimeter_clear_alerts() {
    set_red_led(false);
    set_green_led(false);
    buzzer_no_tone();
}

// ============================================================================
// HISTÓRICO
// ============================================================================

void multimeter_history_add(float value, MultimeterMode mode) {
    history.entries[history.index].value = value;
    history.entries[history.index].mode = mode;
    history.entries[history.index].timestamp = millis();
    history.entries[history.index].valid = true;
    
    history.index++;
    if (history.index >= HISTORY_SIZE) history.index = 0;
    
    if (history.count < HISTORY_SIZE) history.count++;
}

void multimeter_history_clear() {
    history.count = 0;
    history.index = 0;
}

MeasurementHistory* multimeter_get_history() {
    return &history;
}

// ============================================================================
// UTILITÁRIOS
// ============================================================================

void multimeter_format_value(float value, char* buffer, uint8_t maxLen) {
    if (value < 0.0f) {
        strncpy(buffer, "---", maxLen);
        return;
    }
    
    if (value < 1.0f) {
        char temp[16];
        dtostrf(value * 1000.0f, 1, 1, temp);
        snprintf(buffer, maxLen, "%sm", temp);
    } else if (value < 1000.0f) {
        dtostrf(value, 1, 2, buffer);
    } else if (value < 1000000.0f) {
        char temp[16];
        dtostrf(value / 1000.0f, 1, 1, temp);
        snprintf(buffer, maxLen, "%sk", temp);
    } else {
        char temp[16];
        dtostrf(value / 1000000.0f, 1, 1, temp);
        snprintf(buffer, maxLen, "%sM", temp);
    }
}

bool multimeter_detect_voltage_type(float voltage) {
    if (voltage > 180.0f) return true;
    else if (voltage > 80.0f) return false;
    return false;
}