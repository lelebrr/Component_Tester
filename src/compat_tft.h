// ============================================================================
// Sondvolt v3.2 — Camada de Compatibilidade TFT (Adafruit GFX)
// ============================================================================
#ifndef COMPAT_TFT_H
#define COMPAT_TFT_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "config.h"

// Fontes (Stubs para Adafruit GFX)
#define FMB         NULL
#define FMBO        NULL

// Datums (Stubs para Adafruit GFX)
#define MC_DATUM    0
#define TL_DATUM    0

// Nota: TIME_CONFIRM_TIMEOUT, ZMPT_ZERO_POINT e ZMPT_SCALE_FACTOR
// estão definidos em config.h — não redefinir aqui

// Subclasse para adicionar métodos da TFT_eSPI à Adafruit_ILI9341
class Adafruit_ILI9341_Compat : public Adafruit_ILI9341 {
public:
    using Adafruit_ILI9341::Adafruit_ILI9341;

    void drawString(const char* s, int16_t x, int16_t y) {
        setCursor(x, y);
        print(s);
    }
    
    void drawString(const String& s, int16_t x, int16_t y) {
        setCursor(x, y);
        print(s);
    }

    void setTextDatum(uint8_t datum) {
        // Adafruit GFX não suporta datums nativamente da mesma forma
    }

    void setFreeFont(const void* font) {
        // Adafruit GFX usa setFont()
        setFont((const GFXfont*)font);
    }

    bool getTouch(uint16_t* x, uint16_t* y) {
        // Integrado com a biblioteca touch global se necessário
        // Por enquanto retorna falso para evitar erros de linkagem
        return false; 
    }
};

#endif // COMPAT_TFT_H
