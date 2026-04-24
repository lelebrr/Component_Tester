// ============================================================================
// Component Tester PRO v3.0 — Sistema de Menu e Navegação (Implementação)
// Descrição: Implementação do menu principal com grid cards
// ============================================================================

#include "menu.h"
#include "globals.h"
#include "graphics.h"
#include "buttons.h"
#include "buzzer.h"
#include "leds.h"
#include "logger.h"
#include "thermal.h"

// ============================================================================
// MENU CARD — DEFINIÇÕES DO MENU PRINCIPAL
// ============================================================================
const MenuCard MAIN_MENU_CARDS[] = {
    // --- Modo Component Tester ---
    { "Resistor",   STATE_MEASURE_RESISTOR,   ICON_RESISTOR,   UI_COLORS::RESISTOR,    MENU_FLAG_NONE },
    { "Capacitor",  STATE_MEASURE_CAPACITOR,  ICON_CAPACITOR, UI_COLORS::CAPACITOR,   MENU_FLAG_NONE },
    { "Diodo",     STATE_MEASURE_DIODE,     ICON_DIODE,     UI_COLORS::DIODE,       MENU_FLAG_NONE },
    { "Transistor", STATE_MEASURE_TRANSISTOR, ICON_TRANSISTOR,UI_COLORS::TRANSISTOR, MENU_FLAG_NONE },
    { "Indutor",   STATE_MEASURE_INDUCTOR,  ICON_INDUCTOR, UI_COLORS::PRIMARY,    MENU_FLAG_NONE },
    { "IC/CI",     STATE_MEASURE_IC,       ICON_IC,       UI_COLORS::IC,        MENU_FLAG_NONE },

    // --- Modo Multímetro ---
    { "Multimetro", STATE_MULTIMETER,        ICON_MULTIMETER,UI_COLORS::ACCENT,     MENU_FLAG_NONE },

    // --- Ferramentas ---
    { "Temperatura", STATE_THERMAL_PROBE,   ICON_THERMOMETER,UI_COLORS::DANGER, MENU_FLAG_NONE },
    { "Scanner",   STATE_SCANNER,           ICON_SCANNER,   UI_COLORS::SUCCESS,    MENU_FLAG_NONE },

    // --- Configurações ---
    { "Calibrar",  STATE_CALIBRATION,        ICON_CALIBRATE, UI_COLORS::WARNING,    MENU_FLAG_NONE },
    { "Historico", STATE_HISTORY,          ICON_HISTORY,   UI_COLORS::TEXT_DIM,   MENU_FLAG_NONE },
    { "Estatisticas", STATE_STATS,          ICON_STATS,    UI_COLORS::TEXT_DIM,   MENU_FLAG_NONE },
    { "Config",   STATE_SETTINGS,           ICON_SETTINGS,  UI_COLORS::TEXT_DIM,   MENU_FLAG_NONE },
    { "Sobre",    STATE_ABOUT,              ICON_ABOUT,    UI_COLORS::TEXT_DIM,   MENU_FLAG_NONE },
    { "Teste",    STATE_MEASURE_GENERIC,   ICON_TEST,     UI_COLORS::UNKNOWN,   MENU_FLAG_NONE },
};
const uint8_t MAIN_MENU_COUNT = sizeof(MAIN_MENU_CARDS) / sizeof(MenuCard);

// ============================================================================
// MENU CARD — DEFINIÇÕES DO MENU DE MEDIÇÃO
// ============================================================================
const MenuCard MEASURE_MENU_CARDS[] = {
    { "Resistor",   STATE_MEASURE_RESISTOR,  ICON_RESISTOR,   UI_COLORS::RESISTOR,    MENU_FLAG_NONE },
    { "Capacitor", STATE_MEASURE_CAPACITOR, ICON_CAPACITOR, UI_COLORS::CAPACITOR,  MENU_FLAG_NONE },
    { "Diodo",    STATE_MEASURE_DIODE,       ICON_DIODE,     UI_COLORS::DIODE,      MENU_FLAG_NONE },
    { "Transistor", STATE_MEASURE_TRANSISTOR, ICON_TRANSISTOR, UI_COLORS::TRANSISTOR, MENU_FLAG_NONE },
    { "Indutor",  STATE_MEASURE_INDUCTOR,   ICON_INDUCTOR,  UI_COLORS::PRIMARY,    MENU_FLAG_NONE },
    { "IC/CI",    STATE_MEASURE_IC,         ICON_IC,        UI_COLORS::IC,        MENU_FLAG_NONE },
};
const uint8_t MEASURE_MENU_COUNT = sizeof(MEASURE_MENU_CARDS) / sizeof(MenuCard);

// ============================================================================
// MENU CARD — DEFINIÇÕES DO MENU DE CONFIGURAÇÃO
// ============================================================================
const MenuCard SETTINGS_MENU_CARDS[] = {
    { "Brilho",        100, ICON_BRIGHTNESS, UI_COLORS::WARNING,  MENU_FLAG_NONE },
    { "Som",           101, ICON_SOUND,      UI_COLORS::SUCCESS,  MENU_FLAG_NONE },
    { "Modo Escuro",   102, ICON_DARK,       UI_COLORS::PRIMARY,   MENU_FLAG_NONE },
    { "Auto-Dormir",   103, ICON_SLEEP,      UI_COLORS::TEXT_DIM,  MENU_FLAG_NONE },
    { "Temperatura",  104, ICON_THERMOMETER, UI_COLORS::DANGER,   MENU_FLAG_NONE },
    { "ZMPT Calib",    105, ICON_CALIBRATE,   UI_COLORS::WARNING,  MENU_FLAG_NONE },
    { "Reset Stats",   106, ICON_RESET,      UI_COLORS::DANGER,   MENU_FLAG_NONE },
    { "Factory Reset", 107, ICON_DELETE,    UI_COLORS::DANGER,  MENU_FLAG_NONE },
};
const uint8_t SETTINGS_MENU_COUNT = sizeof(SETTINGS_MENU_CARDS) / sizeof(MenuCard);

// ============================================================================
// VARIÁVEIS DO MENU
// ============================================================================
static int currentCardIndex = 0;
static int totalCardsPerScreen = 6;
static int currentPage = 0;
static int totalPages = 1;
static bool menuNeedsRedraw = true;
static unsigned long touchLastX = 0;
static unsigned long touchLastY = 0;

// Dimensões dos cards
static int cardW, cardH, gridStartX, gridStartY;
static int gridCols = 2, gridRows = 3;

// ============================================================================
// INICIALIZAÇÃO
// ============================================================================
void menu_init() {
    // Calcular dimensões dos cards para o grid 2x3
    int margin = 10;
    int padding = 8;
    int gap = 8;

    int contentW = 320;
    int contentH = 240 - STATUS_BAR_HEIGHT - FOOTER_HEIGHT;
    int contentY = STATUS_BAR_HEIGHT;

    cardW = (contentW - margin * 2 - gap) / gridCols;
    cardH = (contentH - padding * 2 - gap * (gridRows - 1)) / gridRows;
    gridStartX = margin;
    gridStartY = contentY + padding;
    totalCardsPerScreen = gridCols * gridRows;

    totalPages = (MAIN_MENU_COUNT + totalCardsPerScreen - 1) / totalCardsPerScreen;

    currentCardIndex = 0;
    currentPage = 0;
    menuNeedsRedraw = true;

    DBG_PRINTLN("[MENU] Menu inicializado");
}

// ============================================================================
// LOOP PRINCIPAL DO MENU
// ============================================================================
void menu_handle() {
    // Verifica se precisa redesenhar
    if (menuNeedsRedraw) {
        menu_draw();
        menuNeedsRedraw = false;
    }

    // Processa botões físicos
    if (btn_just_pressed(BTN_UP) || btn_just_pressed(BTN_LEFT)) {
        menu_navigate(-1);
    } else if (btn_just_pressed(BTN_DOWN) || btn_just_pressed(BTN_RIGHT)) {
        menu_navigate(1);
    } else if (btn_just_pressed(BTN_CENTER) || btn_just_pressed(BTN_OK)) {
        menu_select();
    } else if (btn_just_pressed(BTN_BACK)) {
        // Opcional: mostrar tela sobre ou não fazer nada
    }

    // Processa toque
    int touchedCard = menu_process_touch();
    if (touchedCard >= 0) {
        buzzer_beep(100);
        menu_updateSelection(touchedCard);
        delay(150);
        menu_select();
    }
}

// ============================================================================
// DESENHAR MENU
// ============================================================================
void menu_draw() {
    tft.fillScreen(UI_COLORS::BACKGROUND);

    // Barra de status
    draw_status_bar();

    // Indicador de página (se houver mais de uma página)
    if (totalPages > 1) {
        draw_page_indicator(currentPage, totalPages);
    }

    // Desenha os cards visíveis
    int cardIndex = currentPage * totalCardsPerScreen;
    int cardsOnThisPage = min(totalCardsPerScreen,
                           MAIN_MENU_COUNT - cardIndex);

    for (int i = 0; i < cardsOnThisPage; i++) {
        int idx = cardIndex + i;
        int col = i % gridCols;
        int row = i / gridCols;

        int x = gridStartX + col * (cardW + gap);
        int y = gridStartY + row * (cardH + gap);

        bool selected = (idx == currentCardIndex);
        menu_draw_card(x, y, cardW, cardH,
                     &MAIN_MENU_CARDS[idx], selected);
    }

    // Rodapé
    draw_footer();
}

void menu_refresh() {
    menuNeedsRedraw = true;
}

// ============================================================================
// DESENHAR CARD DO MENU
// ============================================================================
void menu_draw_card(int x, int y, int w, int h,
                   const MenuCard* card, bool selected) {
    uint16_t bgColor = selected ? shiftColor(card->color, 80) : UI_COLORS::CARD_BG;
    uint16_t borderColor = selected ? card->color : UI_COLORS::BORDER;

    // Fundo do card
    tft.fillRoundRect(x, y, w, h, 8, bgColor);

    // Borda
    if (selected) {
        tft.drawRoundRect(x, y, w, h, 8, borderColor);
        tft.drawRoundRect(x + 2, y + 2, w - 4, h - 4, 6,
                          shiftColor(borderColor, -30));
    } else {
        tft.drawRoundRect(x, y, w, h, 8, borderColor);
    }

    // Ícone
    int iconX = x + w / 2 - 20;
    int iconY = y + 12;
    draw_component_icon(card->iconType, iconX, iconY, card->color);

    // Label
    tft.setTextColor(selected ? UI_COLORS::TEXT : UI_COLORS::TEXT_DIM);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(x + w / 2, y + h - 14);
    tft.print(card->label);
    tft.setTextDatum(TL_DATUM);

    // Badge de alerta
    if (card->flags & MENU_FLAG_WARNING) {
        draw_warning_badge(x + w - 14, y + 4);
    }
}

// ============================================================================
// NAVEGAÇÃO
// ============================================================================
void menu_updateSelection(int index) {
    if (index == currentCardIndex) return;
    currentCardIndex = index;
    currentPage = currentCardIndex / totalCardsPerScreen;
    menuNeedsRedraw = true;
}

void menu_navigate(int direction) {
    buzzer_beep(80);

    int newIndex = currentCardIndex + direction;
    if (newIndex < 0) {
        newIndex = MAIN_MENU_COUNT - 1;
    } else if (newIndex >= (int)MAIN_MENU_COUNT) {
        newIndex = 0;
    }

    currentCardIndex = newIndex;
    int newPage = currentCardIndex / totalCardsPerScreen;

    if (newPage != currentPage) {
        currentPage = newPage;
        menuNeedsRedraw = true;
    } else {
        // Apenas atualiza o highlight sem redesenhar tudo
        menuNeedsRedraw = true;
    }

    lastActivityMs = millis();
}

void menu_select() {
    buzzer_beep(150);
    previousAppState = currentAppState;
    currentAppState = MAIN_MENU_CARDS[currentCardIndex].targetState;
    lastActivityMs = millis();
    DBG_PRINTF("[MENU] Selecionado: %s (estado %d)\n",
              MAIN_MENU_CARDS[currentCardIndex].label,
              currentAppState);
}

void menu_back() {
    buzzer_beep(80);
    currentAppState = STATE_MENU;
    menu_refresh();
}

// ============================================================================
// PROCESSAMENTO DE TOQUE
// ============================================================================
int menu_process_touch() {
    TS_Point p = ts.getPoint();

    if (p.z < TOUCH_MIN_Z) {
        touchLastX = 0;
        touchLastY = 0;
        return -1;
    }

    // Mapeia coordenadas para a tela
    int tx = map(p.x, TOUCH_X_MIN, TOUCH_X_MAX, 0, 320);
    int ty = map(p.y, TOUCH_Y_MIN, TOUCH_Y_MAX, 0, 240);

    // Verifica swipe
    if (touchLastX > 0) {
        int dx = abs(tx - (int)touchLastX);
        if (dx > TOUCH_SWIPE_MIN_DX) {
            if (tx > (int)touchLastX) {
                menu_navigate(-1); // swipe right = prev
            } else {
                menu_navigate(1);  // swipe left = next
            }
            touchLastX = tx;
            touchLastY = ty;
            return -1;
        }
    }

    touchLastX = tx;
    touchLastY = ty;

    // Verifica se tocou em algum card
    int cardIndex = currentPage * totalCardsPerScreen;
    int cardsOnThisPage = min(totalCardsPerScreen,
                             MAIN_MENU_COUNT - cardIndex);

    for (int i = 0; i < cardsOnThisPage; i++) {
        int idx = cardIndex + i;
        int col = i % gridCols;
        int row = i / gridCols;

        int x = gridStartX + col * (cardW + 8);
        int y = gridStartY + row * (cardH + 8);

        if (tx >= x && tx <= x + cardW && ty >= y && ty <= y + cardH) {
            return idx;
        }
    }

    return -1;
}

// ============================================================================
// DESENHAR BARRA DE STATUS
// ============================================================================
void draw_status_bar() {
    tft.fillRect(0, 0, 320, STATUS_BAR_HEIGHT, UI_COLORS::STATUS_BG);

    // Linha decorativa inferior
    tft.drawLine(0, STATUS_BAR_HEIGHT - 1, 320, STATUS_BAR_HEIGHT - 1,
                   UI_COLORS::PRIMARY);

    // Versão do firmware
    tft.setTextColor(UI_COLORS::STATUS_TEXT);
    tft.setTextSize(1);
    tft.setCursor(8, 10);
    tft.print(FW_NAME);
    tft.print(F(" v"));
    tft.print(FW_VERSION);

    // Indicador de SD Card
    int sdX = 310;
    if (sdCardPresent) {
        tft.setTextColor(UI_COLORS::SUCCESS);
        tft.print(F(" SD"));
    } else {
        tft.setTextColor(UI_COLORS::TEXT_HINT);
        tft.print(F(" --"));
    }

    // Temperatura (se disponível)
    if (lastTemperature > 0) {
        char tempStr[16];
        dtostrf(lastTemperature, 4, 1, tempStr);
        tft.setTextColor(lastTemperature > TEMP_WARNING ?
                           UI_COLORS::DANGER : UI_COLORS::STATUS_TEXT);
        tft.setCursor(150, 10);
        tft.print(tempStr);
        tft.print(F("C"));
    }
}

// ============================================================================
// DESENHAR RODAPÉ
// ============================================================================
void draw_footer() {
    int y = 240 - FOOTER_HEIGHT;
    tft.fillRect(0, y, 320, FOOTER_HEIGHT, UI_COLORS::STATUS_BG);

    tft.drawLine(0, y, 320, y, UI_COLORS::BORDER);

    tft.setTextColor(UI_COLORS::TEXT_HINT);
    tft.setTextSize(1);
    tft.setCursor(8, y + 5);
    tft.print(F("<<< NAVIGAR >>> OK=SELECIONAR"));

    // Indicador de brilho
    if (backlightOn) {
        tft.setCursor(250, y + 5);
        tft.print(F("Backlight ON"));
    } else {
        tft.setTextColor(UI_COLORS::TEXT_HINT);
        tft.setCursor(250, y + 5);
        tft.print(F("Backlight OFF"));
    }
}

// ============================================================================
// INDICADOR DE PÁGINA
// ============================================================================
void draw_page_indicator(int page, int total) {
    int dotSize = 4;
    int gap = 6;
    int totalDotsW = total * (dotSize + gap) - gap;
    int startX = (320 - totalDotsW) / 2;
    int y = 228;

    for (int i = 0; i < total; i++) {
        int x = startX + i * (dotSize + gap);
        uint16_t color = (i == page) ?
                           UI_COLORS::PRIMARY : UI_COLORS::BORDER;
        tft.fillCircle(x, y, dotSize / 2, color);
    }
}

// ============================================================================
// RESULTADO CARD
// ============================================================================
void draw_result_card(const char* component, const char* value,
                     const char* unit, const char* status, uint16_t statusColor) {
    // Fundo
    tft.fillScreen(UI_COLORS::BACKGROUND);

    // Barra de status
    draw_status_bar();

    // Card central
    int cardX = 40;
    int cardY = 50;
    int cardW = 240;
    int cardH = 140;

    tft.fillRoundRect(cardX, cardY, cardW, cardH, 12,
                      UI_COLORS::CARD_BG);
    tft.drawRoundRect(cardX, cardY, cardW, cardH, 12,
                      statusColor);
    tft.drawRoundRect(cardX + 3, cardY + 3, cardW - 6, cardH - 6,
                      shiftColor(statusColor, -40));

    // Componente
    tft.setTextColor(UI_COLORS::TEXT_DIM);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(cardX + cardW / 2, cardY + 15);
    tft.print(component);
    tft.setTextDatum(TL_DATUM);

    // Valor principal
    tft.setTextColor(UI_COLORS::TEXT);
    tft.setTextSize(4);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(cardX + cardW / 2, cardY + 50);
    tft.print(value);
    tft.setTextSize(2);
    tft.setCursor(cardX + cardW / 2, cardY + 80);
    tft.print(unit);
    tft.setTextDatum(TL_DATUM);

    // Status
    tft.fillRoundRect(cardX + 40, cardY + cardH - 35, cardW - 80, 22,
                        8, statusColor);
    tft.setTextColor(UI_COLORS::TEXT);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(cardX + cardW / 2, cardY + cardH - 29);
    tft.print(status);
    tft.setTextDatum(TL_DATUM);

    // Rodapé
    draw_footer();
}

// ============================================================================
// MODAL DE CONFIRMAÇÃO
// ============================================================================
bool draw_confirm_modal(const char* title, const char* message) {
    tft.fillRoundRect(40, 70, 240, 100, 10, UI_COLORS::CARD_BG);
    tft.drawRoundRect(40, 70, 240, 100, 10, UI_COLORS::WARNING);

    tft.setTextColor(UI_COLORS::TEXT);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(160, 82);
    tft.print(title);
    tft.setTextSize(1);
    tft.setCursor(160, 105);
    tft.print(message);

    tft.setTextColor(UI_COLORS::SUCCESS);
    tft.setCursor(70, 145);
    tft.print(F("TOQUE: OK = CONFIRMAR"));
    tft.setTextColor(UI_COLORS::DANGER);
    tft.setCursor(70, 160);
    tft.print(F("CANCEL = VOLTAR"));
    tft.setTextDatum(TL_DATUM);

    // Espera confirmação ou cancelamento
    while (true) {
        if (btn_just_pressed(BTN_OK) || btn_just_pressed(BTN_CENTER)) {
            return true;
        }
        if (btn_just_pressed(BTN_BACK)) {
            return false;
        }

        TS_Point p = ts.getPoint();
        if (p.z > TOUCH_MIN_Z) {
            return true;
        }

        delay(50);
    }
}

// ============================================================================
// SUB-MENUS — PLACEHOLDERS
// ============================================================================
void measure_menu_draw() {}
void measure_menu_handle() { menu_back(); }
void settings_draw() {}
void settings_handle() { menu_back(); }
void about_draw() {}
void about_handle() {
    if (btn_just_pressed(BTN_BACK) || btn_just_pressed(BTN_OK)) {
        menu_back();
    }
}
void history_draw() {}
void history_handle() { menu_back(); }
void stats_draw() {}
void stats_handle() { menu_back(); }
void scanner_draw() {}
void scanner_handle() { menu_back(); }
void calibration_draw() {}
void calibration_handle() { menu_back(); }
int measure_process_touch() { return -1; }
int settings_process_touch() { return -1; }
void transition_out() {}
void transition_in() {}
void transition_slide_left() {}
void transition_slide_right() {}

// ============================================================================
// HELPERS
// ============================================================================
uint16_t shiftColor(uint16_t color, int shift) {
    uint8_t r = (color >> 11) & 0x1F;
    uint8_t g = (color >> 5) & 0x3F;
    uint8_t b = color & 0x1F;

    r = constrain(r + (shift >> 3), 0, 31);
    g = constrain(g + (shift >> 2), 0, 63);
    b = constrain(b + (shift >> 3), 0, 31);

    return (r << 11) | (g << 5) | b;
}

void draw_warning_badge(int x, int y) {
    tft.fillCircle(x, y, 6, UI_COLORS::WARNING);
    tft.setTextColor(UI_COLORS::TEXT);
    tft.setTextSize(1);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(x, y - 3);
    tft.print(F("!"));
    tft.setTextDatum(TL_DATUM);
}

int draw_slider_modal(const char* title, int min, int max, int value) {
    (void)title; (void)min; (void)max; (void)value;
    return 0;
}