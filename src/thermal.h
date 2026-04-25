// ============================================================================
// Sondvolt v3.2 — Sonda Térmica (Header)
// Hardware: ESP32-2432S028R (Cheap Yellow Display)
// ============================================================================
// Arquivo: thermal.h
// Descrição: Protótipos para leitura de temperatura
// ============================================================================

#ifndef THERMAL_H
#define THERMAL_H

#include <Arduino.h>

void thermal_init();
float thermal_read();
float thermal_get_last();
bool thermal_is_warning();
bool thermal_is_critical();

#endif // THERMAL_H