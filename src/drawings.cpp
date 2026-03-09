#include "drawings.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

// Função para exibir a splash screen
void draw_splash_screen() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println(F("Comp. Tester v2.0")); // Shorter string
  tft.setTextSize(1);
  tft.setCursor(10, 30);
  tft.println(F("- Leandro -"));

  tft.drawRect(100, 50, 40, 40, ILI9341_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 100);
  tft.println(F("Init..."));
}

void draw_capacitor_icon(int x, int y) {
  tft.fillRect(x, y, 4, 20, ILI9341_CYAN);
  tft.fillRect(x + 10, y, 4, 20, ILI9341_CYAN);
}

void draw_resistor_icon(int x, int y) {
  tft.drawRect(x, y + 8, 20, 6, ILI9341_MAROON);
  tft.drawLine(x - 5, y + 11, x, y + 11, ILI9341_WHITE);
  tft.drawLine(x + 20, y + 11, x + 25, y + 11, ILI9341_WHITE);
}

void draw_diode_icon(int x, int y) {
  tft.fillRect(x, y + 5, 15, 10, ILI9341_WHITE);
  tft.fillRect(x + 15, y, 3, 20, ILI9341_WHITE);
}

void draw_led_icon(int x, int y, uint16_t color) {
  tft.fillRect(x + 5, y + 5, 10, 10, color);
  tft.drawLine(x + 5, y - 5, x + 10, y - 10, color);
}

void draw_transistor_icon(int x, int y) {
  tft.fillRect(x + 5, y + 5, 10, 10, ILI9341_DARKGREY);
  tft.drawLine(x + 10, y, x + 10, y - 5, ILI9341_WHITE);
}

void draw_inductor_icon(int x, int y) {
  for (int i = 0; i < 3; i++) {
    tft.drawRect(x + 5 + (i * 8), y + 10, 5, 5, ILI9341_ORANGE);
  }
}
