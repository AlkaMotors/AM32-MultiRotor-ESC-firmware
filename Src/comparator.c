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
}

void changeCompInput() {
//	TIM3->CNT = 0;
//	HAL_COMP_Stop_IT(&hcomp1);            // done in comparator interrupt routine

	if (step == 1 || step == 4) {   // c floating
	//	hcomp1.Init.InvertingInput = COMP_INVERTINGINPUT_IO1;
#ifdef use_A0_B4_C5_comp_order
		COMP->CSR = 0b1010001;        // pa5
#else
		COMP->CSR = 0b1100001;         // pa0
#endif
	}

	if (step == 2 || step == 5) {     // a floating
#ifdef use_A4_B5_C0_comp_order
		//COMP->CSR = 0x40;
	//	hcomp1.Init.InvertingInput = COMP_INVERTINGINPUT_DAC1;
		COMP->CSR = 0b1000001;                        /// if f051k6  step 2 , 5 is dac 1 ( swap comp input)
#endif

#ifdef use_A5_B4_C0_comp_order
	//hcomp1.Init.InvertingInput = COMP_INVERTINGINPUT_DAC2;      // PA5
		COMP->CSR = 0b1010001;
#endif
#ifdef use_A0_B4_C5_comp_order
	                                                              // PA0
		COMP->CSR = 0b1100001;
#endif
	}

	if (step == 3 || step == 6) {      // b floating
#ifdef use_A4_B5_C0_comp_order
		//COMP->CSR = 0x50
	//	hcomp1.Init.InvertingInput = COMP_INVERTINGINPUT_DAC2;
		COMP->CSR = 0b1010001;
#endif
#if defined (use_A5_B4_C0_comp_order) || defined (use_A0_B4_C5_comp_order)
	//	hcomp1.Init.InvertingInput = COMP_INVERTINGINPUT_DAC1;       // PA4
		COMP->CSR = 0b1000001;
#endif
	}
	if (rising){

		EXTI->RTSR = 0x0;
	EXTI->FTSR = 0x200000;

	//	hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_FALLING;   // polarity of comp output reversed
	}else{                          // falling bemf

	EXTI->FTSR = 0x0;
	EXTI->RTSR = 0x200000;
	//	hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_RISING;
	}


}
