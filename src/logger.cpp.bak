// ============================================================================
// Component Tester PRO v3.0 — Logger (Implementação)
// ============================================================================

#include "logger.h"
#include "globals.h"
#include "config.h"
#include <SD.h>

// ============================================================================
// VARIÁVEIS
// ============================================================================
static File logFile;
static bool logFileOpen = false;
static unsigned long lastSaveMs = 0;

// ============================================================================
// INICIALIZAÇÃO
// ============================================================================
bool logger_init() {
    if (!sdCardPresent) {
        DBG_PRINTLN("[LOGGER] SD não disponível");
        return false;
    }

    // Abre o arquivo de log (cria se não existir)
    logFile = SD.open(LOG_FILE_PATH, FILE_APPEND);
    if (!logFile) {
        DBG_PRINTLN("[LOGGER] Erro ao abrir arquivo de log");
        return false;
    }

    // Escreve cabeçalho se o arquivo é novo
    if (logFile.size() == 0) {
        logFile.println(F("# CT PRO v" FW_VERSION " — Log de Medicoes"));
        logFile.println(F("# Timestamp;Component;Value;Unit;Status;Temp;Total"));
        logFile.flush();
    }

    logFileOpen = true;
    DBG_PRINTLN("[LOGGER] Logger SD pronto");
    return true;
}

void logger_close() {
    if (logFileOpen && logFile) {
        logFile.flush();
        logFile.close();
        logFileOpen = false;
    }
}

// ============================================================================
// ESCRITA
// ============================================================================
bool logger_write(const char* component, float value,
               const char* unit, const char* status) {
    return logger_write_ex(millis(), component, value, unit, 0);
}

bool logger_write_ex(unsigned long timestamp, const char* component,
                   float value, const char* unit, uint8_t status) {
    if (!sdCardPresent || !logFileOpen) return false;

    if (!logFile) {
        logFile = SD.open(LOG_FILE_PATH, FILE_APPEND);
        if (!logFile) return false;
    }

    char line[128];
    snprintf(line, sizeof(line),
            "%lu;%s;%.6f;%s;%s;%.2f;%lu",
            timestamp,
            component,
            value,
            unit,
            db_status_string((ComponentStatus)status),
            lastTemperature,
            totalMeasurements);

    size_t written = logFile.println(line);
    if (written > 0 && millis() - lastSaveMs > 1000) {
        logFile.flush();
        lastSaveMs = millis();
    }

    return written > 0;
}

void logger_log(LogLevel level, const char* message) {
    const char* levelStr;
    switch (level) {
        case LOG_DEBUG:    levelStr = "DEBUG"; break;
        case LOG_INFO:    levelStr = "INFO"; break;
        case LOG_WARNING: levelStr = "WARN"; break;
        case LOG_ERROR:   levelStr = "ERROR"; break;
        case LOG_CRITICAL: levelStr = "CRIT"; break;
        default:         levelStr = "LOG"; break;
    }

    char line[160];
    snprintf(line, sizeof(line),
            "%lu;[%s] %s", millis(), levelStr, message);
    DBG_PRINTLN(line);

    if (sdCardPresent && logFileOpen) {
        if (!logFile) {
            logFile = SD.open(LOG_FILE_PATH, FILE_APPEND);
        }
        if (logFile) {
            logFile.println(line);
        }
    }
}

void logger_update() {
    // Placeholder para tarefas periódicas
    // Salva batch se necessário
}