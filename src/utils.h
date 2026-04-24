// ============================================================================
// Sondvolt v3.0 — Utilitários (Header)
// ============================================================================
#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

// ============================================================================
// STRINGS — Sem uso de String class, apenas char*
// ============================================================================

inline bool str_is_empty(const char* s) {
    if (!s) return true;
    while (*s) {
        if (*s > ' ') return false;
        s++;
    }
    return true;
}

inline void str_trim(char* s) {
    if (!s) return;
    char* p = s;
    while (*p && *p <= ' ') p++;
    if (p != s) {
        char* dest = s;
        while (*p) *dest++ = *p++;
        *dest = '\0';
    }
    while (*s) s++;
    while (s > (char*)0x0 && *--s <= ' ') *s = '\0';
}

inline void str_upper(char* s) {
    if (!s) return;
    while (*s) {
        if (*s >= 'a' && *s <= 'z') *s -= 32;
        s++;
    }
}

inline void str_lower(char* s) {
    if (!s) return;
    while (*s) {
        if (*s >= 'A' && *s <= 'Z') *s += 32;
        s++;
    }
}

// ============================================================================
// MATH — Funções inline otimizadas
// ============================================================================

inline float clamp_float(float v, float mn, float mx) {
    return (v < mn) ? mn : (v > mx) ? mx : v;
}

inline int clamp_int(int v, int mn, int mx) {
    return (v < mn) ? mn : (v > mx) ? mx : v;
}

inline float round_to(float v, float step) {
    if (step <= 0.0f) return v;
    return roundf(v / step) * step;
}

// ============================================================================
// TEMPO — Funções com overflow seguro
// ============================================================================

inline unsigned long elapsed_since(unsigned long start) {
    return millis() - start;
}

inline bool has_elapsed(unsigned long start, unsigned long period) {
    return (millis() - start) >= period;
}

// ============================================================================
// MOVING AVERAGE — Filtro de média móvel circular
// ============================================================================

template <uint8_t N>
class MovingAverage {
private:
    float _buffer[N];
    uint8_t _idx;
    uint8_t _count;
    float _sum;
    
public:
    MovingAverage() : _idx(0), _count(0), _sum(0.0f) {
        memset(_buffer, 0, sizeof(_buffer));
    }
    
    inline float update(float value) {
        if (_count < N) {
            _buffer[_idx] = value;
            _sum += value;
            _count++;
        } else {
            _sum -= _buffer[_idx];
            _buffer[_idx] = value;
            _sum += value;
        }
        _idx = (_idx + 1) % N;
        return _sum / (float)_count;
    }
    
    inline float get() const {
        return (_count > 0) ? _sum / (float)_count : 0.0f;
    }
    
    inline void reset() {
        _idx = 0;
        _count = 0;
        _sum = 0.0f;
        memset(_buffer, 0, sizeof(_buffer));
    }
    
    inline uint8_t count() const { return _count; }
};

// ============================================================================
// LOW-PASS FILTER — Filtro IIR simples
// ============================================================================

class LowPass {
private:
    float _value;
    float _alpha;
    
public:
    LowPass() : _value(0.0f), _alpha(0.1f) {}
    
    void set_alpha(float a) { _alpha = a; }
    
    inline float update(float input) {
        _value += _alpha * (input - _value);
        return _value;
    }
    
    inline float get() const { return _value; }
    inline void reset(float v = 0.0f) { _value = v; }
};

// ============================================================================
// DEBOUNCE — Máquina de estados simples
// ============================================================================

class Debouncer {
private:
    unsigned long _last_time;
    uint8_t _state;
    uint8_t _pin;
    uint8_t _threshold;
    
public:
    Debouncer() : _last_time(0), _state(0), _pin(0), _threshold(1) {}
    
    void begin(uint8_t pin, uint8_t threshold = 1) {
        _pin = pin;
        _threshold = threshold;
        pinMode(pin, INPUT_PULLUP);
        _state = digitalRead(pin) ? 0 : 1;
        _last_time = millis();
    }
    
    inline bool pressed() {
        bool current = digitalRead(_pin) == LOW;
        if (current != (_state & 1)) {
            if (millis() - _last_time >= 30) {
                _last_time = millis();
                _state ^= 1;
                return true;
            }
        } else {
            _last_time = millis();
        }
        return false;
    }
    
    inline bool is_down() const {
        return _state & 1;
    }
};

// ============================================================================
// SPI BUS — Wrapper para SPI com lock
// ============================================================================

class SPILock {
private:
    portMUX_TYPE _mutex;
    
public:
    SPILock() {
        portMUX_INITIALIZE(&_mutex);
    }
    
    inline void lock() {
        vPortEnterCritical(&_mutex);
    }
    
    inline void unlock() {
        vPortExitCritical(&_mutex);
    }
};

#endif