// ============================================================================
// Component Tester PRO v3.0 — User_Setup.h (CYD Specific)
// ============================================================================
// Placa: ESP32-2432S028R (Cheap Yellow Display / ESP32-2432S028R)
//Configuração personalizada para TFT_eSPI (complementa build_flags)
// ============================================================================

#ifndef USER_SETUP_H
#define USER_SETUP_H

// ============================================================================
// DRIVER DO DISPLAY
// ============================================================================
#define ILI9341_DRIVER

// ============================================================================
// RESOLUÇÃO DO DISPLAY
// ============================================================================
#define TFT_WIDTH  320
#define TFT_HEIGHT 240

// ============================================================================
// PINOS DO DISPLAY TFT (VSPI — fixos na CYD)
// ============================================================================
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  -1  // Sem pino de reset (controlado por software)
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_MISO 12
#define TFT_BL   21

// ============================================================================
// VELOCIDADE SPI
// ============================================================================
#define SPI_FREQUENCY       40000000
#define SPI_READ_FREQUENCY 20000000

// ============================================================================
// TOUCHSCREEN XPT2046 (pinos alternativos)
// ============================================================================
#define TOUCH_CS   33
#define TOUCH_SCLK 25
#define TOUCH_MOSI 32
#define TOUCH_MISO 39

#define SPI_TOUCH_FREQUENCY 2500000

// ============================================================================
// FONTES SUPORTADAS
// ============================================================================
#define LOAD_GLCD   // Fontes ASCII padrão
#define LOAD_FONT2  //.font2 = Courier New de 16x24
#define LOAD_FONT4  //.font4 = Sans de 16x24
#define LOAD_FONT6  //font6 = Sans de 8x8
#define LOAD_FONT7  //font7 = Seven segment de 16x24
#define LOAD_FONT8  //font8 = LCD symbols
#define LOAD_GFXFF  // FreeFonts libraries
#define SMOOTH_FONT // Suavização de fontes

// ============================================================================
// OUTRAS CONFIGURAÇÕES
// ============================================================================
#define TFT_ROTATION 1  // Paisagem (conector USB à esquerda)
#define SHOW_SPLASH   // Mostrar splash screen do TFT_eSPI

#endif // USER_SETUP_H