// ============================================================================
// Sondvolt v3.x — Sistema de Calibração
//Hardware: ESP32-2432S028R (Cheap Yellow Display)
// ============================================================================
#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <Arduino.h>

// Estrutura de dados de calibração
typedef struct {
    float resistance_offset;   // Offset para pontas em curto
    float capacitance_offset;  // Offset parasitário das pontas
    float voltage_scale;       // Fator de escala para voltagem
    uint32_t checksum;         // Para validar no Flash/SD
} CalibrationData;

extern CalibrationData currentCal;

// Protótipos
void calibration_init();
bool calibration_run_auto();
void calibration_save();
void calibration_load();
void calibration_reset();

#endif // CALIBRATION_H
