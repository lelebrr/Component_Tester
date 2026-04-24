// ============================================================================
// Sondvolt v3.x — Graficos e Icones
// Hardware: ESP32-2432S028R (Cheap Yellow Display)
// ============================================================================
// Arquivo: graphics.h
// Descricao: Prototipos de funcoes de desenho e icones vetoriais
// ============================================================================

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "config.h"

// ============================================================================
// TIPOS DE COMPONENTES
// ============================================================================
enum ComponentType {
    COMP_RESISTOR = 0,
    COMP_CAPACITOR_CERAMIC = 1,
    COMP_CAPACITOR_ELECTRO = 2,
    COMP_DIODE = 3,
    COMP_LED = 4,
    COMP_TRANSISTOR_NPN = 5,
    COMP_TRANSISTOR_PNP = 6,
    COMP_MOSFET_N = 7,
    COMP_MOSFET_P = 8,
    COMP_INDUCTOR = 9,
    COMP_CRYSTAL = 10,
    COMP_FUSE = 11,
    COMP_VARISTOR = 12,
    COMP_POTENTIOMETER = 13,
    COMP_OPTOACOPLER = 14,
    COMP_UNKNOWN = 99
};

// ============================================================================
// STATUS DE MEDICAO
// ============================================================================
enum MeasurementStatus {
    STATUS_GOOD = 0,
    STATUS_WARNING = 1,
    STATUS_BAD = 2,
    STATUS_INVALID = 3
};

// ============================================================================
// ICONES DO SISTEMA
// ============================================================================

// Prototipos das funcoes de icone
void drawIconResistor(int16_t x, int16_t y, uint16_t size, uint16_t color);
void drawIconCapacitorCeramic(int16_t x, int16_t y, uint16_t size, uint16_t color);
void drawIconCapacitorElectro(int16_t x, int16_t y, uint16_t size, uint16_t color);
void drawIconDiode(int16_t x, int16_t y, uint16_t size, uint16_t color);
void drawIconLed(int16_t x, int16_t y, uint16_t size, uint16_t color);
void drawIconTransistorNPN(int16_t x, int16_t y, uint16_t size, uint16_t color);
void drawIconTransistorPNP(int16_t x, int16_t y, uint16_t size, uint16_t color);
void drawIconMosfetN(int16_t x, int16_t y, uint16_t size, uint16_t color);
void drawIconMosfetP(int16_t x, int16_t y, uint16_t size, uint16_t color);
void drawIconInductor(int16_t x, int16_t y, uint16_t size, uint16_t color);
void drawIconCrystal(int16_t x, int16_t y, uint16_t size, uint16_t color);
void drawIconFuse(int16_t x, int16_t y, uint16_t size, uint16_t color);
void drawIconVaristor(int16_t x, int16_t y, uint16_t size, uint16_t color);
void drawIconPotentiometer(int16_t x, int16_t y, uint16_t size, uint16_t color);
void drawIconOptocoupler(int16_t x, int16_t y, uint16_t size, uint16_t color);
void drawIconUnknown(int16_t x, int16_t y, uint16_t size, uint16_t color);

// ============================================================================
// FUNCOES DE DESENHO
// ============================================================================

// Desenhar icone por tipo
void drawComponentIcon(ComponentType type, int16_t x, int16_t y, uint16_t size);

// Desenhar status
void drawStatusIndicator(MeasurementStatus status, int16_t x, int16_t y, uint16_t size);

// Desenhar simbolos graficos
void draw resistorBody(int16_t x, int16_t y, uint16_t w, uint16_t color);
void draw resistorBands(int16_t x, int16_t y, uint16_t w, uint8_t* colors, uint8_t count);
void draw capacitorCeramicBody(int16_t x, int16_t y, uint16_t w, uint16_t color);
void draw capacitorElectroBody(int16_t x, int16_t y, uint16_t w, uint16_t color);
void draw capacitorLeads(int16_t x, int16_t y, uint16_t w);
void draw diodeBody(int16_t x, int16_t y, uint16_t w, uint16_t color);
void draw diodeSymbol(int16_t cx, int16_t cy, uint16_t size, uint16_t color);
void draw ledBody(int16_t x, int16_t y, uint16_t w, uint16_t color);
void draw ledSymbol(int16_t cx, int16_t cy, uint16_t size, uint16_t color);
void draw transistorBody(int16_t x, int16_t y, uint16_t w, bool isNPN, uint16_t color);
void draw transistorSymbol(int16_t cx, int16_t cy, uint16_t size, bool isNPN, uint16_t color);
void draw mosfetBody(int16_t x, int16_t y, uint16_t w, bool isNChannel, uint16_t color);
void draw mosfetSymbol(int16_t cx, int16_t cy, uint16_t size, bool isNChannel, uint16_t color);
void draw inductorCoils(int16_t x, int16_t y, uint16_t w, uint16_t color);
void draw crystalBody(int16_t x, int16_t y, uint16_t w, uint16_t color);
void draw fuseBody(int16_t x, int16_t y, uint16_t w, uint16_t color);
void draw varistorBody(int16_t x, int16_t y, uint16_t w, uint16_t color);
void draw potentiometerBody(int16_t x, int16_t y, uint16_t w, uint16_t color);
void draw optocouplerBody(int16_t x, int16_t y, uint16_t w, uint16_t color);
void drawUnknownBody(int16_t x, int16_t y, uint16_t w, uint16_t color);

// ============================================================================
// BARRA DE STATUS
// ============================================================================
void drawStatusBar(const char* title);
void updateStatusBar(const char* text);

// ============================================================================
// RODAPE
// ============================================================================
void drawFooter(const char* left, const char* right);
void drawFooterIcon(uint8_t icon, int16_t x, int16_t y);

// ============================================================================
// ELEMENTOS UI
// ============================================================================
void drawButton(int16_t x, int16_t y, uint16_t w, uint16_t h, const char* label, bool pressed);
void drawProgressBar(int16_t x, int16_t y, uint16_t w, uint16_t h, float percent, uint16_t color);
void drawGauge(int16_t cx, int16_t cy, uint16_t radius, float value, float minVal, float maxVal, const char* unit);
void drawValueBox(int16_t x, int16_t y, uint16_t w, uint16_t h, const char* label, const char* value, const char* unit);
void drawWarningBox(const char* message);
void drawErrorBox(const char* message);
void drawConfirmBox(const char* message);

// ============================================================================
// SPLASH
// ============================================================================
void drawSplashScreen(void);

// ============================================================================
// INICIALIZACAO
// ============================================================================
void graphicsInit(TFT_eSPI* tft);

#endif // GRAPHICS_H