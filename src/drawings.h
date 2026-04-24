// ============================================================================
// Component Tester PRO v3.0 - Desenhos e Icons
// Descricao: Funcoes graficas para display Adafruit ILI9341
// Versao: CYD Edition para ESP32-2432S028R
// ============================================================================
#ifndef DRAWINGS_H
#define DRAWINGS_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// ============================================================================
// INSTANCIA GLOBAL DO TFT (definida em main.cpp)
// ============================================================================
extern Adafruit_ILI9341 tft;

// ============================================================================
// FUNCOES DE UI MODERNAS
// ============================================================================

// Barra de status superior (versao, temperatura, icone SD)
void draw_status_bar();

// Rodape com dicas de navegacao
void draw_footer_modern();

// Item do grid de menu (card com icone e label)
void draw_grid_item(int16_t x, int16_t y, const char* label, int iconId, bool selected);

// Card moderno com titulo e borda colorida
void draw_modern_card(const char* title, uint16_t color);

// ============================================================================
// ICONES LEGADOS (retro-compatibilidade)
// ============================================================================
void draw_capacitor_icon(int16_t x, int16_t y);
void draw_resistor_icon(int16_t x, int16_t y);
void draw_diode_icon(int16_t x, int16_t y);
void draw_led_icon(int16_t x, int16_t y, uint16_t color);
void draw_transistor_icon(int16_t x, int16_t y);
void draw_inductor_icon(int16_t x, int16_t y);

// ============================================================================
// ICONES DINAMICOS DE COMPONENTES (sistema de icones vetoriais)
// Tipo extra para capacitor eletrolitico (visual diferenciado)
#define CAT_CAP_ELECTRO 100

// Desenha o icone de um componente baseado no seu tipo (categoria do database)
void drawComponentIcon(uint8_t componentType, int16_t x, int16_t y, uint16_t color);

// Desenha icone + label textual abaixo
void drawComponentWithLabel(uint8_t componentType, int16_t x, int16_t y, uint16_t color, const char* label);

// ============================================================================
// ICONES DO MENU PRINCIPAL
// ============================================================================
void draw_menu_icon(int16_t x, int16_t y, int iconId, uint16_t color);

#endif // DRAWINGS_H