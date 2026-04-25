// ============================================================================
// Sondvolt v3.2 — Banco de Dados de Componentes
// Hardware: ESP32-2432S028R (Cheap Yellow Display)
// ============================================================================
// Arquivo: database.h
// Descricao: Estrutura do banco de dados na RAM + SD + busca
// ============================================================================
#ifndef DATABASE_H
#define DATABASE_H

#include <stdint.h>
#include "types.h"
#include "config.h"

// ============================================================================
// ITEM DO BANCO DE DADOS
// ============================================================================
struct ComponentInfo {
    ComponentType type;
    char name[20];
    char description[40];
    char unit[8];
    float minGood;      // Mínimo para status BOM
    float maxGood;     // Máximo para status BOM
    float minSuspect;   // Margem inferior para SUSPEITO
    float maxSuspect;   // Margem superior para SUSPEITO
    ComponentStatus defaultStatus;
};

struct ComponentDatabase {
    ComponentInfo items[20];
    uint16_t count;
    bool loaded;
};

// ============================================================================
// PROTÓTIPOS
// ============================================================================

void db_init();
void db_reload();
ComponentStatus db_judge(ComponentType type, float value);
const char* db_status_string(ComponentStatus status);
uint16_t db_status_color(ComponentStatus status);
uint16_t get_db_count();

#endif // DATABASE_H