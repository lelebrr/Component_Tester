// ============================================================================
// Sondvolt v3.1 — Splash Screen
// Professional startup animation
// ============================================================================

#include "graphics.h"
#include "globals.h"

// ============================================================================
// ANIMATION FRAMES
// ============================================================================

static const uint8_t SPLASH_DURATION_MS = 2500;
static const uint8_t FRAME_COUNT = 20;

// ============================================================================
// SPLASH SCREEN DRAW
// ============================================================================

void draw_splash_screen() {
    clear_screen();
    
    // Background gradient
    draw_rect_gradient(0, 0, SCREEN_W, SCREEN_H, C_DARK, rgb565(20, 25, 30), GRAD_VERT);
    
    // Animated loading spinner
    draw_loading_spinner(SCREEN_W / 2, 100, 25, 0);
    
    // Main icon (multimeter)
    draw_component_icon(ICON_MULTIMETER, SCREEN_W / 2 - 20, 60, C_GREEN);
    
    // Title
    tft.setTextColor(C_WHITE);
    tft.setTextSize(3);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2, 130);
    tft.print("SOND");
    tft.setTextSize(5);
    tft.setTextColor(C_GREEN);
    tft.print("VOLT");
    tft.setTextSize(1);
    tft.setTextColor(C_CYAN);
    tft.setCursor(SCREEN_W / 2, 170);
    tft.print("Component Tester Pro");
    tft.setTextSize(1);
    tft.setTextColor(C_WHITE);
    tft.setCursor(SCREEN_W / 2, 190);
    tft.print("v3.1");
    tft.setTextDatum(TL_DATUM);
    
    // Version
    tft.setTextColor(C_DARK_GREY);
    tft.setTextSize(1);
    tft.setCursor(SCREEN_W / 2, 215);
    tft.setTextDatum(TC_DATUM);
    tft.print("ESP32-2432S028R");
    tft.setTextDatum(TL_DATUM);
    
    // Progress bar with animation
    draw_progress_bar(40, 225, SCREEN_W - 80, 8, 0, C_GREEN, C_DARK_GREY);
}

// ============================================================================
// ANIMATED SPLASH (Complete)
// ============================================================================

void show_splash_animated() {
    uint32_t startTime = millis();
    uint8_t frame = 0;
    
    while (millis() - startTime < SPLASH_DURATION_MS) {
        uint32_t elapsed = millis() - startTime;
        frame = (elapsed / (SPLASH_DURATION_MS / FRAME_COUNT)) % FRAME_COUNT;
        
        // Redraw spinner
        draw_loading_spinner(SCREEN_W / 2, 100, 25, frame);
        
        // Update progress bar
        uint8_t progress = (elapsed * 100) / SPLASH_DURATION_MS;
        draw_progress_bar(40, 225, SCREEN_W - 80, 8, progress, C_GREEN, C_DARK_GREY);
        
        delay(50);
    }
}

// ============================================================================
// BOOT LOGO (Quick flash)
// ============================================================================

void show_boot_logo() {
    draw_splash_screen();
    delay(500);
    
    // Fade effect
    for (int i = 0; i < 10; i++) {
        uint16_t alpha = 255 - (i * 25);
        tft.fillScreen(rgb565(alpha * 0.05f, alpha * 0.06f, alpha * 0.08f));
        delay(30);
    }
}

// ============================================================================
// TRANSITION TO MAIN MENU
// ============================================================================

void transition_to_menu() {
    // Fade out splash
    for (int alpha = 0; alpha <= 255; alpha += 30) {
        tft.fillScreen(rgb565(alpha * 0.08f, alpha * 0.1f, alpha * 0.12f));
    }
    
    clear_screen();
}

// ============================================================================
// ERROR SCREEN (For boot errors)
// ============================================================================

void draw_error_screen(const char* errorMsg) {
    clear_screen();
    
    // Error icon
    draw_error_indicator(SCREEN_W / 2 - 20, 60);
    
    // Message
    tft.setTextColor(C_RED);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2, 120);
    tft.print("ERRO");
    tft.setTextSize(1);
    tft.setTextColor(C_WHITE);
    tft.setCursor(SCREEN_W / 2, 150);
    tft.print(errorMsg);
    tft.setTextDatum(TL_DATUM);
    
    // Retry message
    tft.setTextColor(C_GREY);
    tft.setCursor(SCREEN_W / 2, 190);
    tft.setTextDatum(TC_DATUM);
    tft.print("Reiniciando...");
    tft.setTextDatum(TL_DATUM);
}

// ============================================================================
// INITIALIZING ANIMATION
// ============================================================================

void draw_init_progress(const char* task, uint8_t progress) {
    // Clear task area
    tft.fillRect(0, 180, SCREEN_W, 30, C_DARK);
    
    // Task name
    tft.setTextColor(C_LIGHT_GREY);
    tft.setTextSize(1);
    tft.setCursor(10, 182);
    tft.print(task);
    
    // Progress bar
    draw_progress_bar(10, 200, SCREEN_W - 20, 6, progress, C_GREEN, C_DARK_GREY);
}