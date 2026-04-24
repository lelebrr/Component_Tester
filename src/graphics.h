// ============================================================================
// Component Tester PRO v3.0 — Funções Gráficas (graphics.h)
// Descrição: Declarações dos ícones de componentes e helpers de UI
// ============================================================================
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Adafruit_GFX.h>
#include <stdint.h>

// ============================================================================
// TIPOS DE ÍCONES DE COMPONENTES
// Usados para identificar qual ícone desenhar
// ============================================================================
enum ComponentIconType {
    // Componentes básicos
    ICON_RESISTOR    = 0,
    ICON_CAPACITOR   = 1,
    ICON_ELECTROLYTIC = 2,  // Capacitor eletrolítico
    ICON_DIODE      = 3,
    ICON_LED        = 4,
    ICON_ZENER      = 5,
    ICON_TRANSISTOR  = 6,
    ICON_FET       = 7,
    ICON_INDUCTOR  = 8,
    ICON_RELAY     = 9,
    ICON_IC        = 10,
    ICON_COIL      = 11,

    // Modo multímetro
    ICON_VOLTAGE    = 20,
    ICON_CURRENT   = 21,
    ICON_MULTIMETER = 22,

    // Ferramentas
    ICON_THERMOMETER = 30,
    ICON_SCANNER    = 31,
    ICON_CALIBRATE  = 32,

    // Menu/Settings
    ICON_SETTINGS   = 40,
    ICON_HISTORY   = 41,
    ICON_STATS     = 42,
    ICON_ABOUT     = 43,
    ICON_BRIGHTNESS = 44,
    ICON_SOUND     = 45,
    ICON_DARK      = 46,
    ICON_SLEEP     = 47,
    ICON_RESET     = 48,
    ICON_DELETE    = 49,
    ICON_TEST      = 99
};

// ============================================================================
// PROTÓTIPOS — ÍCONES DE COMPONENTES
// ============================================================================

// Desenha ícone de componente pelo tipo
void draw_component_icon(ComponentIconType type, int16_t x, int16_t y,
                   uint16_t color);

// Ícones individuais (desenhados com primitivas GFX)
void draw_resistor_icon(int16_t x, int16_t y, uint16_t color);
void draw_capacitor_icon(int16_t x, int16_t y, uint16_t color);
void draw_electrolytic_icon(int16_t x, int16_t y, uint16_t color);
void draw_diode_icon(int16_t x, int16_t y, uint16_t color);
void draw_led_icon(int16_t x, int16_t y, uint16_t color);
void draw_zener_icon(int16_t x, int16_t y, uint16_t color);
void draw_transistor_icon(int16_t x, int16_t y, uint16_t color);
void draw_fet_icon(int16_t x, int16_t y, uint16_t color);
void draw_inductor_icon(int16_t x, int16_t y, uint16_t color);
void draw_relay_icon(int16_t x, int16_t y, uint16_t color);
void draw_ic_icon(int16_t x, int16_t y, uint16_t color);
void draw_coil_icon(int16_t x, int16_t y, uint16_t color);

// Multímetro
void draw_voltage_icon(int16_t x, int16_t y, uint16_t color);
void draw_current_icon(int16_t x, int16_t y, uint16_t color);
void draw_multimeter_icon(int16_t x, int16_t y, uint16_t color);

// Ferramentas
void draw_thermometer_icon(int16_t x, int16_t y, uint16_t color);
void draw_scanner_icon(int16_t x, int16_t y, uint16_t color);
void draw_calibrate_icon(int16_t x, int16_t y, uint16_t color);

// Menu/Settings
void draw_settings_icon(int16_t x, int16_t y, uint16_t color);
void draw_history_icon(int16_t x, int16_t y, uint16_t color);
void draw_stats_icon(int16_t x, int16_t y, uint16_t color);
void draw_about_icon(int16_t x, int16_t y, uint16_t color);
void draw_brightness_icon(int16_t x, int16_t y, uint16_t color);
void draw_sound_icon(int16_t x, int16_t y, uint16_t color);
void draw_dark_icon(int16_t x, int16_t y, uint16_t color);
void draw_sleep_icon(int16_t x, int16_t y, uint16_t color);
void draw_reset_icon(int16_t x, int16_t y, uint16_t color);
void draw_delete_icon(int16_t x, int16_t y, uint16_t color);

// ============================================================================
// HELPERS GRÁFICOS
// ============================================================================

// Desenha linha com grossura (grossor 1-5)
void draw_thick_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                   uint16_t color, uint8_t thickness);

// Desenha retângulo arredondado com preenchimento
void draw_rounded_button(int16_t x, int16_t y, int16_t w, int16_t h,
                        const char* label, uint16_t bgColor,
                        uint16_t textColor, uint16_t borderColor);

// Desenha barra de progresso horizontal
void draw_progress_bar(int16_t x, int16_t y, int16_t w, int16_t h,
                      uint8_t percentage, uint16_t fillColor,
                      uint16_t bgColor);

// Desenha barra de progresso animada (chamada no loop)
void draw_progress_bar_animated(int16_t x, int16_t y, int16_t w,
                                int16_t h, uint8_t targetPercent,
                                uint8_t& currentPercent, uint16_t fillColor,
                                uint16_t bgColor);

// Desenha gauge circular (semicírculo com indicador)
void draw_gauge(int16_t cx, int16_t cy, int16_t r,
                float value, float minVal, float maxVal,
                const char* unit, uint16_t color,
                uint16_t bgColor);

// Desenha valor formatado com unidade
void draw_measurement_value(int16_t x, int16_t y,
                           float value, uint16_t valueColor,
                           uint16_t unitColor);

// Desenha barra vertical com label
void draw_vertical_bar(int16_t x, int16_t y, int16_t w, int16_t maxH,
                   float percentage, uint16_t fillColor);

// Escala de cores baseada em valor (verde→amarelo→vermelho)
uint16_t color_by_value(float value, float minGood, float maxGood);

// Cria cor RGB565 a partir de R, G, B (0-255)
uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b);

// Cria cor RGB565 com brilho ajustado
uint16_t rgb565_brightness(uint16_t color, float factor);

// Faz uma cor mais clara ou mais escura
uint16_t adjust_brightness(uint16_t color, int8_t delta);

// Desenha animação de "carregando..."
void draw_loading_animation(int16_t x, int16_t y, int16_t size,
                           unsigned long phase);

// Desenha ícones de seta (← → ↑ ↓)
void draw_arrow_left(int16_t x, int16_t y, uint16_t color);
void draw_arrow_right(int16_t x, int16_t y, uint16_t color);
void draw_arrow_up(int16_t x, int16_t y, uint16_t color);
void draw_arrow_down(int16_t x, int16_t y, uint16_t color);

// Desenha símbolo de graus
void draw_degrees_symbol(int16_t x, int16_t y, uint16_t color);

// ============================================================================
// ANIMAÇÕES
// ============================================================================

// Efeito de "pulse" em um card (brilho pulsante)
void pulse_effect(int16_t x, int16_t y, int16_t w, int16_t h,
                 uint16_t color, uint8_t intensity);

// Animação de "ok" (checkmark animado)
void draw_ok_animation(int16_t x, int16_t y, uint16_t color);

// Animação de "X" (erro animado)
void draw_error_animation(int16_t x, int16_t y, uint16_t color);

// Pisca LED virtual (desenha círculo que pisca)
void draw_virtual_led(int16_t x, int16_t y, uint16_t color, bool on);

#endif // GRAPHICS_H