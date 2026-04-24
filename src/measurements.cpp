// ============================================================================
// Component Tester PRO v3.0 — Medições de Componentes (Implementação)
// Descrição: Algoritmos de medição usando ADC e comparadores do ESP32
// ============================================================================

#include "measurements.h"
#include "globals.h"
#include "config.h"
#include "database.h"
#include "buzzer.h"
#include "leds.h"
#include "logger.h"
#include "graphics.h"

// ============================================================================
// ESTADO DE MEDIÇÃO
// ============================================================================
static uint8_t currentMode = MODE_GENERIC;
static bool measuringActive = false;
static unsigned long measureStartMs = 0;
static ComponentMeasurement currentResult;

// ============================================================================
// FUNÇÕES AUXILIARES DE ADC
// ============================================================================

// Leitura ADC filtrada (média de N amostras)
static int read_adc_filtered(int pin, int samples) {
    long sum = 0;
    for (int i = 0; i < samples; i++) {
        sum += analogRead(pin);
        delayMicroseconds(100);
    }
    return sum / samples;
}

// Converte contagem ADC para volts
static float adc_to_volts(int raw) {
    return raw * ADC_VOLTS_PER_BIT;
}

// Converte tensão para resistência (usando divisor resistivo)
// R = V_out * R_known / (V_in - V_out)
static float voltage_to_resistance(float vOut, float rKnown, float vIn) {
    if (vIn <= vOut || vOut <= 0.001f) return 0;
    return rKnown * vOut / (vIn - vOut);
}

// ============================================================================
// MEDIÇÃO DE RESISTOR
// ============================================================================
float measure_resistance(uint8_t pin) {
    // Usa o ADC com um divisor resistivo conhecido
    // Para a CYD, usamos:
    // - Pino: GPIO 35 (ADC1_7)
    // - Referência: 3.3V
    // - R_known: 10kΩ (interno ou externo)
    const float R_REFERENCE = 10000.0f; // 10kΩ

    int raw = read_adc_filtered(pin, 20);
    float vOut = adc_to_volts(raw);

    float r = voltage_to_resistance(vOut, R_REFERENCE, ADC_REF_VOLTAGE);

    // Aplica offset de calibração
    r -= probeOffsetResistance;
    if (r < 0) r = 0;

    return r;
}

// ============================================================================
// MEDIÇÃO DE CAPACITOR
// ============================================================================
float measure_capacitance(uint8_t pinCharge, uint8_t pinRead) {
    // Método de carga: mede o tempo de carga através de um resistor conhecido
    // Para capacitores grandes (> 1µF), medimos diretamente o voltage
    // Para capacitores pequenos, usamos o método RC

    const float R_CHARGE = 10000.0f; // 10kΩ
    const float V_THRESHOLD = ADC_REF_VOLTAGE * 0.632f; // 63.2% de Vcc

    // Carrega o capacitor
    pinMode(pinCharge, OUTPUT);
    digitalWrite(pinCharge, LOW);
    delay(100); // Descarga completa

    digitalWrite(pinCharge, HIGH);

    unsigned long startUs = micros();
    int raw;
    while (true) {
        raw = analogRead(pinRead);
        float v = adc_to_volts(raw);
        if (v >= V_THRESHOLD) break;
        if (micros() - startUs > 5000000UL) break; // Timeout 5s
    }
    unsigned long elapsedUs = micros() - startUs;

    // Tau = RC, C = Tau / R
    float tau_us = (float)elapsedUs;
    float c = tau_us / R_CHARGE; // em microfarads

    // Converter para unidades legíveis
    c *= 1000000.0f; // µF

    if (c > 1000.0f) {
        return c / 1000.0f; // Retorna em mF se for grande
    }

    c -= probeOffsetCapacitance;
    if (c < 0) c = 0;

    return c; // µF
}

// ============================================================================
// MEDIÇÃO DE DIODO
// ============================================================================
float measure_diode_vf(uint8_t pinAnode, uint8_t pinCatode) {
    // Aplica pequena corrente forward e mede a tensão direta
    // Usa o ADC do ESP32

    int raw = read_adc_filtered(pinAnode, 10);
    float vf = adc_to_volts(raw);
    return vf;
}

// ============================================================================
// MEDIÇÃO DE TRANSISTOR (hFE aproximado)
// Usa método simples: mede corrente de base e coletor
// ============================================================================
float measure_transistor_hfe(uint8_t pinB, uint8_t pinC, uint8_t pinE) {
    // Implementação simplificada
    // hFE = I_C / I_B (ganho de corrente)
    return 0.0f; // Placeholder
}

// ============================================================================
// MEDIÇÃO DE INDUTOR
// ============================================================================
float measure_inductance() {
    // Método LC: usa o período de ressonância
    // L = 1 / (omega² * C)
    // Placeholder: retorna 0
    return 0.0f;
}

// ============================================================================
// AUTO-DETECÇÃO DE COMPONENTE
// ============================================================================
ComponentType auto_detect_component() {
    int raw = read_adc_filtered(PIN_PROBE_1, 10);
    float v = adc_to_volts(raw);

    if (v < 0.01f) return COMP_RESISTOR; // Curto
    if (v > 3.0f) return COMP_RESISTOR; // Aberto ou grande resistor

    // Detecção baseada na tensão
    if (v > 0.5f && v < 1.0f) return COMP_DIODE;
    if (v > 0.1f && v < 0.5f) return COMP_LED;

    return COMP_RESISTOR;
}

// ============================================================================
// CONTINUIDADE
// ============================================================================
bool test_continuity(uint8_t pin1, uint8_t pin2) {
    float r = measure_between_pins(pin1, pin2);
    return (r < THRESHOLD_SHORT);
}

float measure_between_pins(uint8_t pin1, uint8_t pin2) {
    (void)pin2;
    return measure_resistance(pin1);
}

// ============================================================================
// FORMATAR VALOR DE MEDIÇÃO
// ============================================================================
void format_measurement_value(float value, const char* unit,
                               char* output, size_t maxLen) {
    if (isnan(value) || isinf(value) || value <= 0) {
        snprintf(output, maxLen, "---");
        return;
    }

    if (strcmp(unit, "Ω") == 0) {
        if (value >= 1000000.0f) {
            snprintf(output, maxLen, "%.2fM", value / 1000000.0f);
        } else if (value >= 1000.0f) {
            snprintf(output, maxLen, "%.1fK", value / 1000.0f);
        } else {
            snprintf(output, maxLen, "%.0f", value);
        }
    } else if (strcmp(unit, "µF") == 0) {
        if (value >= 1000.0f) {
            snprintf(output, maxLen, "%.2fmF", value / 1000.0f);
        } else if (value >= 1.0f) {
            snprintf(output, maxLen, "%.2fµF", value);
        } else if (value >= 0.001f) {
            snprintf(output, maxLen, "%.2fnF", value * 1000.0f);
        } else {
            snprintf(output, maxLen, "%.2fpF", value * 1000000.0f);
        }
    } else if (strcmp(unit, "V") == 0) {
        snprintf(output, maxLen, "%.3f", value);
    } else {
        dtostrf(value, 8, 4, output);
    }
}

// ============================================================================
// UI DE MEDIÇÃO
// ============================================================================
void measurements_start(uint8_t mode) {
    currentMode = mode;
    measuringActive = true;
    measureStartMs = millis();
    led_flash_rgb(0, 0, 50, 200, 200); // Azul piscando
}

void measurements_stop() {
    measuringActive = false;
    led_off();
}

void measurements_draw(ComponentMeasurement* result) {
    if (!result) return;

    tft.fillScreen(UI_COLORS::BACKGROUND);
    draw_status_bar();

    // Card principal
    int cx = 160, cy = 110;
    tft.fillRoundRect(cx - 130, cy - 70, 260, 140, 12,
                       UI_COLORS::CARD_BG);

    uint16_t statusColor = db_status_color((ComponentStatus)result->status);
    tft.drawRoundRect(cx - 130, cy - 70, 260, 140, 12, statusColor);

    // Nome do componente
    tft.setTextColor(UI_COLORS::TEXT_DIM);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(cx, cy - 55);
    tft.print(result->name);

    // Valor principal
    tft.setTextColor(UI_COLORS::TEXT);
    tft.setTextSize(5);
    tft.setCursor(cx, cy - 20);
    tft.print(result->valueStr);

    // Unidade
    tft.setTextSize(2);
    tft.setCursor(cx + 10, cy + 20);
    tft.print(result->unit);

    // Status
    tft.fillRoundRect(cx - 60, cy + 40, 120, 24, 8, statusColor);
    tft.setTextColor(UI_COLORS::TEXT);
    tft.setTextSize(2);
    tft.setCursor(cx, cy + 48);
    tft.print(db_status_string((ComponentStatus)result->status));

    tft.setTextDatum(TL_DATUM);
    draw_footer();
}

void measurements_handle() {
    lastActivityMs = millis();

    // Atualiza LED RGB
    static unsigned long lastRgbToggle = 0;
    unsigned long now = millis();
    if (now - lastRgbToggle > 300) {
        static int phase = 0;
        phase = !phase;
        if (phase) {
            led_set_rgb(0, 0, 30);
        } else {
            led_set_rgb(0, 0, 10);
        }
        lastRgbToggle = now;
    }

    // Realiza medição baseada no modo
    float value = 0;
    float temp = thermal_read_async();
    const char* unit = "?";
    const char* name = "Desconhecido";

    switch (currentMode) {
        case MODE_RESISTOR:
            value = measure_resistance(PIN_PROBE_1);
            unit = "Ω";
            name = "Resistor";
            break;
        case MODE_CAPACITOR:
            value = measure_capacitance(PIN_PROBE_1, PIN_PROBE_2);
            unit = "µF";
            name = "Capacitor";
            break;
        case MODE_DIODE:
            value = measure_diode_vf(PIN_PROBE_1, PIN_PROBE_2);
            unit = "V";
            name = "Diodo";
            break;
        default:
            break;
    }

    // Formata o valor
    char valueStr[32];
    format_measurement_value(value, unit, valueStr, sizeof(valueStr));

    currentResult.value = value;
    currentResult.unit = unit;
    currentResult.valueStr = valueStr;
    currentResult.temperature = temp;
    currentResult.status = db_judge(
        (ComponentType)currentMode, value);
    currentResult.type = currentMode;
    currentResult.name = name;

    // Desenha atualização
    measurements_draw(&currentResult);

    // Feedback sonoro baseado no status
    static uint8_t lastStatus = 255;
    if (currentResult.status != lastStatus) {
        if (currentResult.status == STATUS_GOOD) {
            buzzer_ok();
        } else if (currentResult.status == STATUS_BAD) {
            buzzer_error();
        }
        lastStatus = currentResult.status;
    }

    // Verifica botões
    if (btn_just_pressed(BTN_BACK)) {
        measurements_stop();
        currentAppState = STATE_MENU;
    }

    delay(50);
}