/*
 * dshot.h
 *
 *  Created on: Apr. 22, 2020
 *      Author: Alka
 */

#ifndef INC_DSHOT_H_
#define INC_DSHOT_H_

#include "main.h"

void computeDshotDMA(void);
void make_dshot_package();

extern char EDT_ARM_ENABLE;
extern char EDT_ARMED;
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
