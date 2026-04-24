#include "buttons.h"
#include "buzzer.h"
#include "config.h"
#include "database.h"
#include "drawings.h"
#include "globals.h"
#include "leds.h"
#include "logger.h"
#include "menu.h"
#include "thermal.h"
#include "utils.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <EEPROM.h>
#include <Arduino.h>
#include <SPI.h>

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS_PIN, TFT_DC_PIN, TFT_RST_PIN);

enum AppState currentAppState = STATE_SPLASH;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
bool greenLedState = false;
bool redLedState = false;
float currentTemperature = 0.0;
int currentMenuItem = 0;
int totalMenuItems = 0;

MeasurementHistory measurementHistory[HISTORY_SIZE];
int historyIndex = 0;
int historyCount = 0;

Settings deviceSettings = {0.0, 0.0, false, false, 30000, 0, 0};

#define TFT_ROTATION 1

#include "measurements.h"

void loadSettings() {
  if (EEPROM.read(0) == 0xAB) {
    EEPROM.get(1, deviceSettings.offset1);
    EEPROM.get(5, deviceSettings.offset2);
    deviceSettings.darkMode = EEPROM.read(9);
    deviceSettings.silentMode = EEPROM.read(10);
    unsigned long storedTimeout;
    EEPROM.get(11, storedTimeout);
    if (storedTimeout > 0) deviceSettings.timeoutDuration = storedTimeout;
  } else {
    deviceSettings.offset1 = 0.0;
    deviceSettings.offset2 = 0.0;
    deviceSettings.darkMode = false;
    deviceSettings.silentMode = false;
    deviceSettings.timeoutDuration = 30000;
  }
}

void saveSettings() {
  EEPROM.write(0, 0xAB);
  EEPROM.put(1, deviceSettings.offset1);
  EEPROM.put(5, deviceSettings.offset2);
  EEPROM.write(9, deviceSettings.darkMode ? 1 : 0);
  EEPROM.write(10, deviceSettings.silentMode ? 1 : 0);
  EEPROM.put(11, deviceSettings.timeoutDuration);
}

void addToHistory(const char* name, float value, float temp, bool isGood) {
  strncpy(measurementHistory[historyIndex].name, name, MAX_MEASUREMENT_NAME - 1);
  measurementHistory[historyIndex].name[MAX_MEASUREMENT_NAME - 1] = '\0';
  measurementHistory[historyIndex].value = value;
  measurementHistory[historyIndex].temp = temp;
  measurementHistory[historyIndex].isGood = isGood;
  historyIndex = (historyIndex + 1) % HISTORY_SIZE;
  if (historyCount < HISTORY_SIZE) historyCount++;
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("CT PRO v2.0"));

  tft.begin();
  tft.setRotation(TFT_ROTATION);
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(F("CT PRO v2.0"));
  tft.setTextSize(1);
  tft.setCursor(10, 30);
  tft.println(F("Leandro"));
  delay(1000);
  tft.fillScreen(ILI9341_BLACK);

  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  buttons_init();

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println(F("SD Err!"));
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.setCursor(10, 50);
    tft.println(F("SD Err!"));
    while (true);
  }
  Serial.println(F("SD OK"));

  loadSettings();

  thermal_init();

  currentAppState = STATE_MENU;
}

void loop() {
  currentMillis = millis();
  buttons_update();
  buzzer_update();

  switch (currentAppState) {
    case STATE_SPLASH:
      break;
    case STATE_MENU:
      menu_handle();
      break;
    case STATE_MEASURING:
      measurements_handle();
      break;
    case STATE_THERMAL_PROBE:
      thermal_handle();
      break;
    case STATE_SETTINGS:
      handle_settings_menu();
      break;
    case STATE_ABOUT:
      break;
    case STATE_HISTORY:
      handle_history();
      break;
    case STATE_SCANNER:
      handle_scanner();
      break;
    case STATE_STATS:
      handle_stats();
      break;
  }

  update_leds();
}