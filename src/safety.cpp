// ============================================================================
// Component Tester PRO v3.1 — Sistema de Segurança Elétrica
// Descrição: Proteção completa contra tensões perigosas (220V AC)
// Versão: CYD Edition para ESP32-2432S028R
// ============================================================================

#include "safety.h"
#include "config.h"
#include "globals.h"
#include "leds.h"
#include "buzzer.h"
#include "graphics.h"

#include <TFT_eSPI.h>
#include <Arduino.h>

// ============================================================================
// VARIÁVEIS GLOBAIS DO MÓDULO
// ============================================================================

SafetyStatus safetyStatus = {
    .state = SAFETY_STATE_SAFE,
    .alertLevel = SAFETY_ALERT_NONE,
    .lastDetectedVoltage = 0.0f,
    .hasFuseInstalled = false,
    .hasVaristorInstalled = false,
    .lockoutEndTime = 0,
    .lastCheckTime = 0,
    .dangerCount = 0,
    .safetyAcknowledged = false
};

bool safetyCheckEnabled = true;
bool safetySoundEnabled = true;
bool safetyLedEnabled = true;
bool safetyAutoLockoutEnabled = true;

// Controle de LED flash
static bool alertFlashActive = false;
static unsigned long alertFlashTimer = 0;
static unsigned long alertFlashInterval = SAFETY_LED_FLASH_FAST;
static bool alertLedState = false;

// Controle de beep
static bool alertSoundActive = false;
static unsigned long alertSoundTimer = 0;
static unsigned long alertSoundDuration = 0;

// ============================================================================
// INICIALIZAÇÃO
// ============================================================================

void safety_init() {
    Serial.println(F("[SAFETY] Initializing safety system..."));
    
    safety_reset();
    
    safetyCheckEnabled = true;
    safetySoundEnabled = true;
    safetyLedEnabled = true;
    safetyAutoLockoutEnabled = true;
    
    alertFlashActive = false;
    alertSoundActive = false;
    
    Serial.println(F("[SAFETY] Safety system ready"));
}

void safety_reset() {
    safetyStatus.state = SAFETY_STATE_SAFE;
    safetyStatus.alertLevel = SAFETY_ALERT_NONE;
    safetyStatus.lastDetectedVoltage = 0.0f;
    safetyStatus.hasFuseInstalled = false;
    safetyStatus.hasVaristorInstalled = false;
    safetyStatus.lockoutEndTime = 0;
    safetyStatus.lastCheckTime = 0;
    safetyStatus.dangerCount = 0;
    safetyStatus.safetyAcknowledged = false;
    
    safety_alert_stop();
}

// ============================================================================
// VERIFICAÇÃO
// ============================================================================

SafetyCheckResult safety_check_voltage(float voltage) {
    SafetyCheckResult result;
    
    result.detectedVoltage = voltage;
    result.isAcDanger = safety_is_ac_danger(voltage);
    result.isDcDanger = safety_is_dc_danger(voltage);
    result.isShortCircuit = false;
    
    // Determina nível de alerta
    if (voltage > SAFETY_VOLTAGE_220V_MIN) {
        result.alertLevel = SAFETY_ALERT_HIGH;
        result.message = SAFETY_MSG_DANGER_220V;
    } else if (voltage > SAFETY_VOLTAGE_DANGER_AC) {
        result.alertLevel = SAFETY_ALERT_MEDIUM;
        result.message = SAFETY_MSG_DANGER_HIGH;
    } else if (voltage > SAFETY_VOLTAGE_110V_MAX) {
        result.alertLevel = SAFETY_ALERT_LOW;
        result.message = " TENSÃO DETECTADA ";
    } else {
        result.alertLevel = SAFETY_ALERT_NONE;
        result.message = " OK ";
    }
    
    result.isSafe = (result.alertLevel == SAFETY_ALERT_NONE);
    
    // Atualiza estado global
    safetyStatus.lastDetectedVoltage = voltage;
    safetyStatus.alertLevel = result.alertLevel;
    
    return result;
}

bool safety_is_ac_danger(float voltage) {
    return (voltage >= SAFETY_VOLTAGE_DANGER_AC);
}

bool safety_is_dc_danger(float voltage) {
    return (voltage >= SAFETY_VOLTAGE_DC_DANGER);
}

bool safety_is_short_circuit(float resistance) {
    return (resistance > 0.0f && resistance < SAFETY_SHORT_CIRCUIT);
}

// ============================================================================
// ALERTAS
// ============================================================================

void safety_trigger_alert(SafetyAlertLevel level) {
    if (level == SAFETY_ALERT_NONE) {
        safety_alert_stop();
        return;
    }
    
    // Atualiza estado
    safetyStatus.state = SAFETY_STATE_DANGER;
    safetyStatus.alertLevel = level;
    safetyStatus.dangerCount++;
    
    // Ativa LED flash
    if (safetyLedEnabled) {
        switch (level) {
            case SAFETY_ALERT_CRITICAL:
            case SAFETY_ALERT_HIGH:
                alertFlashInterval = SAFETY_LED_FLASH_FAST;
                break;
            case SAFETY_ALERT_MEDIUM:
                alertFlashInterval = SAFETY_LED_FLASH_MED;
                break;
            default:
                alertFlashInterval = SAFETY_LED_FLASH_SLOW;
                break;
        }
        safety_alert_led_flash(true);
    }
    
    // Ativa alerta sonoro
    if (safetySoundEnabled) {
        safety_alert_sound_danger();
    }
}

void safety_alert_led_flash(bool enable) {
    alertFlashActive = enable;
    if (enable) {
        alertFlashTimer = millis();
        alertLedState = true;
        set_red_led(true);
    } else {
        set_red_led(false);
    }
}

void safety_alert_sound_danger() {
    if (!safetySoundEnabled) return;
    
    alertSoundActive = true;
    alertSoundTimer = millis();
    alertSoundDuration = SAFETY_BEEP_DURATION_LONG;
    
    // Toca beep de perigo
    buzzer_beep_duration(SAFETY_BEEP_DANGER, SAFETY_BEEP_DURATION_SLOW);
}

void safety_alert_sound_confirm() {
    if (!safetySoundEnabled) return;
    
    buzzer_beep_duration(SAFETY_BEEP_CONFIRM, SAFETY_BEEP_DURATION_FAST);
    delay(150);
    buzzer_beep_duration(SAFETY_BEEP_OK, SAFETY_BEEP_DURATION_FAST);
}

void safety_alert_stop() {
    alertFlashActive = false;
    alertSoundActive = false;
    set_red_led(false);
    set_green_led(false);
    buzzer_no_tone();
}

// ============================================================================
// TELA DE PERIGO
// ============================================================================

void safety_draw_danger_screen(const char* message, float voltage) {
    // Background vermelho
    tft.fillScreen(SAFETY_COLOR_ALERT_BG);
    
    // Banner de perigo
    tft.fillRect(0, 0, SCREEN_W, 50, SAFETY_COLOR_DANGER);
    
    // Ícone de perigo
    tft.setTextColor(C_WHITE);
    tft.setTextSize(3);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2, 15);
    tft.print("⚠ PERIGO ⚠");
    
    // Mensagem principal
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setCursor(SCREEN_W / 2, 70);
    tft.print(message);
    
    // Valor da tensão
    char voltageStr[16];
    dtostrf(voltage, 1, 1, voltageStr);
    
    tft.fillRoundRect(40, 95, SCREEN_W - 80, 45, 8, C_BLACK);
    tft.drawRoundRect(40, 95, SCREEN_W - 80, 45, 8, SAFETY_COLOR_DANGER);
    
    tft.setTextColor(SAFETY_COLOR_DANGER);
    tft.setTextSize(4);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2, 118);
    tft.print(voltageStr);
    
    tft.setTextSize(2);
    tft.setCursor(SCREEN_W / 2 + 90, 118);
    tft.print("V");
    
    // Instrução
    tft.setTextColor(C_WHITE);
    tft.setTextSize(1);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2, 160);
    tft.print(SAFETY_MSG_REMOVE);
    
    // Caixa de instrução
    tft.fillRoundRect(20, 180, SCREEN_W - 40, 35, 6, C_BLACK);
    tft.drawRoundRect(20, 180, SCREEN_W - 40, 35, 6, C_WHITE);
    
    tft.setTextColor(C_YELLOW);
    tft.setTextSize(1);
    tft.setCursor(SCREEN_W / 2, 198);
    tft.print("PRESSIONE OK PARA RECONHECER");
    
    tft.setTextDatum(TL_DATUM);
}

void safety_draw_lockout_screen(unsigned long remainingMs) {
    clear_screen();
    
    // Background escuro
    tft.fillScreen(C_BLACK);
    
    // Banner de bloqueio
    tft.fillRect(0, 0, SCREEN_W, 45, C_RED);
    
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2, 15);
    tft.print("⚠ BLOQUEADO ⚠");
    
    // Mensagem
    tft.setTextColor(C_RED);
    tft.setTextSize(1);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2, 60);
    tft.print("DETECTADA TENSÃO PERIGOSA");
    
    tft.setCursor(SCREEN_W / 2, 80);
    tft.print("EQUIPAMENTO BLOQUEADO");
    
    // Timer
    unsigned long seconds = (remainingMs / 1000) + 1;
    
    tft.fillRoundRect(SCREEN_W / 2 - 60, 105, 120, 50, 8, C_DARK_GREY);
    tft.drawRoundRect(SCREEN_W / 2 - 60, 105, 120, 50, 8, C_RED);
    
    tft.setTextColor(C_WHITE);
    tft.setTextSize(3);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2, 130);
    if (seconds >= 60) {
        tft.print("0:00");
    } else {
        char timeStr[8];
        snprintf(timeStr, sizeof(timeStr), "%lu", seconds);
        tft.print(timeStr);
    }
    
    tft.setTextSize(1);
    tft.setCursor(SCREEN_W / 2, 160);
    tft.print("SEGUNDOS RESTANTES");
    
    // Barra de progresso
    uint8_t progress = (uint8_t)((SAFETY_LOCKOUT_MS - remainingMs) * 100 / SAFETY_LOCKOUT_MS);
    draw_progress_bar(30, 180, SCREEN_W - 60, 10, progress, C_RED, C_DARK_GREY);
    
    tft.setTextColor(C_GREY);
    tft.setCursor(SCREEN_W / 2, 200);
    tft.setTextDatum(TC_DATUM);
    tft.print("AGUARDE DESBLOQUEIO...");
    
    tft.setTextDatum(TL_DATUM);
}

void safety_draw_confirm_screen() {
    clear_screen();
    
    // Background
    tft.fillScreen(C_BLACK);
    
    // Banner amarelo de aviso
    tft.fillRect(0, 0, SCREEN_W, 50, C_YELLOW);
    
    tft.setTextColor(C_BLACK);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2, 15);
    tft.print("⚠ AVISO DE SEGURANÇA ⚠");
    
    // Conteúdo
    tft.setTextColor(C_WHITE);
    tft.setTextSize(1);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2, 70);
    tft.print("Você está prestes a medir");
    
    tft.setTextColor(C_YELLOW);
    tft.setCursor(SCREEN_W / 2, 90);
    tft.print("TENSÃO DA REDE ELÉTRICA!");
    
    // Aviso importante
    tft.fillRoundRect(20, 110, SCREEN_W - 40, 50, 6, C_DARK_GREY);
    tft.drawRoundRect(20, 110, SCREEN_W - 40, 50, 6, C_RED);
    
    tft.setTextColor(C_YELLOW);
    tft.setCursor(SCREEN_W / 2, 125);
    tft.setTextDatum(TC_DATUM);
    tft.print("INSTALE FUSÍVEL E VARISTOR");
    tft.setCursor(SCREEN_W / 2, 145);
    tft.print("ANTES DE CONTINUAR!");
    
    // Botões de confirmação
    int16_t btnW = 120;
    int16_t btnH = 35;
    int16_t btnY = 180;
    
    // Botão CONFIRMAR (verde)
    tft.fillRoundRect(SCREEN_W / 2 - btnW - 10, btnY, btnW, btnH, 6, C_GREEN);
    tft.drawRoundRect(SCREEN_W / 2 - btnW - 10, btnY, btnW, btnH, 6, C_WHITE);
    
    tft.setTextColor(C_BLACK);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2 - btnW / 2 - 10, btnY + btnH / 2);
    tft.setTextSize(1);
    tft.print("TENHO");
    
    tft.setCursor(SCREEN_W / 2 - btnW / 2 - 10, btnY + btnH / 2 + 12);
    tft.print("PROTEÇÃO");
    
    // Botão CANCELAR (vermelho)
    tft.fillRoundRect(SCREEN_W / 2 + 10, btnY, btnW, btnH, 6, C_RED);
    tft.drawRoundRect(SCREEN_W / 2 + 10, btnY, btnW, btnH, 6, C_WHITE);
    
    tft.setTextColor(C_WHITE);
    tft.setCursor(SCREEN_W / 2 + btnW / 2 + 10, btnY + btnH / 2);
    tft.print("CANCELAR");
    
    tft.setTextDatum(TL_DATUM);
}

void safety_draw_check_screen() {
    clear_screen();
    
    // Banner
    tft.fillRect(0, 0, SCREEN_W, 40, C_YELLOW);
    
    tft.setTextColor(C_BLACK);
    tft.setTextSize(1);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2, 12);
    tft.print("VERIFICAÇÃO DE SEGURANÇA");
    
    // Instrução
    tft.setTextColor(C_WHITE);
    tft.setCursor(SCREEN_W / 2, 60);
    tft.setTextSize(1);
    tft.print("Desconecte as pontas de prova");
    tft.setCursor(SCREEN_W / 2, 80);
    tft.print("antes de selecionar modo AC");
    
    // Indicador
    tft.fillCircle(SCREEN_W / 2, 120, 30, C_GREEN);
    tft.drawCircle(SCREEN_W / 2, 120, 30, C_WHITE);
    
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2, 118);
    tft.print("✓");
    
    tft.setTextDatum(TL_DATUM);
}

// ============================================================================
// BLOQUEIO
// ============================================================================

void safety_activate_lockout() {
    if (!safetyAutoLockoutEnabled) return;
    
    safetyStatus.state = SAFETY_STATE_LOCKOUT;
    safetyStatus.lockoutEndTime = millis() + SAFETY_LOCKOUT_MS;
    safetyStatus.dangerCount = 0;
    
    Serial.println(F("[SAFETY] Lockout activated"));
    
    safety_alert_stop();
}

void safety_deactivate_lockout() {
    safetyStatus.state = SAFETY_STATE_SAFE;
    safetyStatus.lockoutEndTime = 0;
    safetyStatus.safetyAcknowledged = false;
    
    Serial.println(F("[SAFETY] Lockout deactivated"));
}

bool safety_is_locked_out() {
    if (safetyStatus.state == SAFETY_STATE_LOCKOUT) {
        if (millis() >= safetyStatus.lockoutEndTime) {
            // Verifica se tempo acabou
            if (safetyStatus.safetyAcknowledged) {
                safety_deactivate_lockout();
                return false;
            } else {
                // Mantém bloqueado até reconhecimento
                return true;
            }
        }
        return true;
    }
    return false;
}

bool safety_can_proceed() {
    // Verifica se não está bloqueado
    if (safety_is_locked_out()) {
        return false;
    }
    
    // Verifica estado de segurança
    if (safetyStatus.state == SAFETY_STATE_DANGER) {
        if (safetyStatus.safetyAcknowledged) {
            return true;
        }
        return false;
    }
    
    return true;
}

void safety_acknowledge_warning() {
    safetyStatus.safetyAcknowledged = true;
    safety_alert_stop();
    
    // Se estava em bloqueio, inicia contagem regressiva
    if (safetyStatus.state == SAFETY_STATE_LOCKOUT) {
        safetyStatus.state = SAFETY_STATE_WARNING;
    }
}

// ============================================================================
// SPLASH DE SEGURANÇA
// ============================================================================

void safety_draw_splash() {
    // Background
    tft.fillScreen(C_BLACK);
    
    // Banner de segurança
    tft.fillRect(0, 0, SCREEN_W, 45, C_RED);
    
    // Ícone de peringatan
    tft.setTextColor(C_WHITE);
    tft.setTextSize(3);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2, 15);
    tft.print("⚠ SEGURANÇA ⚠");
    
    // Título
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2, 65);
    tft.print("SISTEMA DEPROTEÇÃO");
    
    tft.setCursor(SCREEN_W / 2, 85);
    tft.setTextColor(C_YELLOW);
    tft.print("220V AC ATIVO");
    
    // Advertências
    tft.setTextColor(C_WHITE);
    tft.setTextSize(1);
    tft.setCursor(SCREEN_W / 2, 115);
    tft.setTextDatum(TC_DATUM);
    tft.print("NÃO CONECTE DIRETAMENTE EM");
    
    tft.setTextColor(C_YELLOW);
    tft.setCursor(SCREEN_W / 2, 135);
    tft.print("220V SEM PROTEÇÃO!");
    
    // Lista de proteção
    tft.setTextColor(C_GREY);
    tft.setCursor(SCREEN_W / 2, 165);
    tft.setTextDatum(TC_DATUM);
    tft.print("• Fusível de proteção");
    tft.setCursor(SCREEN_W / 2, 185);
    tft.print("• Varistor (MOV)");
    
    // Footer
    tft.setTextColor(C_DARK_GREY);
    tft.setTextSize(1);
    tft.setCursor(SCREEN_W / 2, 215);
    tft.setTextDatum(TC_DATUM);
    tft.print("Component Tester PRO v3.1");
    
    tft.setTextDatum(TL_DATUM);
}

void safety_show_splash_animated() {
    safety_draw_splash();
    
    // Animação de pulsação
    for (int i = 0; i < 3; i++) {
        for (int alpha = 255; alpha >= 100; alpha -= 25) {
            // Pulsing effect
            delay(30);
        }
        for (int alpha = 100; alpha <= 255; alpha += 25) {
            delay(30);
        }
    }
    
    delay(1500);
}

// ============================================================================
// DETECÇÃO AUTOMÁTICA
// ============================================================================

SafetyCheckResult safety_detect_danger() {
    SafetyCheckResult result;
    
    // Lê tensão atual (simulado - em produção usar multimeter)
    extern float lastVoltage;
    float voltage = lastVoltage;
    
    result = safety_check_voltage(voltage);
    
    // Se detectar perigo, ativa alertas
    if (!result.isSafe) {
        safety_trigger_alert(result.alertLevel);
        
        // Se nível alto, ativa bloqueio
        if (result.alertLevel >= SAFETY_ALERT_HIGH) {
            safety_activate_lockout();
        }
    }
    
    safetyStatus.lastCheckTime = millis();
    
    return result;
}

// ============================================================================
// MENU
// ============================================================================

bool safety_check_before_multimeter() {
    // Se verificação desativada, permite
    if (!safetyCheckEnabled) {
        return true;
    }
    
    // Se já está bloqueio, mostra tela de bloqueio
    if (safety_is_locked_out()) {
        unsigned long remaining = 0;
        if (safetyStatus.lockoutEndTime > millis()) {
            remaining = safetyStatus.lockoutEndTime - millis();
        }
        
        // Loop de bloqueio
        while (safety_is_locked_out()) {
            if (safetyStatus.lockoutEndTime > millis()) {
                remaining = safetyStatus.lockoutEndTime - millis();
                safety_draw_lockout_screen(remaining);
            } else {
                // Tempo acabou, precisa reconhecer
                safety_draw_danger_screen(SAFETY_MSG_REMOVE, safetyStatus.lastDetectedVoltage);
            }
            
            delay(100);
        }
        
        return false;
    }
    
    // Verifica se estado perigo
    if (safetyStatus.state == SAFETY_STATE_DANGER && !safetyStatus.safetyAcknowledged) {
        safety_draw_danger_screen(SAFETY_MSG_DANGER_HIGH, safetyStatus.lastDetectedVoltage);
        
        // Aguarda reconhecimento
        delay(500);
        
        return false;
    }
    
    return true;
}

bool safety_confirm_electrical_measurement() {
    // Desenha tela de confirmação
    safety_draw_confirm_screen();
    
    // Aguarda input do usuário
    // Nota: Em implementação real, usar buttons/touch
    delay(2000);
    
    // Por padrão, retorna false (usuário precisa confirmar)
    // Em implementação real, verificar botão pressionado
    
    safety_alert_sound_confirm();
    
    return false;
}

// ============================================================================
// ATUALIZAÇÃO
// ============================================================================

void safety_update() {
    unsigned long now = millis();
    
    // Atualiza LED flash
    if (alertFlashActive) {
        if (now - alertFlashTimer >= alertFlashInterval) {
            alertFlashTimer = now;
            alertLedState = !alertLedState;
            set_red_led(alertLedState);
        }
    }
    
    // Atualiza beep
    if (alertSoundActive) {
        if (now - alertSoundTimer >= alertSoundDuration) {
            alertSoundActive = false;
            buzzer_no_tone();
        }
    }
    
    // Verifica fim de bloqueio
    if (safetyStatus.state == SAFETY_STATE_LOCKOUT) {
        if (now >= safetyStatus.lockoutEndTime) {
            if (!safetyStatus.safetyAcknowledged) {
                // Mostra tela de perigo para reconhecimento
                safety_draw_danger_screen(SAFETY_MSG_DANGER_220V, safetyStatus.lastDetectedVoltage);
            }
        }
    }
}

// ============================================================================
// CONFIGURAÇÃO
// ============================================================================

void safety_set_check_enabled(bool enabled) {
    safetyCheckEnabled = enabled;
}

void safety_set_sound_enabled(bool enabled) {
    safetySoundEnabled = enabled;
}

void safety_set_led_enabled(bool enabled) {
    safetyLedEnabled = enabled;
}

void safety_set_lockout_enabled(bool enabled) {
    safetyAutoLockoutEnabled = enabled;
}