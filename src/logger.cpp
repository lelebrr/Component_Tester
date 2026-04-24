// ============================================================================
// Sondvolt v3.0 — Logger (Implementacao)
// Descricao: Logging de medicoes no MicroSD
// ============================================================================

#include "logger.h"
#include "config.h"
#include "globals.h"
#include "sdcard.h"

#include <Arduino.h>
#include <string.h>

// ============================================================================
// VARIAVEIS
// ============================================================================

static File logFile;
static bool logInitialized = false;
static unsigned long logIntervalMs = 10000; // 10s entre logs
static unsigned long lastLogMs = 0;

// ============================================================================
// INICIALIZACAO
// ============================================================================

bool logger_init() {
    if (logInitialized) {
        return true;
    }

    if (!sdCardPresent) {
        DBG("[LOG] SD nao disponivel");
        return false;
    }

    // Abre arquivo de log (append)
    logFile = SD.open(LOG_FILE, FILE_APPEND);

    if (!logFile) {
        // Cria arquivo se nao existe
        logFile = SD.open(LOG_FILE, FILE_WRITE);
        if (logFile) {
            // Escreve cabecalho
            logFile.println(HISTORY_HEADER);
            logFile.close();
            logFile = SD.open(LOG_FILE, FILE_APPEND);
        }
    }

    if (!logFile) {
        DBG("[LOG] Falha ao abrir arquivo");
        return false;
    }

    logInitialized = true;
    DBG("[LOG] Logger inicializado");
    return true;
}

// ============================================================================
// ESCRITA DE LOG
// ============================================================================

bool logger_write(const char* component, float value,
                  const char* unit, const char* status) {
    if (!logInitialized || !sdCardPresent) {
        return false;
    }

    if (!logFile) {
        logFile = SD.open(LOG_FILE, FILE_APPEND);
        if (!logFile) {
            return false;
        }
    }

    // Formata linha CSV
    char line[128];
    unsigned long ts = millis();

    snprintf(line, sizeof(line),
             "%lu%c%s%c%.6g%c%s%c%s",
             ts, HISTORY_DELIMITER,
             component, HISTORY_DELIMITER,
             value, HISTORY_DELIMITER,
             unit, HISTORY_DELIMITER,
             status);

    size_t written = logFile.println(line);
    logFile.flush();

    return written > 0;
}

bool logger_write_ex(unsigned long timestamp, const char* component,
                     float value, const char* unit, uint8_t status) {
    if (!logInitialized || !sdCardPresent) {
        return false;
    }

    if (!logFile) {
        logFile = SD.open(LOG_FILE, FILE_APPEND);
        if (!logFile) {
            return false;
        }
    }

    const char* statusStr;
    switch (status) {
        case STATUS_GOOD: statusStr = "GOOD"; break;
        case STATUS_BAD: statusStr = "BAD"; break;
        case STATUS_SUSPECT: statusStr = "SUSPECT"; break;
        default: statusStr = "UNKNOWN"; break;
    }

    char line[128];
    snprintf(line, sizeof(line),
             "%lu%c%s%c%.6g%c%s%c%s",
             timestamp, HISTORY_DELIMITER,
             component, HISTORY_DELIMITER,
             value, HISTORY_DELIMITER,
             unit, HISTORY_DELIMITER,
             statusStr);

    size_t written = logFile.println(line);
    logFile.flush();

    return written > 0;
}

// ============================================================================
// LOG COM NIVEL
// ============================================================================

void logger_log(LogLevel level, const char* message) {
    const char* levelStr;
    switch (level) {
        case LOG_DEBUG: levelStr = "[D]"; break;
        case LOG_INFO: levelStr = "[I]"; break;
        case LOG_WARNING: levelStr = "[W]"; break;
        case LOG_ERROR: levelStr = "[E]"; break;
        case LOG_CRITICAL: levelStr = "[C]"; break;
        default: levelStr = "[?]"; break;
    }

    char line[256];
    snprintf(line, sizeof(line), "%lu %s %s", millis(), levelStr, message);

    // Sempre imprime no Serial
    Serial.println(line);

    // Se SD disponivel, salva no arquivo
    if (sdCardPresent) {
        if (!logFile) {
            logFile = SD.open(LOG_FILE, FILE_APPEND);
        }
        if (logFile) {
            logFile.println(line);
            logFile.flush();
        }
    }
}

// ============================================================================
// ROTINA DE LOG (chamada no loop)
// ============================================================================

void logger_update() {
    if (!logInitialized) {
        return;
    }

    unsigned long now = millis();

    // Verifica intervalo
    if ((now - lastLogMs) >= logIntervalMs) {
        // Log estatisticas periodicas
        if (totalMeasurements > 0) {
            char msg[64];
            snprintf(msg, sizeof(msg),
                     "Total:%u Good:%u Bad:%u",
                     totalMeasurements, goodMeasurements, badMeasurements);
            logger_log(LOG_INFO, msg);
        }

        lastLogMs = now;
    }

    // Fecha arquivo periodicamente para salvar
    if (logFile) {
        if (logFile.size() > 1024) {
            logFile.flush();
        }
    }
}

// ============================================================================
// FECHAR ARQUIVO
// ============================================================================

void logger_close() {
    if (logFile) {
        logFile.flush();
        logFile.close();
        logFile = File();
    }
    logInitialized = false;
    DBG("[LOG] Logger fechado");
}