// ============================================================================
// Sondvolt v3.x — Sistema de Calibração (Implementação)
// Hardware: ESP32-2432S028R (Cheap Yellow Display)
// ============================================================================

#include "calibration.h"
#include "ui.h"
#include "measurements.h"
#include "globals.h"
#include <Preferences.h>

CalibrationData currentCal;
static Preferences prefs;

void calibration_init() {
    calibration_load();
}

bool calibration_run_auto() {
    ui_calibration_update_progress(10, "Estabilizando...");
    vTaskDelay(pdMS_TO_TICKS(1000));

    // Passo 1: Resistência (Curto-circuito)
    ui_calibration_update_progress(30, "Medindo Resistencia...");
    float r_sum = 0;
    for(int i=0; i<10; i++) {
        r_sum += measurements_get_raw_resistance();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    currentCal.resistance_offset = r_sum / 10.0f;

    // Passo 2: Capacitância (Aberto)
    ui_calibration_update_progress(60, "Abra as pontas agora!");
    vTaskDelay(pdMS_TO_TICKS(3000));
    
    ui_calibration_update_progress(80, "Medindo Capacitancia...");
    float c_sum = 0;
    for(int i=0; i<10; i++) {
        c_sum += measurements_get_raw_capacitance();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    currentCal.capacitance_offset = c_sum / 10.0f;

    ui_calibration_update_progress(100, "Finalizado");
    
    // Validação básica
    if(currentCal.resistance_offset > 10.0f) { // Se maior que 10 ohms em curto, algo está errado
        ui_calibration_show_result(false, "Resistencia muito alta");
        return false;
    }

    calibration_save();
    ui_calibration_show_result(true, "Offset salvo com sucesso");
    return true;
}

void calibration_save() {
    prefs.begin("calib", false);
    prefs.putBytes("data", &currentCal, sizeof(CalibrationData));
    prefs.end();
    LOG_SERIAL_F("Calibracao salva no NVS");
}

void calibration_load() {
    prefs.begin("calib", true);
    if(prefs.getBytes("data", &currentCal, sizeof(CalibrationData)) != sizeof(CalibrationData)) {
        calibration_reset();
    }
    prefs.end();
}

void calibration_reset() {
    currentCal.resistance_offset = 0;
    currentCal.capacitance_offset = 0;
    currentCal.voltage_scale = 1.0f;
    currentCal.checksum = 0;
}
