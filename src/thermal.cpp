#include "thermal.h"
#include "buzzer.h"
#include "globals.h"
#include "leds.h"
#include "utils.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <OneWire.h>


extern Adafruit_ILI9341 tft;

// Objeto OneWire
OneWire oneWireBus(ONEWIRE_BUS_PIN);

// Variáveis para controle da leitura da temperatura
unsigned long lastTempReadMillis = 0;
const long tempReadInterval = 500; // Leitura a cada 500ms

// Inicializa a sonda térmica
void thermal_init() {
  pinMode(ONEWIRE_BUS_PIN, INPUT_PULLUP);
  lastTempReadMillis = currentMillis;
  currentTemperature = 25.0;
  
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(F("Thermal Probe"));
  tft.setTextSize(1);
  tft.setCursor(10, 40);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.println(F("DS18B20 Sensor"));
  tft.setCursor(10, 60);
  tft.println(F("Reading temp..."));
}

// Manipula o estado da sonda térmica
void thermal_handle() {
  if (currentMillis - lastTempReadMillis >= tempReadInterval) {
    lastTempReadMillis = currentMillis;
    currentTemperature = read_temperature();
    check_temperature_alerts(currentTemperature);

    // Exibe a temperatura na tela
    tft.fillRect(0, 0, tft.width(), tft.height(),
                 ILI9341_BLACK); // Limpa a tela
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.print(F("T: "));
    fprint(tft, currentTemperature, 1);
    tft.println('C');

    tft.setCursor(10, 40);
    if (currentTemperature < TEMP_NORMAL_THRESHOLD) {
      tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
      tft.println(F("OK"));
    } else if (currentTemperature < TEMP_HOT_THRESHOLD) {
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.println(F("HOT"));
    } else {
      tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
      tft.println(F("DANGER"));
    }
  }
}

// Lê a temperatura do sensor DS18B20 (Simplified)
float read_temperature() {
  byte data[2];
  oneWireBus.reset();
  oneWireBus.write(0xCC); // Skip ROM
  oneWireBus.write(0x44); // Convert
  delay(750);
  oneWireBus.reset();
  oneWireBus.write(0xCC);
  oneWireBus.write(0xBE); // Read
  data[0] = oneWireBus.read();
  data[1] = oneWireBus.read();
  return (float)((data[1] << 8) | data[0]) / 16.0;
}

// Verifica alertas de temperatura e aciona LEDs/buzzer
void check_temperature_alerts(float temp) {
  static unsigned long lastAlertMillis = 0;
  static unsigned long beepInterval = 2000;

  if (temp >= TEMP_DANGER_THRESHOLD) {
    set_red_led(true);
    beepInterval = 200;
    if (currentMillis - lastAlertMillis >= beepInterval) {
      lastAlertMillis = currentMillis;
      play_beep(150);
    }
  } else if (temp >= TEMP_HOT_THRESHOLD) {
    flash_red_led_fast();
    beepInterval = 500;
    if (currentMillis - lastAlertMillis >= beepInterval) {
      lastAlertMillis = currentMillis;
      play_beep(100);
    }
  } else if (temp >= TEMP_NORMAL_THRESHOLD) {
    set_red_led(false);
    beepInterval = 1000;
    if (currentMillis - lastAlertMillis >= beepInterval) {
      lastAlertMillis = currentMillis;
      play_beep(50);
    }
  } else {
    set_red_led(false);
    stop_beep();
  }
}