/*
 * dshot.h
 *
 *  Created on: Apr. 22, 2020
 *      Author: Alka
 */

#include "mcu_main.h"
#include "eeprom.h"

#ifndef INC_DSHOT_H_
#define INC_DSHOT_H_

void computeDshotDMA(void);
void make_dshot_package();

extern void playInputTune(void);
extern void playInputTune2(void);
extern void playBeaconTune3(void);
extern void saveEEpromSettings(void);

#endif /* INC_DSHOT_H_ */
