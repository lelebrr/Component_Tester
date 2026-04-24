#include "menu.h"
#include "buttons.h"
#include "buzzer.h"
#include "drawings.h"
#include "measurements.h"
#include "utils.h"
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

MenuItem menuItems[] = {
  {"Medir", STATE_MEASURING},
  {"Termica", STATE_THERMAL_PROBE},
  {"Scanner", STATE_SCANNER},
  {"Historico", STATE_HISTORY},
  {"Estatist", STATE_STATS},
  {"Config", STATE_SETTINGS},
  {"Sobre", STATE_ABOUT}
};

const int NUM_MENU_ITEMS = sizeof(menuItems) / sizeof(menuItems[0]);

void menu_init() {
  totalMenuItems = NUM_MENU_ITEMS;
  currentMenuItem = 0;
  draw_menu();
}

void menu_handle() {
  buttons_update();

  if (isUpPressed()) {
    currentMenuItem--;
    if (currentMenuItem < 0) currentMenuItem = NUM_MENU_ITEMS - 1;
    draw_menu();
  }
  if (isDownPressed()) {
    currentMenuItem++;
    if (currentMenuItem >= NUM_MENU_ITEMS) currentMenuItem = 0;
    draw_menu();
  }
  if (isOkPressed()) {
    currentAppState = menuItems[currentMenuItem].targetState;
    tft.fillScreen(ILI9341_BLACK);
    if (currentAppState == STATE_THERMAL_PROBE) {
    } else if (currentAppState == STATE_MEASURING) {
      draw_measurements_menu();
    } else if (currentAppState == STATE_SETTINGS) {
      draw_settings_menu();
    } else if (currentAppState == STATE_ABOUT) {
      draw_about_screen();
    } else if (currentAppState == STATE_HISTORY) {
      draw_history();
    } else if (currentAppState == STATE_SCANNER) {
      draw_scanner();
    } else if (currentAppState == STATE_STATS) {
      draw_stats();
    }
  }
}

void draw_menu() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);

  for (int i = 0; i < NUM_MENU_ITEMS; i++) {
    if (i == currentMenuItem) {
      tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    } else {
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    }
    tft.setCursor(10, 10 + i * 20);
    tft.println(menuItems[i].text);
  }
  draw_footer();
}

void draw_footer() {
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.fillRect(0, tft.height() - 20, tft.width(), 20, ILI9341_DARKGREY);
  tft.setCursor(5, tft.height() - 15);
  tft.print(F("UP/DW | OK | BCK"));
}

void draw_settings_menu() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(F("Configuracoes"));

  tft.setTextSize(1);
  tft.setCursor(10, 40);
  tft.println(F("1. Calibrar Probes"));
  tft.setCursor(10, 60);
  tft.println(F("2. Modo Escuro"));
  tft.setCursor(10, 80);
  tft.println(F("3. Modo Silencioso"));
  tft.setCursor(10, 100);
  tft.println(F("4. Timeout"));
  tft.setCursor(10, 120);
  tft.println(F("5. SalvarCfg"));

  draw_footer();
}

void handle_settings_menu() {
  static int currentSettingsItem = 0;
  const int NUM_SETTINGS_ITEMS = 5;

  if (isUpPressed()) {
    currentSettingsItem = (currentSettingsItem - 1 + NUM_SETTINGS_ITEMS) % NUM_SETTINGS_ITEMS;
    draw_settings_menu_with_highlights(currentSettingsItem);
  }
  if (isDownPressed()) {
    currentSettingsItem = (currentSettingsItem + 1) % NUM_SETTINGS_ITEMS;
    draw_settings_menu_with_highlights(currentSettingsItem);
  }
  if (isOkPressed()) {
    play_beep(100);
    switch (currentSettingsItem) {
      case 0:
        calibrate_probes();
        draw_settings_menu();
        break;
      case 1:
        deviceSettings.darkMode = !deviceSettings.darkMode;
        toggle_dark_mode();
        draw_settings_menu();
        break;
      case 2:
        deviceSettings.silentMode = !deviceSettings.silentMode;
        toggle_silent_mode();
        draw_settings_menu();
        break;
      case 3:
        deviceSettings.timeoutDuration = (deviceSettings.timeoutDuration == 30000) ? 60000 : 30000;
        draw_settings_menu();
        break;
      case 4:
        saveSettings();
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(10, 10);
        tft.println(F("Salvo!"));
        delay(1000);
        draw_settings_menu();
        break;
    }
  }
  if (isBackPressed()) {
    currentAppState = STATE_MENU;
    tft.fillScreen(ILI9341_BLACK);
    draw_menu();
  }
}

void draw_settings_menu_with_highlights(int highlighted) {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(F("Configuracoes"));

  const char *settings[] = {"1. Calibrar", "2. Modo Escuro", "3. Silencioso", "4. Timeout", "5. Salvar"};

  tft.setTextSize(1);
  for (int i = 0; i < 5; i++) {
    tft.setCursor(10, 40 + i * 20);
    if (i == highlighted) {
      tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    } else {
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    }
    tft.println(settings[i]);
  }
  draw_footer();
}

void draw_about_screen() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(F("Component Tester"));
  tft.setCursor(10, 30);
  tft.println(F("PRO v2.0"));

  tft.setTextSize(1);
  tft.setCursor(10, 60);
  tft.println(F("Desenvolvido por: Leandro"));
  tft.setCursor(10, 80);
  tft.println(F("Baseado em Arduino Uno R3"));
  tft.setCursor(10, 100);
  tft.println(F("Firmware: v2.0.1"));
  tft.setCursor(10, 120);
  tft.println(F("Data: 04/03/2026"));

  draw_footer();
}

void draw_history() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 5);
  tft.println(F("Historico"));

  tft.setTextSize(1);
  if (historyCount == 0) {
    tft.setCursor(10, 40);
    tft.println(F("Nenhuma medicao"));
  } else {
    int startIdx = (historyCount < HISTORY_SIZE) ? 0 : historyIndex;
    for (int i = 0; i < historyCount && i < 7; i++) {
      int idx = (startIdx + i) % HISTORY_SIZE;
      tft.setCursor(10, 25 + i * 15);
      if (measurementHistory[idx].isGood) {
        tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
      } else {
        tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
      }
      tft.print(measurementHistory[idx].name);
      tft.print(F(": "));
      fprint(tft, measurementHistory[idx].value, 1);
    }
  }
  draw_footer();
}

void handle_history() {
  buttons_update();
  if (isBackPressed()) {
    currentAppState = STATE_MENU;
    tft.fillScreen(ILI9341_BLACK);
    draw_menu();
  }
}

void draw_scanner() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(F("Scanner"));
  tft.setTextSize(1);
  tft.setCursor(10, 40);
  tft.println(F("OK = auto medicao"));
  tft.setCursor(10, 60);
  tft.println(F("BCK = sair"));
}

void handle_scanner() {
  buttons_update();
  static bool scanning = false;
  static unsigned long lastScan = 0;
  const unsigned long scanInterval = 2000;

  if (!scanning && isOkPressed()) {
    scanning = true;
    play_beep(100);
  }

  if (scanning && currentMillis - lastScan >= scanInterval) {
    lastScan = currentMillis;
    auto_detect_component();
    delay(500);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println(F("Scanner"));
    tft.setTextSize(1);
    tft.setCursor(10, 40);
    tft.println(F("Escaneando..."));
    tft.setCursor(10, 60);
    tft.println(F("OK=parar BCK=sair"));
  }

  if (scanning && isOkPressed()) {
    scanning = false;
    play_beep(200);
  }

  if (isBackPressed()) {
    scanning = false;
    currentAppState = STATE_MENU;
    tft.fillScreen(ILI9341_BLACK);
    draw_menu();
  }
}

void draw_stats() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(F("Estatisticas"));

  tft.setTextSize(1);
  tft.setCursor(10, 40);
  tft.print(F("Total: "));
  fprint(tft, deviceSettings.totalMeasurements, 0);
  tft.setCursor(10, 60);
  tft.print(F("Defeituosos: "));
  fprint(tft, deviceSettings.faultyMeasurements, 0);

  tft.setCursor(10, 80);
  if (deviceSettings.totalMeasurements > 0) {
    float pct = (float)deviceSettings.faultyMeasurements / deviceSettings.totalMeasurements * 100.0;
    tft.print(F("Taxa falha: "));
    fprint(tft, pct, 1);
    tft.print(F("%"));
  }

  tft.setCursor(10, 100);
  tft.print(F("Temp: "));
  fprint(tft, currentTemperature, 1);
  tft.print(F("C"));

  draw_footer();
}

void handle_stats() {
  buttons_update();
  if (isBackPressed()) {
    currentAppState = STATE_MENU;
    tft.fillScreen(ILI9341_BLACK);
    draw_menu();
  }
}