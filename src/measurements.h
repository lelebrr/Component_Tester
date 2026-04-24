// ============================================================================
// Component Tester PRO v3.0 — Medições de Componentes
// Descrição: Funções de medição de resistores, capacitores, diodos, etc.
// ============================================================================
#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include <stdint.h>

// ============================================================================
// RESULTADO DE MEDIÇÃO
// ============================================================================
struct ComponentMeasurement {
    float value;
    const char* unit;
    const char* valueStr;
    uint8_t status;  // STATUS_GOOD, STATUS_SUSPECT, STATUS_BAD
    uint8_t type;
    const char* name;
    float temperature;
};

// ============================================================================
// PROTÓTIPOS — FUNÇÕES DE MEDIÇÃO
// ============================================================================

// Medição de resistor (ADC)
float measure_resistance(uint8_t pin);

// Medição de capacitor (carrega via resistor conhecido)
float measure_capacitance(uint8_t pinCharge, uint8_t pinRead);

// Medição de diodo (tensão direta)
float measure_diode_vf(uint8_t pinAnode, uint8_t pinCatode);

// Leitura de transistor (hFE estimado)
float measure_transistor_hfe(uint8_t pinB, uint8_t pinC, uint8_t pinE);

// Leitura de indutor (via LM311 ou método LC)
float measure_inductance();

// Auto-detecção de componente
ComponentType auto_detect_component();

// Teste de continuidade
bool test_continuity(uint8_t pin1, uint8_t pin2);

// Medição genérica de resistência entre dois pinos
float measure_between_pins(uint8_t pin1, uint8_t pin2);

// ============================================================================
// PROTÓTIPOS — UI DE MEDIÇÃO
// ============================================================================

// Loop principal de medição (chamado no loop do ESP32)
void measurements_handle();

// Desenhar tela de medição
void measurements_draw(ComponentMeasurement* result);

// Iniciar modo de medição
void measurements_start(uint8_t mode);

// Parar modo de medição
void measurements_stop();

// Modos de medição
#define MODE_RESISTOR    0
#define MODE_CAPACITOR  1
#define MODE_DIODE       2
#define MODE_TRANSISTOR 3
#define MODE_INDUCTOR  4
#define MODE_IC         5
#define MODE_GENERIC    99

#endif // MEASUREMENTS_H