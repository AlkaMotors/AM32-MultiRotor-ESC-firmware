/*
 *
 *  Created on: Aug. 8, 2021
 *      Authors: giacomo892,shellixyz
 */

#ifndef INC_EEPROM_COMMON_H_
#define INC_EEPROM_COMMON_H_

#include "stdbool.h"
#include "stdint.h"
#include "targets.h"

typedef union __attribute__((packed))
{
    uint8_t bytes[2];
    uint16_t word;
} uint8_16_u;

typedef struct __attribute__((packed))
{
    uint8_t boot;
    uint8_t version;
    uint8_t bl_version;
    uint8_t major_version;
    uint8_t minor_version;
    char firmware_name[12];
    bool reversed;
} common_eeprom_t;

typedef struct __attribute__((packed))
{
    bool bidir;
    bool sin_startup;
    bool complementary_pwm;
    bool variable_pwm;
    bool stuck_rotor_protection;
    uint8_t timing_advance_compact;
    uint8_t pwm_frequency_multiplier_compact;
    uint8_t startup_power_compact;
    uint8_t motor_kv_compact;
    uint8_t motor_poles;
    bool brake_on_stop;
    bool stall_protection;
    uint8_t beep_volume;
    bool thirty_ms_telemetry;
    uint8_t servo_low_value_compact;
    uint8_t servo_high_value_compact;
    uint8_t servo_neutral_base_compact;
    uint8_t servo_deadband;
    bool low_voltage_cut_off;
    uint8_t low_voltage_cut_off_threshold_compact;
    uint8_t rc_car_reversing_type;
    bool use_hall_sensor;
    uint8_t sine_mode_changeover_thottle_level;
    uint8_t drag_brake_strength;
} hardware_specific_eeprom_t;

typedef struct __attribute__((packed))
{
    common_eeprom_t common;
    hardware_specific_eeprom_t hardware;
} eeprom_t;

bool loadParameters();
bool saveParameters();
bool restoreDefaultParameters();


extern eeprom_t settings;

#endif /* INC_EEPROM_COMMON_H_ */
