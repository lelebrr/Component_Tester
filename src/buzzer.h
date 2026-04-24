// ============================================================================
// Component Tester PRO v3.0 — Buzzer / Speaker PAM8002
// Descrição: Controle do speaker integrado da CYD (GPIO 26) via LEDC
// ============================================================================
#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

// ============================================================================
// PROTÓTIPOS
// ============================================================================

// Inicialização do buzzer (LEDC)
void buzzer_init();

// Atualização do buzzer (chamar no loop)
void buzzer_update();

// Beep simples
void buzzer_beep(unsigned int freq);

// Beep com duração específica (ms)
void buzzer_beep_duration(unsigned int freq, unsigned long durationMs);

// Tom contínuo (para alertas)
void buzzer_tone(unsigned int freq);
void buzzer_no_tone();

// Melodia / padrão de beeps (array de {frequencia, duracao_ms})
void buzzer_play_pattern(const uint16_t* pattern, size_t count);

// Notificações predefinidas
void buzzer_ok();      // Dois beeps curtos (sucesso)
void buzzer_error();   // Tom longo grave (erro)
void buzzer_alert();   // Padrão de alerta
void buzzer_click();   // Click simples

#endif // BUZZER_H