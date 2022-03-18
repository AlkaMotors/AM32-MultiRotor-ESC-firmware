/*
 * comparator.c
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */

#include "comparator.h"
#include "targets.h"


void maskPhaseInterrupts(){
	EXTI->IMR1 &= (0 << 18);
	LL_EXTI_ClearRisingFlag_0_31(EXTI_LINE);
	LL_EXTI_ClearFallingFlag_0_31(EXTI_LINE);
}

void enableCompInterrupts(){
    EXTI->IMR1 |= (1 << 18);
}

void changeCompInput() {
//	TIM3->CNT = 0;
//	HAL_COMP_Stop_IT(&hcomp1);            // done in comparator interrupt routine

//	LL_COMP_InitTypeDef COMP_InitStruct = {0};



	if (step == 1 || step == 4) {   // c floating

		LL_COMP_ConfigInputs(COMP2, PHASE_C_COMP , LL_COMP_INPUT_PLUS_IO3);      


	}

	if (step == 2 || step == 5) {     // a floating
                                                   
		LL_COMP_ConfigInputs(COMP2, PHASE_A_COMP, LL_COMP_INPUT_PLUS_IO3);



	}

	if (step == 3 || step == 6) {      // b floating

		LL_COMP_ConfigInputs(COMP2, PHASE_B_COMP, LL_COMP_INPUT_PLUS_IO3);     

	}
	//COMP_InitStruct.InputPlus = LL_COMP_INPUT_PLUS_IO3;
	//LL_COMP_Init(COMP2, &COMP_InitStruct);

	if (rising){

		  LL_EXTI_DisableRisingTrig_0_31(LL_EXTI_LINE_18);
		  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_18);

	//	hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_FALLING;   // polarity of comp output reversed
	}else{                          // falling bemf


		  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_18);
		  LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_18);
	//	hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_RISING;
	}

//	 LL_COMP_Enable(COMP2);
}
