/*
 * sounds.c
 *
 *  Created on: May 13, 2020
 *      Author: Alka
 */

#include "sounds.h"
#include "phaseouts.h"
#include "functions.h"

extern int signaltimeout;


void playStartupTune(){
	__disable_irq();
	TIM1->CCR1 = 10; // volume of the beep, (duty cycle) don't go above 25 out of 2000
	TIM1->CCR2 = 10;
	TIM1->CCR3 = 10;
	comStep(2);       // activate a pwm channel

	TIM1->PSC = 55;        // frequency of beep
    delayMillis(200);         // duration of beep
    comStep(4);

    TIM1->PSC = 40;            // next beep is higher frequency
	delayMillis(200);

	comStep(1);
	TIM1->PSC = 25;         // higher again..
	delayMillis(200);
	allOff();                // turn all channels low again
	TIM1->PSC = 0;           // set prescaler back to 0.
	signaltimeout = 0;
	__enable_irq();
}


void playDuskingTune(){
	TIM1->CCR1 = 5; // volume of the beep, (duty cycle) don't go above 25
	TIM1->CCR2 = 5;
	TIM1->CCR3 = 5;
	comStep(2);       // activate a pwm channel

	TIM1->PSC = 60;        // frequency of beep
    delayMillis(200);         // duration of beep
	TIM1->PSC = 55;            // next beep is higher frequency
	delayMillis(150);
	TIM1->PSC = 50;         // higher again..
	delayMillis(150);
	TIM1->PSC = 45;        // frequency of beep
    delayMillis(100);         // duration of beep
	TIM1->PSC = 50;            // next beep is higher frequency
	delayMillis(100);
	TIM1->PSC = 55;         // higher again..
	delayMillis(100);
	TIM1->PSC = 25;         // higher again..
	delayMillis(200);
	TIM1->PSC = 55;         // higher again..
	delayMillis(150);
	allOff();                // turn all channels low again
	TIM1->PSC = 0;           // set prescaler back to 0.
}


void playInputTune2(){

	__disable_irq();

	 LL_IWDG_ReloadCounter(IWDG);
	TIM1->PSC = 60;
	TIM1->CCR1 = 5;
	TIM1->CCR2 = 5;
	TIM1->CCR3 = 5;
	comStep(2);
	delayMillis(75);
	TIM1->PSC = 80;

	delayMillis(75);
	TIM1->PSC = 90;
	LL_IWDG_ReloadCounter(IWDG);
	delayMillis(75);

	allOff();
	TIM1->PSC = 0;
	signaltimeout = 0;
	__enable_irq();
}




void playInputTune(){
	__disable_irq();
	 LL_IWDG_ReloadCounter(IWDG);
	TIM1->PSC = 80;
	TIM1->CCR1 = 5;
	TIM1->CCR2 = 5;
	TIM1->CCR3 = 5;
	comStep(2);
	delayMillis(100);
	TIM1->PSC = 70;

	delayMillis(100);
	TIM1->PSC = 40;

	delayMillis(100);

	allOff();
	TIM1->PSC = 0;
	signaltimeout = 0;
	__enable_irq();
}

void playBeaconTune3(){

	__disable_irq();



	TIM1->CCR1 = 8;
	TIM1->CCR2 = 8;
	TIM1->CCR3 = 8;
	for(int i = 119 ; i > 0 ; i = i- 2){
		LL_IWDG_ReloadCounter(IWDG);
		comStep(i/20);
		TIM1->PSC = 10+(i / 2);
		delayMillis(10);
	}


	allOff();
	TIM1->PSC = 0;
	signaltimeout = 0;
	__enable_irq();
}
