/*
 * comparator.c
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */

#include "comparator.h"
#include "targets.h"


void maskPhaseInterrupts(){
	EXTI->IMR &= (0 << 21);
	LL_EXTI_ClearFlag_0_31(EXTI_LINE);
}

void enableCompInterrupts(){
    EXTI->IMR |= (1 << 21);
}


void changeCompInput() {
//	TIM3->CNT = 0;
//	HAL_COMP_Stop_IT(&hcomp1);            // done in comparator interrupt routine

	if (step == 1 || step == 4) {   // c floating
    	COMP1->CSR = PHASE_C_COMP;
    }

	if (step == 2 || step == 5) {     // a floating
		COMP1->CSR = PHASE_A_COMP;
	}

	if (step == 3 || step == 6) {      // b floating
		COMP1->CSR = PHASE_B_COMP;
	}

	if (rising){
		  LL_EXTI_DisableRisingTrig_0_31(LL_EXTI_LINE_21);
		  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_21);

	}else{                          // falling bemf
		  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_21);
		  LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_21);

	}


}
