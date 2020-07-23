/*
 * serial_telemetry.h
 *
 *  Created on: May 13, 2020
 *      Author: Alka
 */


#include "main.h"

#ifndef SERIAL_TELEMETRY_H_
#define SERIAL_TELEMETRY_H_

void makeTelemPackage(uint8_t temp,
		              uint16_t voltage,
					  uint16_t current,
					  uint16_t consumption,
					  uint16_t e_rpm);


void telem_UART_Init(void);
void send_telem_DMA();

#endif /* SERIAL_TELEMETRY_H_ */
