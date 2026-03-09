#ifndef DRAWINGS_H
#define DRAWINGS_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

// Declaração da instância (definida em main.cpp)
extern Adafruit_ILI9341 tft;

// Funções de desenho
void draw_splash_screen();
void draw_capacitor_icon(int x, int y);
void draw_resistor_icon(int x, int y);
void draw_diode_icon(int x, int y);
void draw_led_icon(int x, int y, uint16_t color);
void draw_transistor_icon(int x, int y);
void draw_inductor_icon(int x, int y);

#endif // DRAWINGS_H
