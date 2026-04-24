// ============================================================================
// Sondvolt v3.0 — Main (TFT_eSPI)
// ============================================================================

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Preferences.h>

#include "display_globals.h"

TFT_eSPI tft = TFT_eSPI();
Preferences nvs;

//Cores
#define C_BLACK    0x0000
#define C_BLUE    0x001F
#define C_RED     0xF800
#define C_GREEN   0x07E0
#define C_CYAN    0x07FF
#define C_WHITE   0xFFFF
#define C_YELLOW  0xFFE0
#define C_GREY    0x7BEF
#define C_NAVY    0x000F

//Menu
int currentMenuItem = 0;
const int NUM_MENU_ITEMS = 7;
const char* menuLabels[] = {"Medir", "Termica", "Scanner", "Multimetro", "Historico", "Estatisticas", "Ajustes"};

// LEDs
int ledGreen = 16;
int ledRed = 17;

// Buzzer
int buzzer = 26;

void drawStatusBar() {
    tft.fillRect(0, 0, 320, 24, C_NAVY);
    tft.drawLine(0, 24, 320, 24, C_CYAN);
    tft.setTextColor(C_WHITE);
    tft.setTextSize(1);
    tft.setCursor(10, 8);
    tft.print(F("CT PRO v3.0 CYD"));
    tft.setCursor(250, 8);
    tft.print(F("25.0C"));
}

void drawFooter() {
    int y = 240 - 20;
    tft.fillRect(0, y, 320, 20, C_NAVY);
    tft.drawLine(0, y, 320, y, C_CYAN);
    tft.setTextColor(C_GREY);
    tft.setTextSize(1);
    tft.setCursor(10, y + 6);
    tft.print(F("[UP/DOWN] Nav  [OK] Sel  [<] Vol"));
}

void drawMenu() {
    tft.fillScreen(C_BLACK);
    drawStatusBar();
    
    int cols = 2;
    for (int i = 0; i < NUM_MENU_ITEMS; i++) {
        int row = i / cols;
        int col = i % cols;
        int x = 10 + col * 155;
        int y = 40 + row * 65;
        
        if (i == currentMenuItem) {
            tft.fillRoundRect(x, y, 145, 55, 8, C_CYAN);
            tft.setTextColor(C_BLACK);
        } else {
            tft.fillRoundRect(x, y, 145, 55, 8, C_NAVY);
            tft.drawRoundRect(x, y, 145, 55, 8, C_GREY);
            tft.setTextColor(C_WHITE);
        }
        
        tft.setCursor(x + 20, y + 22);
        tft.print(menuLabels[i]);
    }
    
    drawFooter();
}

void beep(int freq, int dur) {
    tone(buzzer, freq, dur);
}

void handleMeasure() {
    tft.fillScreen(C_BLACK);
    drawStatusBar();
    
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setCursor(20, 60);
    tft.print(F("Modo Medir"));
    
    tft.setTextSize(1);
    tft.setTextColor(C_GREY);
    tft.setCursor(20, 100);
    tft.print(F("Conecte o componente"));
    tft.setCursor(20, 120);
    tft.print(F("Pressione OK para testar"));
    
    bool running = true;
    while (running) {
        if (digitalRead(0) == LOW) { // Botão OK
            beep(1000, 100);
            delay(200);
        }
        if (digitalRead(35) == LOW) { // Botão Back
            running = false;
            beep(500, 100);
        }
        delay(10);
    }
}

void handleThermal() {
    tft.fillScreen(C_BLACK);
    drawStatusBar();
    
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setCursor(20, 60);
    tft.print(F("Sonda Termica"));
    
    tft.setTextSize(1);
    tft.setTextColor(C_GREY);
    tft.setCursor(20, 100);
    tft.print(F("DS18B20 nao conectado"));
    
    delay(2000);
}

void handleMultimeter() {
    tft.fillScreen(C_BLACK);
    drawStatusBar();
    
    tft.setTextColor(C_YELLOW);
    tft.setTextSize(2);
    tft.setCursor(20, 60);
    tft.print(F("Multimetro AC/DC"));
    
    tft.setTextSize(1);
    tft.setTextColor(C_GREY);
    tft.setCursor(20, 100);
    tft.print(F("ZMPT101B: N/C"));
    tft.setCursor(20, 120);
    tft.print(F("INA219: N/C"));
    
    delay(2000);
}

void handleHistory() {
    tft.fillScreen(C_BLACK);
    drawStatusBar();
    
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setCursor(20, 60);
    tft.print(F("Historico"));
    
    tft.setTextSize(1);
    tft.setTextColor(C_GREY);
    tft.setCursor(20, 100);
    tft.print(F("Nenhuma medicao"));
    
    delay(2000);
}

void handleStats() {
    tft.fillScreen(C_BLACK);
    drawStatusBar();
    
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setCursor(20, 60);
    tft.print(F("Estatisticas"));
    
    tft.setTextSize(1);
    tft.setTextColor(C_GREY);
    tft.setCursor(20, 100);
    tft.print(F("Total: 0"));
    tft.setCursor(20, 120);
    tft.print(F("Bom: 0"));
    tft.setCursor(20, 140);
    tft.print(F("Ruim: 0"));
    
    delay(2000);
}

void handleSettings() {
    tft.fillScreen(C_BLACK);
    drawStatusBar();
    
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setCursor(20, 60);
    tft.print(F("Ajustes"));
    
    tft.setTextSize(1);
    tft.setTextColor(C_GREY);
    tft.setCursor(20, 100);
    tft.print(F("Backlight: 200"));
    tft.setCursor(20, 120);
    tft.print(F("Silencioso: Nao"));
    
    delay(2000);
}

void setup() {
    Serial.begin(115200);
    Serial.println(F("\n=== CT PRO v3.0 CYD ==="));
    
    //TFT
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(C_BLACK);
    
    //Pinos
    pinMode(ledGreen, OUTPUT);
    pinMode(ledRed, OUTPUT);
    pinMode(buzzer, OUTPUT);
    
    //Botões entrada
    pinMode(0, INPUT_PULLUP);   // OK
    pinMode(35, INPUT_PULLUP); // BACK
    pinMode(22, INPUT_PULLUP); // LEFT
    pinMode(27, INPUT_PULLUP); // RIGHT
    pinMode(2, INPUT_PULLUP);  // DOWN
    pinMode(4, INPUT_PULLUP);  // UP
    
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, LOW);
    
    //Splash de segurança
    safety_show_splash_animated();
    
    // Inicializa sistema de segurança
    safety_init();
    
    //Splash normal
    tft.fillScreen(C_BLACK);
    tft.setTextColor(C_CYAN);
    tft.setTextSize(2);
    tft.setCursor(90, 80);
    tft.print(F("CT PRO"));
    tft.setTextSize(1);
    tft.setTextColor(C_WHITE);
    tft.setCursor(70, 110);
    tft.print(F("CYD Edition v3.0"));
    
    for (int i = 0; i <= 100; i += 5) {
        int barW = (200 * i) / 100;
        tft.fillRect(60, 160, barW, 10, C_CYAN);
        delay(20);
    }
    
    delay(300);
    beep(1000, 100);
    
    drawMenu();
    Serial.println(F("Setup OK"));
}

void loop() {
    // Atualiza sistema de seguranca
    safety_update();

    // Navegacao simples por botoes
    if (digitalRead(4) == LOW) { // UP
        beep(500, 30);
        currentMenuItem = (currentMenuItem - 1 + NUM_MENU_ITEMS) % NUM_MENU_ITEMS;
        drawMenu();
        delay(200);
    }
    if (digitalRead(2) == LOW) { // DOWN
        beep(500, 30);
        currentMenuItem = (currentMenuItem + 1) % NUM_MENU_ITEMS;
        drawMenu();
        delay(200);
    }
    if (digitalRead(22) == LOW) { // LEFT
        beep(500, 30);
        currentMenuItem = (currentMenuItem - 2 + NUM_MENU_ITEMS) % NUM_MENU_ITEMS;
        drawMenu();
        delay(200);
    }
    if (digitalRead(27) == LOW) { // RIGHT
        beep(500, 30);
        currentMenuItem = (currentMenuItem + 2) % NUM_MENU_ITEMS;
        drawMenu();
        delay(200);
    }
    if (digitalRead(0) == LOW) { // OK
        beep(1000, 50);
        delay(200);
        
        // Verificacao de seguranca antes do multímetro
        if (currentMenuItem == 2) { // Multímetro
            if (!safety_check_before_multimeter()) {
                beep(200, 200);
                drawMenu();
                return;
            }
        }
        
        switch (currentMenuItem) {
            case 0: handleMeasure(); break;
            case 1: handleThermal(); break;
            case 2: handleMultimeter(); break;
            case 3: handleHistory(); break;
            case 4: handleStats(); break;
            case 5: handleSettings(); break;
            case 6: break;
        }
        drawMenu();
    }
    
    delay(10);
}