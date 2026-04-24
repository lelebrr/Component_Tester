// ============================================================================
// Component Tester PRO v3.0 — Configuração Minimal
// ============================================================================
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define FW_VERSION      "3.0.0"
#define FW_CODENAME     "CYD Edition"
#define FW_BOARD        "ESP32-2432S028R"

// Pinos
#define PIN_TFT_CS      15
#define PIN_TFT_DC      2
#define PIN_TFT_BL      21
#define PIN_SD_CS       5
#define PIN_BUZZER      26
#define PIN_LED_GREEN   16
#define PIN_LED_RED    17
#define PIN_PROBE_1     35
#define PIN_PROBE_2     22
#define PIN_ZMPT_AC    34
#define PIN_ONEWIRE    4
#define PIN_I2C_SDA   32
#define PIN_I2C_SCL   33

// ADC
#define ADC_MAX_VALUE    4095
#define ADC_REF_VOLTAGE 3.3f

// INA219
#define INA219_I2C_ADDR 0x40

// ZMPT
#define ZMPT_ZERO_POINT   2048
#define ZMPT_SCALE_FACTOR 311.0f

// Cores
#define C_BLACK    0x0000
#define C_BLUE    0x001F
#define C_RED     0xF800
#define C_GREEN   0x07E0
#define C_CYAN   0x07FF
#define C_WHITE  0xFFFF
#define C_YELLOW 0xFFE0
#define C_GREY   0x7BEF
#define C_NAVY   0x000F
#define C_DARK   0x0005

// Configurações
#define HISTORY_SIZE     50
#define SERIAL_BAUD    115200
#define DEFAULT_BACKLIGHT 200
#define BTN_DEBOUNCE_MS 50

#define DBG_SERIAL 1
#if DBG_SERIAL
  #define DBG(x) Serial.println(x)
  #define DBGF(x) Serial.print(F(x))
#else
  #define DBG(x)
  #define DBGF(x)
#endif

#endif