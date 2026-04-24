#ifndef MENU_H
#define MENU_H

#include "globals.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

extern Adafruit_ILI9341 tft;

typedef struct {
  const char *text;
  AppState targetState;
} MenuItem;

void menu_init();
void menu_handle();
void draw_menu();
void draw_footer();
void draw_settings_menu();
void handle_settings_menu();
void draw_settings_menu_with_highlights(int highlighted);
void draw_about_screen();
void draw_history();
void handle_history();
void draw_scanner();
void handle_scanner();
void draw_stats();
void handle_stats();

#endif