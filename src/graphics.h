// ============================================================================
// Sondvolt v3.x — Graficos e Icones
// Hardware: ESP32-2432S028R (Cheap Yellow Display)
// ============================================================================
// Arquivo: graphics.h
// Descricao: Prototipos de funcoes de desenho e icones vetoriais
// ============================================================================

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "config.h"
#include "globals.h"

// ============================================================================
// ICONES VETORIAIS
// ============================================================================
void draw_component_icon(IconType icon, int16_t x, int16_t y, uint16_t color);

// Funcoes individuais para icones
void draw_icon_resistor(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_capacitor_ceramic(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_capacitor_electro(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_diode(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_led(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_transistor_npn(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_transistor_pnp(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_mosfet_n(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_mosfet_p(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_inductor(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_crystal(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_fuse(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_varistor(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_potentiometer(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_optocoupler(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_unknown(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_multimeter(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_temp(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_history(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_settings(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_about(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_warning(int16_t x, int16_t y, uint16_t size, uint16_t color);
void draw_icon_voltage(int16_t x, int16_t y, uint16_t size, uint16_t color);

// ============================================================================
// ELEMENTOS DA UI
// ============================================================================
void clear_screen();
void draw_status_bar(const char* title = "Sondvolt", const char* subtitle = nullptr);
void draw_footer(const char* left = nullptr, const char* right = nullptr);
void draw_progress_bar(int16_t x, int16_t y, uint16_t w, uint16_t h, float percent, uint16_t color);
void draw_status_indicator(MeasurementStatus status, int16_t x, int16_t y, uint16_t size);
void draw_splash_screen();

// ============================================================================
// CORES E UTILITARIOS DE DESENHO
// ============================================================================
uint16_t color_darker(uint16_t color, uint8_t shift);
uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b);

// ============================================================================
// INICIALIZACAO
// ============================================================================
void graphics_init(void);

#endif // GRAPHICS_H