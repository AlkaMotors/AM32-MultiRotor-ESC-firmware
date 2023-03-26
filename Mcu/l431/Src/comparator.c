///*
// * comparator.c
// *
// *  Created on: Sep. 26, 2020
// *      Author: Alka
// */
//
//#include "comparator.h"
//#include "targets.h"
//
//
//void maskPhaseInterrupts(){
//	EXTI->IMR1 &= ~(1 << 18);
//	EXTI->RPR1 = EXTI_LINE;
//	EXTI->FPR1 = EXTI_LINE;
////	LL_EXTI_ClearRisingFlag_0_31(EXTI_LINE);
////	LL_EXTI_ClearFallingFlag_0_31(EXTI_LINE);
//}
//
//void enableCompInterrupts(){
//    EXTI->IMR1 |= (1 << 18);
//}
//
//void changeCompInput() {
////	TIM3->CNT = 0;
////	HAL_COMP_Stop_IT(&hcomp1);            // done in comparator interrupt routine
//
////	LL_COMP_InitTypeDef COMP_InitStruct = {0};
//
//
//
//	if (step == 1 || step == 4) {   // c floating
//
//	//	LL_COMP_ConfigInputs(COMP2, PHASE_C_COMP , LL_COMP_INPUT_PLUS_IO3);
//
//		COMP2->CSR = 0x100281;
//
//	}
//
//	if (step == 2 || step == 5) {     // a floating
//
//	//	LL_COMP_ConfigInputs(COMP2, PHASE_A_COMP, LL_COMP_INPUT_PLUS_IO3);
//		COMP2->CSR = 0x100271;
//
//
//	}
//
//	if (step == 3 || step == 6) {      // b floating
//
//	//	LL_COMP_ConfigInputs(COMP2, PHASE_B_COMP, LL_COMP_INPUT_PLUS_IO3);
//		COMP2->CSR = 0x100261;
//	}
//	//COMP_InitStruct.InputPlus = LL_COMP_INPUT_PLUS_IO3;
//	//LL_COMP_Init(COMP2, &COMP_InitStruct);
//
//	if (rising){
//
//	//	  LL_EXTI_DisableRisingTrig_0_31(LL_EXTI_LINE_18);
//	//	  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_18);
//		  EXTI->RTSR1 &= ~(LL_EXTI_LINE_18);
//		  EXTI->FTSR1 |= LL_EXTI_LINE_18;
//
//	//	hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_FALLING;   // polarity of comp output reversed
//	}else{                          // falling bemf
//
//		  EXTI->RTSR1 |= LL_EXTI_LINE_18;
//		  EXTI->FTSR1 &= ~(LL_EXTI_LINE_18);
//	//	  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_18);
//	//	  LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_18);
//	//	hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_RISING;
//	}
//
////	 LL_COMP_Enable(COMP2);
//}

/*
 * comparator.c
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */

#include "comparator.h"
#include "targets.h"
#include "common.h"

COMP_TypeDef* active_COMP = COMP2;
uint32_t current_EXTI_LINE = LL_EXTI_LINE_18;

void maskPhaseInterrupts(){
	EXTI->IMR1 &= ~(1 << 18);
	LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_18);
	LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_18);
#ifdef N_VARIANT
	EXTI->IMR1 &= ~(1 << 17);
	LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_17);
	LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_17);
#endif

}

void enableCompInterrupts(){
    EXTI->IMR1 |= current_EXTI_LINE;
}

void changeCompInput() {
//	TIM3->CNT = 0;
//	HAL_COMP_Stop_IT(&hcomp1);            // done in comparator interrupt routine

//	LL_COMP_InitTypeDef COMP_InitStruct = {0};



	if (step == 1 || step == 4) {   // c floating
#ifdef N_VARIANT
		current_EXTI_LINE = PHASE_C_EXTI_LINE;
		active_COMP = PHASE_C_COMP_NUMBER;
#endif
		LL_COMP_ConfigInputs(active_COMP, PHASE_C_COMP , LL_COMP_INPUT_PLUS_IO3);


	}

	if (step == 2 || step == 5) {     // a floating
#ifdef N_VARIANT
		current_EXTI_LINE = PHASE_A_EXTI_LINE;
		active_COMP = PHASE_A_COMP_NUMBER;
#endif
	LL_COMP_ConfigInputs(active_COMP, PHASE_A_COMP, LL_COMP_INPUT_PLUS_IO3);
	}

	if (step == 3 || step == 6) {      // b floating
#ifdef N_VARIANT
		current_EXTI_LINE = PHASE_B_EXTI_LINE;
		active_COMP = PHASE_B_COMP_NUMBER;
#endif
    LL_COMP_ConfigInputs(active_COMP, PHASE_B_COMP, LL_COMP_INPUT_PLUS_IO3);
	}
	//COMP_InitStruct.InputPlus = LL_COMP_INPUT_PLUS_IO3;
	//LL_COMP_Init(COMP2, &COMP_InitStruct);

	if (rising){

		  LL_EXTI_DisableRisingTrig_0_31(LL_EXTI_LINE_18);
		  LL_EXTI_DisableRisingTrig_0_31(LL_EXTI_LINE_17);
		  LL_EXTI_EnableFallingTrig_0_31(current_EXTI_LINE);

	//	hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_FALLING;   // polarity of comp output reversed
	}else{                          // falling bemf


		  LL_EXTI_EnableRisingTrig_0_31(current_EXTI_LINE);
		  LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_17);
		  LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_18);
	//	hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_RISING;
	}

//	 LL_COMP_Enable(COMP2);
}


