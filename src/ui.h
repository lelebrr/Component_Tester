#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include "globals.h"

// Inicialização e Ciclo de Vida
void ui_init();
void ui_update();

// Toque e Interação
bool ui_handle_touch(uint16_t x, uint16_t y);
void ui_reset_touch_state();

// Calibração
void ui_calibration_update_progress(uint8_t progress, const char* msg);
void ui_calibration_show_result(bool success, const char* msg);

#endif