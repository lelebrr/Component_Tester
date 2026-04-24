// ============================================================================
// Component Tester PRO v3.1 — Graphics Library (Complete Header)
// Designed for ESP32-2432S028R (Cheap Yellow Display)
// ============================================================================
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <stdint.h>

// ============================================================================
// DISPLAY CONSTANTS
// ============================================================================
#define SCREEN_W         320
#define SCREEN_H         240
#define STATUS_BAR_H    28
#define MENU_BAR_H      36
#define FOOTER_H        24

// ============================================================================
// COLOR PALETTE (RGB565)
// ============================================================================
#define C_BLACK         0x0000
#define C_WHITE        0xFFFF
#define C_RED          0xF800
#define C_GREEN        0x07E0
#define C_BLUE         0x001F
#define C_YELLOW       0xFFE0
#define C_CYAN         0x07FF
#define C_MAGENTA      0xF81F
#define C_ORANGE       0xFD20
#define C_PURPLE       0x780F
#define C_PINK         0xF8EE

#define C_DARK         0x0821
#define C_DARK_GREY    0x3186
#define C_GREY         0x7BEF
#define C_LIGHT_GREY   0xC618
#define C_LIGHTER      0xE71C

#define C_OK_GREEN     0x24E0
#define C_WARN_YELLOW 0xFCC0
#define C_ERROR_RED    0xF000
#define C_INFO_BLUE   0x0618

// ============================================================================
// COMPONENT ICONS
// ============================================================================
#define ICON_RESISTOR       0
#define ICON_CAPACITOR     1
#define ICON_ELECTROLYTIC 2
#define ICON_DIODE        3
#define ICON_LED          4
#define ICON_ZENER        5
#define ICON_SCHOTTKY     6
#define ICON_TRANSISTOR_NPN  7
#define ICON_TRANSISTOR_PNP  8
#define ICON_MOSFET_N     9
#define ICON_MOSFET_P    10
#define ICON_INDUCTOR    11
#define ICON_CRYSTAL     12
#define ICON_FUSE       13
#define ICON_RELAY       14
#define ICON_IC         15
#define ICON_OPTO       16
#define ICON_LED_RGB    17
#define ICON_CAP_CERAMIC 18
#define ICON_CAP_MICA   19

// ============================================================================
// UI ICONS
// ============================================================================
#define ICON_VOLTAGE      20
#define ICON_CURRENT     21
#define ICON_RESISTANCE 22
#define ICON_CONTINUITY  23
#define ICON_DIODE_TEST  24
#define ICON_TEMP        25
#define ICON_FREQ        26

#define ICON_MULTIMETER  30
#define ICON_MENU        31
#define ICON_SETTINGS   32
#define ICON_HISTORY    33
#define ICON_ABOUT      34
#define ICON_HOME       35
#define ICON_BACK       36
#define ICON_UP         37
#define ICON_DOWN       38

#define ICON_OK          40
#define ICON_ERROR       41
#define ICON_WARNING     42
#define ICON_INFO        43

// ============================================================================
// BUTTON TYPES
// ============================================================================
#define BTN_RECT        0
#define BTN_ROUND       1
#define BTN_CIRCLE      2
#define BTN_ICON        3
#define BTN_TOGGLE      4
#define BTN_SLIDER      5

// ============================================================================
// GRADIENT DIRECTION
// ============================================================================
#define GRAD_NONE        0
#define GRAD_HORIZ      1
#define GRAD_VERT       2
#define GRAD_DIAG       3

// ============================================================================
// ICON DRAWING FUNCTIONS (Component Icons)
// ============================================================================

void draw_component_icon(uint8_t type, int16_t x, int16_t y, uint16_t color);

void draw_resistor_icon(int16_t x, int16_t y, uint16_t color);
void draw_capacitor_icon(int16_t x, int16_t y, uint16_t color);
void draw_electrolytic_icon(int16_t x, int16_t y, uint16_t color);
void draw_ceramic_cap_icon(int16_t x, int16_t y, uint16_t color);
void draw_diode_icon(int16_t x, int16_t y, uint16_t color);
void draw_led_icon(int16_t x, int16_t y, uint16_t color, bool rgb = false);
void draw_zener_icon(int16_t x, int16_t y, uint16_t color);
void draw_schottky_icon(int16_t x, int16_t y, uint16_t color);
void draw_transistor_npn_icon(int16_t x, int16_t y, uint16_t color);
void draw_transistor_pnp_icon(int16_t x, int16_t y, uint16_t color);
void draw_mosfet_n_icon(int16_t x, int16_t y, uint16_t color);
void draw_mosfet_p_icon(int16_t x, int16_t y, uint16_t color);
void draw_inductor_icon(int16_t x, int16_t y, uint16_t color);
void draw_crystal_icon(int16_t x, int16_t y, uint16_t color);
void draw_fuse_icon(int16_t x, int16_t y, uint16_t color);
void draw_relay_icon(int16_t x, int16_t y, uint16_t color);
void draw_ic_icon(int16_t x, int16_t y, uint16_t color);
void draw_opto_icon(int16_t x, int16_t y, uint16_t color);

// ============================================================================
// UI ICON DRAWING FUNCTIONS
// ============================================================================

void draw_voltage_icon(int16_t x, int16_t y, uint16_t color);
void draw_current_icon(int16_t x, int16_t y, uint16_t color);
void draw_resistance_icon(int16_t x, int16_t y, uint16_t color);
void draw_continuity_icon(int16_t x, int16_t y, uint16_t color);
void draw_diode_test_icon(int16_t x, int16_t y, uint16_t color);
void draw_temp_icon(int16_t x, int16_t y, uint16_t color);
void draw_freq_icon(int16_t x, int16_t y, uint16_t color);

void draw_multimeter_icon(int16_t x, int16_t y, uint16_t color);
void draw_menu_icon(int16_t x, int16_t y, uint16_t color);
void draw_settings_icon(int16_t x, int16_t y, uint16_t color);
void draw_history_icon(int16_t x, int16_t y, uint16_t color);
void draw_about_icon(int16_t x, int16_t y, uint16_t color);
void draw_home_icon(int16_t x, int16_t y, uint16_t color);
void draw_back_icon(int16_t x, int16_t y, uint16_t color);
void draw_up_icon(int16_t x, int16_t y, uint16_t color);
void draw_down_icon(int16_t x, int16_t y, uint16_t color);

void draw_check_icon(int16_t x, int16_t y, uint16_t color);
void draw_x_icon(int16_t x, int16_t y, uint16_t color);
void draw_warning_icon(int16_t x, int16_t y, uint16_t color);
void draw_info_icon(int16_t x, int16_t y, uint16_t color);

// ============================================================================
// UI DRAWING FUNCTIONS
// ============================================================================

void draw_status_bar(const char* left, const char* right);
void draw_menu_bar(uint8_t selected);
void draw_footer();

void draw_button(int16_t x, int16_t y, int16_t w, int16_t h,
                const char* label, uint16_t bg, uint16_t fg, uint8_t style);

void draw_icon_button(int16_t x, int16_t y, uint8_t icon,
                    const char* label, uint16_t bg, uint16_t fg);

void draw_toggle_button(int16_t x, int16_t y, int16_t w, int16_t h,
                       const char* label, bool state, uint16_t on_color, uint16_t off_color);

void draw_slider(int16_t x, int16_t y, int16_t w, int16_t h,
                float value, float min_val, float max_val, uint16_t color);

void draw_progress_bar(int16_t x, int16_t y, int16_t w, int16_t h,
                      uint8_t percent, uint16_t fill_color, uint16_t bg_color);

void draw_gauge_arc(int16_t cx, int16_t cy, int16_t r, float value,
                   float min_val, float max_val, uint16_t color);

void draw_value_display(int16_t x, int16_t y, float value, const char* unit,
                       uint16_t color, uint8_t size);

// ============================================================================
// TEXT DRAWING FUNCTIONS
// ============================================================================

void draw_text_centered(const char* text, int16_t cx, int16_t y,
                         uint16_t color, uint8_t size);

void draw_text_right(const char* text, int16_t rx, int16_t y,
                    uint16_t color, uint8_t size);

void draw_label_value(int16_t x, int16_t y, const char* label, float value,
                     const char* unit, uint16_t label_color, uint16_t value_color);

// ============================================================================
// SHAPE DRAWING FUNCTIONS
// ============================================================================

void draw_rect_gradient(int16_t x, int16_t y, int16_t w, int16_t h,
                       uint16_t color1, uint16_t color2, uint8_t direction);

void draw_rounded_frame(int16_t x, int16_t y, int16_t w, int16_t h,
                      int16_t r, uint16_t fill, uint16_t stroke);

void draw_card(int16_t x, int16_t y, int16_t w, int16_t h,
               const char* title, uint16_t bg_color);

// ============================================================================
// COLOR UTILITY FUNCTIONS
// ============================================================================

uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b);
uint16_t color_lerp(uint16_t c1, uint16_t c2, float t);
uint16_t color_scale(uint16_t color, float scale);
uint16_t color_brighter(uint16_t color, uint8_t steps);
uint16_t color_darker(uint16_t color, uint8_t steps);

uint16_t get_result_color(float deviation, float tolerance);
uint16_t get_status_color(uint8_t status);

// ============================================================================
// ANIMATION FUNCTIONS
// ============================================================================

void draw_loading_dots(int16_t x, int16_t y, uint8_t frame, uint8_t count);
void draw_loading_spinner(int16_t cx, int16_t cy, uint8_t radius, uint8_t frame);
void draw_pulse_animation(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t frame);
void draw_scan_line(int16_t y, uint8_t frame);

// ============================================================================
// FEEDBACK INDICATORS
// ============================================================================

void draw_success_indicator(int16_t x, int16_t y);
void draw_error_indicator(int16_t x, int16_t y);
void draw_warning_indicator(int16_t x, int16_t y);
void draw_info_indicator(int16_t x, int16_t y);

void draw_measurement_result(int16_t x, int16_t y, float value,
                               const char* unit, uint8_t status);

// ============================================================================
// SCREEN FUNCTIONS
// ============================================================================

void clear_screen();
void fill_screen(uint16_t color);
void draw_background();

void draw_splash_screen();
void draw_main_menu();
void draw_measurement_screen();
void draw_settings_screen();
void draw_history_screen();
void draw_about_screen();

// ============================================================================
// TRANSITION EFFECTS
// ============================================================================

void transition_fade_in();
void transition_fade_out();
void transition_slide_left();
void transition_slide_right();
void transition_slide_up();
void transition_slide_down();

// ============================================================================
// TOUCH HIGHLIGHT
// ============================================================================

void draw_touchfeedback(int16_t x, int16_t y);

#endif