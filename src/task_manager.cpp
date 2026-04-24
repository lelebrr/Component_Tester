// ============================================================================
// Sondvolt v3.0 — Gerenciador de Tarefas FreeRTOS
// Descricao: Implementacao das tarefas assincronas
// ============================================================================

#include "task_manager.h"
#include "config.h"
#include "globals.h"
#include "pins.h"
#include "measurements.h"
#include "safety.h"
#include "multimeter.h"
#include "leds.h"
#include "buzzer.h"
#include "display_globals.h"

#include <string.h>

// ============================================================================
// NOMES DAS TAREFAS
// ============================================================================

const char* TASK_NAMES[] = {
    "NONE",
    "Measurement",
    "Display",
    "Safety",
    "Logger",
    "Thermal"
};

// ============================================================================
// VARIAVEIS GLOBAIS
// ============================================================================

// Filas de comunicacao entre tarefas
QueueHandle_t gMeasurementQueue = NULL;
QueueHandle_t gDisplayQueue = NULL;
QueueHandle_t gSafetyQueue = NULL;

// Status de cada tarefa
static TaskStatus gTaskStatus[TASK_COUNT];

// Handles das tarefas
static TaskHandle_t gMeasurementTaskHandle = NULL;
static TaskHandle_t gDisplayTaskHandle = NULL;
static TaskHandle_t gSafetyTaskHandle = NULL;
static TaskHandle_t gLoggerTaskHandle = NULL;
static TaskHandle_t gThermalTaskHandle = NULL;

// Flag de inicializacao
static bool gTaskManagerInitialized = false;

// ============================================================================
// INICIALIZACAO
// ============================================================================

bool task_manager_init() {
    if (gTaskManagerInitialized) {
        return true;
    }

    // Cria as filas de comunicacao
    gMeasurementQueue = xQueueCreate(5, sizeof(TaskMessage));
    gDisplayQueue = xQueueCreate(5, sizeof(TaskMessage));
    gSafetyQueue = xQueueCreate(5, sizeof(TaskMessage));

    if (!gMeasurementQueue || !gDisplayQueue || !gSafetyQueue) {
        DBG("[TASK] Falha ao criar filas");
        return false;
    }

    // Inicializa status de todas as tarefas
    memset(gTaskStatus, 0, sizeof(gTaskStatus));
    for (int i = 0; i < TASK_COUNT; i++) {
        gTaskStatus[i].id = (TaskId)i;
        gTaskStatus[i].state = TASK_STATE_STOPPED;
    }

    gTaskManagerInitialized = true;
    DBG("[TASK] Gerenciador inicializado");

    return true;
}

// ============================================================================
// CRIAR TODAS AS TAREFAS
// ============================================================================

bool task_manager_create_all() {
    if (!gTaskManagerInitialized) {
        if (!task_manager_init()) {
            return false;
        }
    }

    // Tarefa de seguranca (maior prioridade, core 0)
    if (xTaskCreatePinnedToCore(
            task_safety_monitor,
            "SafetyMonitor",
            TASK_STACK_MEDIUM,
            NULL,
            TASK_PRIORITY_HIGH,
            &gSafetyTaskHandle,
            TASK_CORE_0) != pdPASS) {
        DBG("[TASK] Falha ao criar SafetyMonitor");
    }

    // Tarefa de medicao (prioridade media)
    if (xTaskCreatePinnedToCore(
            task_measurement,
            "Measurement",
            TASK_STACK_LARGE,
            NULL,
            TASK_PRIORITY_MED,
            &gMeasurementTaskHandle,
            TASK_CORE_1) != pdPASS) {
        DBG("[TASK] Falha ao criar Measurement");
    }

    // Tarefa de display (prioridade media)
    if (xTaskCreatePinnedToCore(
            task_display,
            "Display",
            TASK_STACK_MEDIUM,
            NULL,
            TASK_PRIORITY_MED,
            &gDisplayTaskHandle,
            TASK_CORE_0) != pdPASS) {
        DBG("[TASK] Falha ao criar Display");
    }

    // Tarefa de log (prioridade baixa)
    if (xTaskCreatePinnedToCore(
            task_logger,
            "Logger",
            TASK_STACK_SMALL,
            NULL,
            TASK_PRIORITY_LOW,
            &gLoggerTaskHandle,
            TASK_CORE_ANY) != pdPASS) {
        DBG("[TASK] Falha ao criar Logger");
    }

    // Tarefa termica (prioridade baixa)
    if (xTaskCreatePinnedToCore(
            task_thermal_read,
            "Thermal",
            TASK_STACK_SMALL,
            NULL,
            TASK_PRIORITY_LOW,
            &gThermalTaskHandle,
            TASK_CORE_ANY) != pdPASS) {
        DBG("[TASK] Falha ao criar Thermal");
    }

    DBG("[TASK] Todas as tarefas criadas");
    return true;
}

// ============================================================================
// DELETAR TODAS AS TAREFAS
// ============================================================================

void task_manager_delete_all() {
    // Deleta tarefas se existirem
    if (gMeasurementTaskHandle) {
        vTaskDelete(gMeasurementTaskHandle);
        gMeasurementTaskHandle = NULL;
    }
    if (gDisplayTaskHandle) {
        vTaskDelete(gDisplayTaskHandle);
        gDisplayTaskHandle = NULL;
    }
    if (gSafetyTaskHandle) {
        vTaskDelete(gSafetyTaskHandle);
        gSafetyTaskHandle = NULL;
    }
    if (gLoggerTaskHandle) {
        vTaskDelete(gLoggerTaskHandle);
        gLoggerTaskHandle = NULL;
    }
    if (gThermalTaskHandle) {
        vTaskDelete(gThermalTaskHandle);
        gThermalTaskHandle = NULL;
    }

    // Deleta filas
    if (gMeasurementQueue) {
        vQueueDelete(gMeasurementQueue);
        gMeasurementQueue = NULL;
    }
    if (gDisplayQueue) {
        vQueueDelete(gDisplayQueue);
        gDisplayQueue = NULL;
    }
    if (gSafetyQueue) {
        vQueueDelete(gSafetyQueue);
        gSafetyQueue = NULL;
    }

    gTaskManagerInitialized = false;
    DBG("[TASK] Todas as tarefas deletadas");
}

// ============================================================================
// PAUSAR/RETOMAR TODAS
// ============================================================================

void task_manager_pause_all() {
    vTaskSuspend(gMeasurementTaskHandle);
    vTaskSuspend(gDisplayTaskHandle);
    vTaskSuspend(gLoggerTaskHandle);
    vTaskSuspend(gThermalTaskHandle);
    // Safety nunca e pausada
}

void task_manager_resume_all() {
    vTaskResume(gMeasurementTaskHandle);
    vTaskResume(gDisplayTaskHandle);
    vTaskResume(gLoggerTaskHandle);
    vTaskResume(gThermalTaskHandle);
}

// ============================================================================
// STATUS
// ============================================================================

TaskStatus* task_manager_get_status(TaskId id) {
    if (id >= TASK_COUNT) return NULL;
    return &gTaskStatus[id];
}

void task_manager_list_tasks() {
    DBG("[TASK] Status das tarefas:");
    for (int i = 1; i < TASK_COUNT; i++) {
        TaskStatus* s = &gTaskStatus[i];
        DBG_VAL(TASK_NAMES[i], s->state);
    }
}

bool task_manager_is_running(TaskId id) {
    if (id >= TASK_COUNT) return false;
    return gTaskStatus[id].state == TASK_STATE_RUNNING;
}

bool task_manager_restart(TaskId id) {
    if (id >= TASK_COUNT) return false;

    switch (id) {
        case TASK_MEASUREMENT:
            if (gMeasurementTaskHandle) {
                vTaskDelete(gMeasurementTaskHandle);
            }
            return xTaskCreatePinnedToCore(
                task_measurement, "Measurement",
                TASK_STACK_LARGE, NULL, TASK_PRIORITY_MED,
                &gMeasurementTaskHandle, TASK_CORE_1) == pdPASS;

        case TASK_DISPLAY:
            if (gDisplayTaskHandle) {
                vTaskDelete(gDisplayTaskHandle);
            }
            return xTaskCreatePinnedToCore(
                task_display, "Display",
                TASK_STACK_MEDIUM, NULL, TASK_PRIORITY_MED,
                &gDisplayTaskHandle, TASK_CORE_0) == pdPASS;

        case TASK_SAFETY:
            if (gSafetyTaskHandle) {
                vTaskDelete(gSafetyTaskHandle);
            }
            return xTaskCreatePinnedToCore(
                task_safety_monitor, "SafetyMonitor",
                TASK_STACK_MEDIUM, NULL, TASK_PRIORITY_HIGH,
                &gSafetyTaskHandle, TASK_CORE_0) == pdPASS;

        default:
            return false;
    }
}

// ============================================================================
// COMUNICACAO
// ============================================================================

bool task_send_measurement(TaskMessage msg) {
    msg.sender = TASK_NONE;
    msg.receiver = TASK_MEASUREMENT;
    return gMeasurementQueue && xQueueSend(gMeasurementQueue, &msg, 0) == pdTRUE;
}

bool task_send_display(TaskMessage msg) {
    msg.sender = TASK_NONE;
    msg.receiver = TASK_DISPLAY;
    return gDisplayQueue && xQueueSend(gDisplayQueue, &msg, 0) == pdTRUE;
}

bool task_send_safety(TaskMessage msg) {
    msg.sender = TASK_NONE;
    msg.receiver = TASK_SAFETY;
    return gSafetyQueue && xQueueSend(gSafetyQueue, &msg, 0) == pdTRUE;
}

// ============================================================================
// MENSAGENS PADRAO
// ============================================================================

void task_msg_start_measurement(uint8_t mode) {
    TaskMessage msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = MSG_TYPE_START;
    msg.param = mode;
    task_send_measurement(msg);
}

void task_msg_stop_measurement() {
    TaskMessage msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = MSG_TYPE_STOP;
    task_send_measurement(msg);
}

void task_msg_update_display(float value, const char* unit, uint16_t color) {
    TaskMessage msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = MSG_TYPE_UPDATE;
    msg.value = value;
    msg.param = (uint32_t)unit;
    task_send_display(msg);
}

void task_msg_safety_alert(float voltage) {
    TaskMessage msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = MSG_TYPE_ERROR;
    msg.value = voltage;
    task_send_safety(msg);
}

// ============================================================================
// TAREFAS SPECIFICAS
// ============================================================================

void task_measurement(void* param) {
    gTaskStatus[TASK_MEASUREMENT].handle = xTaskGetCurrentTaskHandle();
    gTaskStatus[TASK_MEASUREMENT].state = TASK_STATE_RUNNING;

    TaskMessage msg;
    uint8_t currentMode = MODE_RESISTOR;
    bool measuring = false;

    while (1) {
        // Processa mensagens da fila
        if (xQueueReceive(gMeasurementQueue, &msg, pdMS_TO_TICKS(10)) == pdTRUE) {
            switch (msg.type) {
                case MSG_TYPE_START:
                    currentMode = msg.param;
                    measuring = true;
                    measurement_start(currentMode);
                    break;

                case MSG_TYPE_STOP:
                    measuring = false;
                    measurement_stop();
                    break;
            }
        }

        // Se esta medindo, executa loop de medicao
        if (measuring) {
            measurement_update();

            // Envia resultado para display
            task_msg_update_display(
                result.value,
                result.unit,
                db_status_color((ComponentStatus)result.status)
            );
        }

        gTaskStatus[TASK_MEASUREMENT].runCount++;
        gTaskStatus[TASK_MEASUREMENT].lastRunMs = millis();
        gTaskStatus[TASK_MEASUREMENT].stackHighWaterMark =
            uxTaskGetStackHighWaterMark(NULL);

        vTaskDelay(pdMS_TO_TICKS(UPDATE_MEAS));
    }
}

void task_display(void* param) {
    gTaskStatus[TASK_DISPLAY].handle = xTaskGetCurrentTaskHandle();
    gTaskStatus[TASK_DISPLAY].state = TASK_STATE_RUNNING;

    TaskMessage msg;
    float lastValue = 0.0f;
    const char* lastUnit = "?";
    uint16_t lastColor = C_WHITE;

    while (1) {
        // Processa mensagens da fila
        if (xQueueReceive(gDisplayQueue, &msg, pdMS_TO_TICKS(50)) == pdTRUE) {
            switch (msg.type) {
                case MSG_TYPE_UPDATE:
                    lastValue = msg.value;
                    lastUnit = (const char*)msg.param;
                    lastColor = C_PRIMARY;
                    break;

                case MSG_TYPE_ERROR:
                    lastColor = C_ERROR;
                    break;

                case MSG_TYPE_VALUE:
                    lastValue = msg.value;
                    lastUnit = (const char*)msg.param;
                    break;
            }

            // Desenha valor na tela
            tft.fillRect(20, 100, SCREEN_W - 40, 60, C_BLACK);
            tft.setTextColor(lastColor);
            tft.setTextDatum(MC_DATUM);
            tft.setFreeFont(FONT_VALUE);
            char buf[32];
            snprintf(buf, sizeof(buf), "%.2f %s", lastValue, lastUnit);
            tft.drawString(buf, SCREEN_W / 2, 130);
        }

        gTaskStatus[TASK_DISPLAY].runCount++;
        gTaskStatus[TASK_DISPLAY].lastRunMs = millis();
        gTaskStatus[TASK_DISPLAY].stackHighWaterMark =
            uxTaskGetStackHighWaterMark(NULL);

        vTaskDelay(pdMS_TO_TICKS(UPDATE_DISP));
    }
}

void task_safety_monitor(void* param) {
    gTaskStatus[TASK_SAFETY].handle = xTaskGetCurrentTaskHandle();
    gTaskStatus[TASK_SAFETY].state = TASK_STATE_RUNNING;

    TaskMessage msg;
    float lastVoltage = 0.0f;

    while (1) {
        // Verifica tensao periodicamente
        SafetyCheckResult check = safety_detect_danger();

        if (!check.isSafe) {
            safety_trigger_alert(check.alertLevel);
            lastVoltage = check.detectedVoltage;

            // Envia alerta para outras tarefas
            task_msg_safety_alert(check.detectedVoltage);
        }

        // Processa mensagens da fila
        if (xQueueReceive(gSafetyQueue, &msg, pdMS_TO_TICKS(10)) == pdTRUE) {
            if (msg.type == MSG_TYPE_ERROR) {
                lastVoltage = msg.value;
                SafetyCheckResult check = safety_check_voltage(lastVoltage);
                safety_trigger_alert(check.alertLevel);
            }
        }

        gTaskStatus[TASK_SAFETY].runCount++;
        gTaskStatus[TASK_SAFETY].lastRunMs = millis();
        gTaskStatus[TASK_SAFETY].stackHighWaterMark =
            uxTaskGetStackHighWaterMark(NULL);

        // Safety nunca para
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void task_logger(void* param) {
    gTaskStatus[TASK_LOGGER].handle = xTaskGetCurrentTaskHandle();
    gTaskStatus[TASK_LOGGER].state = TASK_STATE_RUNNING;

    TaskMessage msg;

    while (1) {
        // Processa mensagens de log
        if (xQueueReceive(gMeasurementQueue, &msg, pdMS_TO_TICKS(1000)) == pdTRUE) {
            if (msg.type == MSG_TYPE_VALUE) {
                // Salva no SD se disponivel
                if (sdCardPresent) {
                    logger_write(
                        "Measurement",
                        msg.value,
                        (const char*)msg.param,
                        "OK"
                    );
                }
            }
        }

        gTaskStatus[TASK_LOGGER].runCount++;
        gTaskStatus[TASK_LOGGER].lastRunMs = millis();
        gTaskStatus[TASK_LOGGER].stackHighWaterMark =
            uxTaskGetStackHighWaterMark(NULL);

        vTaskDelay(pdMS_TO_TICKS(TIME_SAVE_HISTORY));
    }
}

void task_thermal_read(void* param) {
    gTaskStatus[TASK_THERMAL].handle = xTaskGetCurrentTaskHandle();
    gTaskStatus[TASK_THERMAL].state = TASK_STATE_RUNNING;

    while (1) {
        float temp = thermal_read();

        // Verifica limites
        if (thermal_is_critical()) {
            buzzer_alert();
            led_flash_rgb(255, 0, 0, 100, 100);
        } else if (thermal_is_warning()) {
            buzzer_beep(1000, 50);
        }

        lastTemperature = temp;

        gTaskStatus[TASK_THERMAL].runCount++;
        gTaskStatus[TASK_THERMAL].lastRunMs = millis();
        gTaskStatus[TASK_THERMAL].stackHighWaterMark =
            uxTaskGetStackHighWaterMark(NULL);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}