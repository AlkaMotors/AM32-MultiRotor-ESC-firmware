/*
 * comparator.c
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */

#include "comparator.h"
#include "targets.h"


void maskPhaseInterrupts(){
	EXTI->IMR &= ~(1 << 21);
	LL_EXTI_ClearFlag_0_31(EXTI_LINE);
}

void enableCompInterrupts(){
    EXTI->IMR |= (1 << 21);
}


void changeCompInput()
{
	//	TIM3->CNT = 0;
	//	HAL_COMP_Stop_IT(&hcomp1);            // done in comparator interrupt routine

	if (step == 1 || step == 4)
	{   // c floating
		COMP->CSR = PHASE_C_COMP;
	}
	if (step == 2 || step == 5)
	{   // a floating
		COMP->CSR = PHASE_A_COMP;
	}
	if (step == 3 || step == 6)
	{   // b floating
		COMP->CSR = PHASE_B_COMP;
	}
	if (rising)
	{

		EXTI->RTSR = 0x0;
		EXTI->FTSR = 0x200000;

		//	hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_FALLING;   // polarity of comp output reversed
	}else{
		// falling bemf
		EXTI->FTSR = 0x0;
		EXTI->RTSR = 0x200000;
		//	hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_RISING;
	}


}
