/*
 * comparator.c
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */

#include "comparator.h"
#include "targets.h"
#include "common.h"

uint32_t current_GPIO_PIN;
GPIO_TypeDef* current_GPIO_PORT;
uint32_t current_EXTI_LINE = 0;


void maskPhaseInterrupts(){
		EXTI->IMR &= ~(1 << current_EXTI_LINE);
		EXTI->PR |= 1 << PHASE_A_EXTI_LINE;
		EXTI->PR |= 1 << PHASE_B_EXTI_LINE;
		EXTI->PR |= 1 << PHASE_C_EXTI_LINE;
}

void enableCompInterrupts(){
	EXTI->IMR |= (1 << current_EXTI_LINE);
}

void changeCompInput(){
	if (step == 1 || step == 4){
		current_GPIO_PIN  = PHASE_C_EXTI_PIN;
		current_GPIO_PORT = PHASE_C_EXTI_PORT;
		current_EXTI_LINE = PHASE_C_EXTI_LINE;
		}
	    if (step == 2 || step == 5){            //        in phase two or 5 read from phase A PF
	    current_GPIO_PIN  = PHASE_A_EXTI_PIN;
	    current_GPIO_PORT = PHASE_A_EXTI_PORT;
	    current_EXTI_LINE = PHASE_A_EXTI_LINE;
		}
	    if (step == 3 || step == 6){
	    current_GPIO_PIN  = PHASE_B_EXTI_PIN;
	    current_GPIO_PORT = PHASE_B_EXTI_PORT;
	    current_EXTI_LINE = PHASE_B_EXTI_LINE;
		}
	    if (rising){
			EXTI->RTSR |= (1 << current_EXTI_LINE);
			EXTI->FTSR &= ~(1 << current_EXTI_LINE);
		}else{
			EXTI->FTSR |= (1 << current_EXTI_LINE);
			EXTI->RTSR &= ~(1 << current_EXTI_LINE);
		}
}



