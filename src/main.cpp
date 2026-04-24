// ============================================================================
// Component Tester PRO v3.0 — Main Minimal (TFT_eSPI)
// ============================================================================

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Preferences.h>

#define C_BLACK    0x0000
#define C_BLUE     0x001F
#define C_RED      0xF800
#define C_GREEN    0x07E0
#define C_CYAN    0x07FF
#define C_WHITE   0xFFFF
#define C_YELLOW  0xFFE0
#define C_GREY    0x7BEF
#define C_NAVY    0x000F

TFT_eSPI tft = TFT_eSPI();
Preferences nvs;

int currentMenuItem = 0;
const int NUM_MENU_ITEMS = 7;

void drawStatusBar() {
    tft.fillRect(0, 0, 320, 24, C_NAVY);
    tft.setTextColor(C_WHITE);
    tft.setTextSize(1);
    tft.setCursor(10, 8);
    tft.print(F("CT PRO v3.0 CYD"));
}

void drawMenu() {
    tft.fillScreen(C_BLACK);
    drawStatusBar();
    
    const char* items[] = {"Medir", "Termica", "Scanner", "Multimetro", "Historico", "Ajustes", "Sobre"};
    int cols = 2;
    
    for (int i = 0; i < NUM_MENU_ITEMS; i++) {
        int row = i / cols;
        int col = i % cols;
        int x = 10 + col * 155;
        int y = 40 + row * 60;
        
        if (i == currentMenuItem) {
            tft.fillRoundRect(x, y, 145, 50, 8, C_CYAN);
            tft.setTextColor(C_BLACK);
        } else {
            tft.fillRoundRect(x, y, 145, 50, 8, C_NAVY);
            tft.setTextColor(C_WHITE);
        }
        
        tft.setCursor(x + 20, y + 20);
        tft.print(items[i]);
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println(F("\n=== CT PRO v3.0 CYD ==="));
    
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(C_BLACK);
    
    pinMode(16, OUTPUT);
    pinMode(17, OUTPUT);
    pinMode(26, OUTPUT);
    
    digitalWrite(16, LOW);
    digitalWrite(17, LOW);
    digitalWrite(26, LOW);
    
    drawMenu();
    Serial.println(F("Setup OK"));
}

void loop() {
    delay(100);
}