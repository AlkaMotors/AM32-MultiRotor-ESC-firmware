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
	//EXTI->IMR &= ~(1 << current_EXTI_LINE);
	EXTI->IMR &= ~current_EXTI_LINE;

	// -.\, clear should be done already in the interrupt handler
	//EXTI->PR = (PHASE_A_LL_EXTI_LINE | PHASE_B_LL_EXTI_LINE | PHASE_C_LL_EXTI_LINE); // clear bits
	// Following lines could be removed (same as LL_EXTI_ClearFlag_0_31 in EXTI0_1_IRQHandler)
	//EXTI->PR |= 1 << PHASE_A_EXTI_LINE;
	//EXTI->PR |= 1 << PHASE_B_EXTI_LINE;
	//EXTI->PR |= 1 << PHASE_C_EXTI_LINE;
}

void enableCompInterrupts(){
	//EXTI->IMR |= (1 << current_EXTI_LINE);
	//EXTI->PR = (PHASE_A_LL_EXTI_LINE | PHASE_B_LL_EXTI_LINE | PHASE_C_LL_EXTI_LINE); // clear bits
	EXTI->IMR |= current_EXTI_LINE;
}

void changeCompInput() {
	switch (step) {
		case 1: // fallthrough
		case 4:
			current_GPIO_PIN  = PHASE_C_EXTI_PIN;
			current_GPIO_PORT = PHASE_C_EXTI_PORT;
			//current_EXTI_LINE = PHASE_C_EXTI_LINE;
			current_EXTI_LINE = PHASE_C_LL_EXTI_LINE;
			LL_SYSCFG_SetEXTISource(SYSCFG_EXTI_PORTC, SYSCFG_EXTI_LINEC);
			break;

		case 2: // fallthrough
		case 5:
			current_GPIO_PIN  = PHASE_A_EXTI_PIN;
			current_GPIO_PORT = PHASE_A_EXTI_PORT;
			//current_EXTI_LINE = PHASE_A_EXTI_LINE;
			current_EXTI_LINE = PHASE_A_LL_EXTI_LINE;
			LL_SYSCFG_SetEXTISource(SYSCFG_EXTI_PORTA, SYSCFG_EXTI_LINEA);
			break;

		case 3: // fallthrough
		case 6:
			current_GPIO_PIN  = PHASE_B_EXTI_PIN;
			current_GPIO_PORT = PHASE_B_EXTI_PORT;
			//current_EXTI_LINE = PHASE_B_EXTI_LINE;
			current_EXTI_LINE = PHASE_B_LL_EXTI_LINE;
			LL_SYSCFG_SetEXTISource(SYSCFG_EXTI_PORTB, SYSCFG_EXTI_LINEB);
			break;

		default:
			// nothing to do
			break;
	}

	if (rising) {
		//EXTI->RTSR |= (1 << current_EXTI_LINE);
		//EXTI->RTSR |= current_EXTI_LINE;
		LL_EXTI_EnableRisingTrig_0_31(current_EXTI_LINE);
		//EXTI->FTSR &= ~(1 << current_EXTI_LINE);
		//EXTI->FTSR &= ~current_EXTI_LINE;
		LL_EXTI_DisableFallingTrig_0_31(current_EXTI_LINE);
	} else {
		//EXTI->FTSR |= (1 << current_EXTI_LINE);
		//EXTI->FTSR |= current_EXTI_LINE;
		LL_EXTI_EnableFallingTrig_0_31(current_EXTI_LINE);
		//EXTI->RTSR &= ~(1 << current_EXTI_LINE);
		//EXTI->RTSR &= ~current_EXTI_LINE;
		LL_EXTI_DisableRisingTrig_0_31(current_EXTI_LINE);
	}
	//EXTI->PR = (PHASE_A_LL_EXTI_LINE | PHASE_B_LL_EXTI_LINE | PHASE_C_LL_EXTI_LINE); // clear bits
}



