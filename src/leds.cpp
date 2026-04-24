// ============================================================================
// Component Tester PRO v3.0 — LEDs (Implementação)
// ============================================================================

#include "leds.h"
#include "globals.h"
#include "config.h"
#include <NeoPixelBus.h>

// ============================================================================
// NEOPIXEL (RGB LED WS2812B)
// ============================================================================
static NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(RGB_LED_COUNT, PIN_RGB_LED);

// Estados
static unsigned long rgbFlashOnMs = 0;
static unsigned long rgbFlashOffMs = 0;
static unsigned long rgbFlashStart = 0;
static bool rgbFlashing = false;
static bool rgbLastState = false;
static uint8_t rgbTargetR = 0, rgbTargetG = 0, rgbTargetB = 0;

// ============================================================================
// INICIALIZAÇÃO
// ============================================================================
void leds_init() {
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, LOW);
    DBG_PRINTLN("[LED] LEDs indicadores inicializados");
}

void rgb_init() {
    strip.Begin();
    strip.Show();
    led_off();
    DBG_PRINTLN("[RGB] WS2812B inicializado");
}

// ============================================================================
// LEDs INDICADORES
// ============================================================================
void set_green_led(bool on) {
    digitalWrite(PIN_LED_GREEN, on ? HIGH : LOW);
}

void set_red_led(bool on) {
    digitalWrite(PIN_LED_RED, on ? HIGH : LOW);
}

void set_both_leds(bool on) {
    set_green_led(on);
    set_red_led(on);
}

void update_leds() {
    // Placeholder para lógicas de flashing
}

// ============================================================================
// PADRÕES DE FLAPPING
// ============================================================================
void flash_ok() {
    static unsigned long lastToggle = 0;
    static bool state = false;
    unsigned long now = millis();
    if (now - lastToggle > 200) {
        set_green_led(state);
        state = !state;
        lastToggle = now;
    }
}

void flash_error() {
    static unsigned long lastToggle = 0;
    static bool state = false;
    unsigned long now = millis();
    if (now - lastToggle > 100) {
        set_red_led(state);
        state = !state;
        lastToggle = now;
    }
}

void flash_testing() {
    static unsigned long lastToggle = 0;
    static bool state = false;
    unsigned long now = millis();
    if (now - lastToggle > 500) {
        set_green_led(state);
        state = !state;
        lastToggle = now;
    }
}

void flash_alert() {
    static unsigned long lastToggle = 0;
    static bool state = false;
    unsigned long now = millis();
    if (now - lastToggle > 150) {
        set_red_led(state);
        state = !state;
        lastToggle = now;
    }
}

// ============================================================================
// RGB LED (WS2812B)
// ============================================================================
void led_set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    rgbTargetR = r;
    rgbTargetG = g;
    rgbTargetB = b;
    rgbFlashing = false;
    strip.SetPixelColor(0, RgbColor(r, g, b));
    strip.Show();
}

void led_set_rgb_hex(uint32_t color) {
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    led_set_rgb(r, g, b);
}

void led_flash_rgb(uint8_t r, uint8_t g, uint8_t b,
                  unsigned long onMs, unsigned long offMs) {
    rgbTargetR = r;
    rgbTargetG = g;
    rgbTargetB = b;
    rgbFlashOnMs = onMs;
    rgbFlashOffMs = offMs;
    rgbFlashStart = millis();
    rgbFlashing = true;
    rgbLastState = true;
    strip.SetPixelColor(0, RgbColor(r, g, b));
    strip.Show();
}

void led_off() {
    rgbFlashing = false;
    strip.SetPixelColor(0, RgbColor(0, 0, 0));
    strip.Show();
}

void led_set_color(uint8_t colorIndex) {
    switch (colorIndex) {
        case LEDC_RED:     led_set_rgb(255, 0, 0); break;
        case LEDC_GREEN:   led_set_rgb(0, 255, 0); break;
        case LEDC_BLUE:    led_set_rgb(0, 0, 255); break;
        case LEDC_YELLOW: led_set_rgb(255, 200, 0); break;
        case LEDC_CYAN:    led_set_rgb(0, 255, 255); break;
        case LEDC_MAGENTA: led_set_rgb(255, 0, 255); break;
        case LEDC_WHITE:   led_set_rgb(255, 255, 255); break;
    }
}