/*
 * dshot.h
 *
 *  Created on: Apr. 22, 2020
 *      Author: Alka
 */

#include "main.h"

#ifndef INC_DSHOT_H_
#define INC_DSHOT_H_





void computeDshotDMA(void);
void make_dshot_package();

extern void playInputTune(void);
extern void playInputTune2(void);
extern void playBeaconTune3(void);
extern void saveEEpromSettings(void);

extern char dshot_telemetry;
extern char armed;
extern char dir_reversed;
extern char bi_direction;
extern char buffer_divider;
extern uint8_t last_dshot_command;
extern uint16_t commutation_interval;

//int e_com_time;


#endif /* INC_DSHOT_H_ */
