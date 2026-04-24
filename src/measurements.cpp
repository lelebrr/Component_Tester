#include "measurements.h"
#include "buttons.h"
#include "buzzer.h"
#include "config.h"
#include "drawings.h"
#include "globals.h"
#include "leds.h"
#include "logger.h"
#include "utils.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

extern Adafruit_ILI9341 tft;

void addToHistory(const char* name, float value, float temp, bool isGood);

void setupTela(const __FlashStringHelper* titulo) {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(titulo);
}

const char m0[] PROGMEM = "Cap";
const char m1[] PROGMEM = "Res";
const char m2[] PROGMEM = "Diod";
const char m3[] PROGMEM = "Trans";
const char m4[] PROGMEM = "Ind";
const char m5[] PROGMEM = "Volt";
const char m6[] PROGMEM = "Freq";
const char m7[] PROGMEM = "PWM";
const char m8[] PROGMEM = "Opto";
const char m9[] PROGMEM = "Cable";
const char m10[] PROGMEM = "Bridge";
const char m11[] PROGMEM = "Auto";
const char m12[] PROGMEM = "Conti";

const char *const measurementNames[] PROGMEM = {m0, m1, m2, m3,  m4,  m5, m6,
                                                 m7, m8, m9, m10, m11, m12};

const int NUM_MEASUREMENTS =
    sizeof(measurementNames) / sizeof(measurementNames[0]);

int currentMeasurementItem = 0;

float measure_resistance_value() {
  analogRead(PROBE1_PIN);
  delay(10);

  pinMode(PROBE1_PIN, OUTPUT);
  digitalWrite(PROBE1_PIN, HIGH);
  pinMode(PROBE2_PIN, INPUT);
  delayMicroseconds(100);

  pinMode(PROBE1_PIN, INPUT);
  unsigned long startTime = micros();
  int count = 0;
  while (analogRead(PROBE1_PIN) > 512 && count < 10000) {
    count++;
    delayMicroseconds(10);
  }
  unsigned long dischargeTime = micros() - startTime;

  float resistance = (dischargeTime * 0.001) * 1000;
  return constrain(resistance, 1.0, 1000000.0);
}

void measure_capacitor() {
  setupTela(F("Capacitor:"));
  pinMode(PROBE1_PIN, OUTPUT);
  digitalWrite(PROBE1_PIN, LOW);
  pinMode(PROBE2_PIN, OUTPUT);
  digitalWrite(PROBE2_PIN, LOW);
  delay(500);

  pinMode(PROBE1_PIN, INPUT);
  pinMode(PROBE2_PIN, OUTPUT);
  digitalWrite(PROBE2_PIN, HIGH);

  unsigned long startTime = micros();
  while (analogRead(PROBE1_PIN) < 648) {
    if (micros() - startTime > 1000000)
      break;
  }
  unsigned long duration = micros() - startTime;

  float capacitance = (float)duration / 10000.0;

  tft.setCursor(10, 40);
  fprint(tft, capacitance, 2);
  tft.println(F(" uF"));

  showBackMsg();
  set_green_led(true);
  addToHistory("Cap", capacitance, currentTemperature, true);
}

void measure_resistor() {
  pinMode(PROBE1_PIN, OUTPUT);
  digitalWrite(PROBE1_PIN, LOW);
  pinMode(PROBE2_PIN, INPUT_PULLUP);

  delay(10);
  int raw = analogRead(PROBE2_PIN);

  float resistance;
  if (raw < 1022) {
    resistance = 35000.0 * (float)raw / (1023.0 - (float)raw);
  } else {
    resistance = -1.0;
  }

  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print(F("Res: "));

  if (resistance > 0) {
    if (resistance > 1000) {
      fprint(tft, resistance / 1000.0, 1);
      tft.println(F(" k"));
    } else {
      fprint(tft, resistance, 0);
      tft.println(F(" R"));
    }
    set_green_led(true);
  } else {
    tft.println(F("OPEN/HIGH"));
    set_red_led(true);
  }

  showBackMsg();
  addToHistory("Res", resistance > 0 ? resistance : 0, currentTemperature, resistance > 0);
}

void measure_diode() {
  setupTela(F("Diodo/LED:"));
  pinMode(PROBE1_PIN, OUTPUT);
  pinMode(PROBE2_PIN, OUTPUT);

  digitalWrite(PROBE1_PIN, HIGH);
  digitalWrite(PROBE2_PIN, LOW);
  delay(10);
  int vF1 = analogRead(A0);

  digitalWrite(PROBE1_PIN, LOW);
  digitalWrite(PROBE2_PIN, HIGH);
  delay(10);
  int vF2 = analogRead(A1);

  tft.setCursor(10, 40);
  if (vF1 > 100 && vF1 < 900) {
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.print(F("Anodo P1, Katodo P2"));
    set_green_led(true);
  } else if (vF2 > 100 && vF2 < 900) {
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.print(F("Anodo P2, Katodo P1"));
    set_green_led(true);
  } else {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.print(F("No Diode"));
    set_red_led(true);
    increment_faulty_count();
  }
  showBackMsg();
  addToHistory("Diod", 0, currentTemperature, (vF1 > 100 && vF1 < 900) || (vF2 > 100 && vF2 < 900));
}

void measure_transistor() {
  setupTela(F("Transistor:"));
  tft.setTextSize(1);
  tft.setCursor(10, 40);
  tft.println(F("Identifying..."));

  pinMode(PROBE1_PIN, OUTPUT);
  pinMode(PROBE2_PIN, OUTPUT);

  bool isNPN = false;
  int beta = 0;

  for (int basePin = 0; basePin < 2; basePin++) {
    int pinA = (basePin == 0) ? PROBE1_PIN : PROBE2_PIN;
    int pinB = (basePin == 0) ? PROBE2_PIN : PROBE1_PIN;
    
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
    delay(10);
    int v1 = analogRead(pinB);
    
    if (v1 > 100 && v1 < 900) {
      isNPN = true;
      pinMode(pinB, INPUT_PULLUP);
      digitalWrite(pinA, HIGH);
      delay(10);
      int state = digitalRead(pinB);
      beta = (state == LOW) ? 100 : 50;
      break;
    }
  }

  tft.setCursor(10, 70);
  if (isNPN) {
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.println(F("BJT NPN"));
    tft.print(F("Beta: "));
    fprint(tft, beta, 0);
    set_green_led(true);
  } else {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.println(F("Not Found"));
    set_red_led(true);
    increment_faulty_count();
  }
  showBackMsg();
  addToHistory("Trans", beta, currentTemperature, isNPN);
}

void measure_inductor() {
  setupTela(F("Indutor:"));
  pinMode(PROBE1_PIN, OUTPUT);
  pinMode(PROBE2_PIN, OUTPUT);
  digitalWrite(PROBE1_PIN, LOW);
  digitalWrite(PROBE2_PIN, LOW);
  delay(100);

  digitalWrite(PROBE1_PIN, HIGH);
  unsigned long startTime = micros();
  unsigned long timeout = 100000;
  
  while (digitalRead(PROBE2_PIN) == LOW && (micros() - startTime < timeout)) {
    pinMode(PROBE2_PIN, INPUT_PULLUP);
  }
  
  unsigned long duration = micros() - startTime;
  pinMode(PROBE2_PIN, OUTPUT);
  digitalWrite(PROBE2_PIN, LOW);

  float inductance;
  if (duration > 5000) {
    inductance = duration / 1000.0;
    if (inductance > 1000) {
      tft.setCursor(10, 40);
      fprint(tft, inductance / 1000.0, 1);
      tft.println(F(" mH"));
    } else {
      tft.setCursor(10, 40);
      fprint(tft, inductance, 0);
      tft.println(F(" uH"));
    }
    set_green_led(true);
  } else {
    tft.setCursor(10, 40);
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.println(F("OPEN"));
    set_red_led(true);
    increment_faulty_count();
  }

  showBackMsg();
  addToHistory("Ind", inductance, currentTemperature, inductance > 0);
}

void measure_voltmeter_dc() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 10);
  tft.println(F("Volt DC:"));
  int raw = analogRead(A0);
  float voltage = (raw * 5.0) / 1023.0;
  tft.setCursor(10, 40);
  fprint(tft, voltage, 1);
  tft.println(F(" V"));
  set_green_led(true);
  showBackMsg();
}

void measure_frequency_counter() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(F("Frequency:"));
  tft.setTextSize(1);
  tft.setCursor(10, 40);
  tft.println(F("Pin 5 input"));

  unsigned long pulseCount = 0;
  unsigned long startTime = micros();
  const unsigned long measureTime = 1000000;

  pinMode(5, INPUT);
  int lastState = digitalRead(5);
  
  while (micros() - startTime < measureTime) {
    int currentState = digitalRead(5);
    if (currentState != lastState) {
      if (currentState == HIGH) {
        pulseCount++;
      }
      lastState = currentState;
    }
  }

  tft.setCursor(10, 70);
  if (pulseCount > 0) {
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    fprint(tft, pulseCount, 0);
    tft.println(F(" Hz"));
    set_green_led(true);
  } else {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.println(F("No Signal"));
    set_red_led(true);
  }
  showBackMsg();
}

void generate_pwm() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 10);
  tft.println(F("PWM: 1kHz 50%"));
  pinMode(9, OUTPUT);
  analogWrite(9, 127);
  set_green_led(true);
  showBackMsg();
}

void test_optocoupler() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 10);
  tft.println(F("Optocoupler:"));

  pinMode(PROBE1_PIN, OUTPUT);
  digitalWrite(PROBE1_PIN, HIGH);
  pinMode(PROBE2_PIN, INPUT_PULLUP);

  delay(50);
  bool working = digitalRead(PROBE2_PIN) == LOW;
  
  tft.setCursor(10, 40);
  tft.setTextColor(working ? ILI9341_GREEN : ILI9341_RED, ILI9341_BLACK);
  tft.println(working ? F("WORKING") : F("FAILED"));
  if (working) set_green_led(true);
  else set_red_led(true);
  showBackMsg();
}

void test_cable_continuity() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 10);
  tft.println(F("Cable:"));

  pinMode(PROBE1_PIN, OUTPUT);
  digitalWrite(PROBE1_PIN, LOW);
  pinMode(PROBE2_PIN, INPUT_PULLUP);

  bool connected = digitalRead(PROBE2_PIN) == LOW;
  
  tft.setCursor(10, 40);
  tft.setTextColor(connected ? ILI9341_GREEN : ILI9341_RED, ILI9341_BLACK);
  tft.println(connected ? F("CONNECTED") : F("OPEN"));
  if (connected) { play_beep(200); set_green_led(true); }
  else set_red_led(true);
  showBackMsg();
}

void test_bridge_rectifier() {
  setupTela(F("Bridge Rect:"));
  tft.setTextSize(1);
  tft.setCursor(10, 40);
  tft.println(F("Testing diodes..."));

  int workingDiodes = 0;
  int p1 = PROBE1_PIN;
  int p2 = PROBE2_PIN;
  
  for (int test = 0; test < 4; test++) {
    bool forwardBias = false;
    bool reverseBias = false;
    int probe1, probe2;
    
    switch (test) {
      case 0: probe1 = p1; probe2 = p2; break;
      case 1: probe1 = p2; probe2 = p1; break;
      case 2: probe1 = p1; probe2 = p2; break;
      case 3: probe1 = p2; probe2 = p1; break;
      default: probe1 = p1; probe2 = p2; break;
    }
    
    pinMode(probe1, OUTPUT);
    pinMode(probe2, OUTPUT);
    digitalWrite(probe1, HIGH);
    digitalWrite(probe2, LOW);
    delay(5);
    int forwardRead = analogRead(probe1);
    
    digitalWrite(probe1, LOW);
    digitalWrite(probe2, HIGH);
    delay(5);
    int reverseRead = analogRead(probe2);
    
    if (forwardRead > 100 && forwardRead < 900) forwardBias = true;
    if (reverseRead > 900) reverseBias = true;
    
    if (forwardBias && reverseBias) workingDiodes++;
  }

  pinMode(p1, INPUT);
  pinMode(p2, INPUT);

  tft.setCursor(10, 40);
  tft.setTextColor(workingDiodes == 4 ? ILI9341_GREEN : (workingDiodes >= 2 ? ILI9341_YELLOW : ILI9341_RED), ILI9341_BLACK);
  tft.println(workingDiodes == 4 ? F("GOOD") : (workingDiodes >= 2 ? F("PARTIAL") : F("FAILED")));
  if (workingDiodes >= 2) set_green_led(true);
  else set_red_led(true);
  showBackMsg();
}

void auto_detect_component() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 10);
  tft.println(F("Auto..."));
  pinMode(PROBE1_PIN, OUTPUT);
  pinMode(PROBE2_PIN, OUTPUT);
  digitalWrite(PROBE1_PIN, HIGH);
  digitalWrite(PROBE2_PIN, LOW);
  delay(10);
  int forward1 = analogRead(PROBE2_PIN);
  digitalWrite(PROBE1_PIN, LOW);
  digitalWrite(PROBE2_PIN, HIGH);
  delay(10);
  int forward2 = analogRead(PROBE1_PIN);
  
  tft.setCursor(10, 40);
  if (forward1 > 100 && forward1 < 900 && forward2 > 900) {
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.println(F("DIODE"));
    measure_diode();
  } else if (forward1 < 50 && forward2 < 50) {
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.println(F("RESISTOR"));
    measure_resistor();
  } else {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.println(F("None"));
    set_red_led(true);
    showBackMsg();
    addToHistory("Auto", 0, currentTemperature, false);
  }
}

void test_continuity_buzzer() {
  setupTela(F("Continuidade:"));
  tft.setTextSize(1);
  tft.setCursor(10, 40);
  tft.println(F("Probing..."));

  pinMode(PROBE1_PIN, OUTPUT);
  digitalWrite(PROBE1_PIN, LOW);
  pinMode(PROBE2_PIN, INPUT_PULLUP);

  delay(10);
  int raw = analogRead(PROBE2_PIN);
  float resistance = 35000.0 * (float)raw / (1023.0 - (float)raw);

  tft.setCursor(10, 70);
  if (raw < 100) {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.println(F("OPEN"));
    set_red_led(true);
  } else if (resistance < 10) {
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.println(F("SHORT"));
    play_beep(500);
    set_green_led(true);
  } else if (resistance < 50) {
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.print(F("LOW: "));
    fprint(tft, resistance, 1);
    tft.println(F(" R"));
    play_beep(300);
    set_green_led(true);
  } else {
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    fprint(tft, resistance, 0);
    tft.println(F(" Ohm"));
    play_beep(200);
    set_green_led(true);
  }

  showBackMsg();
  addToHistory("Cont", resistance, currentTemperature, raw >= 100);
}

void draw_measurements_menu() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);

  for (int i = 0; i < NUM_MEASUREMENTS; i++) {
    if (i == currentMeasurementItem) {
      tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    } else {
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    }
    tft.setCursor(10, 10 + i * 20);
    tft.println(measurementNames[i]);
  }

  int iconX = 140;
  int iconY = 80;
  switch (currentMeasurementItem) {
  case 0:
    draw_capacitor_icon(iconX, iconY);
    break;
  case 1:
    draw_resistor_icon(iconX, iconY);
    break;
  case 2:
    draw_led_icon(iconX, iconY, ILI9341_YELLOW);
    break;
  case 3:
    draw_transistor_icon(iconX, iconY);
    break;
  case 4:
    draw_inductor_icon(iconX, iconY);
    break;
  default:
    tft.drawRect(iconX, iconY, 20, 20, ILI9341_WHITE);
    break;
  }

  tft.setTextSize(1);
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.fillRect(0, tft.height() - 20, tft.width(), 20, ILI9341_DARKCYAN);
  tft.setCursor(5, tft.height() - 15);
  tft.print(F("UP/DW | OK | BCK"));
}

// Manipula a seleção e execução das medições
void measurements_handle() {
  buttons_update();

  if (isUpPressed()) {
    currentMeasurementItem--;
    if (currentMeasurementItem < 0) {
      currentMeasurementItem = NUM_MEASUREMENTS - 1;
    }

    draw_measurements_menu();
  }
  if (isDownPressed()) {
    currentMeasurementItem++;
    if (currentMeasurementItem >= NUM_MEASUREMENTS) {
      currentMeasurementItem = 0;
    }
    draw_measurements_menu();
  }
  if (isOkPressed()) {
    increment_measurement_count();
    switch (currentMeasurementItem) {
    case 0:
      measure_capacitor();
      break;
    case 1:
      measure_resistor();
      break;
    case 2:
      measure_diode();
      break;
    case 3:
      measure_transistor();
      break;
    case 4:
      measure_inductor();
      break;
    case 5:
      measure_voltmeter_dc();
      break;
    case 6:
      measure_frequency_counter();
      break;
    case 7:
      generate_pwm();
      break;
    case 8:
      test_optocoupler();
      break;
    case 9:
      test_cable_continuity();
      break;
    case 10:
      test_bridge_rectifier();
      break;
    case 11:
      auto_detect_component();
      break;
    case 12:
      test_continuity_buzzer();
      break;
    }
  }
  if (isBackPressed()) {
    currentAppState = STATE_MENU;
    tft.fillScreen(ILI9341_BLACK);
  }
}
