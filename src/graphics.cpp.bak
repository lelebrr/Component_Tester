// ============================================================================
// Component Tester PRO v3.0 — Funções Gráficas (graphics.cpp)
// Descrição: Ícones de componentes desenhados com primitivas GFX
// Todos os ícones são 40x40 pixels (vetoriais, sem bitmap)
// ============================================================================

#include "graphics.h"
#include "globals.h"
#include <math.h>

// ============================================================================
// INSTÂNCIA GLOBAL DO TFT (definida em main.cpp)
// ============================================================================
extern Adafruit_ILI9341 tft;

// ============================================================================
// ENTRADA PRINCIPAL — DESPACHA PARA O ÍCONE CORRETO
// ============================================================================
void draw_component_icon(ComponentIconType type, int16_t x, int16_t y,
                          uint16_t color) {
    switch (type) {
        case ICON_RESISTOR:     draw_resistor_icon(x, y, color); break;
        case ICON_CAPACITOR:   draw_capacitor_icon(x, y, color); break;
        case ICON_ELECTROLYTIC: draw_electrolytic_icon(x, y, color); break;
        case ICON_DIODE:       draw_diode_icon(x, y, color); break;
        case ICON_LED:          draw_led_icon(x, y, color); break;
        case ICON_ZENER:       draw_zener_icon(x, y, color); break;
        case ICON_TRANSISTOR:  draw_transistor_icon(x, y, color); break;
        case ICON_FET:         draw_fet_icon(x, y, color); break;
        case ICON_INDUCTOR:    draw_inductor_icon(x, y, color); break;
        case ICON_RELAY:        draw_relay_icon(x, y, color); break;
        case ICON_IC:          draw_ic_icon(x, y, color); break;
        case ICON_COIL:        draw_coil_icon(x, y, color); break;
        case ICON_VOLTAGE:     draw_voltage_icon(x, y, color); break;
        case ICON_CURRENT:      draw_current_icon(x, y, color); break;
        case ICON_MULTIMETER: draw_multimeter_icon(x, y, color); break;
        case ICON_THERMOMETER: draw_thermometer_icon(x, y, color); break;
        case ICON_SCANNER:     draw_scanner_icon(x, y, color); break;
        case ICON_CALIBRATE:    draw_calibrate_icon(x, y, color); break;
        case ICON_SETTINGS:    draw_settings_icon(x, y, color); break;
        case ICON_HISTORY:     draw_history_icon(x, y, color); break;
        case ICON_STATS:        draw_stats_icon(x, y, color); break;
        case ICON_ABOUT:       draw_about_icon(x, y, color); break;
        case ICON_BRIGHTNESS:  draw_brightness_icon(x, y, color); break;
        case ICON_SOUND:      draw_sound_icon(x, y, color); break;
        case ICON_DARK:       draw_dark_icon(x, y, color); break;
        case ICON_SLEEP:      draw_sleep_icon(x, y, color); break;
        case ICON_RESET:       draw_reset_icon(x, y, color); break;
        case ICON_DELETE:     draw_delete_icon(x, y, color); break;
        default:              draw_resistor_icon(x, y, color); break;
    }
}

// ============================================================================
// ÍCONE — RESISTOR (símbolo zigzag IEC)
// Símbolo: ─┐┌┐┌┐┌┐┌┐┌┐┌┐┌┐┌┐┌┐┌┐┌┐┌
// ============================================================================
void draw_resistor_icon(int16_t x, int16_t y, uint16_t color) {
    int h = 40; // altura total do ícone
    int lead = 5; // comprimento dos terminais
    int body = h - lead * 2; // corpo
    int seg = 5; // segmentos do zigzag
    int amp = 4; // amplitude vertical do zigzag

    // Linha horizontal esquerda (terminal)
    tft.drawLine(x, y + h / 2, x + lead, y + h / 2, color);

    // Zigzag (5 picos)
    int xp = x + lead;
    int dir = 1;
    for (int i = 0; i < seg; i++) {
        int x1 = xp;
        int x2 = xp + body / seg;
        int y1 = (dir > 0) ? (y + h / 2 - amp) : (y + h / 2 + amp);
        int y2 = (dir > 0) ? (y + h / 2 + amp) : (y + h / 2 - amp);
        tft.drawLine(x1, y + h / 2, x1, y1, color);
        tft.drawLine(x1, y1, x2, y2, color);
        tft.drawLine(x2, y + h / 2, x2, y2, color);
        xp = x2;
        dir = -dir;
    }

    // Linha horizontal direita (terminal)
    tft.drawLine(xp, y + h / 2, x + h, y + h / 2, color);

    // Bandas coloridas (opcional — indica o valor)
    int bandY = y + h / 2;
    int bandX = x + lead + body / 4;
    int bandW = 3;
    tft.fillRect(bandX - bandW / 2, bandY - 6, bandW, 12, rgb565(180, 75, 50));
    tft.fillRect(bandX + body / 5 - bandW / 2, bandY - 6, bandW, 12, rgb565(0, 0, 180));
}

// ============================================================================
// ÍCONE — CAPACITOR (símbolo padrão)
// Símbolo: ─│  │─
// ============================================================================
void draw_capacitor_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;
    int centerX = x + w / 2;
    int centerY = y + h / 2;
    int lead = 8;
    int gap = 10;
    int plateW = 3;

    // Terminal esquerdo
    tft.drawLine(x, centerY, x + lead, centerY, color);

    // Terminal direito
    tft.drawLine(x + w - lead, centerY, x + w, centerY, color);

    // Placas (duas linhas verticais)
    tft.fillRect(centerX - gap / 2 - plateW / 2, centerY - 12,
                   plateW, 24, color);
    tft.fillRect(centerX + gap / 2 - plateW / 2, centerY - 12,
                   plateW, 24, color);
}

// ============================================================================
// ÍCONE — CAPACITOR ELETROLÍTICO (polarizado)
// Símbolo: ─(+)  │─ (com + indicando polaridade)
// ============================================================================
void draw_electrolytic_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;
    int centerX = x + w / 2;
    int centerY = y + h / 2;
    int lead = 6;
    int gap = 12;
    int plateW = 3;
    int bodyH = 22;

    // Terminal esquerdo
    tft.drawLine(x, centerY, x + lead, centerY, color);

    // Terminal direito
    tft.drawLine(x + w - lead, centerY, x + w, centerY, color);

    // Corpo oval do capacitor
    tft.drawRoundRect(centerX - gap / 2 - plateW / 2 - 4,
                        centerY - bodyH / 2, gap / 2 + plateW / 2 + 4,
                        bodyH, 3, color);
    tft.drawLine(centerX + gap / 2, centerY - bodyH / 2 + 2,
                   centerX + gap / 2 + plateW / 2, centerY - bodyH / 2 + 2,
                   color);

    // Símbolo (+) na placa esquerda
    tft.setTextColor(color);
    tft.setTextSize(1);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(centerX - 16, centerY - 5);
    tft.print(F("+"));
    tft.setTextDatum(TL_DATUM);
}

// ============================================================================
// ÍCONE — DIODO (símbolo com barra e triângulo)
// Símbolo: ─▶│─
// ============================================================================
void draw_diode_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;
    int centerX = x + w / 2;
    int centerY = y + h / 2;
    int lead = 6;
    int triSize = 12;

    // Terminal esquerdo
    tft.drawLine(x, centerY, x + lead, centerY, color);

    // Terminal direito
    tft.drawLine(x + w - lead, centerY, x + w, centerY, color);

    // Triângulo (seta)
    tft.fillTriangle(
        centerX - triSize / 2, centerY,
        centerX + triSize / 2, centerY - triSize / 2,
        centerX + triSize / 2, centerY + triSize / 2,
        color);

    // Barra (catodo) — lado direito do triângulo
    int barX = centerX + triSize / 2;
    tft.drawLine(barX, centerY - triSize / 2, barX, centerY + triSize / 2,
                   color);
    tft.drawLine(barX - 2, centerY - triSize / 2, barX + 1,
                   centerY - triSize / 2, color);
}

// ============================================================================
// ÍCONE — LED (diodo emissor de luz)
// Símbolo: ─▶│─ com setas de luz saindo
// ============================================================================
void draw_led_icon(int16_t x, int16_t y, uint16_t color) {
    draw_diode_icon(x, y, color);

    int w = 40, h = 40;
    int centerX = x + w / 2;
    int centerY = y + h / 2;

    // Seta de luz 1 (top-left)
    tft.drawLine(centerX - 10, centerY - 6, centerX - 14, centerY - 12, color);
    tft.drawLine(centerX - 14, centerY - 12, centerX - 10, centerY - 12, color);
    tft.drawLine(centerX - 14, centerY - 12, centerX - 10, centerY - 18, color);

    // Seta de luz 2 (top-right)
    tft.drawLine(centerX + 12, centerY - 6, centerX + 16, centerY - 12, color);
    tft.drawLine(centerX + 16, centerY - 12, centerX + 12, centerY - 12, color);
    tft.drawLine(centerX + 16, centerY - 12, centerX + 12, centerY - 18, color);
}

// ============================================================================
// ÍCONE — ZENER (diodo Zener)
// Símbolo: ─▶│─ com "pernas" na barra
// ============================================================================
void draw_zener_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;
    int centerX = x + w / 2;
    int centerY = y + h / 2;
    int lead = 6;
    int triSize = 12;

    // Terminais
    tft.drawLine(x, centerY, x + lead, centerY, color);
    tft.drawLine(x + w - lead, centerY, x + w, centerY, color);

    // Triângulo
    tft.fillTriangle(
        centerX - triSize / 2, centerY,
        centerX + triSize / 2, centerY - triSize / 2,
        centerX + triSize / 2, centerY + triSize / 2, color);

    // Barra Zener com pernas
    int barX = centerX + triSize / 2;
    tft.drawLine(barX, centerY - triSize / 2, barX, centerY + triSize / 2,
                   color);

    // Pernas diagonais (característica do Zener)
    tft.drawLine(barX, centerY - triSize / 2, barX + 4, centerY - triSize / 2 - 4,
                   color);
    tft.drawLine(barX, centerY + triSize / 2, barX + 4, centerY + triSize / 2 + 4,
                   color);
}

// ============================================================================
// ÍCONE — TRANSISTOR NPN (símbolo)
// Símbolo:
//   B ─┤
//      │C
//       └▷│
//       ──┘E
// ============================================================================
void draw_transistor_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;
    int bx = x + 8;
    int by = y + h / 2;
    int cStart = bx + 8;
    int cEnd = x + w - 6;
    int eStart = bx + 8;
    int eEnd = x + w - 6;
    int vMid = y + h / 2;

    // Base (linha horizontal)
    tft.drawLine(x + 2, by, cStart, by, color);

    // Coletor (para cima)
    tft.drawLine(cStart, vMid, cStart, y + 10, color);
    tft.drawLine(cStart, y + 10, cEnd, y + 18, color);

    // Emissor (para baixo)
    tft.drawLine(eStart, vMid, eStart, y + h - 10, color);
    tft.drawLine(eStart, y + h - 10, eEnd, y + h - 18, color);

    // Seta no emissor (indica NPN)
    tft.fillTriangle(
        eEnd - 4, y + h - 18,
        eEnd + 2, y + h - 22,
        eEnd + 2, y + h - 14, color);

    // Círculo (encapsulamento)
    tft.drawCircle(bx + 10, vMid, 14, color);
}

// ============================================================================
// ÍCONE — MOSFET (símbolo)
// Similar ao transistor mas com linha no gate
// ============================================================================
void draw_fet_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;
    int bx = x + 8;
    int by = y + h / 2;
    int gx = x + 2;
    int cStart = bx + 8;
    int cEnd = x + w - 6;

    // Gate
    tft.drawLine(gx, by - 6, gx, by + 6, color);
    tft.drawLine(gx, by, cStart, by, color);

    // Body (linha vertical no centro)
    tft.drawLine(cStart, y + 8, cStart, y + h - 8, color);

    // Channel (substrate line)
    tft.drawLine(cStart + 2, y + 8, cStart + 2, y + h - 8, color);

    // Dreno (para cima)
    tft.drawLine(cStart, y + 8, cStart + 12, y + 8, color);
    tft.drawLine(cStart + 12, y + 8, cEnd, y + 14, color);

    // Source (para baixo)
    tft.drawLine(cStart, y + h - 8, cStart + 12, y + h - 8, color);
    tft.drawLine(cStart + 12, y + h - 8, cEnd, y + h - 14, color);

    // Seta no source
    tft.fillTriangle(
        cEnd - 4, y + h - 14,
        cEnd + 2, y + h - 18,
        cEnd + 2, y + h - 10, color);
}

// ============================================================================
// ÍCONE — INDUTOR (bobina / solenóide)
// Símbolo: ─⨊⨊⨊──
// ============================================================================
void draw_inductor_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;
    int centerY = y + h / 2;
    int startX = x + 5;
    int endX = x + w - 5;
    int coilStart = startX + 4;
    int coilEnd = endX - 4;
    int coilH = 12;
    int loops = 4;
    int loopW = (coilEnd - coilStart) / loops;

    // Terminal esquerdo
    tft.drawLine(x, centerY, startX, centerY, color);

    // Bobina (semicírculos)
    for (int i = 0; i < loops; i++) {
        int lx = coilStart + i * loopW;
        tft.drawArc(lx, centerY, loopW / 2 - 1, coilH / 2,
                      0, 180, color);
        tft.drawArc(lx + loopW, centerY, loopW / 2 - 1, coilH / 2,
                      180, 360, color);
    }

    // Terminal direito
    tft.drawLine(coilEnd, centerY, endX, centerY, color);
    tft.drawLine(endX, centerY, endX + 3, centerY, color);
}

// ============================================================================
// ÍCONE — RELAY (relé eletromecânico)
// Símbolo: ─┤⟨⟩──
// ============================================================================
void draw_relay_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;
    int centerX = x + w / 2;
    int centerY = y + h / 2;

    // Caixa do relé
    tft.drawRoundRect(centerX - 14, centerY - 10, 28, 20, 3, color);

    // Terminals (esquerda)
    tft.drawLine(x, centerY - 5, centerX - 14, centerY - 5, color);
    tft.drawLine(x, centerY + 5, centerX - 14, centerY + 5, color);

    // Terminals (direita)
    tft.drawLine(centerX + 14, centerY - 5, x + w, centerY - 5, color);
    tft.drawLine(centerX + 14, centerY + 5, x + w, centerY + 5, color);

    // Bobina interna (símbolo)
    tft.drawLine(centerX - 6, centerY - 6, centerX - 6, centerY + 6, color);
    tft.drawLine(centerX - 6, centerY, centerX + 6, centerY, color);
}

// ============================================================================
// ÍCONE — CI / IC (circuito integrado)
// Símbolo: Retângulo com pinos
// ============================================================================
void draw_ic_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;
    int bodyW = w - 6;
    int bodyH = 24;
    int bodyX = x + 3;
    int bodyY = y + 8;
    int notchR = 5;

    // Corpo do IC
    tft.fillRoundRect(bodyX, bodyY, bodyW, bodyH, 3, color);

    // Entalhe (pin 1 indicator)
    tft.fillCircle(bodyX + notchR, bodyY + bodyH / 2, 2,
                    UI_COLORS::BACKGROUND);

    // Pinos (7 de cada lado)
    int pinCount = 7;
    int pinSpacing = (bodyH) / (pinCount + 1);

    for (int i = 1; i <= pinCount; i++) {
        int py = bodyY + pinSpacing * i;
        tft.drawLine(bodyX, py, bodyX - 3, py, color);
        tft.drawLine(bodyX + bodyW, py, bodyX + bodyW + 3, py, color);
    }

    // Texto "IC" no centro
    tft.setTextColor(color == UI_COLORS::IC ?
                       UI_COLORS::TEXT : UI_COLORS::IC);
    tft.setTextSize(1);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(bodyX + bodyW / 2, bodyY + bodyH / 2);
    tft.print(F("IC"));
    tft.setTextDatum(TL_DATUM);
}

// ============================================================================
// ÍCONE — COIL / BOBINA (alternativo)
// Semicírculos empilhados
// ============================================================================
void draw_coil_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;
    int centerY = y + h / 2;
    int startX = x + 6;
    int endX = x + w - 6;

    // Terminal esquerdo
    tft.drawLine(x, centerY, startX, centerY, color);

    // Espiras (arcos)
    int arcR = 4;
    for (int i = 0; i < 4; i++) {
        int ax = startX + i * arcR * 2;
        tft.drawCircle(ax, centerY, arcR, color);
    }

    // Terminal direito
    tft.drawLine(startX + arcR * 2 * 3 + arcR, centerY, endX, centerY, color);
    tft.drawLine(endX, centerY, endX + 4, centerY, color);
}

// ============================================================================
// ÍCONE — TENSÃO (V com barra)
// ============================================================================
void draw_voltage_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;
    int centerX = x + w / 2;
    int centerY = y + h / 2;

    // Letra V
    tft.setTextColor(color);
    tft.setTextSize(3);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(centerX, centerY - 6);
    tft.print(F("V"));

    // Barra superior
    tft.drawLine(centerX - 14, centerY - 14, centerX + 14, centerY - 14, color);

    tft.setTextDatum(TL_DATUM);
}

// ============================================================================
// ÍCONE — CORRENTE (A invertido)
// ============================================================================
void draw_current_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;
    int centerX = x + w / 2;
    int centerY = y + h / 2;

    // Letra A
    tft.setTextColor(color);
    tft.setTextSize(3);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(centerX, centerY - 6);
    tft.print(F("A"));

    // Ponto superior
    tft.fillCircle(centerX - 1, centerY - 16, 2, color);

    tft.setTextDatum(TL_DATUM);
}

// ============================================================================
// ÍCONE — MULTÍMETRO
// Retângulo com display e ponteiros
// ============================================================================
void draw_multimeter_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;

    // Corpo
    tft.fillRoundRect(x + 4, y + 8, 32, 24, 4, color);

    // Display (retângulo interno)
    tft.fillRect(x + 10, y + 12, 20, 8, UI_COLORS::BACKGROUND);
    tft.setTextColor(color);
    tft.setTextSize(1);
    tft.setCursor(x + 12, y + 13);
    tft.print(F("8888"));

    // Botões
    tft.fillCircle(x + 10, y + 26, 2, color);
    tft.fillCircle(x + 16, y + 26, 2, color);
    tft.fillCircle(x + 22, y + 26, 2, color);

    // Pontas de prova (cabos)
    tft.drawLine(x + 30, y + 16, x + 38, y + 10, rgb565(255, 0, 0));
    tft.drawLine(x + 30, y + 24, x + 38, y + 30, rgb565(0, 0, 255));
}

// ============================================================================
// ÍCONE — TERMÔMETRO
// Tubo com escala e bulbo
// ============================================================================
void draw_thermometer_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;
    int centerX = x + w / 2;
    int bulbR = 7;

    // Bulbo (círculo inferior)
    tft.fillCircle(centerX, y + h - bulbR - 4, bulbR, color);

    // Tubo (retângulo arredondado)
    tft.fillRoundRect(centerX - 4, y + 6, 8, h - bulbR - 10, 4, color);

    // Líquido interno
    uint16_t liquidColor = rgb565(200, 30, 30);
    tft.fillRoundRect(centerX - 2, y + 10, 4, h - bulbR - 16, 2, liquidColor);

    // Marcações de escala
    for (int i = 0; i < 4; i++) {
        int sy = y + 10 + i * 6;
        tft.drawLine(centerX + 5, sy, centerX + 8, sy, color);
    }

    // °C
    draw_degrees_symbol(x + w - 10, y + 8, color);
}

// ============================================================================
// ÍCONE — SCANNER (desenho de radar/wave)
// ============================================================================
void draw_scanner_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;
    int centerX = x + w / 2;
    int centerY = y + h / 2;

    // Círculos concêntricos (arcos de radar)
    tft.drawCircle(centerX, centerY, 14, color);
    tft.drawCircle(centerX, centerY, 10, color);
    tft.drawCircle(centerX, centerY, 6, color);

    // Linha de varredura
    tft.drawLine(centerX, centerY, centerX + 14, centerY - 8, color);

    // Ponto central
    tft.fillCircle(centerX, centerY, 2, color);
}

// ============================================================================
// ÍCONE — CALIBRATE (transferência / alvo)
// ============================================================================
void draw_calibrate_icon(int16_t x, int16_t y, uint16_t color) {
    int w = 40, h = 40;
    int centerX = x + w / 2;
    int centerY = y + h / 2;

    // Círculos concêntricos (alvo)
    tft.drawCircle(centerX, centerY, 14, color);
    tft.drawCircle(centerX, centerY, 9, color);
    tft.drawCircle(centerX, centerY, 4, color);

    // Cruz no centro
    tft.drawLine(centerX - 2, centerY, centerX + 2, centerY, color);
    tft.drawLine(centerX, centerY - 2, centerX, centerY + 2, color);
}

// ============================================================================
// ÍCONES — MENU / SETTINGS
// ============================================================================
void draw_settings_icon(int16_t x, int16_t y, uint16_t color) {
    // Engrenagem (8 dentes)
    int cx = x + 20, cy = y + 20, r = 12;
    tft.drawCircle(cx, cy, r, color);
    tft.drawCircle(cx, cy, r - 5, color);

    // Dentes da engrenagem
    for (int i = 0; i < 8; i++) {
        int angle = i * 45;
        int dx = cos(radians(angle)) * r;
        int dy = sin(radians(angle)) * r;
        tft.fillRect(cx + dx - 2, cy + dy - 2, 4, 4, color);
    }
}

void draw_history_icon(int16_t x, int16_t y, uint16_t color) {
    // Documento / lista
    int w = 40, h = 40;
    tft.fillRoundRect(x + 6, y + 4, 28, 32, 3, color);

    // Linhas de texto
    tft.drawLine(x + 10, y + 12, x + 30, y + 12, UI_COLORS::BACKGROUND);
    tft.drawLine(x + 10, y + 18, x + 30, y + 18, UI_COLORS::BACKGROUND);
    tft.drawLine(x + 10, y + 24, x + 24, y + 24, UI_COLORS::BACKGROUND);
    tft.drawLine(x + 10, y + 30, x + 28, y + 30, UI_COLORS::BACKGROUND);
}

void draw_stats_icon(int16_t x, int16_t y, uint16_t color) {
    // Gráfico de barras
    tft.fillRect(x + 6, y + 26, 6, 10, color);
    tft.fillRect(x + 15, y + 16, 6, 20, color);
    tft.fillRect(x + 24, y + 8, 6, 28, color);

    // Linha de tendência
    tft.drawLine(x + 8, y + 22, x + 17, y + 12, rgb565(50, 200, 50));
    tft.drawLine(x + 17, y + 12, x + 26, y + 6, rgb565(50, 200, 50));
}

void draw_about_icon(int16_t x, int16_t y, uint16_t color) {
    // Ícone de informação (i)
    tft.fillRoundRect(x + 14, y + 4, 12, 32, 4, color);

    // Ponto no i
    tft.fillCircle(x + 20, y + 10, 3, UI_COLORS::BACKGROUND);

    // Barra no t
    tft.fillRect(x + 16, y + 16, 8, 2, UI_COLORS::BACKGROUND);
}

void draw_brightness_icon(int16_t x, int16_t y, uint16_t color) {
    // Sol com raios
    tft.fillCircle(x + 20, y + 20, 8, color);
    for (int i = 0; i < 8; i++) {
        int angle = i * 45;
        int dx = cos(radians(angle)) * 13;
        int dy = sin(radians(angle)) * 13;
        tft.drawLine(x + 20, y + 20, x + 20 + dx, y + 20 + dy, color);
    }
}

void draw_sound_icon(int16_t x, int16_t y, uint16_t color) {
    // Alto-falante
    tft.fillRoundRect(x + 4, y + 14, 8, 12, 2, color);

    // Ondas de som
    tft.drawArc(x + 16, y + 20, 4, 6, -45, 45, color);
    tft.drawArc(x + 16, y + 20, 8, 10, -45, 45, color);
}

void draw_dark_icon(int16_t x, int16_t y, uint16_t color) {
    // Lua crescente
    tft.fillCircle(x + 20, y + 20, 10, color);
    tft.fillCircle(x + 24, y + 16, 8, UI_COLORS::BACKGROUND);
}

void draw_sleep_icon(int16_t x, int16_t y, uint16_t color) {
    // Lua + Z (sono)
    draw_dark_icon(x, y, color);

    tft.setTextColor(color);
    tft.setTextSize(1);
    tft.setCursor(x + 26, y + 8);
    tft.print(F("z"));
    tft.setCursor(x + 30, y + 4);
    tft.print(F("z"));
}

void draw_reset_icon(int16_t x, int16_t y, uint16_t color) {
    // Seta circular (refresh)
    tft.drawArc(x + 20, y + 20, 10, 10, 0, 300, color);

    // Ponta da seta
    int ax = cos(radians(300)) * 10 + (x + 20);
    int ay = sin(radians(300)) * 10 + (y + 20);
    tft.fillTriangle(x + 20, y + 20, ax - 3, ay + 5, ax + 3, ay + 5, color);
}

void draw_delete_icon(int16_t x, int16_t y, uint16_t color) {
    // Lixeira
    tft.drawLine(x + 10, y + 14, x + 30, y + 14, color);
    tft.drawLine(x + 10, y + 14, x + 10, y + 34, color);
    tft.drawLine(x + 30, y + 14, x + 30, y + 34, color);
    tft.drawLine(x + 10, y + 34, x + 30, y + 34, color);

    // Tampa
    tft.drawLine(x + 6, y + 12, x + 34, y + 12, color);
    tft.drawLine(x + 16, y + 12, x + 16, y + 8, color);
    tft.drawLine(x + 24, y + 12, x + 24, y + 8, color);
    tft.drawLine(x + 16, y + 8, x + 24, y + 8, color);

    // X
    tft.drawLine(x + 14, y + 20, x + 26, y + 28, color);
    tft.drawLine(x + 26, y + 20, x + 14, y + 28, color);
}

// ============================================================================
// HELPERS GRÁFICOS
// ============================================================================
uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

uint16_t adjust_brightness(uint16_t color, int8_t delta) {
    int8_t r = (color >> 11) & 0x1F;
    int8_t g = (color >> 5) & 0x3F;
    int8_t b = color & 0x1F;
    r = constrain(r + delta, 0, 31);
    g = constrain(g + delta, 0, 63);
    b = constrain(b + delta, 0, 31);
    return (r << 11) | (g << 5) | b;
}

uint16_t color_by_value(float value, float minGood, float maxGood) {
    if (isnan(value) || isinf(value)) return UI_COLORS::UNKNOWN;

    if (value >= minGood && value <= maxGood) {
        return UI_COLORS::SUCCESS;
    } else if (value < minGood * 0.5f || value > maxGood * 2.0f) {
        return UI_COLORS::DANGER;
    } else {
        return UI_COLORS::WARNING;
    }
}

void draw_degrees_symbol(int16_t x, int16_t y, uint16_t color) {
    tft.setTextColor(color);
    tft.setTextSize(1);
    tft.print(F("°"));
}

void draw_thick_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                 uint16_t color, uint8_t thickness) {
    for (int i = 0; i < thickness; i++) {
        tft.drawLine(x0, y0 + i, x1, y1 + i, color);
    }
}

void draw_rounded_button(int16_t x, int16_t y, int16_t w, int16_t h,
                      const char* label, uint16_t bgColor,
                      uint16_t textColor, uint16_t borderColor) {
    tft.fillRoundRect(x, y, w, h, 8, bgColor);
    tft.drawRoundRect(x, y, w, h, 8, borderColor);
    tft.setTextColor(textColor);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(x + w / 2, y + h / 2);
    tft.print(label);
    tft.setTextDatum(TL_DATUM);
}

void draw_progress_bar(int16_t x, int16_t y, int16_t w, int16_t h,
                  uint8_t percentage, uint16_t fillColor, uint16_t bgColor) {
    tft.fillRoundRect(x, y, w, h, h / 2, bgColor);
    int fillW = (w - 2) * constrain(percentage, 0, 100) / 100;
    if (fillW > 0) {
        tft.fillRoundRect(x + 1, y + 1, fillW, h - 2, h / 2, fillColor);
    }
}

void draw_loading_animation(int16_t x, int16_t y, int16_t size,
                         unsigned long phase) {
    int dots = 6;
    for (int i = 0; i < dots; i++) {
        int angle = (i * 360 / dots + phase) % 360;
        int dx = cos(radians(angle)) * (size / 2 - 2);
        int dy = sin(radians(angle)) * (size / 2 - 2);
        uint16_t c = rgb565(
            (sin(radians(angle)) > 0 ? 200 : 50,
            (sin(radians(angle + 120))) > 0 ? 200 : 50,
            (sin(radians(angle + 240))) > 0 ? 200 : 50);
        tft.fillCircle(x + size / 2 + dx, y + size / 2 + dy, 3, c);
    }
}

void draw_ok_animation(int16_t x, int16_t y, uint16_t color) {
    tft.drawCircle(x, y, 16, color);
    tft.drawLine(x - 8, y, x - 2, y + 8, color);
    tft.drawLine(x - 2, y + 8, x + 10, y - 10, color);
}

void draw_error_animation(int16_t x, int16_t y, uint16_t color) {
    tft.drawCircle(x, y, 16, color);
    tft.drawLine(x - 8, y - 8, x + 8, y + 8, color);
    tft.drawLine(x + 8, y - 8, x - 8, y + 8, color);
}

void draw_virtual_led(int16_t x, int16_t y, uint16_t color, bool on) {
    uint16_t c = on ? color : adjust_brightness(color, -8);
    tft.fillCircle(x, y, 4, c);
    if (on) {
        tft.drawCircle(x, y, 4, adjust_brightness(c, 10));
    }
}

void draw_arrow_left(int16_t x, int16_t y, uint16_t color) {
    tft.drawLine(x + 6, y, x, y - 6, color);
    tft.drawLine(x, y - 6, x, y + 6, color);
    tft.drawLine(x, y + 6, x + 6, y, color);
}

void draw_arrow_right(int16_t x, int16_t y, uint16_t color) {
    tft.drawLine(x - 6, y, x, y - 6, color);
    tft.drawLine(x, y - 6, x, y + 6, color);
    tft.drawLine(x, y + 6, x - 6, y, color);
}

void draw_arrow_up(int16_t x, int16_t y, uint16_t color) {
    tft.drawLine(x, y + 6, x - 6, y, color);
    tft.drawLine(x - 6, y, x + 6, y, color);
    tft.drawLine(x + 6, y, x, y + 6, color);
}

void draw_arrow_down(int16_t x, int16_t y, uint16_t color) {
    tft.drawLine(x, y - 6, x - 6, y, color);
    tft.drawLine(x - 6, y, x + 6, y, color);
    tft.drawLine(x + 6, y, x, y - 6, color);
}

void pulse_effect(int16_t x, int16_t y, int16_t w, int16_t h,
              uint16_t color, uint8_t intensity) {
    uint16_t c = adjust_brightness(color, intensity);
    tft.drawRoundRect(x - 1, y - 1, w + 2, h + 2, 9, c);
}

void draw_measurement_value(int16_t x, int16_t y,
                     float value, uint16_t valueColor, uint16_t unitColor) {
    char buf[32];
    dtostrf(value, 8, 4, buf);
    tft.setTextColor(valueColor);
    tft.setTextSize(3);
    tft.setCursor(x, y);
    tft.print(buf);
}

void draw_gauge(int16_t cx, int16_t cy, int16_t r,
               float value, float minVal, float maxVal,
               const char* unit, uint16_t color, uint16_t bgColor) {
    tft.drawCircle(cx, cy, r, bgColor);

    float pct = (value - minVal) / (maxVal - minVal);
    pct = constrain(pct, 0.0f, 1.0f);

    int startAngle = 135;
    int endAngle = (int)(135 + pct * 270);
    tft.drawArc(cx, cy, r - 4, r - 2, startAngle, endAngle, color);

    tft.setTextColor(UI_COLORS::TEXT);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(cx, cy - 6);
    char valBuf[16];
    dtostrf(value, 6, 2, valBuf);
    tft.print(valBuf);
    tft.setTextSize(1);
    tft.setCursor(cx, cy + 8);
    tft.print(unit);
    tft.setTextDatum(TL_DATUM);
}

void draw_vertical_bar(int16_t x, int16_t y, int16_t w, int16_t maxH,
                   float percentage, uint16_t fillColor) {
    int barH = maxH * constrain(percentage, 0, 1);
    tft.fillRect(x, y + maxH - barH, w, barH, fillColor);
}

void draw_progress_bar_animated(int16_t x, int16_t y, int16_t w,
                             int16_t h, uint8_t targetPercent,
                             uint8_t& currentPercent, uint16_t fillColor,
                             uint16_t bgColor) {
    if (currentPercent < targetPercent) {
        currentPercent = min(currentPercent + 2, targetPercent);
        draw_progress_bar(x, y, w, h, currentPercent, fillColor, bgColor);
    }
}