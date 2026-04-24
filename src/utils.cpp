#include "utils.h"
#include "buttons.h"
#include "buzzer.h"
#include "config.h"
#include "globals.h"
#include "leds.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Print.h>

extern Adafruit_ILI9341 tft;

void fprint(Print &p, float val, int prec) {
  if (val < 0) {
    p.print('-');
    val = -val;
  }
  p.print((long)val);
  if (prec > 0) {
    p.print('.');
    float frac = val - (long)val;
    while (prec--) {
      frac *= 10;
      p.print((int)frac);
      frac -= (int)frac;
    }
  }
}

unsigned long timeoutStartMillis = 0;
unsigned long timeoutDuration = 30000;
bool timeoutActive = false;
bool holdActive = false;
bool darkMode = false;
bool silentMode = false;
unsigned long measurementCount = 0;
unsigned long faultyCount = 0;

void showBackMsg() {
  tft.setTextSize(1);
  tft.setCursor(10, 80);
  tft.println(F("BCK exits"));
}
void calibrate_probes() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(F("Calibragem"));

  tft.setTextSize(1);
  tft.setCursor(10, 40);
  tft.println(F("OK p/ iniciar"));

  bool calibrating = true;
  while (calibrating) {
    buttons_update();

    if (isOkPressed()) {
      float offset1 = analogRead(PROBE1_PIN) * 0.0048876;
      float offset2 = analogRead(PROBE2_PIN) * 0.0048876;
      tft.setCursor(10, 10);
      tft.println(F("Calib OK"));
      tft.setCursor(10, 40);
      tft.print(F("O1: "));
      fprint(tft, offset1, 2);
      tft.println('V');
      tft.setCursor(10, 60);
      tft.print(F("O2: "));
      fprint(tft, offset2, 2);
      tft.println('V');

      delay(2000);
      calibrating = false;
    }

    if (isBackPressed()) {
      calibrating = false;
    }
  }
}

void set_offsets() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(F("Offsets Atuais:"));

  tft.setTextSize(1);
  tft.setCursor(10, 40);
  tft.println(F("Offset1: 0.000V"));
  tft.setCursor(10, 60);
  tft.println(F("Offset2: 0.000V"));
  tft.setCursor(10, 80);
  tft.println(F("OK para editar"));
  delay(2000);
}

void handle_timeout() {
  if (!timeoutActive)
    return;

  if (currentMillis - timeoutStartMillis >= timeoutDuration) {
    timeoutActive = false;
    currentAppState = STATE_MENU;
    tft.fillScreen(ILI9341_BLACK);
    flash_both_leds(500);
  }
}

unsigned long safe_millis() { return currentMillis; }

void toggle_dark_mode() {
  darkMode = !darkMode;
  if (darkMode) {
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  } else {
    tft.fillScreen(ILI9341_WHITE);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
  }
}

// Função para ativar/desativar modo silencioso
void toggle_silent_mode() {
  silentMode = !silentMode;
  if (silentMode) {
    stop_beep();
  }
}

void increment_measurement_count() { measurementCount++; }

void increment_faulty_count() { faultyCount++; }

void get_measurement_stats(unsigned long *total, unsigned long *faulty) {
  *total = measurementCount;
  *faulty = faultyCount;
}

void reset_counters() {
  measurementCount = 0;
  faultyCount = 0;
}

void start_timeout(unsigned long duration) {
  timeoutStartMillis = currentMillis;
  timeoutDuration = duration;
  timeoutActive = true;
}

bool is_timeout_active() { return timeoutActive; }

void toggle_hold() {
  holdActive = !holdActive;
  if (holdActive) {
    flash_green_led_slow();
  } else {
    set_green_led(false);
  }
}

bool is_hold_active() { return holdActive; }
