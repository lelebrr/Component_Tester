// ============================================================================
// Component Tester PRO v3.0 — Utilitários
// Descrição: Funções auxiliares gerais
// ============================================================================
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

// ============================================================================
// STRINGS
// ============================================================================

// Verifica se uma string é vazia
bool str_is_empty(const char* s);

// Trim (remove espaços do início e fim)
void str_trim(char* s);

// Converte para maiúsculas
void str_upper(char* s);

// Converte para minúsculas
void str_lower(char* s);

// ============================================================================
// NÚMEROS
// ============================================================================

// Arredonda para o valor mais próximo
float round_to(float value, float step);

// Clamp genérico
float clamp_float(float value, float minVal, float maxVal);
int clamp_int(int value, int minVal, int maxVal);

// ============================================================================
// TEMPO
// ============================================================================

// millis() com overflow seguro (como micros())
unsigned long elapsed_since(unsigned long startMs);

// Verifica se passou o tempo
bool has_elapsed(unsigned long startMs, unsigned long timeoutMs);

// ============================================================================
// DISPLAY
// ============================================================================

// Limpa uma área retangular com a cor de fundo
void clear_area(int16_t x, int16_t y, int16_t w, int16_t h);

// Desenha centered text
void draw_centered_text(const char* text, int16_t x, int16_t y,
                       uint16_t color, uint8_t size);

// ============================================================================
// ARDUINO HELPERS (que podem não estar disponíveis)
// ============================================================================

#ifndef constrain
#define constrain(v, mn, mx) ((v) < (mn) ? (mn) : ((v) > (mx) ? (mx) : (v)))
#endif

#ifndef map
#define map(v, in_min, in_max, out_min, out_max) \
    ((v - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#endif

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#endif // UTILS_H