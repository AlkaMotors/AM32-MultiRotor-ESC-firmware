/*
 * phaseouts.c
 *
 *  Created on: Apr 22, 2020
 *      Author: Alka
 */
#include "phaseouts.h"
#include "targets.h"


extern char comp_pwm;
extern char prop_brake_active;

//////////////////////////////////PHASE 1//////////////////////

void phaseBPWM() {

		if(!comp_pwm  || prop_brake_active){            // for future
		LL_GPIO_SetPinMode(PHASE_B_GPIO_PORT_LOW, PHASE_B_GPIO_LOW, LL_GPIO_MODE_OUTPUT);
		PHASE_B_GPIO_PORT_LOW->BRR = PHASE_B_GPIO_LOW;
		}else{
		LL_GPIO_SetPinMode(PHASE_B_GPIO_PORT_LOW, PHASE_B_GPIO_LOW, LL_GPIO_MODE_ALTERNATE); // low
		}
		LL_GPIO_SetPinMode(PHASE_B_GPIO_PORT_HIGH, PHASE_B_GPIO_HIGH, LL_GPIO_MODE_ALTERNATE);  // high

	}

void phaseBFLOAT() {

		LL_GPIO_SetPinMode(PHASE_B_GPIO_PORT_LOW, PHASE_B_GPIO_LOW, LL_GPIO_MODE_OUTPUT);
		PHASE_B_GPIO_PORT_LOW->BRR = PHASE_B_GPIO_LOW;
		LL_GPIO_SetPinMode(PHASE_B_GPIO_PORT_HIGH, PHASE_B_GPIO_HIGH, LL_GPIO_MODE_OUTPUT);
		PHASE_B_GPIO_PORT_HIGH->BRR = PHASE_B_GPIO_HIGH;
	}

void phaseBLOW() {

	        // low mosfet on
		LL_GPIO_SetPinMode(PHASE_B_GPIO_PORT_LOW, PHASE_B_GPIO_LOW, LL_GPIO_MODE_OUTPUT);
		PHASE_B_GPIO_PORT_LOW->BSRR = PHASE_B_GPIO_LOW;
		LL_GPIO_SetPinMode(PHASE_B_GPIO_PORT_HIGH, PHASE_B_GPIO_HIGH, LL_GPIO_MODE_OUTPUT);
		PHASE_B_GPIO_PORT_HIGH->BRR = PHASE_B_GPIO_HIGH;
	}



//////////////////////////////PHASE 2//////////////////////////////////////////////////

void phaseCPWM() {

		if (!comp_pwm || prop_brake_active){
			LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_LOW, PHASE_C_GPIO_LOW, LL_GPIO_MODE_OUTPUT);
			PHASE_C_GPIO_PORT_LOW->BRR = PHASE_C_GPIO_LOW;
		}else{
		LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_LOW, PHASE_C_GPIO_LOW, LL_GPIO_MODE_ALTERNATE);
		}
		LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_HIGH, PHASE_C_GPIO_HIGH, LL_GPIO_MODE_ALTERNATE);

	}

void phaseCFLOAT() {

	         // floating
		LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_LOW, PHASE_C_GPIO_LOW, LL_GPIO_MODE_OUTPUT);
		PHASE_C_GPIO_PORT_LOW->BRR = PHASE_C_GPIO_LOW;
		LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_HIGH, PHASE_C_GPIO_HIGH, LL_GPIO_MODE_OUTPUT);
		PHASE_C_GPIO_PORT_HIGH->BRR = PHASE_C_GPIO_HIGH;
	}

void phaseCLOW() {

	            // lowside
		LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_LOW, PHASE_C_GPIO_LOW, LL_GPIO_MODE_OUTPUT);
		PHASE_C_GPIO_PORT_LOW->BSRR = PHASE_C_GPIO_LOW;
		LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_HIGH, PHASE_C_GPIO_HIGH, LL_GPIO_MODE_OUTPUT);
		PHASE_C_GPIO_PORT_HIGH->BRR = PHASE_C_GPIO_HIGH;
	}



///////////////////////////////////////////////PHASE 3 /////////////////////////////////////////////////

void phaseAPWM() {
//#endif
		if (!comp_pwm || prop_brake_active){
			LL_GPIO_SetPinMode(PHASE_A_GPIO_PORT_LOW, PHASE_A_GPIO_LOW, LL_GPIO_MODE_OUTPUT);
			PHASE_A_GPIO_PORT_LOW->BRR = PHASE_A_GPIO_LOW;
			}else{
		LL_GPIO_SetPinMode(PHASE_A_GPIO_PORT_LOW, PHASE_A_GPIO_LOW, LL_GPIO_MODE_ALTERNATE);
			}
		LL_GPIO_SetPinMode(PHASE_A_GPIO_PORT_HIGH, PHASE_A_GPIO_HIGH, LL_GPIO_MODE_ALTERNATE);

	}


void phaseAFLOAT() {

		LL_GPIO_SetPinMode(PHASE_A_GPIO_PORT_LOW, PHASE_A_GPIO_LOW, LL_GPIO_MODE_OUTPUT);
		PHASE_A_GPIO_PORT_LOW->BRR = PHASE_A_GPIO_LOW;
		LL_GPIO_SetPinMode(PHASE_A_GPIO_PORT_HIGH, PHASE_A_GPIO_HIGH, LL_GPIO_MODE_OUTPUT);
		PHASE_A_GPIO_PORT_HIGH->BRR = PHASE_A_GPIO_HIGH;
	}


void phaseALOW() {

   	    LL_GPIO_SetPinMode(PHASE_A_GPIO_PORT_LOW, PHASE_A_GPIO_LOW, LL_GPIO_MODE_OUTPUT);
		PHASE_A_GPIO_PORT_LOW->BSRR = PHASE_A_GPIO_LOW;
		LL_GPIO_SetPinMode(PHASE_A_GPIO_PORT_HIGH, PHASE_A_GPIO_HIGH, LL_GPIO_MODE_OUTPUT);
		PHASE_A_GPIO_PORT_HIGH->BRR = PHASE_A_GPIO_HIGH;
	}

void allOff(){
	phaseAFLOAT();
	phaseBFLOAT();
	phaseCFLOAT();
}


void  comStep (int newStep){
//TIM14->CNT = 0;
switch(newStep)
{

        case 1:			//A-B
        	phaseAPWM();
        	phaseBLOW();
        	phaseCFLOAT();
        	break;


        case 2:		// C-B
        	phaseAFLOAT();
        	phaseBLOW();
        	phaseCPWM();
        	break;



        case 3:	// C-A
        	phaseALOW();
        	phaseBFLOAT();
        	phaseCPWM();
        	break;


        case 4:// B-A
        	phaseALOW();
        	phaseBPWM();
        	phaseCFLOAT();
        	break;


        case 5:    // B-C
        	phaseAFLOAT();
        	phaseBPWM();
        	phaseCLOW();
        	break;


        case 6:      // A-C
        	phaseAPWM();
        	phaseBFLOAT();
        	phaseCLOW();
        	break;
	}

//stop_time = TIM14->CNT;

}

void fullBrake(){                     // full braking shorting all low sides
	phaseALOW();
	phaseBLOW();
	phaseCLOW();
}


void allpwm(){                        // for stepper_sine
	phaseAPWM();
	phaseBPWM();
	phaseCPWM();
}
