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
#include "display_globals.h"
#include "display_mutex.h"
#include "buttons.h"


// ============================================================================
// VARIAVEIS GLOBAIS
// ============================================================================



Screen currentScreen = SCREEN_NONE;
MenuData mainMenu;

static HistoryEntry historyBuffer[HISTORY_MAX_ENTRIES];
static uint8_t historyCount = 0;
static uint8_t historyIndex = 0;

// ============================================================================
// INICIALIZACAO
// ============================================================================

void ui_init(void) {
    // Não inicializa display aqui, pois já é feito no main.cpp
    // Evita inicialização dupla e conflitos
    ui_menu_init();
    
    DBG("UI inicializada (display já inicializado em main.cpp)");
}

// ============================================================================
// SPLASH SCREEN
// ============================================================================

void ui_draw_splash(void) {
    tft.fillScreen(C_BLACK);

    tft.setTextColor(C_PRIMARY);
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FMB);
    tft.drawString("Component", SCREEN_W/2, SCREEN_H/2 - 20);
    tft.drawString("Tester PRO", SCREEN_W/2, SCREEN_H/2 + 10);

    tft.setTextColor(C_TEXT_SECONDARY);
    tft.setFreeFont(FM12);
    tft.drawString("v" FW_VERSION, SCREEN_W/2, SCREEN_H/2 + 40);

    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(1);
    tft.drawString(FW_CODENAME, SCREEN_W/2, SCREEN_H - 30);
}

void ui_show_splash_animated(void) {
    currentScreen = SCREEN_SPLASH;

    ui_draw_splash();

    for(uint8_t i = 0; i <= 100; i += 5) {
        uint16_t barWidth = (SCREEN_W - 40) * i / 100;
        tft.fillRect(20, SCREEN_H - 20, barWidth, 4, C_PRIMARY);

        delay(30);
    }

    delay(TIME_SPLASH);
}

void ui_show_splash_basic(void) {
    currentScreen = SCREEN_SPLASH;
    
    // Tenta mostrar o splash normal
    if (tftInitialized) {
        ui_draw_splash();
        delay(TIME_SPLASH);
    } else {
        // Fallback: apenas LEDs e buzzer indicam inicialização
        // LED não disponível neste momento
        buzzer_beep(1000, 100);
        delay(200);
        buzzer_beep(1500, 100);
        // LED não disponível neste momento
        delay(TIME_SPLASH - 300);
    }
}

void ui_show_error_screen(const char* message) {
    if (!tftInitialized) {
        // Fallback sem display: apenas indicadores LED
        // LED não disponível neste momento
        delay(1000);
        // LED não disponível neste momento
        return;
    }
    
    LOCK_TFT();
    tft.fillScreen(C_BLACK);
    
    tft.setTextColor(C_ERROR);
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FMB12);
    tft.drawString("ERRO", SCREEN_W/2, SCREEN_H/2 - 30);
    
    tft.setTextColor(C_TEXT);
    tft.setFreeFont(FM9);
    tft.drawString(message, SCREEN_W/2, SCREEN_H/2 + 10);
    
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(1);
    tft.drawString("Pressione OK para continuar", SCREEN_W/2, SCREEN_H - 30);
    
    UNLOCK_TFT();
    
    // Aguarda usuário pressionar botão
    while (true) { // Removido dependência de botões específicos
        delay(100);
    }
}

void ui_splash_set_progress(uint8_t percent, const char* message) {
    uint16_t barWidth = (SCREEN_W - 40) * percent / 100;

    tft.fillRect(20, SCREEN_H - 20, barWidth, 4, C_PRIMARY);

    if(message) {
        tft.setTextColor(C_BLACK);
        tft.setTextDatum(MC_DATUM);
        tft.setFreeFont(FMBO);
        tft.drawString(message, SCREEN_W/2, SCREEN_H - 30);
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
    LOCK_TFT();
    currentScreen = SCREEN_MENU;

    tft.fillScreen(C_BACKGROUND);

    ui_draw_header(mainMenu.title);

    uint16_t y = CONTENT_Y + 10;
    uint8_t displayCount = min(mainMenu.itemCount, (uint8_t)5);

    for(uint8_t i = 0; i < displayCount; i++) {
        uint8_t idx = (mainMenu.selectedIndex / 5) * 5 + i;

        if(idx >= mainMenu.itemCount) break;

        bool selected = (idx == mainMenu.selectedIndex);
        ui_draw_list_item(y, idx, mainMenu.items[idx].name, selected);

        y += 30;
    }

    ui_draw_footer("OK", "VOLTAR");
    UNLOCK_TFT();

}

void ui_update(void) {
    if (btn_just_pressed(BTN_TOUCH)) {
        TouchPoint p = touch_get_point();
        uint16_t x = p.x;
        uint16_t y = p.y;

        switch(currentScreen) {
            case SCREEN_MENU:
                ui_menu_handle_touch(x, y);
                break;

            case SCREEN_MEASURE:
            case SCREEN_MULTIMETER:
            case SCREEN_HISTORY:
            case SCREEN_ABOUT:
            case SCREEN_CALIBRATION:
                // Se tocar no rodapé (qualquer parte), volta para o menu
                if(y > SCREEN_H - 60) {
                    if(buzzer_enabled()) buzzer_beep(BUZZER_FREQ_BTN, 30);
                    LOG_SERIAL_F("[UI] Voltando ao Menu Principal\n");
                    ui_menu_show();
                }
                break;

            case SCREEN_SETTINGS:
                ui_settings_handle_touch(x, y);
                break;

            case SCREEN_SPLASH:
                if(buzzer_enabled()) buzzer_beep(BUZZER_FREQ_BTN, 30);
                ui_menu_show();
                break;

            case SCREEN_ERROR:
                if(y > SCREEN_H - 60) ui_menu_show();
                break;
            
            default:
                break;
        }
    }
}

bool ui_menu_handle_touch(uint16_t x, uint16_t y) {
    if(y > CONTENT_Y && y < CONTENT_Y + 5 * 30) {
        uint8_t touchedIndex = (y - CONTENT_Y) / 30;

        uint8_t idx = (mainMenu.selectedIndex / 5) * 5 + touchedIndex;

        if(idx < mainMenu.itemCount) {
            mainMenu.selectedIndex = idx;
            ui_menu_show();

            if(buzzer_enabled()) {
                buzzer_beep(BUZZER_FREQ_BTN, 30);
            }

            return true;
        }
    }

    // Deteccao no Rodape (Generosa: 60px do fundo)
    if(y > SCREEN_H - 60) {
        bool okPressed = (x < SCREEN_W / 3);
        bool backPressed = (x > (2 * SCREEN_W) / 3);

        if(okPressed) {
            if(buzzer_enabled()) {
                buzzer_beep(BUZZER_FREQ_OK, 50);
            }
            
            // Acao de OK no menu - Transicao de tela
            uint8_t selectedId = mainMenu.items[mainMenu.selectedIndex].id;
            const char* itemName = mainMenu.items[mainMenu.selectedIndex].name;
            LOG_SERIAL_FMT("[UI] OK selecionado: %s (ID: %d)\n", itemName ? itemName : "NULL", selectedId);
            
            switch(selectedId) {
                case 0: // Componentes
                    ui_measure_show(COMP_UNKNOWN);
                    break;
                case 1: // Multimetro
                    ui_multimeter_show();
                    break;
                case 2: // Historico
                    ui_history_show();
                    break;
                case 3: // Calibracao
                    ui_calibration_show();
                    break;
                case 4: // Configuracoes
                    ui_settings_show();
                    break;
                case 5: // Sobre
                    ui_about_show();
                    break;
            }
            return true;
        }

        if(backPressed) {
            if(buzzer_enabled()) {
                buzzer_beep(BUZZER_FREQ_WARNING, 30);
            }
            LOG_SERIAL_F("[UI] VOLTAR para Splash\n");
            ui_show_splash_basic();
            return true;
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
    LOCK_TFT();
    currentScreen = SCREEN_MEASURE;

    tft.fillScreen(C_BACKGROUND);

    ui_draw_header("Medicao");

    ui_measure_draw_icon(type, 20, CONTENT_Y + 20);

    tft.setTextColor(C_PRIMARY);
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FONT_HEADER);
    tft.drawString("---", SCREEN_W/2, CONTENT_Y + CONTENT_H/2);

    ui_draw_footer("", "MENU");
    UNLOCK_TFT();

}

void ui_measure_update(float value, const char* unit, MeasurementStatus status) {
    LOCK_TFT();
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

    tft.fillRect(SCREEN_W/2 - 50, CONTENT_Y + CONTENT_H/2 - 20, 100, 40, bgColor);

    tft.setTextColor(C_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FONT_HEADER);
    tft.drawString(valueStr, SCREEN_W/2, CONTENT_Y + CONTENT_H/2);

    uint16_t iconX = SCREEN_W - 40;
    uint8_t iconY = 50;

    draw_status_indicator(status, iconX, iconY, 24);
    UNLOCK_TFT();

}

void ui_measure_draw_icon(ComponentType type, int16_t x, int16_t y) {
    IconType icon = ICON_UNKNOWN;
    switch(type) {
        case COMP_RESISTOR: icon = ICON_RESISTOR; break;
        case COMP_CAPACITOR:
        case COMP_CAPACITOR_CERAMIC:
        case COMP_CAPACITOR_ELECTRO:
            icon = ICON_CAPACITOR; break;
        case COMP_DIODE:
        case COMP_LED:
        case COMP_ZENER:
            icon = ICON_DIODE; break;
        case COMP_TRANSISTOR_NPN: icon = ICON_TRANSISTOR_NPN; break;
        case COMP_TRANSISTOR_PNP: icon = ICON_TRANSISTOR_PNP; break;
        case COMP_INDUCTOR: icon = ICON_INDUCTOR; break;
        default: icon = ICON_UNKNOWN; break;
    }
    draw_component_icon(icon, x, y, C_PRIMARY);
}

// ============================================================================
// TELA DE MULTIMETRO
// ============================================================================

void ui_multimeter_show(void) {
    LOCK_TFT();
    currentScreen = SCREEN_MULTIMETER;

    tft.fillScreen(C_BACKGROUND);

    ui_draw_header("Multimetro AC/DC");

    tft.setTextColor(C_PRIMARY);
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FONT_VALUE);
    tft.drawString("0.00 V", SCREEN_W/2, CONTENT_Y + CONTENT_H/2);

    uint8_t buttonY = CONTENT_Y + CONTENT_H - 50;

    ui_draw_button(10, buttonY, 70, 25, "VCC", false);
    ui_draw_button(85, buttonY, 70, 25, "VAC", false);
    ui_draw_button(160, buttonY, 70, 25, "A", false);
    ui_draw_button(235, buttonY, 70, 25, "OHM", false);

    ui_draw_footer("", "MENU");
    UNLOCK_TFT();

}

void ui_multimeter_update(float value, const char* unit, uint16_t color) {
    LOCK_TFT();

    char valueStr[32];
    snprintf(valueStr, sizeof(valueStr), "%.2f %s", value, unit);

    tft.fillRect(SCREEN_W/2 - 60, CONTENT_Y + CONTENT_H/2 - 20, 120, 50, C_SURFACE);

    tft.setTextColor(color);
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FONT_VALUE);
    tft.drawString(valueStr, SCREEN_W/2, CONTENT_Y + CONTENT_H/2);
    UNLOCK_TFT();

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
    LOCK_TFT();

    currentScreen = SCREEN_HISTORY;

    tft.fillScreen(C_BACKGROUND);

    ui_draw_header("Historico");

    for(uint8_t i = 0; i < historyCount && i < 8; i++) {
        ui_history_draw_item(i, historyBuffer[i]);
    }

    ui_draw_footer("LIMPAR", "MENU");
    UNLOCK_TFT();

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

    tft.setTextColor(C_TEXT);
    tft.setTextDatum(TL_DATUM);
    tft.setFreeFont(FONT_NORMAL);
    tft.drawString(entry.componentName, 10, y);

    char valueStr[32];
    snprintf(valueStr, sizeof(valueStr), "%.2f %s", entry.value, entry.unit);

    tft.setTextColor(C_PRIMARY);
    tft.setTextDatum(TR_DATUM);
    tft.drawString(valueStr, SCREEN_W - 10, y);
}

// ============================================================================
// TELA DE CALIBRACAO
// ============================================================================

void ui_calibration_show(void) {
    LOCK_TFT();

    currentScreen = SCREEN_CALIBRATION;

    tft.fillScreen(C_BACKGROUND);

    ui_draw_header("Calibracao");

    tft.setTextColor(C_TEXT);
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FONT_NORMAL);
    tft.drawString("Conecte as pontas de prova", SCREEN_W/2, CONTENT_Y + 40);
    tft.drawString("em CURTO (0 ohm)", SCREEN_W/2, CONTENT_Y + 65);

    tft.setTextColor(C_PRIMARY);
    tft.setFreeFont(FMB);
    tft.drawString("INICIAR", SCREEN_W/2, CONTENT_Y + 120);

    ui_draw_progress(0);

    ui_draw_footer("VOLTAR", "");
    UNLOCK_TFT();

}

void ui_calibration_update_progress(uint8_t percent) {
    ui_draw_progress(percent);
}

void ui_calibration_show_result(bool success) {
    tft.fillRect(SCREEN_W/2 - 50, CONTENT_Y + 140, 100, 40, C_SURFACE);

    if(success) {
        tft.setTextColor(C_SUCCESS);
        tft.setTextDatum(MC_DATUM);
        tft.setFreeFont(FMB);
        tft.drawString("OK", SCREEN_W/2, CONTENT_Y + 160);
    } else {
        tft.setTextColor(C_ERROR);
        tft.setTextDatum(MC_DATUM);
        tft.setFreeFont(FMB);
        tft.drawString("ERRO", SCREEN_W/2, CONTENT_Y + 160);
    }
}

// ============================================================================
// TELA DE CONFIGURACOES
// ============================================================================

void ui_settings_show(void) {
    LOCK_TFT();
    currentScreen = SCREEN_SETTINGS;
    tft.fillScreen(C_BACKGROUND);
    ui_draw_header("Configuracoes");

    tft.setTextColor(C_TEXT);
    tft.setTextDatum(ML_DATUM);
    tft.setFreeFont(FONT_NORMAL);

    uint16_t xText = 20;
    uint16_t xValue = SCREEN_W - 80;

    // Item 0: Backlight
    tft.drawString("Backlight", xText, CONTENT_Y + 30);
    tft.fillRect(xValue, CONTENT_Y + 20, 60, 20, C_SURFACE);
    tft.setTextColor(C_PRIMARY);
    tft.drawNumber(deviceSettings.backlight, xValue + 5, CONTENT_Y + 30);

    // Item 1: Som
    tft.setTextColor(C_TEXT);
    tft.drawString("Som", xText, CONTENT_Y + 70);
    tft.setTextColor(deviceSettings.soundEnabled ? C_SUCCESS : C_ERROR);
    tft.drawString(deviceSettings.soundEnabled ? "ON" : "OFF", xValue, CONTENT_Y + 70);

    // Item 2: Modo Silencioso
    tft.setTextColor(C_TEXT);
    tft.drawString("Silencioso", xText, CONTENT_Y + 110);
    tft.setTextColor(deviceSettings.silentMode ? C_SUCCESS : C_ERROR);
    tft.drawString(deviceSettings.silentMode ? "SIM" : "NAO", xValue, CONTENT_Y + 110);

    // Item 3: Auto-Sleep
    tft.setTextColor(C_TEXT);
    tft.drawString("Auto-Sleep", xText, CONTENT_Y + 150);
    tft.setTextColor(deviceSettings.autoSleep ? C_SUCCESS : C_ERROR);
    tft.drawString(deviceSettings.autoSleep ? "ON" : "OFF", xValue, CONTENT_Y + 150);

    ui_draw_footer("VOLTAR", "SALVAR");
    UNLOCK_TFT();
}

void ui_settings_handle_touch(uint16_t x, uint16_t y) {
    if (y > SCREEN_H - 60) {
        // Lado esquerdo: VOLTAR
        if (x < SCREEN_W / 3) {
            if(buzzer_enabled()) buzzer_beep(BUZZER_FREQ_BTN, 30);
            ui_menu_show();
        }
        // Lado direito: SALVAR
        else if (x > (2 * SCREEN_W) / 3) {
            if(buzzer_enabled()) buzzer_beep(BUZZER_FREQ_OK, 50);
            LOG_SERIAL_F("[UI] Configuracoes salvas\n");
            ui_menu_show();
        }
        return;
    }

    // Processa itens da lista
    if (y > CONTENT_Y && y < SCREEN_H - 60) {
        uint8_t item = (y - CONTENT_Y) / 40;
        
        if(buzzer_enabled()) buzzer_beep(BUZZER_FREQ_BTN, 30);
        
        switch(item) {
            case 0: // Backlight
                deviceSettings.backlight += 64; 
                if(deviceSettings.backlight > 255) deviceSettings.backlight = 64;
                analogWrite(TFT_BL, deviceSettings.backlight);
                break;
            case 1: // Som
                deviceSettings.soundEnabled = !deviceSettings.soundEnabled;
                break;
            case 2: // Silencioso
                deviceSettings.silentMode = !deviceSettings.silentMode;
                break;
            case 3: // Auto-Sleep
                deviceSettings.autoSleep = !deviceSettings.autoSleep;
                break;
        }
        ui_settings_show(); // Redesenha
    }
}
void ui_settings_draw_slider(uint8_t index, uint8_t value) {
    uint16_t y = CONTENT_Y + 30 + index * 40;
    uint16_t sliderW = (SCREEN_W - 100 - 40) * value / 100;

    tft.fillRect(SCREEN_W - 100, y, sliderW, 10, C_PRIMARY);
    tft.drawRect(SCREEN_W - 100, y, SCREEN_W - 100, 10, C_GREY);
}

// ============================================================================
// TELA "SOBRE"
// ============================================================================

void ui_about_show(void) {
    LOCK_TFT();

    currentScreen = SCREEN_ABOUT;

    tft.fillScreen(C_BACKGROUND);

    ui_draw_header("Sobre");

    draw_component_icon(ICON_ABOUT, SCREEN_W/2 - 20, CONTENT_Y + 20, C_PRIMARY);

    tft.setTextColor(C_PRIMARY);
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FMB);
    tft.drawString("Component Tester PRO", SCREEN_W/2, CONTENT_Y + 80);

    tft.setTextColor(C_TEXT);
    tft.setFreeFont(FONT_NORMAL);
    tft.drawString("v" FW_VERSION, SCREEN_W/2, CONTENT_Y + 105);
    tft.drawString(FW_CODENAME, SCREEN_W/2, CONTENT_Y + 130);

    tft.setTextColor(C_TEXT_SECONDARY);
    tft.setFreeFont(FMBO);
    tft.drawString(FW_AUTHOR, SCREEN_W/2, CONTENT_Y + 160);
    tft.drawString(FW_YEAR, SCREEN_W/2, CONTENT_Y + 180);

    tft.setTextColor(C_GREY);
    tft.setTextSize(1);
    tft.drawString(BOARD_NAME, SCREEN_W/2, CONTENT_Y + 205);

    ui_draw_footer("", "MENU");
    UNLOCK_TFT();

}

void ui_about_draw_info(void) {
}

// ============================================================================
// TELA DE ERRO
// ============================================================================

void ui_error_show(const char* title, const char* message) {
    LOCK_TFT();

    currentScreen = SCREEN_ERROR;

    tft.fillScreen(C_BACKGROUND);

    ui_draw_header(title);

    ui_error_draw_icon(0);

    tft.setTextColor(C_TEXT);
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FONT_NORMAL);
    tft.drawString(message, SCREEN_W/2, CONTENT_Y + CONTENT_H/2);

    tft.fillRoundRect(SCREEN_W/2 - 40, SCREEN_H - 50, 80, 25, 4, C_PRIMARY);
    tft.setTextColor(C_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FMB);
    tft.drawString("OK", SCREEN_W/2, SCREEN_H - 38);
    UNLOCK_TFT();

}

void ui_error_draw_icon(uint8_t type) {
    uint16_t color = C_ERROR;

    if(type == 1) {
        color = C_WARNING;
    }

    tft.fillCircle(SCREEN_W/2, CONTENT_Y + 40, 20, color);

    tft.setTextColor(C_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FMB);

    if(type == 0) {
        tft.drawString("!", SCREEN_W/2, CONTENT_Y + 40);
    } else {
        tft.drawString("X", SCREEN_W/2, CONTENT_Y + 40);
    }
}

bool ui_error_wait_confirm(void) {
    uint16_t x, y;

    while(true) {
        if(btn_just_pressed(BTN_TOUCH)) {
            TouchPoint p = touch_get_point();
            uint16_t x = p.x;
            uint16_t y = p.y;
            
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
    tft.fillRect(0, 0, SCREEN_W, STATUS_BAR_H, C_SURFACE);

    tft.setTextColor(C_TEXT);
    tft.setTextDatum(ML_DATUM);
    tft.setFreeFont(FMB);
    tft.drawString(title, 4, STATUS_BAR_H/2);

    tft.drawLine(0, STATUS_BAR_H - 1, SCREEN_W, STATUS_BAR_H - 1, C_GREY);
}

void ui_draw_footer(const char* left, const char* right) {
    uint16_t y = SCREEN_H - FOOTER_H;

    tft.fillRect(0, y, SCREEN_W, FOOTER_H, C_SURFACE);

    tft.drawLine(0, y, SCREEN_W, y, C_GREY);

    tft.setTextColor(C_TEXT_SECONDARY);
    tft.setTextDatum(ML_DATUM);
    tft.setFreeFont(FMBO);

    if(left) {
        tft.drawString(left, 10, y + FOOTER_H/2);
    }

    tft.setTextDatum(MR_DATUM);
    if(right) {
        tft.drawString(right, SCREEN_W - 10, y + FOOTER_H/2);
    }
}

void ui_draw_button(int16_t x, int16_t y, uint16_t w, uint16_t h, const char* label, bool pressed) {
    uint16_t color = pressed ? C_PRIMARY : C_SURFACE;

    tft.fillRoundRect(x, y, w, h, 4, color);

    if(!pressed) {
        tft.drawRoundRect(x, y, w, h, 4, C_GREY);
    }

    tft.setTextColor(pressed ? C_BLACK : C_TEXT);
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FMB);
    tft.drawString(label, x + w/2, y + h/2);
}

void ui_draw_list_item(int16_t y, uint8_t index, const char* text, bool selected) {
    if(selected) {
        tft.fillRect(0, y - 10, SCREEN_W, 28, C_PRIMARY);
    }

    tft.setTextColor(selected ? C_BLACK : C_TEXT);
    tft.setTextDatum(ML_DATUM);
    tft.setFreeFont(FONT_NORMAL);
    tft.drawString(text, 10, y + 4);
}

void ui_draw_progress(uint8_t percent) {
    uint16_t x = 20;
    uint16_t y = CONTENT_Y + 150;
    uint16_t w = SCREEN_W - 40;
    uint16_t h = 15;

    tft.fillRect(x, y, w, h, C_SURFACE);
    tft.drawRect(x, y, w, h, C_GREY);

    uint16_t fillW = (w - 4) * percent / 100;
    tft.fillRect(x + 2, y + 2, fillW, h - 4, C_PRIMARY);
}

void ui_draw_status_bar(const char* text, uint16_t color) {
    tft.fillRect(0, 0, SCREEN_W, STATUS_BAR_H, C_SURFACE);

    tft.setTextColor(color);
    tft.setTextDatum(ML_DATUM);
    tft.setFreeFont(FMB);
    tft.drawString(text, 4, STATUS_BAR_H/2);
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
        if(btn_just_pressed(BTN_TOUCH)) {
            TouchPoint tp = touch_get_point();
            *x = tp.x;
            *y = tp.y;
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
    digitalWrite(PIN_TFT_BL, HIGH);
}

void ui_animate_fade_out(void) {
    digitalWrite(PIN_TFT_BL, LOW);
}
