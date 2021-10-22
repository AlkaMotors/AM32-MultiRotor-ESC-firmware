/*
 * ADC.h
 *
 *  Created on: May 20, 2020
 *      Author: Alka
 */

#include "main.h"
#include "targets.h"


#ifndef ADC_H_
#define ADC_H_



void ADC_DMA_Callback();
void enableADC_DMA();
void activateADC();
void ADC_Init(void);

void Configure_DMA();

void Configure_ADC();

void Activate_ADC();


#endif /* ADC_H_ */
