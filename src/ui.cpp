// ============================================================================
// Sondvolt v3.x — Interface de Usuario (UI)
// Hardware: ESP32-2432S028R (Cheap Yellow Display)
// ============================================================================
// Arquivo: ui.cpp
// Descricao: Implementacao de todas as interfaces e telas do sistema
// ============================================================================

#include "ui.h"
#include "graphics.h"
#include "buzzer.h"
#include "database.h"

// ============================================================================
// VARIAVEIS GLOBAIS
// ============================================================================

static TFT_eSPI* pTFT = nullptr;

Screen currentScreen = SCREEN_NONE;
MenuData mainMenu;

static HistoryEntry historyBuffer[HISTORY_MAX_ENTRIES];
static uint8_t historyCount = 0;
static uint8_t historyIndex = 0;

// ============================================================================
// INICIALIZACAO
// ============================================================================

void ui_init(TFT_eSPI* tft) {
    pTFT = tft;
    graphicsInit(tft);

    ui_menu_init();

    DBG("UI inicializada");
}

// ============================================================================
// SPLASH SCREEN
// ============================================================================

void ui_draw_splash(void) {
    pTFT->fillScreen(C_BLACK);

    pTFT->setTextColor(C_PRIMARY);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString("Component", SCREEN_W/2, SCREEN_H/2 - 20);
    pTFT->drawString("Tester PRO", SCREEN_W/2, SCREEN_H/2 + 10);

    pTFT->setTextColor(C_TEXT_SECONDARY);
    pTFT->setFreeFont(FMBO);
    pTFT->drawString("v" FW_VERSION, SCREEN_W/2, SCREEN_H/2 + 40);

    pTFT->setTextDatum(MC_DATUM);
    pTFT->setTextSize(1);
    pTFT->drawString(FW_CODENAME, SCREEN_W/2, SCREEN_H - 30);
}

void ui_show_splash_animated(void) {
    currentScreen = SCREEN_SPLASH;

    ui_draw_splash();

    for(uint8_t i = 0; i <= 100; i += 5) {
        uint16_t barWidth = (SCREEN_W - 40) * i / 100;
        pTFT->fillRect(20, SCREEN_H - 20, barWidth, 4);

        delay(30);
    }

    delay(TIME_SPLASH);
}

void ui_splash_set_progress(uint8_t percent, const char* message) {
    uint16_t barWidth = (SCREEN_W - 40) * percent / 100;

    pTFT->fillRect(20, SCREEN_H - 20, barWidth, 4);

    if(message) {
        pTFT->setTextColor(C_BLACK);
        pTFT->setTextDatum(MC_DATUM);
        pTFT->setFreeFont(FMBO);
        pTFT->drawString(message, SCREEN_W/2, SCREEN_H - 30);
    }
}

// ============================================================================
// MENU PRINCIPAL
// ============================================================================

void ui_menu_init(void) {
    mainMenu.itemCount = 8;

    mainMenu.items[0].id = 0;
    mainMenu.items[0].name = "Componentes";
    mainMenu.items[0].icon = "C";
    mainMenu.items[0].enabled = true;

    mainMenu.items[1].id = 1;
    mainMenu.items[1].name = "Multimetro";
    mainMenu.items[1].icon = "V";
    mainMenu.items[1].enabled = true;

    mainMenu.items[2].id = 2;
    mainMenu.items[2].name = "Historico";
    mainMenu.items[2].icon = "H";
    mainMenu.items[2].enabled = true;

    mainMenu.items[3].id = 3;
    mainMenu.items[3].name = "Calibracao";
    mainMenu.items[3].icon = "K";
    mainMenu.items[3].enabled = true;

    mainMenu.items[4].id = 4;
    mainMenu.items[4].name = "Configuracoes";
    mainMenu.items[4].icon = "S";
    mainMenu.items[4].enabled = true;

    mainMenu.items[5].id = 5;
    mainMenu.items[5].name = "Sobre";
    mainMenu.items[5].icon = "i";
    mainMenu.items[5].enabled = true;

    mainMenu.selectedIndex = 0;

    snprintf(mainMenu.title, sizeof(mainMenu.title), "Menu Principal");
}

void ui_menu_show(void) {
    currentScreen = SCREEN_MENU;

    pTFT->fillScreen(C_BACKGROUND);

    ui_draw_header(mainMenu.title);

    uint16_t y = CONTENT_Y + 10;
    uint8_t displayCount = min(mainMenu.itemCount, (uint8_t)6);

    for(uint8_t i = 0; i < displayCount; i++) {
        uint8_t idx = (mainMenu.selectedIndex / 6) * 6 + i;

        if(idx >= mainMenu.itemCount) break;

        bool selected = (idx == mainMenu.selectedIndex);
        ui_draw_list_item(y, idx, mainMenu.items[idx].name, selected);

        y += 30;
    }

    ui_draw_footer("OK", "VOLTAR");
}

void ui_menu_update(void) {
    if(currentScreen != SCREEN_MENU) return;

    ui_menu_show();
}

bool ui_menu_handle_touch(uint16_t x, uint16_t y) {
    if(y > CONTENT_Y && y < CONTENT_Y + mainMenu.itemCount * 30) {
        uint8_t touchedIndex = (y - CONTENT_Y) / 30;

        uint8_t idx = (mainMenu.selectedIndex / 6) * 6 + touchedIndex;

        if(idx < mainMenu.itemCount) {
            mainMenu.selectedIndex = idx;
            ui_menu_show();

            if(buzzer_enabled()) {
                buzzer_beep(BUZZER_FREQ_BTN, 30);
            }

            return true;
        }
    }

    if(y > SCREEN_H - FOOTER_H) {
        bool okPressed = (x > 20 && x < 80);
        bool backPressed = (x > SCREEN_W - 80 && x < SCREEN_W - 20);

        if(okPressed) {
            if(buzzer_enabled()) {
                buzzer_beep(BUZZER_FREQ_OK, 50);
            }
            return true;
        }

        if(backPressed) {
            if(buzzer_enabled()) {
                buzzer_beep(BUZZER_FREQ_WARNING, 30);
            }
        }
    }

    return false;
}

void ui_menu_navigate(int8_t direction) {
    int8_t newIndex = (int8_t)mainMenu.selectedIndex + direction;

    if(newIndex < 0) newIndex = mainMenu.itemCount - 1;
    if(newIndex >= mainMenu.itemCount) newIndex = 0;

    mainMenu.selectedIndex = newIndex;

    ui_menu_show();

    if(buzzer_enabled()) {
        buzzer_beep(BUZZER_FREQ_BTN, 20);
    }
}

// ============================================================================
// TELA DE MEDICAO
// ============================================================================

void ui_measure_show(ComponentType type) {
    currentScreen = SCREEN_MEASURE;

    pTFT->fillScreen(C_BACKGROUND);

    ui_draw_header("Medicao");

    ui_measure_draw_icon(type, 20, CONTENT_Y + 20);

    pTFT->setTextColor(C_PRIMARY);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FONT_HEADER);
    pTFT->drawString("---", SCREEN_W/2, CONTENT_Y + CONTENT_H/2);

    ui_draw_footer("", "MENU");
}

void ui_measure_update(float value, const char* unit, MeasurementStatus status) {
    char valueStr[32];

    if(value < 1.0f) {
        snprintf(valueStr, sizeof(valueStr), "%.3f %s", value, unit);
    } else if(value < 1000.0f) {
        snprintf(valueStr, sizeof(valueStr), "%.2f %s", value, unit);
    } else {
        snprintf(valueStr, sizeof(valueStr), "%.1f %s", value, unit);
    }

    uint16_t bgColor = (status == STATUS_GOOD) ? C_SUCCESS :
                     (status == STATUS_WARNING) ? C_WARNING : C_ERROR;

    pTFT->fillRect(SCREEN_W/2 - 50, CONTENT_Y + CONTENT_H/2 - 20, 100, 40);

    pTFT->setTextColor(C_BLACK);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FONT_HEADER);
    pTFT->drawString(valueStr, SCREEN_W/2, CONTENT_Y + CONTENT_H/2);

    uint8_t iconX = SCREEN_W - 40;
    uint8_t iconY = 50;

    drawStatusIndicator(status, iconX, iconY, 24);
}

void ui_measure_draw_icon(ComponentType type, int16_t x, int16_t y) {
    drawComponentIcon(type, x, y, 40);
}

// ============================================================================
// TELA DE MULTIMETRO
// ============================================================================

void ui_multimeter_show(void) {
    currentScreen = SCREEN_MULTIMETER;

    pTFT->fillScreen(C_BACKGROUND);

    ui_draw_header("Multimetro AC/DC");

    pTFT->setTextColor(C_PRIMARY);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FONT_VALUE);
    pTFT->drawString("0.00 V", SCREEN_W/2, CONTENT_Y + CONTENT_H/2);

    uint8_t buttonY = CONTENT_Y + CONTENT_H - 50;

    ui_draw_button(10, buttonY, 70, 25, "VCC", false);
    ui_draw_button(85, buttonY, 70, 25, "VAC", false);
    ui_draw_button(160, buttonY, 70, 25, "A", false);
    ui_draw_button(235, buttonY, 70, 25, "OHM", false);

    ui_draw_footer("", "MENU");
}

void ui_multimeter_update(float value, const char* unit, uint16_t color) {
    char valueStr[32];
    snprintf(valueStr, sizeof(valueStr), "%.2f %s", value, unit);

    pTFT->fillRect(SCREEN_W/2 - 60, CONTENT_Y + CONTENT_H/2 - 20, 120, 50);

    pTFT->setTextColor(color);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FONT_VALUE);
    pTFT->drawString(valueStr, SCREEN_W/2, CONTENT_Y + CONTENT_H/2);
}

void ui_multimeter_set_mode(uint8_t mode) {
    uint8_t buttonY = CONTENT_Y + CONTENT_H - 50;

    ui_draw_button(10, buttonY, 70, 25, "VCC", (mode == 0));
    ui_draw_button(85, buttonY, 70, 25, "VAC", (mode == 1));
    ui_draw_button(160, buttonY, 70, 25, "A", (mode == 2));
    ui_draw_button(235, buttonY, 70, 25, "OHM", (mode == 3));
}

// ============================================================================
// TELA DE HISTORICO
// ============================================================================

void ui_history_show(void) {
    currentScreen = SCREEN_HISTORY;

    pTFT->fillScreen(C_BACKGROUND);

    ui_draw_header("Historico");

    for(uint8_t i = 0; i < historyCount && i < 8; i++) {
        ui_history_draw_item(i, historyBuffer[i]);
    }

    ui_draw_footer("LIMPAR", "MENU");
}

void ui_history_add(HistoryEntry entry) {
    historyBuffer[historyIndex] = entry;
    historyIndex = (historyIndex + 1) % HISTORY_MAX_ENTRIES;

    if(historyCount < HISTORY_MAX_ENTRIES) {
        historyCount++;
    }

    if(currentScreen == SCREEN_HISTORY) {
        ui_history_show();
    }
}

void ui_history_clear(void) {
    historyCount = 0;
    historyIndex = 0;
    memset(historyBuffer, 0, sizeof(historyBuffer));
}

void ui_history_draw_item(uint8_t index, HistoryEntry entry) {
    uint16_t y = CONTENT_Y + 10 + index * 25;

    pTFT->setTextColor(C_TEXT);
    pTFT->setTextDatum(TL_DATUM);
    pTFT->setFreeFont(FONT_NORMAL);
    pTFT->drawString(entry.componentName, 10, y);

    char valueStr[32];
    snprintf(valueStr, sizeof(valueStr), "%.2f %s", entry.value, entry.unit);

    pTFT->setTextColor(C_PRIMARY);
    pTFT->setTextDatum(TR_DATUM);
    pTFT->drawString(valueStr, SCREEN_W - 10, y);
}

// ============================================================================
// TELA DE CALIBRACAO
// ============================================================================

void ui_calibration_show(void) {
    currentScreen = SCREEN_CALIBRATION;

    pTFT->fillScreen(C_BACKGROUND);

    ui_draw_header("Calibracao");

    pTFT->setTextColor(C_TEXT);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FONT_NORMAL);
    pTFT->drawString("Conecte as pontas de prova", SCREEN_W/2, CONTENT_Y + 40);
    pTFT->drawString("em CURTO (0 ohm)", SCREEN_W/2, CONTENT_Y + 65);

    pTFT->setTextColor(C_PRIMARY);
    pTFT->setFreeFont(FMB);
    pTFT->drawString("INICIAR", SCREEN_W/2, CONTENT_Y + 120);

    ui_draw_progress(0);

    ui_draw_footer("VOLTAR", "");
}

void ui_calibration_update_progress(uint8_t percent) {
    ui_draw_progress(percent);
}

void ui_calibration_show_result(bool success) {
    pTFT->fillRect(SCREEN_W/2 - 50, CONTENT_Y + 140, 100, 40);

    if(success) {
        pTFT->setTextColor(C_SUCCESS);
        pTFT->setTextDatum(MC_DATUM);
        pTFT->setFreeFont(FMB);
        pTFT->drawString("OK", SCREEN_W/2, CONTENT_Y + 160);
    } else {
        pTFT->setTextColor(C_ERROR);
        pTFT->setTextDatum(MC_DATUM);
        pTFT->setFreeFont(FMB);
        pTFT->drawString("ERRO", SCREEN_W/2, CONTENT_Y + 160);
    }
}

// ============================================================================
// TELA DE CONFIGURACOES
// ============================================================================

void ui_settings_show(void) {
    currentScreen = SCREEN_SETTINGS;

    pTFT->fillScreen(C_BACKGROUND);

    ui_draw_header("Configuracoes");

    pTFT->setTextColor(C_TEXT);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FONT_NORMAL);
    pTFT->drawString("Backlight", 80, CONTENT_Y + 40);
    pTFT->drawString("Som", 80, CONTENT_Y + 80);
    pTFT->drawString("Seguranca", 80, CONTENT_Y + 120);
    pTFT->drawString("Auto-Sleep", 80, CONTENT_Y + 160);

    ui_draw_footer("SALVAR", "VOLTAR");
}

void ui_settings_toggle_item(uint8_t index) {
}

void ui_settings_draw_slider(uint8_t index, uint8_t value) {
    uint16_t y = CONTENT_Y + 30 + index * 40;
    uint16_t sliderW = (SCREEN_W - 100 - 40) * value / 100;

    pTFT->fillRect(SCREEN_W - 100, y, sliderW, 10);
    pTFT->drawRect(SCREEN_W - 100, y, SCREEN_W - 100, 10);
}

// ============================================================================
// TELA "SOBRE"
// ============================================================================

void ui_about_show(void) {
    currentScreen = SCREEN_ABOUT;

    pTFT->fillScreen(C_BACKGROUND);

    ui_draw_header("Sobre");

    drawComponentIcon(COMP_UNKNOWN, SCREEN_W/2 - 20, CONTENT_Y + 20, 40);

    pTFT->setTextColor(C_PRIMARY);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString("Component Tester PRO", SCREEN_W/2, CONTENT_Y + 80);

    pTFT->setTextColor(C_TEXT);
    pTFT->setFreeFont(FONT_NORMAL);
    pTFT->drawString("v" FW_VERSION, SCREEN_W/2, CONTENT_Y + 105);
    pTFT->drawString(FW_CODENAME, SCREEN_W/2, CONTENT_Y + 130);

    pTFT->setTextColor(C_TEXT_SECONDARY);
    pTFT->setFreeFont(FMBO);
    pTFT->drawString(FW_AUTHOR, SCREEN_W/2, CONTENT_Y + 160);
    pTFT->drawString(FW_YEAR, SCREEN_W/2, CONTENT_Y + 180);

    pTFT->setTextColor(C_GREY);
    pTFT->setTextSize(1);
    pTFT->drawString(BOARD_NAME, SCREEN_W/2, CONTENT_Y + 205);

    ui_draw_footer("", "MENU");
}

void ui_about_draw_info(void) {
}

// ============================================================================
// TELA DE ERRO
// ============================================================================

void ui_error_show(const char* title, const char* message) {
    currentScreen = SCREEN_ERROR;

    pTFT->fillScreen(C_BACKGROUND);

    ui_draw_header(title);

    ui_error_draw_icon(0);

    pTFT->setTextColor(C_TEXT);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FONT_NORMAL);
    pTFT->drawString(message, SCREEN_W/2, CONTENT_Y + CONTENT_H/2);

    pTFT->fillRoundRect(SCREEN_W/2 - 40, SCREEN_H - 50, 80, 25, 4);
    pTFT->setTextColor(C_TEXT);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString("OK", SCREEN_W/2, SCREEN_H - 38);
}

void ui_error_draw_icon(uint8_t type) {
    uint16_t color = C_ERROR;

    if(type == 1) {
        color = C_WARNING;
    }

    pTFT->fillCircle(SCREEN_W/2, CONTENT_Y + 40, 20);

    pTFT->setTextColor(C_BLACK);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);

    if(type == 0) {
        pTFT->drawString("!", SCREEN_W/2, CONTENT_Y + 40);
    } else {
        pTFT->drawString("X", SCREEN_W/2, CONTENT_Y + 40);
    }
}

bool ui_error_wait_confirm(void) {
    uint16_t x, y;

    while(true) {
        if(pTFT->getTouch(&x, &y)) {
            if(y > SCREEN_H - 60 && y < SCREEN_H - 30) {
                if(x > SCREEN_W/2 - 40 && x < SCREEN_W/2 + 40) {
                    return true;
                }
            }
        }

        delay(50);
    }

    return false;
}

// ============================================================================
// ELEMENTOS COMUNS
// ============================================================================

void ui_draw_header(const char* title) {
    pTFT->fillRect(0, 0, SCREEN_W, STATUS_BAR_H);

    pTFT->setTextColor(C_TEXT);
    pTFT->setTextDatum(ML_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString(title, 4, STATUS_BAR_H/2);

    pTFT->drawLine(0, STATUS_BAR_H - 1, SCREEN_W, STATUS_BAR_H - 1);
}

void ui_draw_footer(const char* left, const char* right) {
    uint16_t y = SCREEN_H - FOOTER_H;

    pTFT->fillRect(0, y, SCREEN_W, FOOTER_H);

    pTFT->drawLine(0, y, SCREEN_W, y);

    pTFT->setTextColor(C_TEXT_SECONDARY);
    pTFT->setTextDatum(ML_DATUM);
    pTFT->setFreeFont(FMBO);

    if(left) {
        pTFT->drawString(left, 10, y + FOOTER_H/2);
    }

    pTFT->setTextDatum(MR_DATUM);
    if(right) {
        pTFT->drawString(right, SCREEN_W - 10, y + FOOTER_H/2);
    }
}

void ui_draw_button(int16_t x, int16_t y, uint16_t w, uint16_t h, const char* label, bool pressed) {
    uint16_t color = pressed ? C_PRIMARY : C_SURFACE;

    pTFT->fillRoundRect(x, y, w, h, 4, color);

    if(!pressed) {
        pTFT->drawRoundRect(x, y, w, h, 4, C_GREY);
    }

    pTFT->setTextColor(pressed ? C_BLACK : C_TEXT);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString(label, x + w/2, y + h/2);
}

void ui_draw_list_item(int16_t y, uint8_t index, const char* text, bool selected) {
    if(selected) {
        pTFT->fillRect(0, y - 10, SCREEN_W, 28);
    }

    pTFT->setTextColor(selected ? C_BLACK : C_TEXT);
    pTFT->setTextDatum(ML_DATUM);
    pTFT->setFreeFont(FONT_NORMAL);
    pTFT->drawString(text, 10, y + 4);
}

void ui_draw_progress(uint8_t percent) {
    uint16_t x = 20;
    uint16_t y = CONTENT_Y + 150;
    uint16_t w = SCREEN_W - 40;
    uint16_t h = 15;

    pTFT->fillRect(x, y, w, h, C_SURFACE);
    pTFT->drawRect(x, y, w, h, C_GREY);

    uint16_t fillW = (w - 4) * percent / 100;
    pTFT->fillRect(x + 2, y + 2, fillW, h - 4);
}

void ui_draw_status_bar(const char* text, uint16_t color) {
    pTFT->fillRect(0, 0, SCREEN_W, STATUS_BAR_H);

    pTFT->setTextColor(color);
    pTFT->setTextDatum(ML_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString(text, 4, STATUS_BAR_H/2);
}

// ============================================================================
// TRATAMENTO DE TOUCH
// ============================================================================

bool ui_touch_is_valid(uint16_t x, uint16_t y) {
    return (x > 0 && x < SCREEN_W && y > 0 && y < SCREEN_H);
}

bool ui_touch_wait(uint16_t* x, uint16_t* y, uint32_t timeoutMs) {
    uint32_t start = millis();

    while(millis() - start < timeoutMs) {
        if(pTFT->getTouch(x, y)) {
            if(ui_touch_is_valid(*x, *y)) {
                return true;
            }
        }

        delay(10);
    }

    return false;
}

bool ui_touch_in_area(uint16_t tx, uint16_t ty, int16_t x, int16_t y, uint16_t w, uint16_t h) {
    return (tx >= x && tx <= x + w && ty >= y && ty <= y + h);
}

// ============================================================================
// ANIMACOES
// ============================================================================

void ui_animate_progress(uint8_t start, uint8_t end, uint16_t durationMs) {
    uint16_t steps = 20;
    uint16_t stepTime = durationMs / steps;

    for(uint16_t i = 0; i <= steps; i++) {
        uint8_t percent = start + (end - start) * i / steps;
        ui_draw_progress(percent);
        delay(stepTime);
    }
}

void ui_animate_fade_in(void) {
    pTFT->setBrightness(0);

    for(uint8_t b = 0; b <= 255; b += 15) {
        pTFT->setBrightness(b);
        delay(20);
    }
}

void ui_animate_fade_out(void) {
    for(int8_t b = 255; b >= 0; b -= 15) {
        pTFT->setBrightness(b);
        delay(20);
    }

    pTFT->setBrightness(0);
}