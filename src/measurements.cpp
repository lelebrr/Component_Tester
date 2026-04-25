// ============================================================================
// Sondvolt v3.2 — Medições e Julgamento Inteligente
// Hardware: ESP32-2432S028R (Cheap Yellow Display)
// ============================================================================
// Arquivo: measurements.cpp
// Descrição: Lógica de leitura de sensores e análise de componentes
// ============================================================================

#include "measurements.h"
#include "config.h"
#include "database.h"
#include "thermal.h"
#include "display_globals.h"
#include "globals.h"

// Estado Global
static float lastValue = 0;
static ComponentStatus lastStatus = STATUS_UNKNOWN;

void measurements_init() {
    analogReadResolution(12);
    pinMode(PIN_PROBE_1, INPUT);
    pinMode(PIN_PROBE_2, INPUT); // Configurado como entrada/saída dinamicamente
    pinMode(PIN_ZMPT_OUT, INPUT);
}

void measurements_update() {
    // Realiza múltiplas leituras para estabilizar
    uint32_t sum = 0;
    for(int i=0; i<10; i++) {
        sum += analogRead(PIN_ZMPT_OUT);
        delayMicroseconds(100);
    }
    uint16_t raw = sum / 10;
    lastValue = (float)raw * ADC_REF_VOLT / ADC_MAX_VAL;
    
    // Análise de status baseada no banco de dados se disponível
    if (componentDB.loaded && componentDB.count > 0) {
        lastStatus = db_judge(COMP_GENERIC, lastValue);
    } else {
        // Fallback para lógica fixa
        if (lastValue > 2.0f) lastStatus = STATUS_GOOD;
        else if (lastValue > 1.0f) lastStatus = STATUS_SUSPECT;
        else lastStatus = STATUS_BAD;
    }
}

float measurements_get_last_value() {
    return lastValue;
}

ComponentStatus measurements_get_last_status() {
    return lastStatus;
}

// Implementação de True RMS para Tensão AC (ZMPT101B)
float measurements_read_ac_rms() {
    long sumSquares = 0;
    int samples = TRUE_RMS_SAMPLES;
    
    for (int i = 0; i < samples; i++) {
        long val = analogRead(PIN_ZMPT_OUT) - 2048; // Offset do ZMPT (centro do ADC)
        sumSquares += (val * val);
        delayMicroseconds(500); // 2kHz sample rate
    }
    
    float rms = sqrt(sumSquares / samples);
    return rms * (MAX_VOLTAGE_AC / 2048.0f); // Calibração básica
}

float measurements_get_raw_resistance() {
    // Implementa divisor de tensão para medir resistência
    // R = R_pullup * (V_out / (V_in - V_out))
    uint16_t raw = analogRead(PIN_PROBE_1);
    float voltage = (float)raw * ADC_REF_VOLT / ADC_MAX_VAL;
    if (voltage >= 3.25f) return 999999.0f; // Aberto
    if (voltage <= 0.05f) return 0.0f;      // Curto
    
    float resistance = 10000.0f * voltage / (ADC_REF_VOLT - voltage);
    return resistance;
}

float measurements_get_raw_capacitance() {
    // Heurística de tempo de carga simplificada
    pinMode(PIN_PROBE_1, OUTPUT);
    digitalWrite(PIN_PROBE_1, LOW);
    delay(10);
    pinMode(PIN_PROBE_1, INPUT);
    
    uint32_t start = micros();
    while(analogRead(PIN_PROBE_1) < 2600 && (micros() - start < 1000000));
    uint32_t duration = micros() - start;
    
    // C = t / (R * ln(2)) -> Aproximação linear para fins de UI
    return (float)duration / 1000.0f; // Resultado em uF (escala arbitrária para teste)
}