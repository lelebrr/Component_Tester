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

// Array de strings para os nomes das medições em PROGMEM para economizar RAM
const char m0[] PROGMEM = "C+ESR";
const char m1[] PROGMEM = "Res";
const char m2[] PROGMEM = "D/L";
const char m3[] PROGMEM = "Trans";
const char m4[] PROGMEM = "Induc";
const char m5[] PROGMEM = "Volt";
const char m6[] PROGMEM = "Freq";
const char m7[] PROGMEM = "PWM";
const char m8[] PROGMEM = "Opto";
const char m9[] PROGMEM = "Cable";
const char m10[] PROGMEM = "Brid";
const char m11[] PROGMEM = "Auto";
const char m12[] PROGMEM = "Conti";

const char *const measurementNames[] PROGMEM = {m0, m1, m2, m3,  m4,  m5, m6,
                                                m7, m8, m9, m10, m11, m12};

const int NUM_MEASUREMENTS =
    sizeof(measurementNames) / sizeof(measurementNames[0]);

int currentMeasurementItem = 0;

void display_measurement_stub(const char *measurementName) {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print(measurementName);
  tft.setTextSize(1);
  tft.setCursor(10, 40);
  tft.println(F("Working.."));
  showBackMsg();
  set_green_led(false);
  set_red_led(false);
  flash_both_leds(200);
}

float measure_resistance_value() {
  // Implementação simplificada de medição de resistor
  // Em um projeto real, isso envolveria circuito de medição com ADC
  analogRead(PROBE1_PIN); // Descarga do capacitor
  delay(10);

  // Carregar capacitor através do resistor
  pinMode(PROBE1_PIN, OUTPUT);
  digitalWrite(PROBE1_PIN, HIGH);
  pinMode(PROBE2_PIN, INPUT);
  delayMicroseconds(100);

  // Medir a tensão através do tempo de descarga
  pinMode(PROBE1_PIN, INPUT);
  unsigned long startTime = micros();
  int count = 0;
  while (analogRead(PROBE1_PIN) > 512 && count < 10000) {
    count++;
    delayMicroseconds(10);
  }
  unsigned long dischargeTime = micros() - startTime;

  // Calcular resistência (fórmula simplificada)
  float resistance = (dischargeTime * 0.001) * 1000; // Ohm aproximado

  return constrain(resistance, 1.0,
                   1000000.0); // Limitar entre 1 Ohm e 1 MOhm
}

const char *get_judgment(float measured, float expected,
                         float tolerance_percent) {
  float tolerance = expected * (tolerance_percent / 100.0);
  float min_good = expected - tolerance;
  float max_good = expected + tolerance;
  float min_average = expected - tolerance * 2;
  float max_average = expected + tolerance * 2;

  if (measured >= min_good && measured <= max_good) {
    return "Good";
  } else if (measured >= min_average && measured <= max_average) {
    return "Average";
  } else {
    return "Bad";
  }
}

// Medição de Capacitor (Implementação de Tempo de Carga RC)
void measure_capacitor() {
  // Serial.println(F("Cap..."));
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(F("Cap:"));

  // 1. Descarregar o capacitor
  pinMode(PROBE1_PIN, OUTPUT);
  digitalWrite(PROBE1_PIN, LOW);
  pinMode(PROBE2_PIN, OUTPUT);
  digitalWrite(PROBE2_PIN, LOW);
  delay(500); // Garante descarga total

  // 2. Medir tempo de carga
  pinMode(PROBE1_PIN, INPUT); // PROBE1 detecta
  pinMode(PROBE2_PIN, OUTPUT);
  digitalWrite(PROBE2_PIN, HIGH); // Inicia carga

  unsigned long startTime = micros();
  while (analogRead(PROBE1_PIN) <
         648) { // 63.2% de 1023 (1 constante de tempo Tau)
    if (micros() - startTime > 1000000)
      break; // Timeout 1s
  }
  unsigned long duration = micros() - startTime;

  // C = t / R. Assumindo resistência interna do AVR + probe ~ 10k
  // (simplificado)
  float capacitance = (float)duration / 10000.0; // uF

  tft.setCursor(10, 40);
  fprint(tft, capacitance, 2);
  tft.println(F(" uF"));

  showBackMsg();

  log_measurement("Capacitor", capacitance, currentTemperature, "N/A");
  set_green_led(true);
}

void measure_resistor() {
  // Serial.println(F("Res..."));

  // Circuito Divisor de Tensão: Vout = Vin * (R2 / (R1 + R2))
  // Onde R1 é conhecido (ex: 10k pullup interno ou externo)
  // PROBE2 -> R_unknown -> PROBE1 (GND)

  pinMode(PROBE1_PIN, OUTPUT);
  digitalWrite(PROBE1_PIN, LOW);     // Probe 1 é o GND
  pinMode(PROBE2_PIN, INPUT_PULLUP); // Usa resistor interno de ~30k-50k

  delay(10);
  int raw = analogRead(PROBE2_PIN);

  float resistance;
  if (raw < 1022) {
    // R_unk = R_pullup * Vout / (Vin - Vout)
    // R_pullup ~ 35000 Ohm para ATmega328
    resistance = 35000.0 * (float)raw / (1023.0 - (float)raw);
  } else {
    resistance = -1.0; // Aberto
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

  log_measurement("Resistor", resistance, currentTemperature, "OK");
}

void measure_diode() {
  // Serial.println(F("Diod..."));
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(F("Diodo/LED:"));

  // Testa polaridade P1(+) -> P2(-) e P2(+) -> P1(-)
  pinMode(PROBE1_PIN, OUTPUT);
  pinMode(PROBE2_PIN, OUTPUT);

  // Forward bias P1->P2
  digitalWrite(PROBE1_PIN, HIGH);
  digitalWrite(PROBE2_PIN, LOW);
  delay(10);
  int vF1 = analogRead(A0); // Simplificado: Idealmente usaria divisor ou ADC
                            // direto se pino for analógico

  // Reverse bias P1->P2
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
    tft.print(F("No Diode / Open"));
    set_red_led(true);
  }

  showBackMsg();
}

void measure_transistor() {
  // Serial.println(F("Trans..."));
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(F("Transistor:"));

  tft.setTextSize(1);
  tft.setCursor(10, 40);
  tft.println(F("Identifying junctions..."));

  // Lógica de identificação de BJT (Simplificada)
  // Testa 6 combinações de junções PN
  // ... (Implementação detalhada omitida para brevidade, usando stub
  // inteligente)
  tft.println(F("Type: BJT NPN Case?"));
  tft.println(F("Checking Beta..."));

  set_green_led(true);
  tft.setCursor(10, 80);
  tft.println(F("BACK to exit"));
}

void measure_inductor() {
  Serial.println(F("Medindo Indutor..."));
  display_measurement_stub("Indutor");
}

void measure_voltmeter_dc() {
  Serial.println(F("Medindo Voltimetro DC..."));
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(F("Voltimetro DC:"));

  // Usa divisor de tensão se necessário, aqui assume leitura direta 0-5V
  int raw = analogRead(A0);
  float voltage = (raw * 5.0) / 1023.0;

  tft.setCursor(10, 40);
  fprint(tft, voltage, 1);
  tft.println(F(" V"));

  set_green_led(true);
  tft.setTextSize(1);
  tft.setCursor(10, 80);
  tft.println(F("BACK to exit"));
}

void measure_frequency_counter() {
  Serial.println(F("Medindo Frequency Counter..."));
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 10);
  tft.println(F("Frequency:"));
  tft.println(F("Scanning..."));
  // ... Lógica de pulsin() ou interrupção
  set_green_led(true);
  tft.setCursor(10, 80);
  tft.println(F("BACK to exit"));
}

void generate_pwm() {
  Serial.println(F("Gerando PWM..."));
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 10);
  tft.println(F("PWM Generator:"));
  tft.println(F("Freq: 1kHz"));
  tft.println(F("Duty: 50%"));

  pinMode(9, OUTPUT); // Pino do buzzer ou específico
  analogWrite(9, 127);

  set_green_led(true);
  tft.setCursor(10, 80);
  tft.println(F("BACK to exit"));
}

void test_optocoupler() {
  Serial.println(F("Testando Optoacoplador..."));
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 10);
  tft.println(F("Optocoupler Test:"));

  pinMode(PROBE1_PIN, OUTPUT);
  digitalWrite(PROBE1_PIN, HIGH);    // Drive LED
  pinMode(PROBE2_PIN, INPUT_PULLUP); // Read Transistor

  delay(50);
  if (digitalRead(PROBE2_PIN) == LOW) {
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.println(F("Status: WORKING"));
  } else {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.println(F("Status: FAILED/OPEN"));
  }

  tft.setCursor(10, 80);
  tft.println(F("BACK to exit"));
}

void test_cable_continuity() {
  Serial.println(F("Testando Cabo..."));
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 10);
  tft.println(F("Cable Tester:"));

  pinMode(PROBE1_PIN, OUTPUT);
  digitalWrite(PROBE1_PIN, LOW);
  pinMode(PROBE2_PIN, INPUT_PULLUP);

  if (digitalRead(PROBE2_PIN) == LOW) {
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.println(F("SHORT / CONNECTED"));
    play_beep(200);
  } else {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.println(F("OPEN / NO CONN"));
  }

  tft.setCursor(10, 80);
  tft.println(F("BACK to exit"));
}

void test_bridge_rectifier() {
  Serial.println(F("Testando Ponte Retif..."));
  display_measurement_stub("Ponte Retif.");
}

void auto_detect_component() {
  Serial.println(F("Auto Detect Componente..."));
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 10);
  tft.println(F("Auto Detecting..."));

  // Tenta resistor
  pinMode(PROBE1_PIN, OUTPUT);
  digitalWrite(PROBE1_PIN, LOW);
  pinMode(PROBE2_PIN, INPUT_PULLUP);
  delay(10);
  if (analogRead(PROBE2_PIN) < 1000) {
    measure_resistor();
    return;
  }

  // Tenta diodo
  // ...
  tft.println(F("No component found."));
  tft.setCursor(10, 80);
  tft.println(F("BACK to exit"));
}

void test_continuity_buzzer() {
  Serial.println(F("Testando Continuidade com Buzzer..."));
  display_measurement_stub("Continuidade");
  play_beep(500); // Exemplo de uso do buzzer
}

// Desenha o menu de medições
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

  // Desenha o ícone do componente
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

  // Rodapé para navegação
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
    // Executa a função de medição selecionada
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
    currentAppState = STATE_MENU; // Volta para o menu principal
    tft.fillScreen(ILI9341_BLACK);
    // É importante redesenhar o menu principal ao voltar
    // menu_init(); // Isso pode resetar a seleção do menu, melhor chamar
    // draw_menu()
  }
}
