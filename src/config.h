// ============================================================================
// Component Tester PRO v3.0 — Configuração de Hardware (CYD Edition)
// OTIMIZADO PARA ESP32 COM DUAL CORE
// ============================================================================
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================================
// VERSAO DO FIRMWARE
// ============================================================================
#define FW_VERSION        "3.0.0"
#define FW_CODENAME        "CYD Edition"
#define FW_BOARD           "ESP32-2432S028R"
#define FW_BUILD           __DATE__ " " __TIME__

// ============================================================================
// PINOS DO DISPLAY TFT (VSPI - fixo na CYD)
// ============================================================================
#define PIN_TFT_CS         15      // TFT Chip Select
#define PIN_TFT_DC         2       // TFT Data/Command
#define PIN_TFT_RST        -1      // TFT Reset (não implementado)
#define PIN_TFT_MOSI       13      // VSPI MOSI
#define PIN_TFT_SCLK       14      // VSPI Clock
#define PIN_TFT_MISO       12      // VSPI MISO
#define PIN_TFT_BL         21      // TFT Backlight

// ============================================================================
// PINOS DO TOUCH (TOUCH_VSPI)
// ============================================================================
#define PIN_TOUCH_CS       33      // Touch Chip Select
#define PIN_TOUCH_SCLK     25      // Touch Clock
#define PIN_TOUCH_MOSI     32      // Touch MOSI
#define PIN_TOUCH_MISO     39      // Touch MISO
#define PIN_TOUCH_IRQ      -1      // Touch IRQ (não utilizado)

// ============================================================================
// PINOS DO SD CARD (HSPI - dedicado)
// ============================================================================
#define PIN_SD_CS         5       // SD Card Chip Select
#define PIN_SD_MOSI       23      // HSPI MOSI
#define PIN_SD_SCLK       18      // HSPI Clock
#define PIN_SD_MISO       19      // HSPI MISO

// ============================================================================
// PINOS DE MEDICAO (ADC)
// ============================================================================
#define PIN_PROBE_1        35      // Probe 1 (ADC1_CH7)
#define PIN_PROBE_2        22      // Probe 2 (GPIO22, não ADC!)
#define PIN_ZMPT_AC        34      // Sensor Tensão AC (ADC1_CH6)
#define PIN_PROBE_1_CH     ADC1_CHANNEL_7
#define PIN_ZMPT_AC_CH     ADC1_CHANNEL_6

// ============================================================================
// PINOS DE PERIFERICOS
// ============================================================================
#define PIN_BUZZER         26      // Buzzer PWM
#define PIN_LED_GREEN      16      // LED Verde
#define PIN_LED_RED        17      // LED Vermelho
#define PIN_ONEWIRE       4       // DS18B20 OneWire

// ============================================================================
// PINOS I2C (expansão futura)
// ============================================================================
#define PIN_I2C_SDA        32      // I2C SDA (compartilhado com Touch!)
#define PIN_I2C_SCL        33      // I2C SCL (compartilhado com Touch!)
#define I2C_FREQ          100000  // I2C Frequency

// ============================================================================
// CONSTANTES ADC
// ============================================================================
#define ADC_MAX_VALUE     4095
#define ADC_REF_VOLTAGE  3.3f
#define ADC_SCALE        (ADC_REF_VOLTAGE / 4096.0f)

// ============================================================================
// CONSTANTES ZMPT (Sensor de Tensão AC)
// ============================================================================
#define ZMPT_ZERO_POINT  2048        // Offset do centro
#define ZMPT_SCALE     311.0f      // Fator de conversão (para 220V RMS)

// ============================================================================
// CONSTANTES INA219 (Sensor de Corrente)
// ============================================================================
#define INA219_ADDR      0x40      // Endereço I2C padrão
#define INA219_SHUNT     0.1f      // Shunt resistor (ohms)
#define INA219_MAX_AMPS 3.2f       // Corrente máxima teórica

// ============================================================================
// CONSTANTES DE TEMPO (us)
// ============================================================================
#define DEBOUNCE_TIME    30000     // 30ms debounce
#define UPDATE_DISP     100000    // 100ms refresh display
#define UPDATE_MEAS     50000     // 50ms refresh medições
#define CALIB_DELAY     1000000    // 1s delay calibração
#define AUTOOFF_TIME    300000000  // 5min autooff (em micsegundos!)

// ============================================================================
// RESOLUÇÕES DE TELA
// ============================================================================
#define SCREEN_W         320
#define SCREEN_H         240
#define STATUS_BAR_H     28
#define FOOTER_H         20

// ============================================================================
// CORES UI (RGB565)
// ============================================================================
#define C_BLACK         0x0000
#define C_BLUE          0x001F
#define C_RED          0xF800
#define C_GREEN        0x07E0
#define C_CYAN         0x07FF
#define C_WHITE        0xFFFF
#define C_YELLOW       0xFFE0
#define C_GREY         0x7BEF
#define C_NAVY         0x000F
#define C_ORANGE       0xFD20
#define C_PURPLE       0x780F
#define C_DARK         0x0005
#define C_DARK_GREY    0x3186
#define C_LIGHT_GREY   0xC618

// ============================================================================
// MODO DE DEBUG
// ============================================================================
#define DEBUG_MODE      0
#if DEBUG_MODE
  #define DBG(x)        Serial.println(x)
  #define DBGF(x)       Serial.print(F(x))
  #define DBG_VAL(x,y)  Serial.print(x); Serial.println(y)
#else
  #define DBG(x)
  #define DBGF(x)
  #define DBG_VAL(x,y)
#endif

// ============================================================================
// OTIMIZACOES DE MEMORIA
// ============================================================================
#define PROGMEM_READONLY  PROGMEM  // Uso para dados em flash
#define RAM_ATTR        IRAM_ATTR  // Dados acessados frequentemente

#endif