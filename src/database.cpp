// ============================================================================
// Sondvolt v3.2 — Banco de Dados na RAM
// Hardware: ESP32-2432S028R (Cheap Yellow Display)
// ============================================================================
// Arquivo: database.cpp
// Descrição: Gerenciamento do banco de dados de componentes na memória
// ============================================================================

#include "database.h"
#include "config.h"
#include <SdFat.h>

extern SdFat sd;

// Estrutura de Componente no Banco
struct ComponentEntry {
    char name[20];
    float minVal;
    float maxVal;
    uint8_t type;
};

// Alocação na RAM (Aproximadamente 200 itens = 200 * 32 bytes = 6.4KB)
// Podemos expandir conforme necessário para usar mais RAM.
#define MAX_DB_ENTRIES 500
static ComponentEntry db_memory[MAX_DB_ENTRIES];
static uint16_t g_db_count = 0;

uint16_t get_db_count() {
    return g_db_count;
}

void db_init() {
    g_db_count = 0;
    
    // Tenta carregar do SD Card
    // (O caminho do arquivo deve estar definido em config.h ou pins.h)
    #ifdef DB_FILE_CSV
    FsFile file = sd.open(DB_FILE_CSV, O_RDONLY);
    if (file) {
        while (file.available() && g_db_count < MAX_DB_ENTRIES) {
            String line = file.readStringUntil('\n');
            if (line.length() > 5) {
                g_db_count++;
            }
        }
        file.close();
        Serial.printf("Banco de dados carregado: %d itens\n", g_db_count);
    } else {
        Serial.println("Arquivo de banco de dados não encontrado no SD.");
        // Carrega alguns padrões se falhar
        strcpy(db_memory[0].name, "Resistor 10k");
        db_memory[0].minVal = 9500;
        db_memory[0].maxVal = 10500;
        db_memory[0].type = 1;
        g_db_count = 1;
    }
    #endif
}

void db_reload() {
    Serial.println("Recarregando banco de dados...");
    db_init();
}

ComponentStatus db_judge(ComponentType type, float value) {
    // Busca no banco em RAM
    for (int i = 0; i < g_db_count; i++) {
        if (db_memory[i].type == (uint8_t)type) {
            if (value >= db_memory[i].minVal && value <= db_memory[i].maxVal) {
                return STATUS_GOOD;
            }
            if (value >= db_memory[i].minVal * 0.8f && value <= db_memory[i].maxVal * 1.2f) {
                return STATUS_SUSPECT;
            }
        }
    }
    return STATUS_BAD;
}

const char* db_status_string(ComponentStatus status) {
    switch (status) {
        case STATUS_GOOD:    return "BOM";
        case STATUS_SUSPECT: return "SUSPEITO";
        case STATUS_BAD:     return "RUIM";
        default:             return "---";
    }
}

uint16_t db_status_color(ComponentStatus status) {
    switch (status) {
        case STATUS_GOOD:    return C_SUCCESS;
        case STATUS_SUSPECT: return C_WARNING;
        case STATUS_BAD:     return C_ERROR;
        default:             return C_TEXT_SECONDARY;
    }
}