/*
 * peripherals.h
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */

#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

#include "main.h"

void initAfterJump(void);
void initCorePeripherals(void);
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void crsf_UART_Init(void);
void MX_COMP1_Init(void);
void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_IWDG_Init(void);
void MX_TIM16_Init(void);
void MX_TIM14_Init(void);
void MX_TIM6_Init(void);
void MX_TIM17_Init(void);
//static void MX_USART1_UART_Init(void);
void crsf_UART_Init(void);
void receiveCRSF();
void UN_TIM_Init(void);

void LED_GPIO_init(void);

#endif /* PERIPHERALS_H_ */
