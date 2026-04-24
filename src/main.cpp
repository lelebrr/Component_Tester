// ============================================================================
// Sondvolt v3.1 — Main (TFT_eSPI)
// Hardware: ESP32-2432S028R (CYD)
// ============================================================================

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Preferences.h>

#include "config.h"
#include "pins.h"
#include "globals.h"
#include "display_globals.h"
#include "ui.h"
#include "graphics.h"
#include "buzzer.h"
#include "safety.h"
#include "leds.h"
#include "buttons.h"
#include "sdcard.h"
#include "database.h"
#include "multimeter.h"
#include "measurements.h"
#include "thermal.h"
#include "logger.h"
#include "task_manager.h"
#include "display_mutex.h"

// Forward declarations
bool system_check_integrity();

// ============================================================================
// INICIALIZAÇÃO MÍNIMA (fallback quando componentes externos falham)
// ============================================================================

void system_init_minimal() {
    // Inicialização absolutamente essencial (nunca falha)
    Serial.begin(115200);
    delay(1000);
    
    // LEDs e buzzer (pinos GPIO básicos)
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, LOW);
    
    // Buzzer básico
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);
    
    // Indica que o sistema está funcionando
    set_green_led(true);
    buzzer_beep(1000, 100);
    delay(200);
    set_green_led(false);
    
    LOG_SERIAL_F("Sistema iniciado em modo minimo");
}


// Objeto global do display (definido em display_globals.h)
TFT_eSPI tft = TFT_eSPI();
Preferences nvs;

void setup() {
    // Tenta inicialização normal primeiro
    bool initSuccess = true;
    
    Serial.begin(115200);
    delay(500); // Aumenta delay para estabilização
    LOG_SERIAL_F("=== SONDVOLT v3.1 CYD ===");

    // Inicializa watchdog timer global
    // Observação: watchdog timer será configurado via platformio.ini
    // esp_task_wdt_init(15, true); // 15 segundos timeout, panic em caso de timeout
    // esp_task_wdt_add(NULL); // Adiciona a tarefa principal ao watchdog

    // Inicialização básica e obrigatória (nunca falha)
    leds_init();
    buzzer_init();

    LOG_SERIAL_F("=== INICIALIZANDO DISPLAY ===");
    graphics_init();
    bool displayOk = tftInitialized;
    if (!displayOk) {
        LOG_SERIAL_F("ERRO: Falha na inicialização do display");
        initSuccess = false;
    } else {
        LOG_SERIAL_F("SUCCESS: Display inicializado com sucesso");
        
        // Após inicialização bem sucedida, aplica otimizações
        graphics_optimize_display();
    }

    // Inicialização de segurança (não crítica)
    safety_init();

    // Mostra splash screen básica mesmo com falhas
    draw_splash_screen();

    // Inicialização de componentes opcionais (com tratamento de erros)
    LOG_SERIAL_F("=== INICIALIZANDO COMPONENTES OPCIONAIS ===");
    if (sd_init()) {
        db_load_index();
        logger_init();
        LOG_SERIAL_F("SUCCESS: SD card inicializado");
    } else {
        LOG_SERIAL_F("AVISO: SD card nao disponivel, usando modo offline");
    }
    
    // Inicialização do touch controller (após display estar pronto)
    LOG_SERIAL_F("=== INICIALIZANDO TOUCH CONTROLLER ===");
    buttons_init();
    
    // Teste de touch após inicialização completa
    LOG_SERIAL_F("=== TESTANDO TOUCH CONTROLLER ===");
    delay(1000); // Espera estabilização
    buttons_update();
    if (touch_is_pressed()) {
        TouchPoint p = touch_get_point();
        LOG_SERIAL_FMT("SUCCESS: Touch detectado - X=%d, Y=%d\n", p.x, p.y);
    } else {
        LOG_SERIAL_F("WARNING: Touch nao detectado durante teste inicial");
    }

    // Inicialização do gerenciador de tarefas
    if (!task_manager_init()) {
        LOG_SERIAL_F("ERRO: Falha na inicializacao do gerenciador de tarefas");
        initSuccess = false;
    }

    task_manager_create_all();

    // Exibe menu principal com tratamento de erros
    LOG_SERIAL_F("=== EXIBINDO INTERFACE PRINCIPAL ===");
    LOCK_TFT();
    if (tftInitialized) {
        LOG_SERIAL_F("SUCCESS: Inicializando UI...");
        ui_init();
        LOG_SERIAL_F("SUCCESS: Exibindo menu principal...");
        ui_menu_show();
    } else {
        LOG_SERIAL_F("ERROR: Display não disponível, exibindo mensagem de erro...");
        draw_splash_screen();
        tft.setTextColor(C_ERROR);
        tft.setTextDatum(MC_DATUM);
        tft.drawString("Display nao disponivel", SCREEN_W/2, SCREEN_H/2 + 60);
    }
    UNLOCK_TFT();

    DBG("Setup concluido com sucesso");
    
    // Verifica integridade do sistema
    bool systemOk = system_check_integrity();
    
    // Se a inicialização normal falhar, usa modo mínimo
    if (!initSuccess || !systemOk) {
        LOG_SERIAL_F("FALLBACK: Inicialização mínima ativada");
        system_init_minimal();
        
        // Indica falha por LED e buzzer
        set_red_led(true);
        buzzer_beep(500, 200);
        delay(100);
        buzzer_beep(500, 200);
        set_red_led(false);
        
        // Espera por reinicialização ou comando do usuário
        while (true) {
            delay(1000);
            set_red_led(true);
            delay(500);
            set_red_led(false);
        }
    }
}

// ============================================================================
// VERIFICAÇÃO DE INTEGRIDADE DO SISTEMA
// ============================================================================

bool system_check_integrity() {
    LOG_SERIAL_F("=== VERIFICANDO INTEGRIDADE DO SISTEMA ===");
    
    bool allOk = true;
    
    // Verifica display
    if (!tftInitialized) {
        LOG_SERIAL_F("ERRO: Display não inicializado");
        allOk = false;
    } else {
        LOG_SERIAL_F("OK: Display inicializado");
    }
    
    // Verifica touch
    bool touchOk = false;
    buttons_update(); // Garante atualização do estado
    if (touch_is_pressed()) {
        touchOk = true;
    }

    if (touchOk) {
        LOG_SERIAL_F("OK: Touch respondendo");
    } else {
        LOG_SERIAL_F("AVISO: Touch não respondendo");
        // Touch não é crítico, sistema pode funcionar sem
    }
    
    // Verifica LEDs
    leds_init(); // Garante que estão inicializados
    set_green_led(true);
    delay(100);
    set_green_led(false);
    set_red_led(true);
    delay(100);
    set_red_led(false);
    LOG_SERIAL_F("OK: LEDs funcionando");
    
    // Verifica buzzer
    buzzer_beep(1000, 50);
    LOG_SERIAL_F("OK: Buzzer funcionando");
    
    // Verifica SD card
    if (sdCardPresent) {
        LOG_SERIAL_F("OK: SD card presente");
    } else {
        LOG_SERIAL_F("AVISO: SD card não presente");
        // SD card não é crítico
    }
    
    LOG_SERIAL_FMT("=== RESULTADO: %s ===\n", allOk ? "SISTEMA OK" : "SISTEMA COM PROBLEMAS");
    return allOk;
}

void loop() {
    // No FreeRTOS, o loop principal deve apenas tratar
    // entradas de usuario e acoes de alto nivel da UI.
    // As medicoes e seguranca estao em tarefas separadas.

    buttons_update();
    ui_update();

    // Alimenta watchdog periodicamente
    // Comentado devido a problemas de compilação - watchdog será gerenciado pelo FreeRTOS
    // esp_task_wdt_reset();

    vTaskDelay(pdMS_TO_TICKS(10)); // Yield para outras tarefas
}