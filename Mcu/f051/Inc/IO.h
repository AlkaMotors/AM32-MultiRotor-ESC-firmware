/*
 * IO.h
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */

#ifndef IO_H_
#define IO_H_

#endif /* IO_H_ */


#include "mcu_main.h"
#include "../../../Src/main.h"

extern char buffer_divider;
extern char out_put;

void changeToOutput();
void changeToInput();
void receiveDshotDma();
void sendDshotDma();
void detectInput();
