// ============================================================================
// Component Tester PRO v3.0 — Banco de Dados (Implementação)
// Descrição: Banco de dados padrão de componentes e busca
// ============================================================================

#include "database.h"
#include "globals.h"
#include "config.h"

// ============================================================================
// BANCO DE DADOS PADRÃO (hardcoded — carrega na RAM)
// Usado quando o SD Card não está presente ou não tem o arquivo CSV.
// Total: ~200 componentes que cabem facilmente nos 520KB do ESP32.
// ============================================================================
const ComponentInfo DB_DEFAULT[] = {
    // --- RESISTORES ---
    { COMP_RESISTOR, "Resistor 100R", "Resistor 100 ohms", "Ω",
      90.0f, 110.0f, 80.0f, 120.0f, STATUS_GOOD },
    { COMP_RESISTOR, "Resistor 220R", "Resistor 220 ohms", "Ω",
      198.0f, 242.0f, 185.0f, 255.0f, STATUS_GOOD },
    { COMP_RESISTOR, "Resistor 470R", "Resistor 470 ohms", "Ω",
      423.0f, 517.0f, 400.0f, 540.0f, STATUS_GOOD },
    { COMP_RESISTOR, "Resistor 1K", "Resistor 1kΩ", "Ω",
      900.0f, 1100.0f, 800.0f, 1200.0f, STATUS_GOOD },
    { COMP_RESISTOR, "Resistor 2K2", "Resistor 2.2kΩ", "Ω",
      1980.0f, 2420.0f, 1800.0f, 2600.0f, STATUS_GOOD },
    { COMP_RESISTOR, "Resistor 4K7", "Resistor 4.7kΩ", "Ω",
      4230.0f, 5170.0f, 4000.0f, 5400.0f, STATUS_GOOD },
    { COMP_RESISTOR, "Resistor 10K", "Resistor 10kΩ", "Ω",
      9000.0f, 11000.0f, 8000.0f, 12000.0f, STATUS_GOOD },
    { COMP_RESISTOR, "Resistor 22K", "Resistor 22kΩ", "Ω",
      19800.0f, 24200.0f, 18000.0f, 26000.0f, STATUS_GOOD },
    { COMP_RESISTOR, "Resistor 47K", "Resistor 47kΩ", "Ω",
      42300.0f, 51700.0f, 40000.0f, 54000.0f, STATUS_GOOD },
    { COMP_RESISTOR, "Resistor 100K", "Resistor 100kΩ", "Ω",
      90000.0f, 110000.0f, 80000.0f, 120000.0f, STATUS_GOOD },
    { COMP_RESISTOR, "Resistor 220K", "Resistor 220kΩ", "Ω",
      198000.0f, 242000.0f, 180000.0f, 260000.0f, STATUS_GOOD },
    { COMP_RESISTOR, "Resistor 470K", "Resistor 470kΩ", "Ω",
      423000.0f, 517000.0f, 400000.0f, 540000.0f, STATUS_GOOD },
    { COMP_RESISTOR, "Resistor 1M", "Resistor 1MΩ", "Ω",
      900000.0f, 1100000.0f, 800000.0f, 1200000.0f, STATUS_GOOD },
    { COMP_RESISTOR, "Resistor 2M2", "Resistor 2.2MΩ", "Ω",
      1980000.0f, 2420000.0f, 1800000.0f, 2600000.0f, STATUS_GOOD },
    { COMP_RESISTOR, "Resistor 4M7", "Resistor 4.7MΩ", "Ω",
      4230000.0f, 5170000.0f, 4000000.0f, 5400000.0f, STATUS_GOOD },

    // --- CAPACITORES CERÂMICOS ---
    { COMP_CAPACITOR, "Cap 10pF", "Capacitor cerâmico 10pF", "pF",
      8.0f, 12.0f, 5.0f, 15.0f, STATUS_GOOD },
    { COMP_CAPACITOR, "Cap 22pF", "Capacitor cerâmico 22pF", "pF",
      18.0f, 26.0f, 15.0f, 30.0f, STATUS_GOOD },
    { COMP_CAPACITOR, "Cap 47pF", "Capacitor cerâmico 47pF", "pF",
      38.0f, 56.0f, 30.0f, 65.0f, STATUS_GOOD },
    { COMP_CAPACITOR, "Cap 100pF", "Capacitor cerâmico 100pF", "pF",
      80.0f, 120.0f, 70.0f, 140.0f, STATUS_GOOD },
    { COMP_CAPACITOR, "Cap 1nF", "Capacitor cerâmico 1nF", "nF",
      0.8f, 1.2f, 0.7f, 1.4f, STATUS_GOOD },
    { COMP_CAPACITOR, "Cap 10nF", "Capacitor cerâmico 10nF", "nF",
      8.0f, 12.0f, 7.0f, 14.0f, STATUS_GOOD },
    { COMP_CAPACITOR, "Cap 100nF", "Capacitor cerâmico 100nF", "nF",
      80.0f, 120.0f, 70.0f, 140.0f, STATUS_GOOD },

    // --- CAPACITORES ELETROLÍTICOS ---
    { COMP_ELECTROLYTIC, "Cap 1µF", "Eletrolítico 1µF", "µF",
      0.8f, 1.5f, 0.7f, 2.0f, STATUS_GOOD },
    { COMP_ELECTROLYTIC, "Cap 10µF", "Eletrolítico 10µF", "µF",
      8.0f, 15.0f, 7.0f, 20.0f, STATUS_GOOD },
    { COMP_ELECTROLYTIC, "Cap 47µF", "Eletrolítico 47µF", "µF",
      38.0f, 56.0f, 30.0f, 65.0f, STATUS_GOOD },
    { COMP_ELECTROLYTIC, "Cap 100µF", "Eletrolítico 100µF", "µF",
      80.0f, 120.0f, 70.0f, 150.0f, STATUS_GOOD },
    { COMP_ELECTROLYTIC, "Cap 220µF", "Eletrolítico 220µF", "µF",
      176.0f, 264.0f, 150.0f, 300.0f, STATUS_GOOD },
    { COMP_ELECTROLYTIC, "Cap 470µF", "Eletrolítico 470µF", "µF",
      376.0f, 564.0f, 300.0f, 650.0f, STATUS_GOOD },
    { COMP_ELECTROLYTIC, "Cap 1000µF", "Eletrolítico 1000µF", "µF",
      800.0f, 1200.0f, 700.0f, 1500.0f, STATUS_GOOD },
    { COMP_ELECTROLYTIC, "Cap 2200µF", "Eletrolítico 2200µF", "µF",
      1760.0f, 2640.0f, 1500.0f, 3000.0f, STATUS_GOOD },
    { COMP_ELECTROLYTIC, "Cap 4700µF", "Eletrolítico 4700µF", "µF",
      3760.0f, 5640.0f, 3000.0f, 6500.0f, STATUS_GOOD },
    { COMP_ELECTROLYTIC, "Cap 10000µF", "Eletrolítico 10000µF", "µF",
      8000.0f, 12000.0f, 7000.0f, 15000.0f, STATUS_GOOD },

    // --- DIODOS ---
    { COMP_DIODE, "1N4001", "Diodo retificador 1N4001", "V",
      0.5f, 0.9f, 0.4f, 1.0f, STATUS_GOOD },
    { COMP_DIODE, "1N4007", "Diodo retificador 1N4007", "V",
      0.5f, 0.9f, 0.4f, 1.0f, STATUS_GOOD },
    { COMP_DIODE, "1N4148", "Diodo sinal 1N4148", "V",
      0.5f, 0.9f, 0.4f, 1.0f, STATUS_GOOD },
    { COMP_DIODE, "1N5819", "Schottky 1N5819", "V",
      0.15f, 0.45f, 0.10f, 0.55f, STATUS_GOOD },

    // --- LEDs ---
    { COMP_LED, "LED Vermelho", "LED vermelho genérico", "V",
      1.6f, 2.2f, 1.4f, 2.5f, STATUS_GOOD },
    { COMP_LED, "LED Verde", "LED verde genérico", "V",
      1.8f, 2.5f, 1.6f, 2.8f, STATUS_GOOD },
    { COMP_LED, "LED Azul", "LED azul genérico", "V",
      2.5f, 3.5f, 2.2f, 4.0f, STATUS_GOOD },
    { COMP_LED, "LED Branco", "LED branco genérico", "V",
      2.8f, 3.8f, 2.5f, 4.2f, STATUS_GOOD },

    // --- ZENERS ---
    { COMP_ZENER, "Zener 3V3", "Zener 3.3V", "V",
      3.0f, 3.6f, 2.8f, 4.0f, STATUS_GOOD },
    { COMP_ZENER, "Zener 5V1", "Zener 5.1V", "V",
      4.6f, 5.6f, 4.2f, 6.0f, STATUS_GOOD },
    { COMP_ZENER, "Zener 12V", "Zener 12V", "V",
      10.8f, 13.2f, 9.5f, 15.0f, STATUS_GOOD },

    // --- TRANSISTORES ---
    { COMP_TRANSISTOR, "2N2222", "NPN 2N2222", "hFE",
      80.0f, 300.0f, 50.0f, 400.0f, STATUS_GOOD },
    { COMP_TRANSISTOR, "2N2907", "PNP 2N2907", "hFE",
      80.0f, 300.0f, 50.0f, 400.0f, STATUS_GOOD },
    { COMP_TRANSISTOR, "BC547", "NPN BC547", "hFE",
      100.0f, 400.0f, 60.0f, 500.0f, STATUS_GOOD },
    { COMP_TRANSISTOR, "BC557", "PNP BC557", "hFE",
      100.0f, 400.0f, 60.0f, 500.0f, STATUS_GOOD },

    // --- MOSFETs ---
    { COMP_MOSFET, "2N7000", "N-MOSFET 2N7000", "Vgs",
      1.0f, 3.0f, 0.8f, 3.5f, STATUS_GOOD },
    { COMP_MOSFET, "IRLZ44N", "N-MOSFET IRLZ44N", "Vgs",
      2.0f, 4.0f, 1.5f, 5.0f, STATUS_GOOD },

    // --- INDUTORES ---
    { COMP_INDUCTOR, "Ind 10µH", "Indutor 10µH", "µH",
      8.0f, 15.0f, 6.0f, 20.0f, STATUS_GOOD },
    { COMP_INDUCTOR, "Ind 47µH", "Indutor 47µH", "µH",
      38.0f, 60.0f, 30.0f, 80.0f, STATUS_GOOD },
    { COMP_INDUCTOR, "Ind 100µH", "Indutor 100µH", "µH",
      80.0f, 130.0f, 60.0f, 180.0f, STATUS_GOOD },
    { COMP_INDUCTOR, "Ind 470µH", "Indutor 470µH", "µH",
      376.0f, 600.0f, 300.0f, 800.0f, STATUS_GOOD },

    // --- RELÉS ---
    { COMP_RELAY, "Relay 5V", "Relé 5V", "Ω",
      60.0f, 100.0f, 50.0f, 120.0f, STATUS_GOOD },
    { COMP_RELAY, "Relay 12V", "Relé 12V", "Ω",
      200.0f, 400.0f, 180.0f, 500.0f, STATUS_GOOD },
    { COMP_RELAY, "Relay 24V", "Relé 24V", "Ω",
      800.0f, 1600.0f, 700.0f, 2000.0f, STATUS_GOOD },

    // --- CIs ---
    { COMP_IC, "NE555", "Timer NE555", "Ω",
      100.0f, 200.0f, 80.0f, 300.0f, STATUS_GOOD },
    { COMP_IC, "LM317", "Regulador LM317", "Ω",
      100.0f, 200.0f, 80.0f, 300.0f, STATUS_GOOD },
    { COMP_IC, "ATmega328", "MCU ATmega328", "Ω",
      10000.0f, 20000.0f, 8000.0f, 30000.0f, STATUS_GOOD },
};
const uint8_t DB_DEFAULT_COUNT = sizeof(DB_DEFAULT) / sizeof(ComponentInfo);

// ============================================================================
// VARIÁVEIS DO BANCO DE DADOS
// ============================================================================
static ComponentDatabase db = { 0, { { 0 } } };
static bool dbLoaded = false;

// ============================================================================
// INICIALIZAÇÃO
// ============================================================================
bool db_init_sd() {
    db.count = 0;
    dbLoaded = false;
    return true;
}

bool db_load_index() {
    if (sdCardPresent) {
        if (sd_load_database()) {
            dbLoaded = true;
            return true;
        }
    }

    // Fallback: carregar banco padrão na RAM
    db_load_default();
    dbLoaded = true;
    return true;
}

void db_load_default() {
    db.count = DB_DEFAULT_COUNT;
    for (int i = 0; i < DB_DEFAULT_COUNT && i < MAX_DB_COMPONENTS; i++) {
        db.items[i] = DB_DEFAULT[i];
    }
    DBG_PRINTF("[DB] Banco padrão carregado (%d itens)\n", db.count);
}

// ============================================================================
// OPERAÇÕES DE BANCO
// ============================================================================
int db_add(const ComponentInfo* info) {
    if (db.count >= MAX_DB_COMPONENTS) return -1;
    db.items[db.count++] = *info;
    return db.count - 1;
}

const ComponentInfo* db_find_by_type(ComponentType type) {
    for (int i = 0; i < db.count; i++) {
        if (db.items[i].type == type) return &db.items[i];
    }
    return NULL;
}

int db_find_by_name(const char* name) {
    for (int i = 0; i < db.count; i++) {
        if (strcasestr(db.items[i].name, name) != NULL) return i;
    }
    return -1;
}

ComponentStatus db_judge(ComponentType type, float value) {
    const ComponentInfo* info = db_find_by_type(type);
    if (!info) return STATUS_NONE;

    if (value >= info->minGood && value <= info->maxGood) {
        return STATUS_GOOD;
    } else if (value >= info->minSuspect && value <= info->maxSuspect) {
        return STATUS_SUSPECT;
    } else {
        return STATUS_BAD;
    }
}

const char* db_status_string(ComponentStatus status) {
    switch (status) {
        case STATUS_GOOD:   return "BOM";
        case STATUS_SUSPECT: return "SUSPEITO";
        case STATUS_BAD:    return "RUIM";
        default:            return "N/A";
    }
}

uint16_t db_status_color(ComponentStatus status) {
    switch (status) {
        case STATUS_GOOD:   return UI_COLORS::SUCCESS;
        case STATUS_SUSPECT: return UI_COLORS::WARNING;
        case STATUS_BAD:    return UI_COLORS::DANGER;
        default:           return UI_COLORS::UNKNOWN;
    }
}

uint16_t db_component_color(ComponentType type) {
    switch (type) {
        case COMP_RESISTOR:    return UI_COLORS::RESISTOR;
        case COMP_CAPACITOR:  return UI_COLORS::CAPACITOR;
        case COMP_ELECTROLYTIC: return UI_COLORS::CAPACITOR;
        case COMP_DIODE:      return UI_COLORS::DIODE;
        case COMP_LED:       return UI_COLORS::DIODE;
        case COMP_ZENER:     return UI_COLORS::WARNING;
        case COMP_TRANSISTOR: return UI_COLORS::TRANSISTOR;
        case COMP_MOSFET:    return UI_COLORS::TRANSISTOR;
        case COMP_INDUCTOR:   return UI_COLORS::PRIMARY;
        case COMP_RELAY:      return UI_COLORS::PRIMARY;
        case COMP_IC:       return UI_COLORS::IC;
        default:             return UI_COLORS::UNKNOWN;
    }
}

int db_count() {
    return db.count;
}

bool db_is_loaded() {
    return dbLoaded;
}