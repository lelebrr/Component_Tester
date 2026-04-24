#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <Print.h>

void fprint(Print &p, float val, int prec);
void showBackMsg();
void calibrate_probes();
void set_offsets();
void handle_timeout();
unsigned long safe_millis();
void toggle_dark_mode();
void toggle_silent_mode();
void start_timeout(unsigned long duration);
bool is_timeout_active();
void toggle_hold();
bool is_hold_active();
void increment_measurement_count();
void increment_faulty_count();
void get_measurement_stats(unsigned long *total, unsigned long *faulty);
void reset_counters();

#endif