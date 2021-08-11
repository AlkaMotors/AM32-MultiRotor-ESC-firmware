
#pragma once

#include <stdint.h>

extern int e_com_time;
extern int zero_crosses;
extern char send_telemetry;
extern int smoothedinput;
extern uint8_t max_duty_cycle_change;
extern char play_tone_flag;
extern char prop_brake_active;
extern uint32_t gcr[];
extern uint32_t dma_buffer[64];
extern char armed;
extern int newinput;
extern int dshot_frametime;
extern char dshot_telemetry;
extern int dshot_badcounts;
extern int dshotcommand;
extern uint16_t input_signal_missing;
extern int dshot_goodcounts;
extern int zero_input_count;
extern int forward;
extern int running;
extern uint8_t last_dshot_command;
extern int k_erpm;
extern uint16_t commutation_interval;
extern uint16_t process_time;
extern char dshot;
extern char servoPwm;
extern int armed_count_threshold;
extern char inputSet;
extern char rising;
extern char step;
extern uint16_t ADC_raw_volts;
extern uint16_t ADC_raw_temp;
extern uint16_t ADC_raw_current;
extern uint16_t servo_low_threshold; // anything below this point considered 0
extern uint16_t servo_high_threshold;  // anything above this point considered 2000 (max)
extern uint16_t servo_neutral;
extern uint8_t servo_dead_band;


extern char send_telemetry;
extern char telemetry_done;
extern char servoPwm;
