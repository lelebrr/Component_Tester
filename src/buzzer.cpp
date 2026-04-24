// ============================================================================
// Component Tester PRO v3.0 — Buzzer (Implementação)
// ============================================================================

#include "buzzer.h"
#include "globals.h"
#include "config.h"

// ============================================================================
// VARIÁVEIS
// ============================================================================
static bool buzzerActive = false;
static unsigned long buzzerStartTime = 0;
static unsigned long buzzerDuration = 0;
static unsigned int currentFreq = 0;

// ============================================================================
// INICIALIZAÇÃO
// ============================================================================
void buzzer_init() {
    // Configura canal LEDC para o buzzer
    ledcSetup(LEDC_CH_BUZZER, LEDC_FREQ_BUZZER, LEDC_TIMER_BIT);
    ledcAttachPin(PIN_BUZZER, LEDC_CH_BUZZER);
    ledcWrite(LEDC_CH_BUZZER, 0); // Silencioso

    DBG_PRINTF("[BUZZER] Inicializado (GPIO %d, %dHz)\n",
               PIN_BUZZER, LEDC_FREQ_BUZZER);
}

// ============================================================================
// BEEPS BÁSICOS
// ============================================================================
void buzzer_beep(unsigned int freq) {
    buzzer_beep_duration(freq, 100);
}

void buzzer_beep_duration(unsigned int freq, unsigned long durationMs) {
    if (deviceSettings.silentMode) return;

    buzzerStartTime = millis();
    buzzerDuration = durationMs;
    buzzerActive = true;
    currentFreq = freq;

    ledcSetup(LEDC_CH_BUZZER, freq, LEDC_TIMER_BIT);
    ledcAttachPin(PIN_BUZZER, LEDC_CH_BUZZER);
    ledcWrite(LEDC_CH_BUZZER, 128); // 50% duty
}

void buzzer_tone(unsigned int freq) {
    if (deviceSettings.silentMode) return;
    ledcSetup(LEDC_CH_BUZZER, freq, LEDC_TIMER_BIT);
    ledcWrite(LEDC_CH_BUZZER, 128);
    buzzerActive = true;
    currentFreq = freq;
}

void buzzer_no_tone() {
    ledcWrite(LEDC_CH_BUZZER, 0);
    buzzerActive = false;
}

// ============================================================================
// ATUALIZAÇÃO (chamar no loop)
// ============================================================================
void buzzer_update() {
    if (buzzerActive) {
        unsigned long now = millis();
        if (now - buzzerStartTime >= buzzerDuration) {
            buzzer_no_tone();
            buzzerActive = false;
        }
    }
}

// ============================================================================
// PADRÕES DE NOTIFICAÇÃO
// ============================================================================
void buzzer_ok() {
    if (deviceSettings.silentMode) return;
    buzzer_beep_duration(1000, 80);
    delay(100);
    buzzer_beep_duration(1500, 120);
}

void buzzer_error() {
    if (deviceSettings.silentMode) return;
    buzzer_beep_duration(300, 300);
}

void buzzer_alert() {
    if (deviceSettings.silentMode) return;
    buzzer_beep_duration(800, 150);
    delay(200);
    buzzer_beep_duration(600, 150);
    delay(200);
    buzzer_beep_duration(800, 150);
}

void buzzer_click() {
    if (deviceSettings.silentMode) return;
    buzzer_beep_duration(2000, 30);
}

void buzzer_play_pattern(const uint16_t* pattern, size_t count) {
    if (deviceSettings.silentMode) return;
    for (size_t i = 0; i < count; i++) {
        unsigned int freq = pattern[i * 2];
        unsigned int dur = pattern[i * 2 + 1];
        buzzer_beep_duration(freq, dur);
        delay(dur + 30);
    }
}