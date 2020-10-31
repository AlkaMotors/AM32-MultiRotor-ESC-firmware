/*
 * comparator.c
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */

#include "comparator.h"
#include "targets.h"

#ifdef use_A0_B4_C5_comp_order
#define PHASE_A_COMP COMP_PA0
#define PHASE_B_COMP COMP_PA4
#define PHASE_C_COMP COMP_PA5
#endif

#ifdef use_A4_B5_C0_comp_order
#define PHASE_A_COMP COMP_PA4
#define PHASE_B_COMP COMP_PA5
#define PHASE_C_COMP COMP_PA0
#endif

#ifdef use_A5_B4_C0_comp_order
#define PHASE_A_COMP COMP_PA5
#define PHASE_B_COMP COMP_PA4
#define PHASE_C_COMP COMP_PA0
#endif

void maskPhaseInterrupts(){
	EXTI->IMR &= (0 << 21);
}

void changeCompInput()
{
	//	TIM3->CNT = 0;
	//	HAL_COMP_Stop_IT(&hcomp1);            // done in comparator interrupt routine

	if (step == 1 || step == 4)
	{   // c floating
		//	hcomp1.Init.InvertingInput = COMP_INVERTINGINPUT_IO1;
		COMP->CSR = PHASE_C_COMP;
	}

	if (step == 2 || step == 5) 
	{   // a floating
		//	hcomp1.Init.InvertingInput = COMP_INVERTINGINPUT_DAC1;
		COMP->CSR = PHASE_A_COMP;
	}

	if (step == 3 || step == 6) 
	{   // b floating
		//	hcomp1.Init.InvertingInput = COMP_INVERTINGINPUT_DAC2;
		COMP->CSR = PHASE_B_COMP;
	}

	if (rising)
	{
		EXTI->RTSR = 0x0;
		EXTI->FTSR = 0x200000;

		//	hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_FALLING;   // polarity of comp output reversed
		}else{                          // falling bemf

		EXTI->FTSR = 0x0;
		EXTI->RTSR = 0x200000;
		//	hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_RISING;
	}


}
