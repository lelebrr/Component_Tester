// ============================================================================
// Sondvolt v3.x — Graficos e Icones (Implementacao)
// Hardware: ESP32-2432S028R (Cheap Yellow Display)
// ============================================================================
// Arquivo: graphics.cpp
// Descricao: Implementacao de funcoes de desenho e icones vetoriais
// ============================================================================

#include "graphics.h"
#include "config.h"

// Variavel global do display
static TFT_eSPI* pTFT = nullptr;

// ============================================================================
// INICIALIZACAO
// ============================================================================
void graphicsInit(TFT_eSPI* tft) {
    pTFT = tft;
}

// ============================================================================
// ICONE: RESISTOR
// ============================================================================
// Desenha um resistor com corpo e faixas coloridas
void drawIconResistor(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    uint16_t w = size;
    uint16_t h = size / 3;
    uint16_t ledLen = size / 6;

    pTFT->fillRect(x, y + h/2 - 2, ledLen, 4);
    pTFT->fillRect(x + w - ledLen, y + h/2 - 2, ledLen, 4);

    uint16_t bodyX = x + ledLen;
    uint16_t bodyW = w - ledLen * 2;

    pTFT->fillRoundRect(bodyX, y, bodyW, h, 4);

    uint8_t bandColors[] = {C_RESISTOR, C_RED, C_GREEN};
    uint8_t numBands = 3;
    uint16_t bandW = bodyW / (numBands + 1);

    for(uint8_t i = 0; i < numBands; i++) {
        int16_t bx = bodyX + bandW * (i + 1) - bandW/2;
        pTFT->fillRect(bx, y + 1, bandW - 1, h - 2);
        pTFT->fillRect(bx, y + 1, bandW - 1, h - 2);
    }
}

// ============================================================================
// ICONE: CAPACITOR CERAMICO
// ============================================================================
// Desenha um capacitor ceramico (disco/lente)
void drawIconCapacitorCeramic(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    uint16_t leadLen = size / 5;
    uint16_t discR = size / 3;

    pTFT->setFreeFont(FMB);
    pTFT->fillRect(x + size/2 - leadLen/2, y, leadLen, size/4);
    pTFT->fillRect(x + size/2 - leadLen/2, y + size - size/4, leadLen, size/4);

    pTFT->fillCircle(x + size/2, y + size/2, discR);
}

// ============================================================================
// ICONE: CAPACITOR ELETROLITICO
// ============================================================================
// Desenha um capacitor eletrolitico (cilindro)
void drawIconCapacitorElectro(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    uint16_t w = size * 2 / 3;
    uint16_t h = size * 3 / 4;
    uint16_t leadLen = size / 4;

    pTFT->fillRect(x + w/4, y, leadLen, size/6);
    pTFT->fillRect(x + w/4 + w/2, y, leadLen, size/6);

    pTFT->fillRoundRect(x, y + size/6, w, h - size/6, 4);

    for(uint8_t i = 0; i < 3; i++) {
        pTFT->drawCircle(x + w/2, y + size/2 + i*4, w/4 - i*2);
    }
}

// ============================================================================
// ICONE: DIODO
// ============================================================================
// Desenha um diodo (seta + barra)
void drawIconDiode(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    uint16_t cx = x + size/2;
    uint16_t cy = y + size/2;
    uint16_t triSize = size / 2;

    pTFT->fillTriangle(cx, cy - triSize/2, cx, cy + triSize/2, cx + triSize/2, cy);
    pTFT->fillRect(cx - triSize/2, cy - triSize/4, triSize/4, triSize/2);
}

// ============================================================================
// ICONE: LED
// ============================================================================
// Desenha um LED com simbolo
void drawIconLed(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    uint16_t cx = x + size/2;
    uint16_t cy = y + size/2;

    pTFT->fillCircle(cx, cy, size/3);

    int16_t triSize = size / 3;
    pTFT->fillTriangle(cx, cy - triSize, cx, cy + triSize/2, cx + triSize, cy);
    pTFT->fillRect(cx - triSize/2, cy - triSize/4, triSize/4, triSize/2);

    pTFT->drawPixel(cx - 2, cy - triSize - 2);
    pTFT->drawPixel(cx + 2, cy - triSize - 2);
    pTFT->drawPixel(cx, cy - triSize - 4);
}

// ============================================================================
// ICONE: TRANSISTOR NPN
// ============================================================================
// Desenha transistor NPN
void drawIconTransistorNPN(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    drawIconTransistorPNP(x, y, size, color);
}

// ============================================================================
// ICONE: TRANSISTOR PNP
// ============================================================================
// Desenha transistor PNP
void drawIconTransistorPNP(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    uint16_t cx = x + size/2;
    uint16_t cy = y + size/2;
    uint16_t len = size / 2;

    pTFT->drawLine(cx, cy - len, cx - len/2, cy);
    pTFT->drawLine(cx, cy + len, cx - len/2, cy);

    pTFT->drawLine(cx - len/2, cy, cx + len/2, cy);
    pTFT->fillCircle(cx + len/2, cy, 3);

    pTFT->drawLine(cx, cy, cx + len, cy - len/3);
    pTFT->drawLine(cx, cy, cx + len, cy + len/3);

    pTFT->fillTriangle(cx + len - 4, cy - len/3 - 2,
                       cx + len - 4, cy - len/3 + 4,
                       cx + len + 2, cy);
}

// ============================================================================
// ICONE: MOSFET CANAL N
// ============================================================================
// Desenha MOSFET canal N
void drawIconMosfetN(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    drawIconMosfetP(x, y, size, color);
}

// ============================================================================
// ICONE: MOSFET CANAL P
// ============================================================================
// Desenha MOSFET canal P
void drawIconMosfetP(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    uint16_t cx = x + size/2;
    uint16_t cy = y + size/2;
    uint16_t len = size / 2;

    pTFT->drawLine(cx, cy - len, cx - len/2, cy);
    pTFT->fillCircle(cx - len/2 - 2, cy, 3);

    pTFT->drawLine(cx - len/2, cy, cx + len/2, cy);

    pTFT->drawLine(cx, cy - len, cx, cy - len/3);
    pTFT->fillRect(cx, cy - len/3 - 2, 4, 4);

    pTFT->drawLine(cx, cy, cx + len/2, cy - len/3);
    pTFT->fillTriangle(cx + len/2 + 2, cy - len/3,
                       cx + len/2 + 2, cy - len/3 - 4,
                       cx + len/2 - 2, cy - len/3 - 2);

    pTFT->drawLine(cx, cy, cx + len/2, cy + len/3);
    pTFT->fillTriangle(cx + len/2 + 2, cy + len/3,
                       cx + len/2 + 2, cy + len/3 + 4,
                       cx + len/2 - 2, cy + len/3 + 2);
}

// ============================================================================
// ICONE: INDUTOR
// ============================================================================
// Desenha um indutor (espirais)
void drawIconInductor(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    uint16_t cx = x + size/2;
    uint16_t cy = y + size/2;
    uint16_t len = size * 2 / 3;

    pTFT->drawLine(x, cy, cx - len/2, cy);

    for(uint8_t i = 0; i < 4; i++) {
        int16_t arcY = cy - 6 + i * 4;
        pTFT->drawArc(cx - len/3 + i*4, arcY, 4, 4, 180, 0);
    }

    pTFT->drawLine(cx + len/2 + 4, cy, x + size, cy);
}

// ============================================================================
// ICONE: CRISTAL
// ============================================================================
// Desenha um cristal (XTAL)
void drawIconCrystal(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    uint16_t w = size * 3 / 4;
    uint16_t h = size / 2;
    uint16_t cx = x + size/2;
    uint16_t cy = y + size/2;

    pTFT->drawRect(x + (size-w)/2, y + (size-h)/2, w, h);

    pTFT->drawLine(cx - w/4, cy - h/2, cx + w/4, cy);
    pTFT->drawLine(cx - w/4, cy + h/2, cx + w/4, cy);

    pTFT->drawLine(x, cy - h/2, x + w/6, cy - h/2);
    pTFT->drawLine(x, cy + h/2, x + w/6, cy + h/2);

    pTFT->drawLine(x + w - w/6, cy - h/2, x + w, cy - h/2);
    pTFT->drawLine(x + w - w/6, cy + h/2, x + w, cy + h/2);
}

// ============================================================================
// ICONE: FUSIVEL
// ============================================================================
// Desenha um fusivel (tubo com fio)
void drawIconFuse(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    uint16_t w = size * 2 / 3;
    uint16_t h = size / 3;
    uint16_t leadLen = size / 4;

    pTFT->fillRect(x, y + size/2 - 1, leadLen, 2);
    pTFT->fillRect(x + w - leadLen, y + size/2 - 1, leadLen, 2);

    pTFT->drawRoundRect(x + leadLen, y + (size-h)/2, w - leadLen*2, h, 2);

    pTFT->drawLine(x + leadLen + 2, y + size/2 - 1, x + w - leadLen - 2, y + size/2 + 1);
}

// ============================================================================
// ICONE: VARISTOR
// ============================================================================
// Desenha um varistor (simbolo semicondutor)
void drawIconVaristor(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    uint16_t w = size * 2 / 3;
    uint16_t h = size / 2;

    pTFT->fillRect(x, y + size/2 - 2, w, 4);

    for(uint8_t i = 0; i < 3; i++) {
        pTFT->drawLine(x + 4 + i*4, y + size/2 + 4,
                       x + 8 + i*4, y + size - 4);
    }

    pTFT->drawRect(x, y + size/2 - h/2, w, h);
}

// ============================================================================
// ICONE: POTENCIOMETRO
// ============================================================================
// Desenha um potenciometro
void drawIconPotentiometer(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    uint16_t r = size / 3;
    uint16_t cx = x + size/2;
    uint16_t cy = y + size/2;

    pTFT->fillCircle(cx, cy, r);

    pTFT->fillRect(cx - 2, y, 4, size/3);

    pTFT->drawLine(cx - r + 2, cy, cx + r - 2, cy);
    pTFT->drawLine(cx, cy - r + 2, cx, cy + r - 2);
}

// ============================================================================
// ICONE: OPTOACOPLADOR
// ============================================================================
// Desenha um optoacoplador (LED + transistor)
void drawIconOptocoupler(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    uint16_t sizeHalf = size / 2;

    pTFT->fillCircle(x + sizeHalf/2, y + sizeHalf/2, sizeHalf/3);

    int16_t triSize = sizeHalf / 3;
    pTFT->fillTriangle(x + sizeHalf/2, y + sizeHalf/2 - triSize/2,
                       x + sizeHalf/2, y + sizeHalf/2 + triSize/2,
                       x + sizeHalf/2 + triSize/2, y + sizeHalf/2);

    pTFT->drawRect(x + sizeHalf - 2, y + size/4, sizeHalf - 2, size/2);

    pTFT->drawLine(x + sizeHalf + sizeHalf/2, y + sizeHalf/2,
                   x + size - 4, y + sizeHalf/2);

    pTFT->fillCircle(x + size - 4, y + sizeHalf/2, 3);
}

// ============================================================================
// ICONE: COMPONENTE DESCONHECIDO
// ============================================================================
// Desenha um ponto de interrogacao
void drawIconUnknown(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    uint16_t cx = x + size/2;
    uint16_t cy = y + size/2;
    uint16_t r = size/3;

    pTFT->fillCircle(cx, cy, r);
    pTFT->setTextColor(C_BLACK);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->drawString("?", cx, cy);
    pTFT->setTextColor(C_WHITE);
}

// ============================================================================
// FUNCAO PRINCIPAL: DESENHAR ICONE POR TIPO
// ============================================================================
void drawComponentIcon(ComponentType type, int16_t x, int16_t y, uint16_t size) {
    switch(type) {
        case COMP_RESISTOR:
            drawIconResistor(x, y, size, C_RESISTOR);
            break;
        case COMP_CAPACITOR_CERAMIC:
            drawIconCapacitorCeramic(x, y, size, C_CAPACITOR);
            break;
        case COMP_CAPACITOR_ELECTRO:
            drawIconCapacitorElectro(x, y, size, C_CAPACITOR);
            break;
        case COMP_DIODE:
            drawIconDiode(x, y, size, C_DIODE);
            break;
        case COMP_LED:
            drawIconLed(x, y, size, C_LED);
            break;
        case COMP_TRANSISTOR_NPN:
            drawIconTransistorNPN(x, y, size, C_TRANSISTOR);
            break;
        case COMP_TRANSISTOR_PNP:
            drawIconTransistorPNP(x, y, size, C_TRANSISTOR);
            break;
        case COMP_MOSFET_N:
            drawIconMosfetN(x, y, size, C_TRANSISTOR);
            break;
        case COMP_MOSFET_P:
            drawIconMosfetP(x, y, size, C_TRANSISTOR);
            break;
        case COMP_INDUCTOR:
            drawIconInductor(x, y, size, C_INDUCTOR);
            break;
        case COMP_CRYSTAL:
            drawIconCrystal(x, y, size, C_CRYSTAL);
            break;
        case COMP_FUSE:
            drawIconFuse(x, y, size, C_WHITE);
            break;
        case COMP_VARISTOR:
            drawIconVaristor(x, y, size, C_DIODE);
            break;
        case COMP_POTENTIOMETER:
            drawIconPotentiometer(x, y, size, C_WHITE);
            break;
        case COMP_OPTOACOPLER:
            drawIconOptocoupler(x, y, size, C_ACCENT);
            break;
        case COMP_UNKNOWN:
        default:
            drawIconUnknown(x, y, size, C_UNKNOWN);
            break;
    }
}

// ============================================================================
// STATUS INDICATOR
// ============================================================================
void drawStatusIndicator(MeasurementStatus status, int16_t x, int16_t y, uint16_t size) {
    uint16_t color;
    switch(status) {
        case STATUS_GOOD:
            color = C_GREEN;
            break;
        case STATUS_WARNING:
            color = C_YELLOW;
            break;
        case STATUS_BAD:
            color = C_RED;
            break;
        default:
            color = C_GREY;
    }

    pTFT->fillCircle(x + size/2, y + size/2, size/2 - 1);
    pTFT->drawCircle(x + size/2, y + size/2, size/2 - 1);

    pTFT->setTextColor(C_BLACK);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);

    if(status == STATUS_GOOD) {
        pTFT->drawString("OK", x + size/2, y + size/2);
    } else if(status == STATUS_WARNING) {
        pTFT->drawString("!", x + size/2, y + size/2);
    } else if(status == STATUS_BAD) {
        pTFT->drawString("X", x + size/2, y + size/2);
    }
}

// ============================================================================
// BARRA DE STATUS
// ============================================================================
void drawStatusBar(const char* title) {
    pTFT->fillRect(0, 0, SCREEN_W, STATUS_BAR_H);

    pTFT->setTextColor(C_TEXT);
    pTFT->setTextDatum(ML_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString(title, 4, STATUS_BAR_H/2);

    pTFT->drawLine(0, STATUS_BAR_H - 1, SCREEN_W, STATUS_BAR_H - 1);
}

void updateStatusBar(const char* text) {
    pTFT->fillRect(0, 0, SCREEN_W, STATUS_BAR_H);
    pTFT->setTextColor(C_TEXT);
    pTFT->setTextDatum(ML_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString(text, 4, STATUS_BAR_H/2);
}

// ============================================================================
// RODAPE
// ============================================================================
void drawFooter(const char* left, const char* right) {
    uint16_t y = SCREEN_H - FOOTER_H;

    pTFT->setTextColor(C_TEXT_SECONDARY);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMBO);

    if(left) pTFT->drawString(left, 50, y + FOOTER_H/2);
    if(right) pTFT->drawString(right, SCREEN_W - 50, y + FOOTER_H/2);
}

void drawFooterIcon(uint8_t icon, int16_t x, int16_t y) {
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMBO);

    switch(icon) {
        case 0:
            pTFT->drawString("ESC", x, y);
            break;
        case 1:
            pTFT->drawString("OK", x, y);
            break;
        default:
            break;
    }
}

// ============================================================================
// ELEMENTOS UI
// ============================================================================
void drawButton(int16_t x, int16_t y, uint16_t w, uint16_t h, const char* label, bool pressed) {
    uint16_t color = pressed ? C_PRIMARY : C_SURFACE;

    pTFT->fillRoundRect(x, y, w, h, 4);
    pTFT->drawRoundRect(x, y, w, h, 4);

    pTFT->setTextColor(pressed ? C_BLACK : C_TEXT);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString(label, x + w/2, y + h/2);
}

void drawProgressBar(int16_t x, int16_t y, uint16_t w, uint16_t h, float percent, uint16_t color) {
    pTFT->fillRect(x, y, w, h);

    if(percent > 0) {
        uint16_t pw = (uint16_t)((w - 2) * percent / 100.0f);
        pTFT->fillRect(x + 1, y + 1, pw, h - 2);
    }

    pTFT->drawRect(x, y, w, h);
}

void drawGauge(int16_t cx, int16_t cy, uint16_t radius, float value, float minVal, float maxVal, const char* unit) {
    float angle = 270.0f;
    float startAngle = 135.0f;

    pTFT->drawArc(cx, cy, radius, radius - 5, 0, 360);

    float percent = (value - minVal) / (maxVal - minVal);
    if(percent < 0) percent = 0;
    if(percent > 1) percent = 1;

    pTFT->drawArc(cx, cy, radius, radius - 5, startAngle, startAngle + angle * percent);

    char valStr[32];
    if(value >= 1000) {
        snprintf(valStr, sizeof(valStr), "%.1f", value / 1000.0f);
    } else {
        snprintf(valStr, sizeof(valStr), "%.1f", value);
    }

    char fullStr[40];
    snprintf(fullStr, sizeof(fullStr), "%s %s", valStr, unit);

    pTFT->setTextColor(C_TEXT);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString(fullStr, cx, cy);
}

void drawValueBox(int16_t x, int16_t y, uint16_t w, uint16_t h, const char* label, const char* value, const char* unit) {
    pTFT->fillRect(x, y, w, h);
    pTFT->drawRect(x, y, w, h);

    pTFT->setTextColor(C_TEXT_SECONDARY);
    pTFT->setTextDatum(TC_DATUM);
    pTFT->setFreeFont(FMBO);
    pTFT->drawString(label, x + w/2, y + 8);

    pTFT->setTextColor(C_TEXT);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString(value, x + w/2, y + h/2 + 4);

    pTFT->setTextColor(C_PRIMARY);
    pTFT->setTextSize(1);
    pTFT->drawString(unit, x + w/2, y + h - 6);
}

void drawWarningBox(const char* message) {
    pTFT->fillRect(SCREEN_W/4, SCREEN_H/3, SCREEN_W/2, SCREEN_H/3);
    pTFT->drawRect(SCREEN_W/4, SCREEN_H/3, SCREEN_W/2, SCREEN_H/3);

    pTFT->setTextColor(C_WARNING);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString("!", SCREEN_W/4 + 15, SCREEN_H/3 + 15);

    pTFT->setTextColor(C_TEXT);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString(message, SCREEN_W/2, SCREEN_H/2);
}

void drawErrorBox(const char* message) {
    pTFT->fillRect(SCREEN_W/4, SCREEN_H/3, SCREEN_W/2, SCREEN_H/3);
    pTFT->drawRect(SCREEN_W/4, SCREEN_H/3, SCREEN_W/2, SCREEN_H/3);

    pTFT->setTextColor(C_ERROR);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString("X", SCREEN_W/4 + 15, SCREEN_H/3 + 15);

    pTFT->setTextColor(C_TEXT);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString(message, SCREEN_W/2, SCREEN_H/2);
}

void drawConfirmBox(const char* message) {
    pTFT->fillRect(SCREEN_W/4, SCREEN_H/3, SCREEN_W/2, SCREEN_H/3);
    pTFT->drawRect(SCREEN_W/4, SCREEN_H/3, SCREEN_W/2, SCREEN_H/3);

    pTFT->setTextColor(C_SUCCESS);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString("?", SCREEN_W/4 + 15, SCREEN_H/3 + 15);

    pTFT->setTextColor(C_TEXT);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString(message, SCREEN_W/2, SCREEN_H/2);

    pTFT->fillRoundRect(SCREEN_W/4 + 20, SCREEN_H/3 + 40, 40, 20, 4);
    pTFT->fillRoundRect(SCREEN_W/2 + 20, SCREEN_H/3 + 40, 40, 20, 4);

    pTFT->setTextColor(C_TEXT);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString("N", SCREEN_W/4 + 40, SCREEN_H/3 + 50);
    pTFT->drawString("S", SCREEN_W/2 + 40, SCREEN_H/3 + 50);
}

// ============================================================================
// SPLASH SCREEN
// ============================================================================
void drawSplashScreen(void) {
    pTFT->fillScreen(C_BLACK);

    pTFT->setTextColor(C_PRIMARY);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMB);
    pTFT->drawString("Component", SCREEN_W/2, SCREEN_H/2 - 30);
    pTFT->drawString("Tester PRO", SCREEN_W/2, SCREEN_H/2);

    pTFT->setTextColor(C_TEXT_SECONDARY);
    pTFT->setTextDatum(MC_DATUM);
    pTFT->setFreeFont(FMBO);
    pTFT->drawString("v" FW_VERSION, SCREEN_W/2, SCREEN_H/2 + 30);

    pTFT->setTextColor(C_TEXT_SECONDARY);
    pTFT->setTextSize(1);
    pTFT->drawCenterString(FW_CODENAME, SCREEN_W/2, SCREEN_H - 30);
}