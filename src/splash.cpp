// ============================================================================
// Sondvolt v3.2 — Tela Inicial (Splash)
// ============================================================================
// Arquivo: splash.cpp
// Descricao: Animacao de inicializacao profissional
// ============================================================================

#include "graphics.h"
#include "display_mutex.h"
#include "display_globals.h"
#include "config.h"

void draw_splash_screen() {
    LOCK_TFT();
    tft.fillScreen(COLOR_BACKGROUND);
    
    // Circulo de brilho ao fundo
    for(int r = 100; r > 0; r -= 10) {
        uint16_t c = tft.color565(0, r/2, 0); // Verde degradê
        tft.fillCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 20, r, c);
    }
    
    // Nome do Projeto
    tft.setTextSize(3);
    tft.setTextColor(COLOR_TEXT);
    tft.setCursor(SCREEN_WIDTH/2 - 70, SCREEN_HEIGHT/2 - 30);
    tft.print("SONDVOLT");
    
    // Slogan
    tft.setTextSize(1);
    tft.setTextColor(COLOR_PRIMARY);
    tft.setCursor(SCREEN_WIDTH/2 - 60, SCREEN_HEIGHT/2 + 15);
    tft.print("Component Tester PRO");
    
    // Animacao de barra de progresso
    int16_t pbW = 200;
    int16_t pbH = 6;
    int16_t pbX = (SCREEN_WIDTH - pbW) / 2;
    int16_t pbY = SCREEN_HEIGHT - 60;
    
    tft.drawRoundRect(pbX, pbY, pbW, pbH, 3, COLOR_TEXT_DIM);
    
    for(int i = 0; i <= 100; i += 5) {
        int16_t fillW = (pbW - 4) * i / 100;
        tft.fillRoundRect(pbX + 2, pbY + 2, fillW, pbH - 4, 2, COLOR_PRIMARY);
        
        // Efeito de pulso no texto
        if (i % 20 == 0) {
            tft.setTextColor(i % 40 == 0 ? COLOR_TEXT : COLOR_PRIMARY);
            tft.setCursor(SCREEN_WIDTH/2 - 60, SCREEN_HEIGHT - 40);
            tft.print("Iniciando Hardware...");
        }
        
        delay(30);
    }
    
    tft.setTextColor(COLOR_TEXT_DIM);
    tft.setTextSize(1);
    tft.setCursor(SCREEN_WIDTH/2 - 60, SCREEN_HEIGHT - 20);
    tft.print("v3.2.0 - Build 2026");
    
    UNLOCK_TFT();
    delay(500); // Pausa final
}
