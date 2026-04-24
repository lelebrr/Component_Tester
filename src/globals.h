// ============================================================================
// Component Tester PRO v3.0 — Variáveis Globais e Estruturas
// Descrição: Definições globais compartilhadas entre todos os módulos
// ============================================================================
#ifndef GLOBALS_H
#define GLOBALS_H

#include "config.h"

// ============================================================================
// ESTADO DA APLICAÇÃO
// ============================================================================
#ifndef AppState
typedef enum AppState {
    STATE_SPLASH            = 0,
    STATE_MENU              = 1,
    STATE_MEASURE_RESISTOR   = 10,
    STATE_MEASURE_CAPACITOR  = 11,
    STATE_MEASURE_DIODE      = 12,
    STATE_MEASURE_TRANSISTOR = 13,
    STATE_MEASURE_INDUCTOR   = 14,
    STATE_MEASURE_IC        = 15,
    STATE_MEASURE_GENERIC   = 19,
    STATE_MULTIMETER        = 20,
    STATE_THERMAL_PROBE      = 30,
    STATE_SCANNER          = 31,
    STATE_CALIBRATION      = 32,
    STATE_SETTINGS        = 40,
    STATE_ABOUT           = 41,
    STATE_HISTORY         = 42,
    STATE_STATS           = 43
} AppState;
#endif

// ============================================================================
// INSTÂNCIA GLOBAL DO TFT
// ============================================================================
#ifndef TFT_GLOBAL_DECL
#define TFT_GLOBAL_DECL
extern Adafruit_ILI9341 tft;
#endif

// ============================================================================
// INSTÂNCIA GLOBAL DO TOUCH
// ============================================================================
extern XPT2046_Touchscreen ts;

// ============================================================================
// CONTROLE DE ENERGIA
// ============================================================================
extern unsigned long lastActivityMs;
extern bool backlightOn;

// ============================================================================
// CONFIGURAÇÕES DO DISPOSITIVO
// ============================================================================
struct Settings {
    uint8_t backlight;
    bool darkMode;
    bool silentMode;
    bool autoSleep;
    unsigned long autoSleepMs;
    bool soundEnabled;
    bool calibrated;
};
extern Settings deviceSettings;

// ============================================================================
// CALIBRAÇÃO DOS PROBES
// ============================================================================
extern float probeOffsetResistance;
extern float probeOffsetCapacitance;

// ============================================================================
// MEDIÇÕES ATUAIS
// ============================================================================
extern float lastResistance;
extern float lastCapacitance;
extern float lastInductance;
extern float lastVoltage;
extern float lastCurrent;
extern float lastTemperature;

// ============================================================================
// ESTATÍSTICAS
// ============================================================================
extern uint32_t totalMeasurements;
extern uint32_t goodMeasurements;
extern uint32_t badMeasurements;

// ============================================================================
// SD CARD
// ============================================================================
extern bool sdCardPresent;
extern bool sdCardError;

// ============================================================================
// BANCO DE DADOS
// ============================================================================
#define MAX_DB_COMPONENTS 200
struct DbItem {
    uint8_t type;
    char name[20];
    char unit[8];
    float minGood;
    float maxGood;
};
struct ComponentDatabase {
    int count;
    DbItem items[MAX_DB_COMPONENTS];
};
extern ComponentDatabase componentDB;

// ============================================================================
// RESULTADOS FREE RTOS
// ============================================================================
struct MeasurementResult {
    float resistance;
    float capacitance;
    float inductance;
    uint8_t componentType;
    uint8_t status;
};
struct DisplayMessage {
    uint8_t type;
    float value;
    const char* text;
};
#define MSG_TEMPERATURE 1
#define MSG_ALERT 2
#define MSG_RESULT 3
#define MSG_PROGRESS 4
struct LogEntry {
    unsigned long timestamp;
    char componentName[20];
    float value;
    const char* status;
};

// ============================================================================
// CONTROLE DE LEDs / BUZZER
// ============================================================================
extern unsigned long ledFlashTimer;
extern unsigned long ledFlashInterval;
extern unsigned long ledFlashPhase;
extern bool flashingLeds;
extern unsigned long buzzerStartTime;
extern unsigned long buzzerDuration;

// ============================================================================
// HISTÓRICO
// ============================================================================
struct MeasurementHistory {
    unsigned long timestamp;
    char name[20];
    float value;
    float temp;
    uint8_t status;
};
extern MeasurementHistory measurementHistory[HISTORY_SIZE];
extern int historyIndex;
extern int historyCount;

// ============================================================================
// PROTÓTIPOS GLOBAIS
// ============================================================================
void loadSettings();
void saveSettings();
void addToHistory(const char* name, float value, float temp, uint8_t status);
void buzzer_beep(unsigned int freq);
void buzzer_beep_pattern(const uint16_t* pattern, size_t len);
void led_set_rgb(uint8_t r, uint8_t g, uint8_t b);
void led_flash_rgb(uint8_t r, uint8_t g, uint8_t b,
                 unsigned long onMs, unsigned long offMs);

// Funções auxiliares
void format_measurement_value(float value, const char* unit,
                             char* output, size_t maxLen);
void menu_refresh();
void menu_back();

// Estado global (main loop precisa)
extern AppState currentAppState;
extern AppState previousAppState;

// Protótipos do main.cpp que outros módulos usam
void backlight_on();
void backlight_off();
void systemReset();

// DB (database.cpp)
bool sd_load_database();
const char* db_status_string(ComponentStatus status);
ComponentStatus db_judge(ComponentType type, float value);

// Sensors
float thermal_read_async();

// LED RGB
void led_off();

// Botões
bool btn_just_pressed(int btnId);
int menu_process_touch();

#endif // GLOBALS_H