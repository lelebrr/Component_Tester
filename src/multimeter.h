// ============================================================================
// Component Tester PRO v3.0 - Multimestro AC/DC com True RMS
// Descricao: Medicoes de tensao e corrente usando ZMPT101B e INA219
// Versao: CYD Edition para ESP32-2432S028R
// ============================================================================
#ifndef MULTIMETER_H
#define MULTIMETER_H

#include <stdint.h>

// ============================================================================
// MODOS DO MULTIMETRO
// ============================================================================
enum MultimeterMode {
    MMODE_DC_VOLTAGE = 0,  // Tensao DC (INA219)
    MMODE_AC_VOLTAGE  = 1,  // Tensao AC (ZMPT101B)
    MMODE_DC_CURRENT = 2,  // Corrente DC (INA219)
    MMODE_AC_CURRENT = 3,  // Corrente AC (estimado)
    MMODE_RESISTANCE = 4,  // Resistencia (ADC)
    MMODE_CONTINUITY = 5   // Continuidade
};

// ============================================================================
// RESULTADO DE MEDICAO
// ============================================================================
struct MultimeterReading {
    float value;
    const char* unit;
    MultimeterMode mode;
    bool valid;
    unsigned long timestamp;
    uint16_t statusColor;
};

// ============================================================================
// PROTOTIPOS
// ============================================================================

// Inicializacao dos sensores
void multimeter_init();

// Leitura de tensao DC
float multimeter_read_dc_voltage();

// Leitura de tensao AC (True RMS)
float multimeter_read_ac_voltage();

// Leitura de corrente DC (INA219)
float multimeter_read_dc_current();

// Leitura de corrente AC (estimada)
float multimeter_read_ac_current();

// Leitura completa (depende do modo atual)
MultimeterReading multimeter_read();

// Troca modo
void multimeter_set_mode(MultimeterMode mode);
MultimeterMode multimeter_get_mode();

// Calibracao do ZMPT101B
void multimeter_calibrate_zmpt(float measuredRealVoltage);

// Handle da UI (loop)
void multimeter_handle();

// Desenhar tela do multimetro
void multimeter_draw(const MultimeterReading* reading);

// True RMS calculation helper
float calculate_true_rms(float* samples, int count);

#endif // MULTIMETER_H