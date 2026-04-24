// ============================================================================
// Sondvolt v3.0 — Tipos Globais
// ============================================================================
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

// ============================================================================
// TIPOS DE COMPONENTES
// ============================================================================
enum ComponentType {
    COMP_RESISTOR    = 0,
    COMP_CAPACITOR   = 1,
    COMP_CAPACITOR_CERAMIC = 1,
    COMP_CAPACITOR_ELECTRO = 2,
    COMP_DIODE       = 3,
    COMP_LED         = 4,
    COMP_ZENER       = 5,
    COMP_TRANSISTOR  = 6,
    COMP_TRANSISTOR_NPN = 6,
    COMP_TRANSISTOR_PNP = 7,
    COMP_MOSFET      = 8,
    COMP_MOSFET_N    = 8,
    COMP_MOSFET_P    = 9,
    COMP_INDUCTOR   = 10,
    COMP_CRYSTAL    = 11,
    COMP_FUSE       = 12,
    COMP_VARISTOR   = 13,
    COMP_POTENTIOMETER = 14,
    COMP_OPTOCOUPLER = 15,
    COMP_RELAY      = 16,
    COMP_IC         = 17,
    COMP_COIL       = 18,
    COMP_UNKNOWN    = 99,
    COMP_NONE       = 100
};

// ============================================================================
// STATUS DE MEDICAO
// ============================================================================
enum ComponentStatus {
    STATUS_GOOD     = 0,
    STATUS_SUSPECT  = 1,
    STATUS_WARNING  = 1,
    STATUS_BAD      = 2,
    STATUS_INVALID  = 3,
    STATUS_NONE     = 4,
    STATUS_LEAKY    = 5,
    STATUS_SHORT    = 6,
    STATUS_OPEN     = 7
};

// ============================================================================
// TIPOS DE ICONES
// ============================================================================
enum IconType {
    ICON_RESISTOR,
    ICON_CAPACITOR,
    ICON_DIODE,
    ICON_LED,
    ICON_TRANSISTOR_NPN,
    ICON_TRANSISTOR_PNP,
    ICON_MOSFET_N,
    ICON_MOSFET_P,
    ICON_INDUCTOR,
    ICON_CRYSTAL,
    ICON_FUSE,
    ICON_VARISTOR,
    ICON_POTENTIOMETER,
    ICON_OPTOCOUPLER,
    ICON_MULTIMETER,
    ICON_TEMP,
    ICON_HISTORY,
    ICON_SETTINGS,
    ICON_ABOUT,
    ICON_WARNING,
    ICON_VOLTAGE,
    ICON_UNKNOWN
};

#endif
