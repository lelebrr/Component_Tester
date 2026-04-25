// ============================================================================
// Sondvolt v3.2 — Desenhos de Componentes
// ============================================================================
// Arquivo: drawings.h
// Descricao: Comandos graficos puros para cada componente eletronico
// ============================================================================

#ifndef DRAWINGS_H
#define DRAWINGS_H

#include <TFT_eSPI.h>
#include "display_globals.h"

// Macro para facilitar acesso ao objeto tft global
#define GFX tft

// ----------------------------------------------------------------------------
// RESISTOR
// ----------------------------------------------------------------------------
inline void draw_resistor_shape(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    int16_t w = size;
    int16_t h = size / 3;
    int16_t lead = size / 6;
    
    // Terminais
    GFX.fillRect(x, y + h/2 - 1, lead, 2, C_GREY);
    GFX.fillRect(x + w - lead, y + h/2 - 1, lead, 2, C_GREY);
    
    // Corpo
    GFX.fillRoundRect(x + lead, y, w - lead*2, h, 3, color);
    GFX.drawRoundRect(x + lead, y, w - lead*2, h, 3, C_WHITE);
    
    // Faixas (exemplo)
    int16_t bodyX = x + lead;
    int16_t bodyW = w - lead*2;
    GFX.fillRect(bodyX + bodyW/4, y + 1, 2, h - 2, C_RED);
    GFX.fillRect(bodyX + bodyW/2, y + 1, 2, h - 2, C_YELLOW);
    GFX.fillRect(bodyX + 3*bodyW/4, y + 1, 2, h - 2, 0xD6BA); // Ouro/Marrom
}

// ----------------------------------------------------------------------------
// CAPACITOR CERAMICO
// ----------------------------------------------------------------------------
inline void draw_capacitor_ceramic_shape(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    int16_t r = size / 3;
    int16_t cx = x + size/2;
    int16_t cy = y + size/2;
    
    // Terminais
    GFX.drawLine(cx - 3, y, cx - 3, cy - r, C_GREY);
    GFX.drawLine(cx + 3, y, cx + 3, cy - r, C_GREY);
    GFX.drawLine(cx - 3, cy + r, cx - 3, y + size, C_GREY);
    GFX.drawLine(cx + 3, cy + r, cx + 3, y + size, C_GREY);
    
    // Corpo (Disco)
    GFX.fillCircle(cx, cy, r, color);
    GFX.drawCircle(cx, cy, r, C_WHITE);
}

// ----------------------------------------------------------------------------
// CAPACITOR ELETROLITICO
// ----------------------------------------------------------------------------
inline void draw_capacitor_electro_shape(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    int16_t w = size / 2;
    int16_t h = size * 2 / 3;
    int16_t cx = x + size/2;
    
    // Terminais
    GFX.fillRect(cx - 4, y + h, 2, size - h, C_GREY);
    GFX.fillRect(cx + 2, y + h, 2, size - h, C_GREY);
    
    // Corpo (Cilindro)
    GFX.fillRoundRect(cx - w/2, y, w, h, 2, color);
    GFX.drawRoundRect(cx - w/2, y, w, h, 2, C_WHITE);
    
    // Faixa negativa
    GFX.fillRect(cx + w/4, y + 2, w/4, h - 4, C_GREY);
}

// ----------------------------------------------------------------------------
// DIODO
// ----------------------------------------------------------------------------
inline void draw_diode_shape(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    int16_t cx = x + size/2;
    int16_t cy = y + size/2;
    int16_t s = size / 3;
    
    // Terminais
    GFX.drawLine(x, cy, cx - s, cy, C_GREY);
    GFX.drawLine(cx + s, cy, x + size, cy, C_GREY);
    
    // Símbolo
    GFX.fillTriangle(cx - s, cy - s, cx - s, cy + s, cx + s, cy, color);
    GFX.fillRect(cx + s, cy - s, 2, s * 2, C_WHITE);
}

// ----------------------------------------------------------------------------
// LED
// ----------------------------------------------------------------------------
inline void draw_led_shape(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    int16_t cx = x + size/2;
    int16_t cy = y + size/2;
    int16_t r = size / 3;
    
    // Base plana
    GFX.fillCircle(cx, cy, r, color);
    GFX.fillRect(cx - r, cy + r - 2, r * 2, 4, color);
    GFX.drawCircle(cx, cy, r, C_WHITE);
    
    // Brilho
    GFX.fillCircle(cx - r/3, cy - r/3, r/4, C_WHITE);
}

// ----------------------------------------------------------------------------
// TRANSISTOR NPN
// ----------------------------------------------------------------------------
inline void draw_transistor_npn_shape(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    int16_t cx = x + size/2;
    int16_t cy = y + size/2;
    int16_t s = size / 3;
    
    // Círculo externo
    GFX.drawCircle(cx, cy, size/2 - 2, C_GREY);
    
    // Base
    GFX.drawLine(cx - s/2, cy - s, cx - s/2, cy + s, C_WHITE);
    GFX.drawLine(x, cy, cx - s/2, cy, C_GREY); // Terminal Base
    
    // Coletor e Emissor
    GFX.drawLine(cx - s/2, cy - s/4, cx + s, cy - s, color); // Coletor
    GFX.drawLine(cx - s/2, cy + s/4, cx + s, cy + s, color); // Emissor
    
    // Seta NPN (Emissor para fora)
    GFX.fillTriangle(cx + s, cy + s, cx + s/2, cy + s, cx + s, cy + s/2, color);
}

// ----------------------------------------------------------------------------
// TRANSISTOR PNP
// ----------------------------------------------------------------------------
inline void draw_transistor_pnp_shape(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    int16_t cx = x + size/2;
    int16_t cy = y + size/2;
    int16_t s = size / 3;
    
    GFX.drawCircle(cx, cy, size/2 - 2, C_GREY);
    GFX.drawLine(cx - s/2, cy - s, cx - s/2, cy + s, C_WHITE);
    GFX.drawLine(x, cy, cx - s/2, cy, C_GREY);
    
    GFX.drawLine(cx - s/2, cy - s/4, cx + s, cy - s, color);
    GFX.drawLine(cx - s/2, cy + s/4, cx + s, cy + s, color);
    
    // Seta PNP (Emissor para dentro)
    GFX.fillTriangle(cx - s/2, cy + s/4, cx, cy + s/2, cx - s/4, cy + s/2, color);
}

// ----------------------------------------------------------------------------
// MOSFET N
// ----------------------------------------------------------------------------
inline void draw_mosfet_n_shape(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    int16_t cx = x + size/2;
    int16_t cy = y + size/2;
    int16_t s = size / 3;
    
    GFX.drawCircle(cx, cy, size/2 - 2, C_GREY);
    
    // Gate
    GFX.drawLine(cx - s/2, cy - s, cx - s/2, cy + s, C_WHITE);
    GFX.drawLine(cx - s, cy + s/2, cx - s/2, cy + s/2, C_GREY);
    
    // Canal (tracejado simplificado)
    GFX.drawLine(cx, cy - s, cx, cy - s/3, color);
    GFX.drawLine(cx, cy - s/6, cx, cy + s/6, color);
    GFX.drawLine(cx, cy + s/3, cx, cy + s, color);
    
    // Conexões
    GFX.drawLine(cx, cy - s, cx + s, cy - s, color); // Drain
    GFX.drawLine(cx, cy + s, cx + s, cy + s, color); // Source
    
    // Seta interna (N-channel aponta para o canal)
    GFX.fillTriangle(cx, cy, cx + s/2, cy - s/4, cx + s/2, cy + s/4, color);
}

// ----------------------------------------------------------------------------
// INDUTOR
// ----------------------------------------------------------------------------
inline void draw_inductor_shape(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    int16_t cy = y + size/2;
    int16_t r = size / 8;
    
    GFX.drawLine(x, cy, x + 4, cy, C_GREY);
    for(int i=0; i<4; i++) {
        GFX.drawCircleHelper(x + 8 + i*8, cy, r, 1, color);
        GFX.drawCircleHelper(x + 8 + i*8, cy, r, 2, color);
    }
    GFX.drawLine(x + size - 4, cy, x + size, cy, C_GREY);
}

// ----------------------------------------------------------------------------
// CRISTAL
// ----------------------------------------------------------------------------
inline void draw_crystal_shape(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    int16_t cx = x + size/2;
    int16_t cy = y + size/2;
    int16_t w = size / 2;
    int16_t h = size / 4;
    
    GFX.drawRect(cx - w/2, cy - h/2, w, h, color);
    GFX.drawLine(cx - w/2 - 4, cy - h, cx - w/2 - 4, cy + h, C_WHITE);
    GFX.drawLine(cx + w/2 + 4, cy - h, cx + w/2 + 4, cy + h, C_WHITE);
}

// ----------------------------------------------------------------------------
// FUSIVEL
// ----------------------------------------------------------------------------
inline void draw_fuse_shape(int16_t x, int16_t y, uint16_t size, uint16_t color) {
    int16_t cy = y + size/2;
    int16_t w = size * 2 / 3;
    int16_t h = size / 4;
    
    GFX.drawRect(x + (size-w)/2, cy - h/2, w, h, color);
    GFX.drawLine(x, cy, x + size, cy, color); // Fio interno
}

#endif // DRAWINGS_H
