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

void proportionalBrake(){  // alternate all channels between braking (ABC LOW) and coasting (ABC float)
	                        // put lower channel into alternate mode and turn upper OFF for each channel
	// turn all HIGH channels off for ABC

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT);
	GPIOA->BRR = LL_GPIO_PIN_9;

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT);
	GPIOA->BRR = LL_GPIO_PIN_10;

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);
	GPIOA->BRR = LL_GPIO_PIN_8;


	// set low channel to PWM, duty cycle will now control braking
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_14, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_15, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_13, LL_GPIO_MODE_ALTERNATE);
}



#ifndef PWM_ENABLE_BRIDGE
void phaseBPWM() {
		if(!comp_pwm){            // for future
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
	if (!comp_pwm){
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
		LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_LOW, PHASE_C_GPIO_LOW, LL_GPIO_MODE_OUTPUT);
		PHASE_C_GPIO_PORT_LOW->BSRR = PHASE_C_GPIO_LOW;
		LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_HIGH, PHASE_C_GPIO_HIGH, LL_GPIO_MODE_OUTPUT);
		PHASE_C_GPIO_PORT_HIGH->BRR = PHASE_C_GPIO_HIGH;
	}



///////////////////////////////////////////////PHASE 3 /////////////////////////////////////////////////

void phaseAPWM() {
		if (!comp_pwm){
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

#else

//////////////////////////////////PHASE 1//////////////////////
void phaseBPWM() {
		if(!comp_pwm){            // for future
		//LL_GPIO_SetPinMode(PHASE_B_GPIO_PORT_LOW, PHASE_B_GPIO_LOW, LL_GPIO_MODE_OUTPUT);
		//PHASE_B_GPIO_PORT_LOW->BRR = PHASE_B_GPIO_LOW;
		}else{
		LL_GPIO_SetPinMode(PHASE_B_GPIO_PORT_ENABLE, PHASE_B_GPIO_ENABLE, LL_GPIO_MODE_OUTPUT); // enable on
		PHASE_B_GPIO_PORT_ENABLE->BSRR = PHASE_B_GPIO_ENABLE;
		}
		LL_GPIO_SetPinMode(PHASE_B_GPIO_PORT_PWM, PHASE_B_GPIO_PWM, LL_GPIO_MODE_ALTERNATE);  // high pwm

	}


void phaseBFLOAT() {
	LL_GPIO_SetPinMode(PHASE_B_GPIO_PORT_ENABLE, PHASE_B_GPIO_ENABLE, LL_GPIO_MODE_OUTPUT); // enable off
	PHASE_B_GPIO_PORT_ENABLE->BRR = PHASE_B_GPIO_ENABLE;
		LL_GPIO_SetPinMode(PHASE_B_GPIO_PORT_PWM, PHASE_B_GPIO_PWM, LL_GPIO_MODE_OUTPUT);  // pwm off
		PHASE_B_GPIO_PORT_PWM->BRR = PHASE_B_GPIO_PWM;
	}


void phaseBLOW() {
	        // low mosfet on
	LL_GPIO_SetPinMode(PHASE_B_GPIO_PORT_ENABLE, PHASE_B_GPIO_ENABLE, LL_GPIO_MODE_OUTPUT);  // enable on
	PHASE_B_GPIO_PORT_ENABLE->BSRR = PHASE_B_GPIO_ENABLE;
		LL_GPIO_SetPinMode(PHASE_B_GPIO_PORT_PWM, PHASE_B_GPIO_PWM, LL_GPIO_MODE_OUTPUT);  // pwm off
		PHASE_B_GPIO_PORT_PWM->BRR = PHASE_B_GPIO_PWM;
}



//////////////////////////////PHASE 2//////////////////////////////////////////////////


void phaseCPWM() {
	if (!comp_pwm){
		//	LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_LOW, PHASE_C_GPIO_LOW, LL_GPIO_MODE_OUTPUT);
			//PHASE_C_GPIO_PORT_LOW->BRR = PHASE_C_GPIO_LOW;
		}else{
			LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_ENABLE, PHASE_C_GPIO_ENABLE, LL_GPIO_MODE_OUTPUT);  // enable on
			PHASE_C_GPIO_PORT_ENABLE->BSRR = PHASE_C_GPIO_ENABLE;
		}
		LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_PWM, PHASE_C_GPIO_PWM, LL_GPIO_MODE_ALTERNATE);

	}


void phaseCFLOAT() {
	         // floating
	LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_ENABLE, PHASE_C_GPIO_ENABLE, LL_GPIO_MODE_OUTPUT);  // enable off
				PHASE_C_GPIO_PORT_ENABLE->BRR = PHASE_C_GPIO_ENABLE;
		LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_PWM, PHASE_C_GPIO_PWM, LL_GPIO_MODE_OUTPUT);
		PHASE_C_GPIO_PORT_PWM->BRR = PHASE_C_GPIO_PWM;
	}



void phaseCLOW() {
	LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_ENABLE, PHASE_C_GPIO_ENABLE, LL_GPIO_MODE_OUTPUT);  // enable on
		PHASE_C_GPIO_PORT_ENABLE->BSRR = PHASE_C_GPIO_ENABLE;
		LL_GPIO_SetPinMode(PHASE_C_GPIO_PORT_PWM, PHASE_C_GPIO_PWM, LL_GPIO_MODE_OUTPUT);
		PHASE_C_GPIO_PORT_PWM->BRR = PHASE_C_GPIO_PWM;
	}



///////////////////////////////////////////////PHASE 3 /////////////////////////////////////////////////

void phaseAPWM() {
		if (!comp_pwm){
		//	LL_GPIO_SetPinMode(PHASE_A_GPIO_PORT_LOW, PHASE_A_GPIO_LOW, LL_GPIO_MODE_OUTPUT);
			//PHASE_A_GPIO_PORT_LOW->BRR = PHASE_A_GPIO_LOW;
			}else{
				LL_GPIO_SetPinMode(PHASE_A_GPIO_PORT_ENABLE, PHASE_A_GPIO_ENABLE, LL_GPIO_MODE_OUTPUT);  // enable on
				PHASE_A_GPIO_PORT_ENABLE->BSRR = PHASE_A_GPIO_ENABLE;
			}
		LL_GPIO_SetPinMode(PHASE_A_GPIO_PORT_PWM, PHASE_A_GPIO_PWM, LL_GPIO_MODE_ALTERNATE);
	}

void phaseAFLOAT() {
		LL_GPIO_SetPinMode(PHASE_A_GPIO_PORT_ENABLE, PHASE_A_GPIO_ENABLE, LL_GPIO_MODE_OUTPUT);  // enable on
		PHASE_A_GPIO_PORT_ENABLE->BRR = PHASE_A_GPIO_ENABLE;
		LL_GPIO_SetPinMode(PHASE_A_GPIO_PORT_PWM, PHASE_A_GPIO_PWM, LL_GPIO_MODE_OUTPUT);
		PHASE_A_GPIO_PORT_PWM->BRR = PHASE_A_GPIO_PWM;
	}

void phaseALOW() {
		LL_GPIO_SetPinMode(PHASE_A_GPIO_PORT_ENABLE, PHASE_A_GPIO_ENABLE, LL_GPIO_MODE_OUTPUT);  // enable on
		PHASE_A_GPIO_PORT_ENABLE->BSRR = PHASE_A_GPIO_ENABLE;
		LL_GPIO_SetPinMode(PHASE_A_GPIO_PORT_PWM, PHASE_A_GPIO_PWM, LL_GPIO_MODE_OUTPUT);
		PHASE_A_GPIO_PORT_PWM->BRR = PHASE_A_GPIO_PWM;
	}



#endif

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
