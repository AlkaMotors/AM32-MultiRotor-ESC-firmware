/*
 * IO.h
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */

#ifndef IO_H_
#define IO_H_

#endif /* IO_H_ */


#include "main.h"

void changeToOutput();
void changeToInput();
void receiveDshotDma();
void sendDshotDma();
void detectInput();

extern char bi_direction;
extern int input;
extern int newinput;
extern char inputSet;
extern char dshot;
extern char servoPwm;
extern uint32_t gcr[];
extern int armed_count_threshold;
extern char send_telemetry;
extern uint8_t degrees_celsius;
extern char crawler_mode;

extern uint16_t ADC_raw_volts;
extern uint16_t servo_low_threshold; // anything below this point considered 0
extern uint16_t servo_high_threshold;  // anything above this point considered 2000 (max)
extern uint16_t servo_neutral;
extern uint8_t servo_dead_band;



