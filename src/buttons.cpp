// ============================================================================
// Component Tester PRO v3.0 — Botões e Touchscreen (Implementação)
// Descrição: Leitura do touchscreen XPT2046 e botões físicos
// ============================================================================

#include "buttons.h"
#include "globals.h"
#include "buzzer.h"
#include "config.h"

// ============================================================================
// VARIÁVEIS DE ESTADO
// ============================================================================
static bool touchPressedLast = false;
static unsigned long touchPressTimeMs = 0;
static unsigned long touchDebounceMs = 0;

static bool physButtonLast[BTN_TOTAL] = { false };
static unsigned long physButtonPressTime[BTN_TOTAL] = { 0 };
static unsigned long physButtonDebounce[BTN_TOTAL] = { 0 };
static ButtonState physButtonState[BTN_TOTAL] = { BTN_STATE_IDLE };

static TouchPoint lastTouchPoint = { 0, 0, 0 };
static bool touchActive = false;

// ============================================================================
// INICIALIZAÇÃO
// ============================================================================
void buttons_init() {
    DBG_PRINTLN("[BTN] Inicializando sistema de entrada...");

    // Touchscreen — inicializado em main.cpp via ts.begin()
    // Mas garantimos que os pinos estejam como OUTPUTs se necessário

    // Botões físicos
#if HAS_PHYSICAL_BUTTONS
    pinMode(BTN_UP, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
    pinMode(BTN_OK, INPUT_PULLUP);
    pinMode(BTN_BACK, INPUT_PULLUP);

    for (int i = 0; i < BTN_TOTAL; i++) {
        physButtonState[i] = BTN_STATE_IDLE;
        physButtonPressTime[i] = 0;
        physButtonDebounce[i] = 0;
    }
#endif

    DBG_PRINTLN("[BTN] Sistema de entrada pronto");
}

// ============================================================================
// TOUCHSCREEN — POLLING
// ============================================================================
bool touch_is_pressed() {
    TS_Point p = ts.getPoint();
    return (p.z > TOUCH_MIN_Z);
}

TouchPoint touch_get_raw_point() {
    TS_Point p = ts.getPoint();
    TouchPoint tp;
    tp.x = p.x;
    tp.y = p.y;
    tp.z = p.z;
    return tp;
}

TouchPoint touch_get_point() {
    TS_Point p = ts.getPoint();
    TouchPoint tp;

    if (p.z > TOUCH_MIN_Z) {
        // Mapeia as coordenadas do touchscreen para a tela
        // A rotação 1 do XPT2046 mapeia:
        // X: 0-4096 → 0-320
        // Y: 0-4096 → 0-240
        if (TOUCH_XY_SWAP) {
            tp.x = map(p.y, TOUCH_Y_MIN, TOUCH_Y_MAX, 0, 320);
            tp.y = map(p.x, TOUCH_X_MIN, TOUCH_X_MAX, 0, 240);
        } else {
            tp.x = map(p.x, TOUCH_X_MIN, TOUCH_X_MAX, 0, 320);
            tp.y = map(p.y, TOUCH_Y_MIN, TOUCH_Y_MAX, 0, 240);
        }

        if (TOUCH_X_INVERT) {
            tp.x = 320 - tp.x;
        }
        if (TOUCH_Y_INVERT) {
            tp.y = 240 - tp.y;
        }

        tp.z = p.z;
    } else {
        tp.x = 0;
        tp.y = 0;
        tp.z = 0;
    }

    return tp;
}

// ============================================================================
// BOTÕES FÍSICOS — LEITURA
// ============================================================================
#if HAS_PHYSICAL_BUTTONS
static bool read_phys_button(int btnId) {
    int pin = -1;
    switch (btnId) {
        case BTN_UP:    pin = BTN_UP; break;
        case BTN_DOWN:  pin = BTN_DOWN; break;
        case BTN_LEFT: pin = BTN_LEFT; break;
        case BTN_RIGHT: pin = BTN_RIGHT; break;
        case BTN_CENTER: pin = BTN_OK; break;
        case BTN_BACK: pin = BTN_BACK; break;
    }
    if (pin < 0) return false;
    return (digitalRead(pin) == LOW);
}
#endif

// ============================================================================
// ATUALIZAÇÃO (chamar no loop)
// ============================================================================
void buttons_update() {
    unsigned long now = millis();
    bool touchNow = touch_is_pressed();

    // --- Touch debounce ---
    if (touchNow && !touchPressedLast) {
        touchDebounceMs = now;
    }
    if (!touchNow) {
        touchPressedLast = false;
    }
    touchActive = touchNow;

    // --- Botões físicos ---
#if HAS_PHYSICAL_BUTTONS
    for (int i = 0; i < BTN_TOTAL; i++) {
        if (i == BTN_TOUCH) continue; // Skip touch

        bool pressed = read_phys_button(i);
        unsigned long elapsed = now - physButtonPressTime[i];

        if (pressed && !physButtonLast[i]) {
            // Borda de subida detectada
            if (now - physButtonDebounce[i] > DEBOUNCE_MS) {
                physButtonPressTime[i] = now;
                physButtonState[i] = BTN_STATE_PRESSED;
                physButtonDebounce[i] = now;
            }
        } else if (pressed && (elapsed > LONG_PRESS_MS) &&
                 physButtonState[i] == BTN_STATE_PRESSED) {
            // Transição para hold
            physButtonState[i] = BTN_STATE_HOLD;
        } else if (!pressed) {
            physButtonState[i] = BTN_STATE_IDLE;
        }

        physButtonLast[i] = pressed;
    }
#endif
}

// ============================================================================
// QUERY DE AÇÃO (detecta borda de subida)
// ============================================================================
bool btn_just_pressed(int btnId) {
    if (btnId == BTN_TOUCH) {
        return touchActive && !touchPressedLast &&
               (millis() - touchDebounceMs > TOUCH_DEBOUNCE_MS);
    }

#if HAS_PHYSICAL_BUTTONS
    if (physButtonState[btnId] == BTN_STATE_PRESSED) {
        physButtonState[btnId] = BTN_STATE_HOLD; // Marca como "consumido"
        touchPressedLast = true;
        return true;
    }
#endif
    return false;
}

bool btn_is_pressed(int btnId) {
    if (btnId == BTN_TOUCH) {
        return touchActive;
    }
#if HAS_PHYSICAL_BUTTONS
    return read_phys_button(btnId);
#else
    return false;
#endif
}

bool btn_long_pressed(int btnId) {
#if HAS_PHYSICAL_BUTTONS
    return physButtonState[btnId] == BTN_STATE_HOLD;
#else
    return false;
#endif
}

bool btn_long_pressed_any() {
    for (int i = 0; i < BTN_TOTAL; i++) {
        if (i == BTN_TOUCH) continue;
        if (btn_long_pressed(i)) return true;
    }
    return false;
}