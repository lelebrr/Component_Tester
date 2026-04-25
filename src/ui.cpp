// ============================================================================
// Sondvolt v3.2 — Gerenciador de UI
// ============================================================================

#include "ui.h"
#include "graphics.h"
#include "menu.h"
#include "fonts.h"
#include "display_globals.h" 
#include "display_mutex.h"   // Adicionado para LOCK/UNLOCK
#include "globals.h"         
#include <Arduino.h>

static AppState lastKnownState = STATE_SPLASH;
static bool isMenuMode = true;
static void draw_instrument_content(); // Protótipo interno
static void draw_about_screen();
static void draw_settings_screen();
static void draw_history_screen();

// Controle de Toque (Tap vs Swipe)
static uint32_t touchStartTime = 0;
static int16_t touchStartX = 0;
static int16_t touchStartY = 0;
static int16_t lastTouchX = 0; 
static int16_t lastTouchY = 0;
static bool isTouching = false;
static bool hasMoved = false;

void ui_init() {
    menu_init();
    graphics_draw_splash();
    delay(2000); // Splash por 2s
    currentAppState = STATE_MENU;
}

void ui_update() {
    // Detecta mudança de estado para redesenho total
    if (currentAppState != lastKnownState) {
        LOCK_TFT();
        tft.fillScreen(TFT_BLACK);
        UNLOCK_TFT();
        
        switch (currentAppState) {
            case STATE_MENU:
                isMenuMode = true;
                menu_draw();
                break;
            case STATE_ABOUT:
                isMenuMode = false;
                graphics_draw_header("SOBRE O SISTEMA");
                graphics_draw_back_button();
                draw_about_screen();
                break;
            case STATE_SETTINGS:
                isMenuMode = false;
                graphics_draw_header("CONFIGURACOES");
                graphics_draw_back_button();
                draw_settings_screen();
                break;
            case STATE_HISTORY:
                isMenuMode = false;
                graphics_draw_header("HISTORICO");
                graphics_draw_back_button();
                draw_history_screen();
                break;
            default:
                isMenuMode = false;
                graphics_draw_header("MODO INSTRUMENTO");
                graphics_draw_back_button();
                break;
        }
        lastKnownState = currentAppState;
    }

    // Atualização contínua da tela ativa
    if (isMenuMode) {
        menu_draw();
    } 
    else {
        // DESENHAR CONTEÚDO DO INSTRUMENTO AQUI
        draw_instrument_content();
    }
}

// Função para desenhar o conteúdo dinâmico (Valores, etc)
static void draw_instrument_content() {
    LOCK_TFT();
    char valBuf[32];
    const char* title = "INSTRUMENTO";
    uint16_t color = COLOR_PRIMARY;

    switch (currentAppState) {
        case STATE_MEASURE_RESISTOR:
            title = "RESISTOR";
            sprintf(valBuf, "%.2f Ohms", lastResistance);
            break;
        case STATE_MEASURE_CAPACITOR:
            title = "CAPACITOR";
            sprintf(valBuf, "%.2f uF", lastCapacitance);
            break;
        case STATE_MEASURE_DIODE:
            title = "DIODO";
            sprintf(valBuf, "%.3f V", lastVoltage);
            break;
        case STATE_MULTIMETER:
            title = "MULTIMETRO";
            sprintf(valBuf, "%.2f V", lastVoltage);
            break;
        case STATE_THERMAL_PROBE:
            title = "TERMOMETRO";
            sprintf(valBuf, "%.1f C", lastTemperature);
            break;
        default:
            title = "MEDINDO...";
            sprintf(valBuf, "---");
            break;
    }

    graphics_draw_header(title);

    // Área Central de Leitura
    tft.fillRect(10, 60, 300, 100, COLOR_SURFACE);
    tft.drawRect(10, 60, 300, 100, color);
    
    // Valor Grande
    int16_t textLen = strlen(valBuf) * 6 * 4;
    draw_text_5x7(tft, 160 - textLen/2, 110, valBuf, color, 4);
    
    // Rodapé de Status
    tft.fillRect(0, 210, 320, 30, COLOR_SURFACE);
    draw_text_5x7(tft, 20, 220, "Status: ATIVO | Estavel", C_TEXT_SECONDARY, 1);
    
    UNLOCK_TFT();
}

static void draw_about_screen() {
    LOCK_TFT();
    tft.fillRect(10, 50, 300, 150, COLOR_SURFACE);
    draw_text_5x7(tft, 20, 70, "SONDVOLT v3.2 PRO", COLOR_PRIMARY, 2);
    draw_text_5x7(tft, 20, 100, "Hardware: ESP32-CYD", TFT_WHITE, 1);
    draw_text_5x7(tft, 20, 120, "Display: ILI9341 320x240", TFT_WHITE, 1);
    draw_text_5x7(tft, 20, 140, "Desenvolvido para", TFT_WHITE, 1);
    draw_text_5x7(tft, 20, 155, "Alta Precisao", COLOR_PRIMARY, 1);
    UNLOCK_TFT();
}

static void draw_settings_screen() {
    LOCK_TFT();
    tft.fillRect(10, 50, 300, 150, COLOR_SURFACE);
    draw_text_5x7(tft, 20, 70, "1. Brilho: 80%", TFT_WHITE, 2);
    draw_text_5x7(tft, 20, 100, "2. Som: LIGADO", TFT_WHITE, 2);
    draw_text_5x7(tft, 20, 130, "3. Tema: DARK", TFT_WHITE, 2);
    UNLOCK_TFT();
}

static void draw_history_screen() {
    LOCK_TFT();
    tft.fillRect(10, 50, 300, 150, COLOR_SURFACE);
    draw_text_5x7(tft, 20, 70, "Sem registros recentes", C_TEXT_SECONDARY, 1);
    UNLOCK_TFT();
}

bool ui_handle_touch(uint16_t x, uint16_t y) {
    // Botão Voltar (Sempre disponível fora do menu)
    if (!isMenuMode) {
        if (x < 60 && y < 50) {
            currentAppState = STATE_MENU;
            return true;
        }
    }

    // Lógica de Menu (Tap vs Swipe)
    if (isMenuMode) {
        if (!isTouching) {
            touchStartTime = millis();
            touchStartX = x;
            touchStartY = y;
            lastTouchX = x;
            lastTouchY = y;
            isTouching = true;
            hasMoved = false;
        } else {
            if (abs((int16_t)x - touchStartX) > 15 || abs((int16_t)y - touchStartY) > 15) {
                hasMoved = true;
            }
            lastTouchX = x;
            lastTouchY = y;
        }
        return true;
    }
    
    return false;
}

void ui_reset_touch_state() {
    if (isMenuMode && isTouching) {
        uint32_t duration = millis() - touchStartTime;
        int16_t deltaX = lastTouchX - touchStartX;

        if (abs(deltaX) > 50 && duration < 500) {
            // Swipe detectado
            if (deltaX < -50) menu_scroll(1);   // Swipe Left -> Next
            else if (deltaX > 50) menu_scroll(-1); // Swipe Right -> Prev
        } 
        else if (!hasMoved) {
            // Apenas Tap - Passa as coordenadas corretas para o menu
            menu_handle_touch(lastTouchX, lastTouchY);
        }
    }
    isTouching = false;
    hasMoved = false;
}

void ui_calibration_update_progress(uint8_t progress, const char* msg) {
    LOCK_TFT();
    tft.fillRect(20, 160, 280, 40, COLOR_SURFACE);
    char buf[32];
    sprintf(buf, "Calibrando: %d%%", progress);
    draw_text_5x7(tft, 30, 165, buf, TFT_WHITE, 1);
    draw_text_5x7(tft, 30, 180, msg, COLOR_PRIMARY, 1);
    UNLOCK_TFT();
}

void ui_calibration_show_result(bool success, const char* msg) {
    LOCK_TFT();
    tft.fillRect(20, 100, 280, 80, success ? COLOR_GOOD : COLOR_BAD);
    draw_text_5x7(tft, 40, 120, success ? "SUCESSO!" : "FALHA!", TFT_WHITE, 2);
    draw_text_5x7(tft, 40, 150, msg, TFT_WHITE, 1);
    UNLOCK_TFT();
    delay(3000);
}
