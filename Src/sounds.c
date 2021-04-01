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
extern char play_tone_flag;
uint8_t beep_volume;


void pause(uint16_t ms){
	TIM1->CCR1 = 0; // volume of the beep, (duty cycle) don't go above 25
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 0;

	delayMillis(ms);
	TIM1->CCR1 = 5; // volume of the beep, (duty cycle) don't go above 25
	TIM1->CCR2 = 5;
	TIM1->CCR3 = 5;
}

void setVolume(uint8_t volume){
	if(volume > 11){
		volume = 11;
	}
	if(volume < 0){
		volume = 0;
	}
	beep_volume = volume * 2;           // volume variable from 0 - 11 equates to CCR value of 0-22
}

void setCaptureCompare(){
	TIM1->CCR1 = beep_volume; // volume of the beep, (duty cycle) don't go above 25 out of 2000
	TIM1->CCR2 = beep_volume;
	TIM1->CCR3 = beep_volume;
}

void playStartupTune(){
	__disable_irq();
	setCaptureCompare();
	comStep(3);       // activate a pwm channel

	TIM1->PSC = 55;        // frequency of beep
    delayMillis(200);         // duration of beep
    comStep(5);

    TIM1->PSC = 40;            // next beep is higher frequency
	delayMillis(200);

	comStep(6);
	TIM1->PSC = 25;         // higher again..
	delayMillis(200);
	allOff();                // turn all channels low again
	TIM1->PSC = 0;           // set prescaler back to 0.
	signaltimeout = 0;
	__enable_irq();
}

void playBrushedStartupTune(){
	__disable_irq();
	setCaptureCompare();
	comStep(3);       // activate a pwm channel
	TIM1->PSC = 25;        // frequency of beep
    delayMillis(300);         // duration of beep
	comStep(6);
	TIM1->PSC = 55;         // higher again..
	delayMillis(300);
	allOff();                // turn all channels low again
	TIM1->PSC = 0;           // set prescaler back to 0.
	signaltimeout = 0;
	__enable_irq();
}

void playDuskingTune(){
	setCaptureCompare();
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
	setCaptureCompare();
	comStep(1);
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
	setCaptureCompare();
	comStep(3);
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

void playDefaultTone(){
	 LL_IWDG_ReloadCounter(IWDG);
	TIM1->PSC = 50;
	setCaptureCompare();
	comStep(2);
	delayMillis(100);
	TIM1->PSC = 30;
	delayMillis(100);
	allOff();
	TIM1->PSC = 0;
	signaltimeout = 0;

}

void playChangedTone(){
	 LL_IWDG_ReloadCounter(IWDG);
	TIM1->PSC = 40;
	setCaptureCompare();
	comStep(2);
	delayMillis(100);
	TIM1->PSC = 80;
	delayMillis(100);
	allOff();
	TIM1->PSC = 0;
	signaltimeout = 0;

}


void playBeaconTune3(){

	__disable_irq();
	setCaptureCompare();
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
