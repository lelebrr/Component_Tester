// ============================================================================
// Component Tester PRO v3.0 — Utilitários (Implementação)
// ============================================================================

#include "utils.h"
#include <ctype.h>
#include <string.h>
#include <strings.h>

// ============================================================================
// STRINGS
// ============================================================================
bool str_is_empty(const char* s) {
    if (!s) return true;
    while (*s) {
        if (*s > ' ') return false;
        s++;
    }
    return true;
}

void str_trim(char* s) {
    if (!s) return;
    char* start = s;
    while (*start && *start <= ' ') start++;
    char* end = s + strlen(s) - 1;
    while (end > start && *end <= ' ') end--;
    *(++end) = '\0';
    if (s != start) {
        size_t len = end - start;
        memmove(s, start, len + 1);
    }
}

void str_upper(char* s) {
    if (!s) return;
    while (*s) {
        if (*s >= 'a' && *s <= 'z') *s -= 32;
        s++;
    }
}

void str_lower(char* s) {
    if (!s) return;
    while (*s) {
        if (*s >= 'A' && *s <= 'Z') *s += 32;
        s++;
    }
}

// Case-insensitive substring search (POSIX strcasestr fallback)
char* strcasestr(const char* haystack, const char* needle) {
    if (!haystack || !needle) return NULL;
    size_t needleLen = strlen(needle);
    if (needleLen == 0) return (char*)haystack;

    while (*haystack) {
        if (strncasecmp(haystack, needle, needleLen) == 0) {
            return (char*)haystack;
        }
        haystack++;
    }
    return NULL;
}

// ============================================================================
// NÚMEROS
// ============================================================================
float round_to(float value, float step) {
    if (step <= 0) return value;
    return round(value / step) * step;
}

float clamp_float(float value, float minVal, float maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

int clamp_int(int value, int minVal, int maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

// ============================================================================
// TEMPO
// ============================================================================
unsigned long elapsed_since(unsigned long startMs) {
    unsigned long now = millis();
    if (now >= startMs) return now - startMs;
    return (ULONG_MAX - startMs) + now;
}

bool has_elapsed(unsigned long startMs, unsigned long timeoutMs) {
    return elapsed_since(startMs) >= timeoutMs;
}

// ============================================================================
// DISPLAY HELPERS
// ============================================================================
void clear_area(int16_t x, int16_t y, int16_t w, int16_t h) {
    tft.fillRect(x, y, w, h, UI_COLORS::BACKGROUND);
}

void draw_centered_text(const char* text, int16_t x, int16_t y,
                    uint16_t color, uint8_t size) {
    tft.setTextColor(color);
    tft.setTextSize(size);
    tft.setTextDatum(TC_DATUM);
    tft.setCursor(x, y);
    tft.print(text);
    tft.setTextDatum(TL_DATUM);
}