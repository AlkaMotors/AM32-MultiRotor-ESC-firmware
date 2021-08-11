/*
 *
 *  Created on: Aug. 8, 2021
 *      Authors: giacomo892,shellixyz
 */
#include <string.h>
#include "eeprom_common.h"
#include "eeprom.h"

eeprom_t settings;

bool loadParameters()
{

    read_eeprom(&settings, EEPROM_START_ADDRESS);

    //TO-DO
    //manage CRC when the configurator changes settings
    //check crc field
    //if crc fails reset the parameters and load safe defaults and prevent arming

    //if (crc fail) restoreDefaultParameters();

    return true;
}

bool saveParameters()
{

    save_flash_nolib(&settings, EEPROM_START_ADDRESS);
    //TO-DO
    //update crc and then save to eeprom, manage CRC when the configurator changes settings

    return true;
}

bool restoreDefaultParameters()
{
    //first boot / crc fail...
    memset(&settings, 0, sizeof(settings));

    settings.common.boot = 1;
    settings.common.version = 1;
    settings.common.bl_version = 1;
    settings.hardware.complementary_pwm = 1;
    settings.hardware.variable_pwm = 1;
    settings.hardware.motor_poles = 14;
    settings.hardware.timing_advance_compact = 2;
    settings.hardware.pwm_frequency_multiplier_compact = 24;
    settings.hardware.beep_volume = 5;
    settings.hardware.startup_power_compact = 100;
    settings.hardware.motor_kv_compact = 55;            //motor_kv_compact*40 + 20
    settings.hardware.servo_low_value_compact = 128; //175;    //750 + servo_low_value_compact*2
    settings.hardware.servo_high_value_compact = 128;  // 75;    //1750 + servo_high_value_compact*2
    settings.hardware.servo_neutral_base_compact = 128;  // //1374 + servo_neutral_base_compact
    settings.hardware.servo_deadband = 50;
    settings.hardware.low_voltage_cut_off_threshold_compact = 80; //250+low_voltage_cut_off_threshold_compact
    settings.hardware.sine_mode_changeover_thottle_level = 5;
    settings.hardware.drag_brake_strength = 10;

    //other settings are ok to be zero or they are defaulted (constrained in other parts of the code)

    return true;
}
