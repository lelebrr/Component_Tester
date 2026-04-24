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

// Objeto global do display (definido em display_globals.h)
TFT_eSPI tft = TFT_eSPI();
Preferences nvs;

void setup() {
    Serial.begin(115200);
    delay(100);
    LOG_SERIAL_F("=== SONDVOLT v3.1 CYD ===");

    leds_init();
    buzzer_init();

    ui_init();

    safety_show_splash_animated();
    safety_init();

    ui_show_splash_animated();

    if (sdCardPresent) {
        db_load_index();
        logger_init();
    }

    task_manager_init();
    task_manager_create_all();

    ui_menu_show();

    DBG("Setup concluido com sucesso");
}

void loop() {
    safety_update();

    buttons_update();
    ui_menu_update();

    buzzer_update();
    update_leds();

    measurement_update();

    logger_update();

    task_manager_list_tasks();

    delay(5);
}