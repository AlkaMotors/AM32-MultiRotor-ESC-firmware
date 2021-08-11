/*
 * phaseouts.h
 *
 *  Created on: Apr 22, 2020
 *      Author: Alka
 */

#ifndef INC_PHASEOUTS_H_
#define INC_PHASEOUTS_H_

#include "mcu_main.h"

void allOff();
void comStep (int newStep);
void fullBrake();
void allpwm();
void proportionalBrake();

#endif /* INC_PHASEOUTS_H_ */
