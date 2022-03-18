/*
 * peripherals.h
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */

#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_



#endif /* PERIPHERALS_H_ */

#include "main.h"

void initAfterJump(void);
void initCorePeripherals(void);
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_TIM14_Init(void);
void MX_TIM16_Init(void);
void MX_TIM1_Init(void);
void MX_TIM3_Init(void);
void MX_TIM17_Init(void);
void MX_IWDG_Init(void);
//void MX_ADC_Init(void);
void MX_TIM2_Init(void);
void TEN_KHZ_Timer_Init(void);
/* USER CODE BEGIN PFP */
//static void MX_TIM16_Init2(void);
void UN_GPIO_Init(void);
void UN_TIM_Init(void);
