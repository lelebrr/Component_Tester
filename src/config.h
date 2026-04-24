// ============================================================================
// Component Tester PRO v3.0 — Configuração de Hardware
// Placa: ESP32-2432S028R (Cheap Yellow Display)
// Descrição: Definições de pinos, constantes e configurações do sistema
// ============================================================================
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================================
// INFORMAÇÕES DO FIRMWARE
// ============================================================================
#define FW_NAME           "CT_PRO"
#define FW_VERSION        "3.0.0"
#define FW_CODENAME       "CYD_Edition"
#define FW_BOARD          "ESP32-2432S028R"
#define FW_AUTHOR         "lelebrr"

// ============================================================================
// PINOS DO DISPLAY TFT ILI9341 (Interface SPI — fixos na CYD)
// Usa VSPI (default): MOSI=23, MISO=19, SCLK=18
// ============================================================================
#define PIN_TFT_CS       15      // Chip Select do display
#define PIN_TFT_DC        2       // Data/Command
#define PIN_TFT_RST     -1       // Reset (-1 = controlado por software)
#define PIN_TFT_BL       21       // Backlight (PWM control)
#define PIN_TFT_SCK      18       // Clock (VSPI)
#define PIN_TFT_MISO     19       // MISO (VSPI)
#define PIN_TFT_MOSI     23       // MOSI (VSPI)

#define TFT_SPI_SPEED    40000000UL   // 40MHz — máxima estável para ILI9341

// Resolução do display
#define TFT_WIDTH         320
#define TFT_HEIGHT        240
#define TFT_ROTATION      0              // 0=Retrato, 1=Paisagem (recomendado)
#define TFT_INVERTION    1              // 1=inverter cores (0x93 is common)
#define TFT_COLORSWAP     0

// ============================================================================
// PINOS DO TOUCH XPT2046 (Interface SPI separada — bitbang)
// ============================================================================
#define PIN_TOUCH_CS     33
#define PIN_TOUCH_SCLK   25
#define PIN_TOUCH_MOSI   32
#define PIN_TOUCH_MISO   39
#define PIN_TOUCH_IRQ    36    // IRQ (Interrupt Request)
#define TOUCH_SPI_SPEED  2500000UL     // 2.5MHz
#define TOUCH_MIN_Z      100           // Pressão mínima detectada
#define TOUCH_MAX_Z      1000          // Pressão máxima
#define TOUCH_X_MIN      200           // Valores de calibração
#define TOUCH_X_MAX      3800
#define TOUCH_Y_MIN      200
#define TOUCH_Y_MAX      3800
#define TOUCH_XY_SWAP    1
#define TOUCH_X_INVERT   0
#define TOUCH_Y_INVERT   1

// ============================================================================
// PINOS DO SD CARD (Interface SPI — HSPI separado do display)
// HSPI: MOSI=13, MISO=12, SCLK=14, CS=5
// NOTA: A CYD usa HSPI para o SD, MAS os pinos físicos são:
// HSPI conector: MOSI=15, MISO=2, SCLK=14, CS=5
// Na verdade os pinos fisicos do SD na CYD sao: CS=5, CLK=18, MOSI=23, MISO=19
// Confirmação: o SD da CYD é conectado ao HSPI do ESP32
// Pinos HSPI: GPIO14=CLK, GPIO12=MISO, GPIO13=MOSI, GPIO15=CS
// Porém o hardware da CYD pode ter fiação diferente, então testamos
// ============================================================================
#define PIN_SD_CS       5
#define PIN_SD_SCK      18
#define PIN_SD_MOSI     23
#define PIN_SD_MISO     19
#define SD_SPI_SPEED    4000000UL      // 4MHz para SD card
#define SD_CARD_DETECT  1     // Detecção de cartão (se disponível)

// ============================================================================
// PINOS DE MEDIÇÃO (Sistema de Probes)
// Esses pinos são para testes de componentes (LCMeter/Transistor Tester)
// ============================================================================
#define PIN_PROBE_1     35   // Probe 1 (canal ADC1 — WiFi free)
#define PIN_PROBE_2     22   // Probe 2 (GPIO livre)
#define PIN_CHARGE_CAP   22   // Carregamento de capacitor (mesmo pino)

// ============================================================================
// PINOS DO SENSOR ZMPT101B (Tensão AC)
// Conectado ao ADC do ESP32
// ============================================================================
#define PIN_ZMPT_AC     34    // Canal ADC1_6 (WiFi free)

// ============================================================================
// PINOS DO SENSOR INA219 (Corrente DC / Tensão DC)
// Interface I2C (GPIO 21=SDA, GPIO 22=SCL)
// ============================================================================
#define PIN_I2C_SDA     21
#define PIN_I2C_SCL     22
#define INA219_I2C_ADDR  0x40          // Endereço I2C do INA219
#define INA219_SHUNT_R   0.1f         // Resistor shunt em ohms (0.1Ω)

// ============================================================================
// PINOS DA SONDA TÉRMICA DS18B20 (OneWire)
// Conectado a um GPIO livre
// ============================================================================
#define PIN_ONEWIRE    4

// ============================================================================
// PINOS DO SPEAKER PAM8002 (já integrado na CYD)
// Usa GPIO 26 com controle PWM via LEDC
// ============================================================================
#define PIN_BUZZER      26

// ============================================================================
// PINOS DO RGB LED WS2812B (já integrado na CYD)
// Controlado via RMT (Remote Control)
// ============================================================================
#define PIN_RGB_LED    17
#define RGB_LED_COUNT   1

// ============================================================================
// CONFIGURAÇÃO DO LEDC (PWM do ESP32)
// Usado para: backlight do display, buzzer
// ============================================================================
#define LEDC_TIMER_BIT   8              // Resolução de 8 bits (0-255)
#define LEDC_MODE        LEDC_HIGH_SPEED_MODE

// Canal do buzzer
#define LEDC_CH_BUZZER     0
#define LEDC_FREQ_BUZZER   2000          // 2kHz para buzzers piezelétricos
#define LEDC_DUTY_BUZZER  128           // 50% duty cycle

// Canal do backlight
#define LEDC_CH_BACKLIGHT 1
#define LEDC_FREQ_BL      5000          // 5kHz (indetectável pelo olho)
#define LEDC_DUTY_DEFAULT  100            // Brilho padrão (40%)

// ============================================================================
// CONFIGURAÇÃO DO ADC (ESP32 — 12 bits, 3.3V)
// ESP32 tem 2 ADCs: ADC1 (WiFi) e ADC2 (disponível durante WiFi)
// Para medições precisas, usar ADC1 (pinos 32-39)
// ============================================================================
#define ADC_RESOLUTION     12             // 12 bits
#define ADC_MAX_VALUE     4095          // 2^12 - 1
#define ADC_REF_VOLTAGE    3.3f
#define ADC_VOLTS_PER_BIT  (ADC_REF_VOLTAGE / ADC_MAX_VALUE)

// Calibração ADC do ESP32 (高原浩典)
#define ADC_CAL_EFUSE    1              // Usar calibração eFuse se disponível
#define ADC_ATTEN        ADC_ATTEN_11db // Atenuação 11dB = 0-3.3V range

// ============================================================================
// CONFIGURAÇÃO DO ZMPT101B (Sensor de Tensão AC)
// ============================================================================
#define ZMPT_SAMPLES       1000        // Amostras por medição
#define ZMPT_SAMPLE_RATE    10000         // Taxa de amostragem (Hz)
#define ZMPT_ZERO_POINT     2048          // Ponto zero (meio de 4096)
#define ZMPT_SCALE_FACTOR   311.0f      // Fator de escala (√2 × 220V ≈ 311V)

// ============================================================================
// CONFIGURAÇÕES DO SISTEMA DE MEDIÇÃO
// ============================================================================
// Limiar de tensão para detectar componente
#define THRESHOLD_OPEN        10           // >10kΩ = aberto (desconectado)
#define THRESHOLD_SHORT      5            // <5Ω = curto circuito
#define THRESHOLD_DIODE_FWD  200          // 200mV = queda direta típica de diodo
#define THRESHOLD_LEAKAGE    10000        // >10kΩ = fuga excessiva

// Tempo de medição (ms)
#define MEASURE_DELAY_SHORT  50
#define MEASURE_DELAY_LONG  200
#define MEASURE_TIMEOUT      5000

// ============================================================================
// LIMITES DE TEMPERATURA (DS18B20)
// ============================================================================
#define TEMP_NORMAL_MAX      60.0f     // Temperatura normal
#define TEMP_WARNING         80.0f     // Alerta (amarelo)
#define TEMP_CRITICAL        100.0f   // Crítico (vermelho)

// ============================================================================
// CORES DO SISTEMA (formato RGB565 — para Adafruit GFX)
// Formato: RRRR RGGG GGGB BBBB (16 bits)
// ============================================================================
namespace UI_COLORS {
    // Fundos
    const uint16_t BACKGROUND      = 0x10A2;  // Azul escuro (#102030)
    const uint16_t BACKGROUND_ALT   = 0x0841;  // Azul mais escuro
    const uint16_t CARD_BG          = 0x18E3;  // Azul acinzentado

    // Cores principais
    const uint16_t PRIMARY          = 0x0DFE;  // Ciano (#00BCD4)
    const uint16_t PRIMARY_DARK     = 0x07BE;  // Ciano escuro
    const uint16_t ACCENT         = 0x07FF;  // Ciano claro

    // Texto
    const uint16_t TEXT            = 0xFFFF;  // Branco
    const uint16_t TEXT_DIM        = 0xBDF7;  // Branco dim
    const uint16_t TEXT_HINT       = 0x7BEF;  // Cinza azulado

    // Status
    const uint16_t SUCCESS         = 0x07E0;  // Verde (#4CAF50)
    const uint16_t SUCCESS_LIGHT  = 0x4F81;  // Verde claro
    const uint16_t WARNING        = 0xFD20;  // Laranja (#FF9800)
    const uint16_t DANGER         = 0xF800;  // Vermelho (#F44336)
    const uint16_t DANGER_LIGHT   = 0xF52F;  // Vermelho claro

    // Cores especiais
    const uint16_t HILIGHT        = 0x03EF;  // Azul highlight
    const uint16_t BORDER         = 0x0841;  // Borda
    const uint16_t SEPARATOR      = 0x18C3;  // Separador

    // Componentes
    const uint16_t RESISTOR       = 0xDED1;  // Marrom (#A0A0A0)
    const uint16_t CAPACITOR      = 0x03EF;  // Azul (#2196F3)
    const uint16_t DIODE         = 0xFCFF;  // Amarelo (#FFEB3B)
    const uint16_t TRANSISTOR    = 0x4F81;  // Verde claro
    const uint16_t IC            = 0x8410;  // Azul escuro (#333333)
    const uint16_t UNKNOWN       = 0x7BEF;  // Cinza

    // Barra de status
    const uint16_t STATUS_BG      = 0x0008;  // Fundo escuro
    const uint16_t STATUS_TEXT    = 0x4DF7;  // Texto claro

    // Splash
    const uint16_t SPLASH_BG      = 0x18E3;  // Fundo do splash
    const uint16_t SPLASH_ACCENT  = 0x0DFE;  // Destaque do splash
}

// ============================================================================
// LAYOUT DA UI
// ============================================================================
#define STATUS_BAR_HEIGHT   28          // Altura da barra de status
#define FOOTER_HEIGHT       20          // Altura do rodapé
#define CONTENT_Y           STATUS_BAR_HEIGHT  // Início do conteúdo

// Grid do menu principal
#define MENU_GRID_COLS      2           // Colunas do grid
#define MENU_GRID_ROWS      3           // Linhas do grid
#define MENU_ITEM_PADDING   8           // Padding interno dos cards
#define MENU_GRID_GAP       6           // Gap entre cards

// Cards de medição
#define CARD_PADDING        12           // Padding interno
#define CARD_BORDER_RADIUS 10           // Raio das bordas arredondadas
#define CARD_ICON_SIZE      48           // Tamanho do ícone

// Tela de medição
#define RESULT_VALUE_SIZE  4            // Tamanho do valor principal
#define RESULT_UNIT_SIZE   2             // Tamanho da unidade
#define RESULT_LABEL_SIZE  2             // Tamanho do label

// ============================================================================
// NAVEGAÇÃO E TOQUE
// ============================================================================
#define BTN_DEBOUNCE_MS      50          // Debounce dos botões físicos
#define TOUCH_DEBOUNCE_MS    150          // Debounce do touch
#define TOUCH_LONG_PRESS_MS  500         // Pressão longa
#define TOUCH_SWIPE_MIN_DX  30           // Distância mínima para swipe

// ============================================================================
// TIMINGS DO SISTEMA
// ============================================================================
#define SPLASH_DURATION_MS     3000        // Duração do splash
#define AUTO_SLEEP_MS         60000       // Tempo para dormir (1 min)
#define BACKLIGHT_FADE_MS    500         // Fade do backlight
#define LOG_SAVE_INTERVAL_MS  5000        // Intervalo para salvar log
#define HISTORY_SIZE         50          // Tamanho do histórico

// ============================================================================
// SD CARD
// ============================================================================
#define SD_SPI_FREQ          4000000UL   // 4MHz
#define SD_RETRY_COUNT       3           // Tentativas de acesso
#define SD_RETRY_DELAY_MS     100         // Delay entre tentativas

// Caminhos dos arquivos
#define DB_INDEX_PATH         "/ct_db.idx"     // Índice do banco de dados
#define DB_COMPONENTS_PATH  "/ct_db.csv"     // Componentes
#define LOG_FILE_PATH       "/ct_log.csv"     // Log de medições
#define CALIB_FILE_PATH     "/ct_calib.dat"   // Calibração
#define SETTINGS_FILE_PATH  "/ct_settings.dat" // Configurações

// ============================================================================
// FREE RTOS — TASKS
// ============================================================================
#define TASK_PRIORITY_HIGH    5          // Prioridade alta (UI)
#define TASK_PRIORITY_MEDIUM   3          // Prioridade média (medição)
#define TASK_PRIORITY_LOW      1          // Prioridade baixa (logging)

// Stacks sizes (em words = 4 bytes no ESP32)
#define TASK_STACK_MEASURE   4096         // Task de medição
#define TASK_STACK_UI       3072         // Task de UI
#define TASK_STACK_SD       2048         // Task de SD/logging
#define TASK_STACK_THERMAL   2048         // Task térmica

// ============================================================================
// SERIAL DEBUG
// ============================================================================
#define SERIAL_BAUD          115200
#if 1
    #define DBG_PRINT(x)       Serial.print(x)
    #define DBG_PRINTLN(x)     Serial.println(x)
    #define DBG_PRINTF(f, ...) Serial.printf(f, ##__VA_ARGS__)
#else
    #define DBG_PRINT(x)
    #define DBG_PRINTLN(x)
    #define DBG_PRINTF(f, ...)
#endif

// ============================================================================
// VERSÃO DO BANCO DE DADOS
// ============================================================================
#define DB_VERSION          3           // Versão do formato
#define DB_MAGIC_NUMBER     0x43545052  // "CTPR" (Component Tester PRO)

#endif // CONFIG_H