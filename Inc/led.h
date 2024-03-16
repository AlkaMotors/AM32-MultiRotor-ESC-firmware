/*
 * led.h
 *
 *  Created on: July 9, 2023
 *      Author: frank26080115
 */

#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void led_init();        // called from main, initalizes GPIO structures and clocks

void led_blink_100ms(); // call this every 100ms

// use these functions in the main application
void led_set_unarmed();
void led_set_armed();
void led_set_armstate(bool);
void led_set_running();
void led_sig_currentlimited();
void led_sig_stalled();
void led_sig_lowbatt();
void led_sig_desync();
void led_set_red();
void led_set_green();
void led_set_blue();

#endif
