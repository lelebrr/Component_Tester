// ============================================================================
// Sondvolt v3.0 — Gerenciador de Tarefas FreeRTOS
// Descricao: Tarefas assincronas para medicao e exibicao
// ============================================================================
#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <stdint.h>
#include <Arduino.h>

// ============================================================================
// CONFIGURACOES DE TAREFAS
// ============================================================================

// Tamanho de stack das tarefas
#define TASK_STACK_SMALL      2048
#define TASK_STACK_MEDIUM     4096
#define TASK_STACK_LARGE      8192

// Prioridades das tarefas
#define TASK_PRIORITY_LOW     1
#define TASK_PRIORITY_MED     3
#define TASK_PRIORITY_HIGH    5
#define TASK_PRIORITY_CRITICAL 10

// Core do ESP32 (0 ou 1)
#define TASK_CORE_0           0
#define TASK_CORE_1           1
#define TASK_CORE_ANY         tskNO_AFFINITY

// ============================================================================
// IDENTIFICADORES DE TAREFAS
// ============================================================================

enum TaskId {
    TASK_NONE = 0,
    TASK_MEASUREMENT = 1,
    TASK_DISPLAY = 2,
    TASK_SAFETY = 3,
    TASK_LOGGER = 4,
    TASK_THERMAL = 5,
    TASK_COUNT
};

// Nomes das tarefas para debug
extern const char* TASK_NAMES[];

// ============================================================================
// ESTADO DAS TAREFAS
// ============================================================================

enum TaskState {
    TASK_STATE_STOPPED = 0,
    TASK_STATE_RUNNING = 1,
    TASK_STATE_PAUSED = 2,
    TASK_STATE_ERROR = 3
};

typedef struct {
    TaskId id;
    TaskHandle_t handle;
    TaskState state;
    unsigned long lastRunMs;
    unsigned long runCount;
    uint32_t stackHighWaterMark;
} TaskStatus;

// ============================================================================
// FILAS DE COMUNICACAO
// ============================================================================

// Mensagens entre tarefas
typedef struct {
    TaskId sender;
    TaskId receiver;
    uint16_t type;
    uint32_t param;
    float value;
    void* data;
} TaskMessage;

// Filas principais
extern QueueHandle_t gMeasurementQueue;
extern QueueHandle_t gDisplayQueue;
extern QueueHandle_t gSafetyQueue;

// ============================================================================
// PROTOTIPOS — CONTROLE DE TAREFAS
// ============================================================================

// Inicializa o gerenciador de tarefas
bool task_manager_init();

// Cria todas as tarefas do sistema
bool task_manager_create_all();

// Deleta todas as tarefas
void task_manager_delete_all();

// Pausa todas as tarefas
void task_manager_pause_all();

// Retoma todas as tarefas
void task_manager_resume_all();

// ============================================================================
// PROTOTIPOS — STATUS
// ============================================================================

// Obtem status de uma tarefa
TaskStatus* task_manager_get_status(TaskId id);

// Lista todas as tarefas e seus status
void task_manager_list_tasks();

// Verifica se tarefa esta ativa
bool task_manager_is_running(TaskId id);

// Reinicia uma tarefa especifica
bool task_manager_restart(TaskId id);

// ============================================================================
// PROTOTIPOS — COMUNICACAO
// ============================================================================

// Envia mensagem para fila de medicao
bool task_send_measurement(TaskMessage msg);

// Envia mensagem para fila de display
bool task_send_display(TaskMessage msg);

// Envia mensagem para fila de seguranca
bool task_send_safety(TaskMessage msg);

// ============================================================================
// PROTOTIPOS — TAREFAS SPECIFICAS
// ============================================================================

// Tarefa de medicao (roda em background)
void task_measurement(void* param);

// Tarefa de display (atualizacao da tela)
void task_display(void* param);

// Tarefa de seguranca (monitoramento constante)
void task_safety_monitor(void* param);

// Tarefa de log (salva no SD)
void task_logger(void* param);

// Tarefa termica (DS18B20)
void task_thermal_read(void* param);

// ============================================================================
// PROTOTIPOS — MENSAGENS PADRAO
// ============================================================================

// Tipos de mensagem
#define MSG_TYPE_VALUE        1
#define MSG_TYPE_STATUS       2
#define MSG_TYPE_ERROR        3
#define MSG_TYPE_UPDATE       4
#define MSG_TYPE_STOP         5
#define MSG_TYPE_START        6

// Mensagens pre-definidas
void task_msg_start_measurement(uint8_t mode);
void task_msg_stop_measurement();
void task_msg_update_display(float value, const char* unit, uint16_t color);
void task_msg_safety_alert(float voltage);

#endif // TASK_MANAGER_H