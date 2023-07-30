/*
 * ADC.h
 *
 *  Created on: May 20, 2020
 *      Author: Alka
 */

#ifndef ADC_H_
#define ADC_H_

#include "main.h"
#include "targets.h"

void ADC_DMA_Callback();
void enableADC_DMA();
void activateADC();
void ADC_Init(void);

#endif /* ADC_H_ */
