// ============================================================================
// Sondvolt v3.x — Interface de Usuario (UI)
// Hardware: ESP32-2432S028R (Cheap Yellow Display)
// ============================================================================
// Arquivo: ui.h
// Descricao: Prototipos de funcoes de interface (telas, menus, etc)
// ============================================================================

#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include "config.h"
#include "pins.h"
#include "globals.h"

// ============================================================================
// TELAS DO SISTEMA
// ============================================================================

enum Screen {
    SCREEN_NONE = 0,
    SCREEN_SPLASH = 1,
    SCREEN_MENU = 2,
    SCREEN_MEASURE = 3,
    SCREEN_MULTIMETER = 4,
    SCREEN_HISTORY = 5,
    SCREEN_CALIBRATION = 6,
    SCREEN_SETTINGS = 7,
    SCREEN_ABOUT = 8,
    SCREEN_ERROR = 9,
    SCREEN_SAFETY = 10
};

// ============================================================================
// ESTRUTURAS DE DADOS
// ============================================================================

typedef struct {
    uint8_t id;
    const char* name;
    const char* icon;
    bool enabled;
} MenuItem;

typedef struct {
    char title[32];
    uint8_t itemCount;
    uint8_t selectedIndex;
    MenuItem items[12];
} MenuData;

typedef struct {
    char componentName[32];
    float value;
    const char* unit;
    uint8_t status;
    unsigned long timestamp;
} HistoryEntry;

// ============================================================================
// VARIAVEIS EXTERNAS
// ============================================================================

extern Screen currentScreen;
extern MenuData mainMenu;

// ============================================================================
// SPLASH SCREEN
// ============================================================================

void ui_draw_splash(void);
void ui_show_splash_animated(void);
void ui_splash_set_progress(uint8_t percent, const char* message);

// ============================================================================
// MENU PRINCIPAL
// ============================================================================

void ui_menu_init(void);
void ui_menu_show(void);
void ui_menu_update(void);
bool ui_menu_handle_touch(uint16_t x, uint16_t y);
void ui_menu_navigate(int8_t direction);

// ============================================================================
// TELA DE MEDICAO
// ============================================================================

void ui_measure_show(ComponentType type);
void ui_measure_update(float value, const char* unit, MeasurementStatus status);
void ui_measure_draw_icon(ComponentType type, int16_t x, int16_t y);

// ============================================================================
// TELA DE MULTIMETRO
// ============================================================================

void ui_multimeter_show(void);
void ui_multimeter_update(float value, const char* unit, uint16_t color);
void ui_multimeter_set_mode(uint8_t mode);

// ============================================================================
// TELA DE HISTORICO
// ============================================================================

void ui_history_show(void);
void ui_history_add(HistoryEntry entry);
void ui_history_clear(void);
void ui_history_draw_item(uint8_t index, HistoryEntry entry);

// ============================================================================
// TELA DE CALIBRACAO
// ============================================================================

void ui_calibration_show(void);
void ui_calibration_update_progress(uint8_t percent);
void ui_calibration_show_result(bool success);

// ============================================================================
// TELA DE CONFIGURACOES
// ============================================================================

void ui_settings_show(void);
void ui_settings_toggle_item(uint8_t index);
void ui_settings_draw_slider(uint8_t index, uint8_t value);

// ============================================================================
// TELA "SOBRE"
// ============================================================================

void ui_about_show(void);
void ui_about_draw_info(void);

// ============================================================================
// TELA DE ERRO
// ============================================================================

void ui_error_show(const char* title, const char* message);
void ui_error_draw_icon(uint8_t type);
bool ui_error_wait_confirm(void);

// ============================================================================
// ELEMENTOS COMUNS
// ============================================================================

void ui_draw_header(const char* title);
void ui_draw_footer(const char* left, const char* right);
void ui_draw_button(int16_t x, int16_t y, uint16_t w, uint16_t h, const char* label, bool pressed);
void ui_draw_list_item(int16_t y, uint8_t index, const char* text, bool selected);
void ui_draw_progress(uint8_t percent);
void ui_draw_status_bar(const char* text, uint16_t color);

// ============================================================================
// TRATAMENTO DE TOUCH
// ============================================================================

bool ui_touch_is_valid(uint16_t x, uint16_t y);
bool ui_touch_wait(uint16_t* x, uint16_t* y, uint32_t timeoutMs);
bool ui_touch_in_area(uint16_t tx, uint16_t ty, int16_t x, int16_t y, uint16_t w, uint16_t h);

// ============================================================================
// ANIMACOES
// ============================================================================

void ui_animate_progress(uint8_t start, uint8_t end, uint16_t durationMs);
void ui_animate_fade_in(void);
void ui_animate_fade_out(void);

// ============================================================================
// INICIALIZACAO
// ============================================================================

void ui_init(TFT_eSPI* tft);

#endif // UI_H