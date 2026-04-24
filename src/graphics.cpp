// ============================================================================
// Component Tester PRO v3.1 — Graphics Implementation
// All icons drawn with vector primitives (no bitmaps)
// Designed for ESP32-2432S028R (Cheap Yellow Display)
// ============================================================================

#include "graphics.h"
#include "config.h"

// ============================================================================
// INTERNAL MACROS
// ============================================================================

#define ICON_SIZE      40
#define ICON_MARGIN    8
#define CENTER_X(x)   ((x) + ICON_SIZE / 2)
#define CENTER_Y(y)   ((y) + ICON_SIZE / 2)

// ============================================================================
// COMPONENT ICONS
// ============================================================================

void draw_component_icon(uint8_t type, int16_t x, int16_t y, uint16_t color) {
    switch (type) {
        case ICON_RESISTOR:       draw_resistor_icon(x, y, color); break;
        case ICON_CAPACITOR:      draw_capacitor_icon(x, y, color); break;
        case ICON_ELECTROLYTIC:   draw_electrolytic_icon(x, y, color); break;
        case ICON_CAP_CERAMIC:    draw_ceramic_cap_icon(x, y, color); break;
        case ICON_DIODE:          draw_diode_icon(x, y, color); break;
        case ICON_LED:            draw_led_icon(x, y, color, false); break;
        case ICON_LED_RGB:       draw_led_icon(x, y, color, true); break;
        case ICON_ZENER:         draw_zener_icon(x, y, color); break;
        case ICON_SCHOTTKY:      draw_schottky_icon(x, y, color); break;
        case ICON_TRANSISTOR_NPN: draw_transistor_npn_icon(x, y, color); break;
        case ICON_TRANSISTOR_PNP: draw_transistor_pnp_icon(x, y, color); break;
        case ICON_MOSFET_N:       draw_mosfet_n_icon(x, y, color); break;
        case ICON_MOSFET_P:      draw_mosfet_p_icon(x, y, color); break;
        case ICON_INDUCTOR:       draw_inductor_icon(x, y, color); break;
        case ICON_CRYSTAL:        draw_crystal_icon(x, y, color); break;
        case ICON_FUSE:          draw_fuse_icon(x, y, color); break;
        case ICON_RELAY:         draw_relay_icon(x, y, color); break;
        case ICON_IC:            draw_ic_icon(x, y, color); break;
        case ICON_OPTO:          draw_opto_icon(x, y, color); break;
        default:                 draw_resistor_icon(x, y, color); break;
    }
}

// ---------------------------------------------------------------------------
// RESISTOR - IEC Zigzag Symbol
// ---------------------------------------------------------------------------
void draw_resistor_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cy = y + 20;
    
    // Lead wires
    tft.drawLine(x, cy, x + 3, color);
    tft.drawLine(x + 37, cy, x + 40, color);
    
    // Zigzag pattern
    tft.drawLine(x + 3, cy, x + 5, cy, color);
    tft.drawLine(x + 5, cy, x + 7, cy - 6, color);
    tft.drawLine(x + 7, cy - 6, x + 10, cy + 6, color);
    tft.drawLine(x + 10, cy + 6, x + 13, cy - 6, color);
    tft.drawLine(x + 13, cy - 6, x + 16, cy + 6, color);
    tft.drawLine(x + 16, cy + 6, x + 19, cy - 6, color);
    tft.drawLine(x + 19, cy - 6, x + 22, cy + 6, color);
    tft.drawLine(x + 22, cy + 6, x + 25, cy - 6, color);
    tft.drawLine(x + 25, cy - 6, x + 28, cy + 6, color);
    tft.drawLine(x + 28, cy + 6, x + 31, cy - 6, color);
    tft.drawLine(x + 31, cy - 6, x + 34, cy, color);
    tft.drawLine(x + 34, cy, x + 37, cy, color);
}

// ---------------------------------------------------------------------------
// CAPACITOR - Two Parallel Plates
// ---------------------------------------------------------------------------
void draw_capacitor_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Lead wires
    tft.drawLine(x, cy, x + 6, color);
    tft.drawLine(x + 34, cy, x + 40, color);
    
    // Plate 1 (left)
    tft.drawLine(cx - 8, cy - 14, cx - 8, cy + 14, color);
    tft.drawLine(cx - 4, cy - 14, cx - 4, cy + 14, color);
    tft.fillRect(cx - 8, cy - 14, 4, 28, color);
    
    // Plate 2 (right)
    tft.drawLine(cx + 4, cy - 14, cx + 4, cy + 14, color);
    tft.drawLine(cx + 8, cy - 14, cx + 8, cy + 14, color);
    tft.fillRect(cx + 4, cy - 14, 4, 28, color);
}

// ---------------------------------------------------------------------------
// ELECTROLYTIC CAPACITOR - With Polarity Mark
// ---------------------------------------------------------------------------
void draw_electrolytic_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Outer can
    tft.drawLine(x + 4, cy, x + 8, cy, color);
    tft.drawLine(x + 32, cy, x + 36, cy, color);
    tft.drawRect(x + 8, cy - 14, 24, 28, color);
    
    // Polarity indicator line
    tft.drawLine(x + 10, cy - 14, x + 20, cy - 14, color);
    tft.drawLine(x + 10, cy - 14, x + 10, cy - 8, color);
    
    // Positive sign
    tft.setTextColor(color);
    tft.setTextSize(2);
    tft.setCursor(x + 6, y + 12);
    tft.print("+");
}

// ---------------------------------------------------------------------------
// CERAMIC CAPACITOR - Disc Type
// ---------------------------------------------------------------------------
void draw_ceramic_cap_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Lead wires
    tft.drawLine(x, cy, x + 5, color);
    tft.drawLine(x + 35, cy, x + 40, color);
    
    // Disc body
    tft.fillCircle(cx, cy, 10, color);
    tft.fillCircle(cx, cy, 6, C_WHITE);
    
    // Inner circle
    tft.drawCircle(cx, cy, 4, color);
}

// ---------------------------------------------------------------------------
// DIODE - Triangle + Bar Symbol
// ---------------------------------------------------------------------------
void draw_diode_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Lead wires
    tft.drawLine(x, cy, x + 4, color);
    tft.drawLine(x + 36, cy, x + 40, color);
    
    // Triangle (anode)
    tft.fillTriangle(cx - 10, cy - 8, cx - 10, cy + 8, cx + 2, cy, color);
    
    // Bar (cathode)
    tft.fillRect(cx + 2, cy - 10, 4, 20, color);
}

// ---------------------------------------------------------------------------
// LED - With Light Rays
// ---------------------------------------------------------------------------
void draw_led_icon(int16_t x, int16_t y, uint16_t color, bool rgb) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Lead wires
    tft.drawLine(x + 2, cy, x + 8, color);
    tft.drawLine(x + 32, cy, x + 38, color);
    
    // LED body (dome)
    tft.fillCircle(cx, cy, 8, color);
    tft.fillCircle(cx - 2, cy - 2, 4, C_WHITE);
    
    // Light rays
    tft.drawLine(cx, cy - 10, cx, cy - 16, color);
    tft.drawLine(cx - 7, cy - 7, cx - 11, cy - 12, color);
    tft.drawLine(cx + 7, cy - 7, cx + 11, cy - 12, color);
    tft.drawLine(cx - 10, cy, cx - 16, cy, color);
    tft.drawLine(cx + 10, cy, cx + 16, cy, color);
    tft.drawLine(cx - 7, cy + 7, cx - 11, cy + 12, color);
    tft.drawLine(cx + 7, cy + 7, cx + 11, cy + 12, color);
    
    // For RGB LEDs, add color dots
    if (rgb) {
        tft.fillCircle(cx - 4, cy + 2, 2, C_RED);
        tft.fillCircle(cx, cy + 2, 2, C_GREEN);
        tft.fillCircle(cx + 4, cy + 2, 2, C_BLUE);
    }
}

// ---------------------------------------------------------------------------
// ZENER DIODE - With "Z" Mark
// ---------------------------------------------------------------------------
void draw_zener_icon(int16_t x, int16_t y, uint16_t color) {
    draw_diode_icon(x, y, color);
    
    // Z marker
    tft.setTextColor(color);
    tft.setTextSize(1);
    tft.setCursor(x + 14, y + 16);
    tft.print("Z");
}

// ---------------------------------------------------------------------------
// SCHOTTKY DIODE - With S Mark
// ---------------------------------------------------------------------------
void draw_schottky_icon(int16_t x, int16_t y, uint16_t color) {
    draw_diode_icon(x, y, color);
    
    // S marker
    tft.setTextColor(color);
    tft.setTextSize(1);
    tft.setCursor(x + 14, y + 16);
    tft.print("S");
}

// ------------------------------------------------------------------------ ---
// TRANSISTOR NPN - Symbol with Arrow on Emitter
// ---------------------------------------------------------------------------
void draw_transistor_npn_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Emitter (left) with arrow pointing OUT
    tft.drawLine(x, cy, x + 14, color);
    tft.drawLine(x + 10, cy - 4, x + 14, cy, color);
    tft.drawLine(x + 10, cy + 4, x + 14, cy, color);
    
    // Collector (right)
    tft.drawLine(x + 26, cy, x + 40, color);
    
    // Base line (vertical)
    tft.drawLine(cx, cy - 12, cx, cy + 12, color);
    
    // Base leads to emitter and collector
    tft.drawLine(cx, cy, cx - 8, cy - 6, color);
    tft.drawLine(cx, cy, cx - 8, cy + 6, color);
    
    // Circle (transistor envelope)
    tft.drawCircle(cx, cy, 16, color);
}

// ---------------------------------------------------------------------------
// TRANSISTOR PNP - Symbol with Arrow Pointing In
// ---------------------------------------------------------------------------
void draw_transistor_pnp_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Emitter (left) with arrow pointing IN
    tft.drawLine(x, cy, x + 14, color);
    tft.drawLine(x + 14, cy - 4, x + 10, cy, color);
    tft.drawLine(x + 14, cy + 4, x + 10, cy, color);
    
    // Collector (right)
    tft.drawLine(x + 26, cy, x + 40, color);
    
    // Base line (vertical)
    tft.drawLine(cx, cy - 12, cx, cy + 12, color);
    
    // Base leads
    tft.drawLine(cx, cy, cx - 8, cy - 6, color);
    tft.drawLine(cx, cy, cx - 8, cy + 6, color);
    
    // Circle
    tft.drawCircle(cx, cy, 16, color);
}

// ---------------------------------------------------------------------------
// MOSFET N-CHANNEL - With Parallel Lines
// ---------------------------------------------------------------------------
void draw_mosfet_n_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Source (left)
    tft.drawLine(x + 2, cy - 4, x + 14, cy - 4, color);
    tft.drawLine(x + 2, cy + 4, x + 14, cy + 4, color);
    tft.drawLine(x + 2, cy - 4, x + 2, cy + 4, color);
    
    // Drain (right)
    tft.drawLine(x + 26, cy - 4, x + 38, cy - 4, color);
    tft.drawLine(x + 26, cy + 4, x + 38, cy + 4, color);
    tft.drawLine(x + 38, cy - 4, x + 38, cy + 4, color);
    
    // Channel (three lines)
    tft.drawLine(x + 16, cy - 4, x + 24, cy - 4, color);
    tft.drawLine(x + 16, cy, x + 24, cy, color);
    tft.drawLine(x + 16, cy + 4, x + 24, cy + 4, color);
    
    // Gate
    tft.drawLine(x + 16, cy - 12, x + 24, cy - 12, color);
    tft.drawLine(x + 16, cy + 12, x + 24, cy + 12, color);
    tft.drawLine(x + 16, cy - 12, x + 16, cy + 12, color);
    
    // Body connection
    tft.drawLine(x + 10, cy + 4, x + 10, cy + 12, color);
    tft.drawLine(x + 2, cy + 12, x + 18, cy + 12, color);
}

// ---------------------------------------------------------------------------
// MOSFET P-CHANNEL - Inverted Symbol
// ---------------------------------------------------------------------------
void draw_mosfet_p_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Source (left)
    tft.drawLine(x + 2, cy - 4, x + 14, cy - 4, color);
    tft.drawLine(x + 2, cy + 4, x + 14, cy + 4, color);
    tft.drawLine(x + 2, cy - 4, x + 2, cy + 4, color);
    
    // Drain (right)
    tft.drawLine(x + 26, cy - 4, x + 38, cy - 4, color);
    tft.drawLine(x + 26, cy + 4, x + 38, cy + 4, color);
    tft.drawLine(x + 38, cy - 4, x + 38, cy + 4, color);
    
    // Channel (with circles for P-channel)
    tft.drawLine(x + 16, cy - 6, x + 24, cy - 6, color);
    tft.drawLine(x + 16, cy, x + 24, cy, color);
    tft.drawLine(x + 16, cy + 6, x + 24, cy + 6, color);
    
    // Gate (inverted position)
    tft.drawLine(x + 16, cy - 12, x + 24, cy - 12, color);
    tft.drawLine(x + 16, cy + 12, x + 24, cy + 12, color);
    tft.drawLine(x + 24, cy - 12, x + 24, cy + 12, color);
    
    // Body connection
    tft.drawLine(x + 10, cy - 4, x + 10, cy - 12, color);
    tft.drawLine(x + 2, cy - 12, x + 18, cy - 12, color);
}

// ---------------------------------------------------------------------------
// INDUCTOR - Coils/Spirals
// ---------------------------------------------------------------------------
void draw_inductor_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cy = y + 20;
    
    // Lead wire
    tft.drawLine(x, cy, x + 4, color);
    
    // Series of semicircles (coils)
    tft.drawArc(x + 6, cy, 4, 4, 180, 0, color);
    tft.drawArc(x + 14, cy, 4, 4, 180, 0, color);
    tft.drawArc(x + 22, cy, 4, 4, 180, 0, color);
    tft.drawArc(x + 30, cy, 4, 4, 180, 0, color);
    
    // Lead wire
    tft.drawLine(x + 36, cy, x + 40, color);
}

// ---------------------------------------------------------------------------
// CRYSTAL - Two Plates with Leads
// ---------------------------------------------------------------------------
void draw_crystal_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Leads
    tft.drawLine(x + 2, cy, x + 8, cy);
    tft.drawLine(x + 32, cy, x + 38, cy);
    
    // Crystal body (rectangular)
    tft.fillRect(cx - 10, cy - 6, 20, 12, color);
    tft.fillRect(cx - 6, cy - 10, 12, 4, color);
    tft.fillRect(cx - 6, cy + 6, 12, 4, color);
    
    // Internal plates
    tft.fillRect(cx - 6, cy - 2, 4, 4, C_WHITE);
    tft.fillRect(cx + 2, cy - 2, 4, 4, C_WHITE);
}

// ---------------------------------------------------------------------------
// FUSE - Wire in Glass Tube
// ---------------------------------------------------------------------------
void draw_fuse_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // End caps
    tft.fillRect(x + 2, cy - 6, 6, 12, color);
    tft.fillRect(x + 32, cy - 6, 6, 12, color);
    
    // Glass body
    tft.fillRect(x + 8, cy - 4, 24, 8, color_lerp(color, C_WHITE, 0.3f));
    tft.drawRect(x + 8, cy - 4, 24, 8, color);
    
    // Fuse element (thin wire inside)
    tft.drawLine(x + 12, cy, x + 28, cy, color);
}

// ---------------------------------------------------------------------------
// RELAY - Coil + Contacts
// ---------------------------------------------------------------------------
void draw_relay_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Coil (inductor symbol)
    tft.drawLine(x + 2, cy, x + 6, color);
    tft.drawArc(x + 8, cy, 3, 3, 180, 0, color);
    tft.drawArc(x + 13, cy, 3, 3, 180, 0, color);
    tft.drawArc(x + 18, cy, 3, 3, 180, 0, color);
    tft.drawLine(x + 21, cy, x + 25, color);
    
    // Contact switches
    tft.drawLine(x + 28, cy - 10, x + 36, cy - 6, color);
    tft.drawLine(x + 28, cy - 10, x + 36, cy - 14, color);
    
    tft.drawLine(x + 28, cy + 10, x + 36, cy + 6, color);
    tft.drawLine(x + 28, cy + 4, x + 34, cy + 10, color);
}

// ---------------------------------------------------------------------------
// IC - Chip with Pins
// ---------------------------------------------------------------------------
void draw_ic_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Body
    tft.fillRoundRect(x + 8, cy - 12, 24, 24, 3, color);
    
    // Notch (top center)
    tft.fillTriangle(cx, cy - 12, cx - 3, cy - 9, cx + 3, cy - 9, C_DARK);
    
    // Pins (four on each side)
    for (int i = 0; i < 4; i++) {
        // Left pins
        tft.drawLine(x + 4, cy - 8 + i * 5, x + 8, cy - 8 + i * 5, color);
        // Right pins
        tft.drawLine(x + 32, cy - 8 + i * 5, x + 36, cy - 8 + i * 5, color);
    }
    
    // Dot indicator
    tft.fillCircle(cx + 6, cy + 6, 2, C_WHITE);
}

// ---------------------------------------------------------------------------
// OPTOISOLATOR - LED + Transistor Symbol
// ---------------------------------------------------------------------------
void draw_opto_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Inner split line
    tft.drawLine(x + 8, y + 4, x + 8, y + 36, color);
    tft.drawLine(x + 32, y + 4, x + 32, y + 36, color);
    tft.drawLine(x + 8, y + 4, x + 32, y + 4, color);
    tft.drawLine(x + 8, y + 36, x + 32, y + 36, color);
    
    // LED side (left)
    tft.fillTriangle(cx - 8, cy - 6, cx - 8, cy + 6, cx - 2, cy, color);
    tft.drawLine(x + 2, cy, x + 8, cy, color);
    tft.drawLine(x + 20, cy, x + 40, color);
    
    // Transistor side (right - NPN)
    tft.drawLine(cx + 4, cy - 12, cx + 4, cy + 12, color);
    tft.drawLine(cx, cy, cx + 4, cy - 4, color);
    tft.drawLine(cx, cy, cx + 4, cy + 4, color);
    
    // Arrows for isolation
    tft.drawLine(cx, cy - 18, cx, cy - 10, color);
    tft.drawLine(cx, cy + 18, cx, cy + 10, color);
}

// ============================================================================
// UI ICONS
// ============================================================================

// ---------------------------------------------------------------------------
// VOLTAGE - V Symbol
// ---------------------------------------------------------------------------
void draw_voltage_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    tft.setTextColor(color);
    tft.setTextSize(3);
    tft.setCursor(cx - 10, y + 6);
    tft.print("V");
}

// ---------------------------------------------------------------------------
// CURRENT - A Symbol
// ---------------------------------------------------------------------------
void draw_current_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    tft.setTextColor(color);
    tft.setTextSize(3);
    tft.setCursor(cx - 12, y + 6);
    tft.print("A");
}

// ---------------------------------------------------------------------------
// RESISTANCE - Ohm Symbol
// ---------------------------------------------------------------------------
void draw_resistance_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    tft.setTextColor(color);
    tft.setTextSize(2);
    tft.setCursor(cx - 10, y + 8);
    tft.print("Ohm");
}

// ---------------------------------------------------------------------------
// CONTINUITY - Sound Waves
// ---------------------------------------------------------------------------
void draw_continuity_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Center dot
    tft.fillCircle(cx, cy, 3, color);
    
    // Sound waves
    tft.drawArc(cx, cy, 8, 8, 210, 330, color);
    tft.drawArc(cx, cy, 14, 14, 210, 330, color);
}

// ---------------------------------------------------------------------------
// DIODE TEST
// ---------------------------------------------------------------------------
void draw_diode_test_icon(int16_t x, int16_t y, uint16_t color) {
    draw_diode_icon(x, y, color);
}

// ---------------------------------------------------------------------------
// TEMPERATURE
// ---------------------------------------------------------------------------
void draw_temp_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    
    // Bulb
    tft.fillCircle(cx, y + 26, 8, color);
    tft.fillRect(cx - 2, y + 8, 4, 18, color);
    
    // Mercury
    tft.fillCircle(cx, y + 26, 4, C_RED);
}

// ---------------------------------------------------------------------------
// FREQUENCY
// ---------------------------------------------------------------------------
void draw_freq_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Sine wave
    for (int i = 0; i < 30; i++) {
        int sy = cy + sin((i * 20) * 0.105f) * 10;
        tft.drawPixel(x + 5 + i, sy, color);
    }
}

// ---------------------------------------------------------------------------
// MULTIMETER - Body with Display
// ---------------------------------------------------------------------------
void draw_multimeter_icon(int16_t x, int16_t y, uint16_t color) {
    // Body
    tft.fillRoundRect(x + 4, y + 4, 32, 28, 3, color);
    // Screen
    tft.fillRect(x + 6, y + 6, 28, 12, C_WHITE);
    tft.drawRect(x + 6, y + 6, 28, 12, C_DARK_GREY);
    // Dial
    tft.fillCircle(x + 20, y + 28, 4, C_DARK_GREY);
}

// ---------------------------------------------------------------------------
// MENU - Hamburger Lines
// ---------------------------------------------------------------------------
void draw_menu_icon(int16_t x, int16_t y, uint16_t color) {
    tft.fillRect(x + 6, y + 10, 28, 3, color);
    tft.fillRect(x + 6, y + 18, 28, 3, color);
    tft.fillRect(x + 6, y + 26, 28, 3, color);
}

// ---------------------------------------------------------------------------
// SETTINGS - Gear
// ---------------------------------------------------------------------------
void draw_settings_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Outer ring
    tft.drawCircle(cx, cy, 14, color);
    tft.drawCircle(cx, cy, 10, color);
    
    // Teeth
    for (int i = 0; i < 8; i++) {
        float angle = i * 0.785f;
        int x1 = cx + cosf(angle) * 14;
        int y1 = cy + sinf(angle) * 14;
        int x2 = cx + cosf(angle) * 18;
        int y2 = cy + sinf(angle) * 18;
        tft.drawLine(x1, y1, x2, y2, color);
    }
    
    // Center dot
    tft.fillCircle(cx, cy, 3, C_WHITE);
}

// ---------------------------------------------------------------------------
// HISTORY - Clock
// ---------------------------------------------------------------------------
void draw_history_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Clock face
    tft.drawCircle(cx, cy, 14, color);
    tft.drawCircle(cx, cy, 12, color);
    
    // Hour hand
    tft.drawLine(cx, cy, cx, cy - 8, color);
    // Minute hand
    tft.drawLine(cx, cy, cx + 6, cy, color);
    
    // Center dot
    tft.fillCircle(cx, cy, 2, color);
}

// ---------------------------------------------------------------------------
// ABOUT - Info Circle
// ---------------------------------------------------------------------------
void draw_about_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    // Circle
    tft.drawCircle(cx, cy, 14, color);
    tft.fillCircle(cx, cy, 12, C_WHITE);
    
    // "i" letter
    tft.setTextColor(color);
    tft.setTextSize(2);
    tft.setCursor(cx - 4, cy - 6);
    tft.print("i");
}

// ---------------------------------------------------------------------------
// HOME - House Shape
// ---------------------------------------------------------------------------
void draw_home_icon(int16_t x, int16_t y, uint16_t color) {
    // Roof
    tft.fillTriangle(x + 20, y + 4, x + 4, y + 20, x + 36, y + 20, color);
    // Body
    tft.fillRect(x + 8, y + 20, 24, 16, color);
    // Door
    tft.fillRect(x + 16, y + 26, 8, 10, C_DARK);
}

// ---------------------------------------------------------------------------
// BACK - Arrow Left
// ---------------------------------------------------------------------------
void draw_back_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    tft.fillTriangle(cx - 6, cy, cx + 8, cy - 8, cx + 8, cy + 8, color);
    tft.fillRect(x + 4, cy - 3, 10, 6, color);
}

// ---------------------------------------------------------------------------
// UP - Arrow Up
// ---------------------------------------------------------------------------
void draw_up_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    tft.fillTriangle(cx, cy - 10, cx - 10, cy + 6, cx + 10, cy + 6, color);
}

// ---------------------------------------------------------------------------
// DOWN - Arrow Down
// ---------------------------------------------------------------------------
void draw_down_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    tft.fillTriangle(cx, cy + 10, cx - 10, cy - 6, cx + 10, cy - 6, color);
}

// ---------------------------------------------------------------------------
// CHECK - Success Mark
// ---------------------------------------------------------------------------
void draw_check_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    tft.drawLine(cx - 8, cy + 2, cx - 2, cy + 8, color);
    tft.drawLine(cx - 2, cy + 8, cx + 10, cy - 6, color);
}

// ---------------------------------------------------------------------------
// X - Error Mark
// ---------------------------------------------------------------------------
void draw_x_icon(int16_t x, int16_t y, uint16_t color) {
    tft.drawLine(x + 10, y + 10, x + 30, y + 30, color);
    tft.drawLine(x + 30, y + 10, x + 10, y + 30, color);
}

// ---------------------------------------------------------------------------
// WARNING - Triangle
// ---------------------------------------------------------------------------
void draw_warning_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    tft.fillTriangle(cx, cy - 12, cx - 12, cy + 10, cx + 12, cy + 10, color);
    tft.setTextColor(C_DARK);
    tft.setTextSize(2);
    tft.setCursor(cx - 3, cy);
    tft.print("!");
}

// ---------------------------------------------------------------------------
// INFO - Circle with i
// ---------------------------------------------------------------------------
void draw_info_icon(int16_t x, int16_t y, uint16_t color) {
    int16_t cx = x + 20;
    int16_t cy = y + 20;
    
    tft.drawCircle(cx, cy, 14, color);
    tft.fillRect(cx - 2, cy - 8, 4, 6, color);
    tft.fillCircle(cx, cy + 8, 3, color);
}

// ============================================================================
// UI DRAWING FUNCTIONS
// ============================================================================

void draw_status_bar(const char* left, const char* right) {
    tft.fillRect(0, 0, SCREEN_W, STATUS_BAR_H, C_DARK);
    tft.setTextColor(C_WHITE);
    tft.setTextSize(1);
    tft.setCursor(4, 4);
    tft.print(left);
    tft.setTextDatum(TR_DATUM);
    tft.setCursor(SCREEN_W - 4, 4);
    tft.print(right);
    tft.setTextDatum(TL_DATUM);
}

void draw_menu_bar(uint8_t selected) {
    int16_t btn_w = SCREEN_W / 4;
    const char* labels[] = {"Home", "Meas", "Hist", "Cfg"};
    uint8_t icons[] = {ICON_HOME, ICON_VOLTAGE, ICON_HISTORY, ICON_SETTINGS};
    
    for (int i = 0; i < 4; i++) {
        int16_t x = i * btn_w;
        uint16_t bg = (i == selected) ? C_GREEN : C_DARK_GREY;
        uint16_t fg = (i == selected) ? C_DARK : C_WHITE;
        
        tft.fillRect(x, SCREEN_H - MENU_BAR_H, btn_w, MENU_BAR_H, bg);
        tft.drawLine(x, SCREEN_H - MENU_BAR_H, x, SCREEN_H, C_DARK);
        
        draw_component_icon(icons[i], x + 10, SCREEN_H - MENU_BAR_H + 4, fg);
        tft.setTextColor(fg);
        tft.setTextSize(1);
        tft.setCursor(x + 10, SCREEN_H - 12);
        tft.print(labels[i]);
    }
}

void draw_footer() {
    tft.fillRect(0, SCREEN_H - FOOTER_H, SCREEN_W, FOOTER_H, C_DARK_GREY);
    tft.setTextColor(C_LIGHT_GREY);
    tft.setTextSize(1);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W / 2, SCREEN_H - FOOTER_H + 4);
    tft.print("Component Tester PRO v3.1");
    tft.setTextDatum(TL_DATUM);
}

void draw_button(int16_t x, int16_t y, int16_t w, int16_t h,
                const char* label, uint16_t bg, uint16_t fg, uint8_t style) {
    if (style == BTN_ROUND) {
        tft.fillRoundRect(x, y, w, h, 6, bg);
        tft.drawRoundRect(x, y, w, h, 6, fg);
    } else if (style == BTN_CIRCLE) {
        tft.fillCircle(x + w/2, y + h/2, w/2, bg);
        tft.drawCircle(x + w/2, y + h/2, w/2, fg);
    } else {
        tft.fillRect(x, y, w, h, bg);
        tft.drawRect(x, y, w, h, fg);
    }
    
    tft.setTextColor(fg);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(x + w/2, y + h/2);
    tft.print(label);
    tft.setTextDatum(TL_DATUM);
}

void draw_icon_button(int16_t x, int16_t y, uint8_t icon,
                      const char* label, uint16_t bg, uint16_t fg) {
    tft.fillRoundRect(x, y, ICON_SIZE + 8, ICON_SIZE + 20, 4, bg);
    tft.drawRoundRect(x, y, ICON_SIZE + 8, ICON_SIZE + 20, 4, fg);
    draw_component_icon(icon, x + 4, y + 4, fg);
    tft.setTextColor(fg);
    tft.setTextSize(1);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(x + (ICON_SIZE + 8)/2, y + ICON_SIZE + 12);
    tft.print(label);
    tft.setTextDatum(TL_DATUM);
}

void draw_toggle_button(int16_t x, int16_t y, int16_t w, int16_t h,
                      const char* label, bool state, uint16_t on_color, uint16_t off_color) {
    uint16_t bg = state ? on_color : off_color;
    tft.fillRoundRect(x, y, w, h, 6, bg);
    tft.drawRoundRect(x, y, w, h, 6, C_WHITE);
    
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(x + w/2, y + h/2);
    tft.print(label);
    tft.setTextDatum(TL_DATUM);
}

void draw_slider(int16_t x, int16_t y, int16_t w, int16_t h,
              float value, float min_val, float max_val, uint16_t color) {
    tft.fillRect(x, y + h/2 - 2, w, 4, C_GREY);
    
    float pct = (value - min_val) / (max_val - min_val);
    int16_t thumb_x = x + pct * w;
    
    tft.fillCircle(thumb_x, y + h/2, 6, color);
    tft.fillCircle(thumb_x, y + h/2, 3, C_WHITE);
}

void draw_progress_bar(int16_t x, int16_t y, int16_t w, int16_t h,
                    uint8_t percent, uint16_t fill_color, uint16_t bg_color) {
    tft.fillRect(x, y, w, h, bg_color);
    tft.fillRect(x, y, (w * percent) / 100, h, fill_color);
    tft.drawRect(x, y, w, h, C_DARK_GREY);
}

void draw_gauge_arc(int16_t cx, int16_t cy, int16_t r, float value,
                   float min_val, float max_val, uint16_t color) {
    // Background arc
    tft.drawArc(cx, cy, r, r - 4, 225, 315, C_GREY);
    
    // Value arc
    float angle = 225.0f + 90.0f * ((value - min_val) / (max_val - min_val));
    tft.drawArc(cx, cy, r, r - 4, 225, angle, color);
    
    // Needle
    float rad = (angle - 180) * 0.01745f;
    int16_t nx = cx + cosf(rad) * (r - 8);
    int16_t ny = cy + sinf(rad) * (r - 8);
    tft.drawLine(cx, cy, nx, ny, color);
    
    // Center
    tft.fillCircle(cx, cy, 4, C_WHITE);
}

void draw_value_display(int16_t x, int16_t y, float value, const char* unit,
                   uint16_t color, uint8_t size) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%.2f %s", value, unit);
    
    tft.setTextColor(color);
    tft.setTextSize(size);
    tft.setCursor(x, y);
    tft.print(buf);
}

// ============================================================================
// TEXT DRAWING FUNCTIONS
// ============================================================================

void draw_text_centered(const char* text, int16_t cx, int16_t y,
                      uint16_t color, uint8_t size) {
    tft.setTextColor(color);
    tft.setTextSize(size);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(cx, y);
    tft.print(text);
    tft.setTextDatum(TL_DATUM);
}

void draw_text_right(const char* text, int16_t rx, int16_t y,
                   uint16_t color, uint8_t size) {
    tft.setTextColor(color);
    tft.setTextSize(size);
    tft.setTextDatum(TR_DATUM);
    tft.setCursor(rx, y);
    tft.print(text);
    tft.setTextDatum(TL_DATUM);
}

void draw_label_value(int16_t x, int16_t y, const char* label, float value,
                    const char* unit, uint16_t label_color, uint16_t value_color) {
    // Label
    tft.setTextColor(label_color);
    tft.setTextSize(1);
    tft.setCursor(x, y);
    tft.print(label);
    
    // Value
    char buf[32];
    snprintf(buf, sizeof(buf), "%.2f %s", value, unit);
    tft.setTextColor(value_color);
    tft.setTextSize(2);
    tft.setCursor(x, y + 12);
    tft.print(buf);
}

// ============================================================================
// SHAPE DRAWING FUNCTIONS
// ============================================================================

void draw_rect_gradient(int16_t x, int16_t y, int16_t w, int16_t h,
                     uint16_t color1, uint16_t color2, uint8_t direction) {
    if (direction == GRAD_VERT || direction == GRAD_NONE) {
        for (int i = 0; i < h; i++) {
            float t = (float)i / h;
            uint16_t c = color_lerp(color1, color2, t);
            tft.drawLine(x, y + i, x + w, y + i, c);
        }
    } else {
        for (int i = 0; i < w; i++) {
            float t = (float)i / w;
            uint16_t c = color_lerp(color1, color2, t);
            tft.drawLine(x + i, y, x + i, y + h, c);
        }
    }
}

void draw_rounded_frame(int16_t x, int16_t y, int16_t w, int16_t h,
                        int16_t r, uint16_t fill, uint16_t stroke) {
    tft.fillRoundRect(x, y, w, h, r, fill);
    tft.drawRoundRect(x, y, w, h, r, stroke);
}

void draw_card(int16_t x, int16_t y, int16_t w, int16_t h,
               const char* title, uint16_t bg_color) {
    // Card background
    tft.fillRoundRect(x, y, w, h, 6, bg_color);
    tft.drawRoundRect(x, y, w, h, 6, color_darker(bg_color, 2));
    
    // Title bar
    tft.fillRoundRect(x, y, w, 20, 6, color_darker(bg_color, 1));
    
    // Title text
    if (title) {
        tft.setTextColor(C_WHITE);
        tft.setTextSize(1);
        tft.setCursor(x + 8, y + 4);
        tft.print(title);
    }
}

// ============================================================================
// COLOR UTILITY FUNCTIONS
// ============================================================================

uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

uint16_t color_lerp(uint16_t c1, uint16_t c2, float t) {
    uint8_t r1 = (c1 >> 11) << 3;
    uint8_t g1 = ((c1 >> 5) & 0x3F) << 2;
    uint8_t b1 = (c1 & 0x1F) << 3;
    
    uint8_t r2 = (c2 >> 11) << 3;
    uint8_t g2 = ((c2 >> 5) & 0x3F) << 2;
    uint8_t b2 = (c2 & 0x1F) << 3;
    
    uint8_t r = r1 + (r2 - r1) * t;
    uint8_t g = g1 + (g2 - g1) * t;
    uint8_t b = b1 + (b2 - b1) * t;
    
    return rgb565(r, g, b);
}

uint16_t color_scale(uint16_t color, float scale) {
    uint8_t r = (color >> 11) << 3;
    uint8_t g = ((color >> 5) & 0x3F) << 2;
    uint8_t b = (color & 0x1F) << 3;
    
    r = min(255, r * scale);
    g = min(255, g * scale);
    b = min(255, b * scale);
    
    return rgb565(r, g, b);
}

uint16_t color_brighter(uint16_t color, uint8_t steps) {
    return color_scale(color, 1.0f + (steps * 0.1f));
}

uint16_t color_darker(uint16_t color, uint8_t steps) {
    return color_scale(color, 1.0f - (steps * 0.1f));
}

uint16_t get_result_color(float deviation, float tolerance) {
    if (deviation <= tolerance) return C_OK_GREEN;
    if (deviation <= tolerance * 2) return C_WARN_YELLOW;
    return C_ERROR_RED;
}

uint16_t get_status_color(uint8_t status) {
    switch (status) {
        case 0: return C_OK_GREEN;
        case 1: return C_WARN_YELLOW;
        case 2: return C_ERROR_RED;
        default: return C_GREY;
    }
}

// ============================================================================
// ANIMATION FUNCTIONS
// ============================================================================

void draw_loading_dots(int16_t x, int16_t y, uint8_t frame, uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
        bool active = ((frame + i) % count) == 0;
        uint16_t c = active ? C_GREEN : C_GREY;
        tft.fillCircle(x + i * 12, y, 4, c);
    }
}

void draw_loading_spinner(int16_t cx, int16_t cy, uint8_t radius, uint8_t frame) {
    for (uint8_t i = 0; i < 8; i++) {
        uint8_t offset = (frame + i) % 8;
        if (offset < 6) {
            float angle = (i * 45 + frame * 15) * 0.01745f;
            int16_t px = cx + cosf(angle) * radius;
            int16_t py = cy + sinf(angle) * radius;
            tft.fillCircle(px, py, 3, C_GREEN);
        }
    }
}

void draw_pulse_animation(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t frame) {
    uint8_t alpha = (frame % 20) * 12;
    uint16_t c = rgb565(0, 200 - alpha, 0);
    tft.fillRoundRect(x, y, w, h, 4, c);
}

void draw_scan_line(int16_t y, uint8_t frame) {
    static int scan_y = 0;
    scan_y = (scan_y + 2) % 240;
    
    tft.drawLine(0, scan_y, SCREEN_W, scan_y, C_GREEN);
    tft.drawLine(0, scan_y - 1, SCREEN_W, scan_y - 1, C_DARK);
}

// ============================================================================
// FEEDBACK INDICATORS
// ============================================================================

void draw_success_indicator(int16_t x, int16_t y) {
    tft.fillCircle(x + 15, y + 15, 18, C_OK_GREEN);
    tft.setTextColor(C_DARK);
    tft.setTextSize(2);
    tft.setCursor(x + 4, y + 4);
    tft.print("OK");
}

void draw_error_indicator(int16_t x, int16_t y) {
    tft.fillCircle(x + 15, y + 15, 18, C_ERROR_RED);
    tft.setTextColor(C_WHITE);
    tft.setTextSize(3);
    tft.setCursor(x + 6, y + 2);
    tft.print("X");
}

void draw_warning_indicator(int16_t x, int16_t y) {
    tft.fillTriangle(x + 15, y, x, y + 30, x + 30, y + 30, C_WARN_YELLOW);
    tft.setTextColor(C_DARK);
    tft.setTextSize(2);
    tft.setCursor(x + 12, y + 8);
    tft.print("!");
}

void draw_info_indicator(int16_t x, int16_t y) {
    tft.fillCircle(x + 15, y + 15, 18, C_INFO_BLUE);
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setCursor(x + 10, y + 4);
    tft.print("i");
}

void draw_measurement_result(int16_t x, int16_t y, float value,
                             const char* unit, uint8_t status) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%.3f %s", value, unit);
    
    uint16_t color = get_status_color(status);
    tft.setTextColor(color);
    tft.setTextSize(3);
    tft.setCursor(x, y);
    tft.print(buf);
    
    // Status indicator
    if (status == 0) {
        draw_success_indicator(x + 80, y);
    } else if (status == 2) {
        draw_error_indicator(x + 80, y);
    }
}

// ============================================================================
// SCREEN FUNCTIONS
// ============================================================================

void clear_screen() {
    tft.fillScreen(C_DARK);
}

void fill_screen(uint16_t color) {
    tft.fillScreen(color);
}

void draw_background() {
    draw_rect_gradient(0, 0, SCREEN_W, SCREEN_H, C_DARK, C_DARK_GREY, GRAD_VERT);
}

void draw_splash_screen() {
    draw_background();
    
    // Logo area
    draw_component_icon(ICON_MULTIMETER, SCREEN_W/2 - 20, 60, C_GREEN);
    
    // Title
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W/2, 120);
    tft.print("Component");
    tft.setTextSize(3);
    tft.setCursor(SCREEN_W/2, 140);
    tft.print("Tester");
    tft.setTextSize(1);
    tft.setTextColor(C_GREEN);
    tft.setCursor(SCREEN_W/2, 165);
    tft.print("PRO v3.1");
    
    // Version
    tft.setTextColor(C_GREY);
    tft.setCursor(SCREEN_W/2, 195);
    tft.print("ESP32-2432S028R");
    
    // Loading dots
    draw_loading_dots(SCREEN_W/2 - 24, 210, 0, 4);
    
    tft.setTextDatum(TL_DATUM);
}

void draw_main_menu() {
    clear_screen();
    draw_status_bar("Component Tester", "PRO v3.1");
    
    // Component grid (2x3)
    uint8_t components[] = {
        ICON_RESISTOR, ICON_CAPACITOR, ICON_DIODE, ICON_LED,
        ICON_TRANSISTOR_NPN, ICON_INDUCTOR
    };
    
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 3; col++) {
            int i = row * 3 + col;
            int16_t x = col * (SCREEN_W/3) + 20;
            int16_t y = STATUS_BAR_H + 20 + row * 80;
            
            if (i < 6) {
                tft.fillRoundRect(x - 8, y - 8, ICON_SIZE + 16, ICON_SIZE + 16, 6, C_DARK_GREY);
                draw_component_icon(components[i], x, y, C_WHITE);
            }
        }
    }
    
    draw_footer();
}

void draw_measurement_screen() {
    clear_screen();
    draw_status_bar("Testing...", "R: 1.0k");
    
    // Large value display
    char buf[32];
    snprintf(buf, sizeof(buf), "%.2f kOhm", 1.0f);
    tft.setTextColor(C_GREEN);
    tft.setTextSize(3);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W/2, 80);
    tft.print(buf);
    tft.setTextDatum(TL_DATUM);
    
    // Progress bar
    draw_progress_bar(20, 120, SCREEN_W - 40, 20, 75, C_GREEN, C_GREY);
    
    // Component type
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W/2, 160);
    tft.print("Resistor");
    tft.setCursor(SCREEN_W/2, 180);
    tft.setTextSize(1);
    tft.setTextColor(C_GREY);
    tft.print("Carbon Film");
    tft.setTextDatum(TL_DATUM);
    
    // Status
    draw_success_indicator(SCREEN_W - 50, SCREEN_H - 50);
}

void draw_settings_screen() {
    clear_screen();
    draw_status_bar("Settings", "");
    
    draw_card(20, 40, SCREEN_W - 40, 40, "Display");
    draw_toggle_button(30, 50, 60, 20, "ON", true, C_GREEN, C_GREY);
    
    draw_card(20, 90, SCREEN_W - 40, 40, "Sound");
    draw_toggle_button(30, 100, 60, 20, "ON", true, C_GREEN, C_GREY);
    
    draw_card(20, 140, SCREEN_W - 40, 40, "Auto-OFF");
    draw_toggle_button(30, 150, 60, 20, "ON", false, C_GREEN, C_GREY);
    
    draw_footer();
}

void draw_history_screen() {
    clear_screen();
    draw_status_bar("History", "");
    
    // Header
    tft.setTextColor(C_WHITE);
    tft.setTextSize(1);
    tft.setCursor(10, 35);
    tft.print("Last Measurements:");
    
    // Items
    struct { const char* name; float value; const char* unit; } items[] = {
        {"Resistor 1k", 0.98, "kOhm"},
        {"Cap 100uF", 94.5, "uF"},
        {"LED Red", 1.82, "V"},
        {"1N4007", 0.62, "V"}
    };
    
    for (int i = 0; i < 4; i++) {
        int16_t y = 50 + i * 40;
        
        tft.setTextColor(C_WHITE);
        tft.setTextSize(1);
        tft.setCursor(10, y);
        tft.print(items[i].name);
        
        char buf[16];
        snprintf(buf, sizeof(buf), "%.2f %s", items[i].value, items[i].unit);
        tft.setTextColor(C_GREEN);
        tft.setTextSize(2);
        tft.setTextDatum(TR_DATUM);
        tft.setCursor(SCREEN_W - 10, y + 10);
        tft.print(buf);
        tft.setTextDatum(TL_DATUM);
    }
    
    draw_footer();
}

void draw_about_screen() {
    clear_screen();
    draw_status_bar("About", "");
    
    draw_component_icon(ICON_MULTIMETER, SCREEN_W/2 - 20, 50, C_GREEN);
    
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(SCREEN_W/2, 100);
    tft.print("Component Tester");
    tft.setTextSize(1);
    tft.setColor(C_GREEN);
    tft.setCursor(SCREEN_W/2, 120);
    tft.print("PRO v3.1");
    tft.setColor(C_GREY);
    tft.setCursor(SCREEN_W/2, 140);
    tft.print("ESP32-2432S028R");
    tft.setCursor(SCREEN_W/2, 160);
    tft.print("Build: " __DATE__);
    tft.setTextDatum(TL_DATUM);
    
    // Features
    tft.setColor(C_WHITE);
    tft.setCursor(20, 180);
    tft.print("Supported:");
    tft.setColor(C_GREY);
    tft.setCursor(20, 195);
    tft.print("R, C, L, D, LED, TR, MOSFET");
    
    draw_footer();
}

// ============================================================================
// TRANSITION EFFECTS (Placeholder for future enhancement)
// ============================================================================

void transition_fade_in() {
    for (int alpha = 0; alpha <= 255; alpha += 15) {
        fill_screen(rgb565(alpha * 0.1f, alpha * 0.1f, alpha * 0.15f));
    }
}

void transition_fade_out() {
    for (int alpha = 255; alpha >= 0; alpha -= 15) {
        fill_screen(rgb565(alpha * 0.1f, alpha * 0.1f, alpha * 0.15f));
    }
}

void transition_slide_left() {
    // Slide current screen left while drawing new one
}

void transition_slide_right() {
    // Slide current screen right
}

void transition_slide_up() {
    // Slide current screen up
}

void transition_slide_down() {
    // Slide current screen down
}

// ============================================================================
// TOUCH HIGHLIGHT
// ============================================================================

void draw_touchfeedback(int16_t x, int16_t y) {
    tft.fillCircle(x, y, 20, color_lerp(C_WHITE, C_GREEN, 0.3f));
    tft.drawCircle(x, y, 20, C_GREEN);
}