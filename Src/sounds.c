/*
 * sounds.c
 *
 *  Created on: May 13, 2020
 *      Author: Alka
 */

#include "sounds.h"
#include "phaseouts.h"
#include "functions.h"
#include "eeprom.h"
#include "targets.h"
#include "common.h"


uint8_t beep_volume;

//uint8_t blueJayTuneBuffer[128] = {};

void pause(uint16_t ms){
	TIM1->CCR1 = 0; // volume of the beep, (duty cycle) don't go above 25
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 0;

	delayMillis(ms);
	TIM1->CCR1 = beep_volume; // volume of the beep, (duty cycle) don't go above 25 out of 2000
	TIM1->CCR2 = beep_volume;
	TIM1->CCR3 = beep_volume;
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

void playBJNote(uint16_t freq, uint16_t bduration){        // hz and ms
	uint16_t timerOne_reload = TIM1_AUTORELOAD;
 
	TIM1->PSC = 10;
	timerOne_reload = 4800000 / freq;
	
	TIM1->ARR = timerOne_reload;
	TIM1->CCR1 = beep_volume * timerOne_reload /TIM1_AUTORELOAD ; // volume of the beep, (duty cycle) don't go above 25 out of 2000
	TIM1->CCR2 = beep_volume * timerOne_reload /TIM1_AUTORELOAD;
	TIM1->CCR3 = beep_volume * timerOne_reload /TIM1_AUTORELOAD;

	delayMillis(bduration);
}


uint16_t getBlueJayNoteFrequency(uint8_t bjarrayfreq){
	return 10000000/(bjarrayfreq * 247 + 4000);
}

void playBlueJayTune(){
	uint8_t full_time_count = 0;
	uint16_t duration;
	float frequency;
	comStep(3);
	//read_flash_bin(blueJayTuneBuffer , EEPROM_START_ADD + 48 , 128);
	for(int i = 52 ; i < 176 ; i+=2){
		LL_IWDG_ReloadCounter(IWDG);
		signaltimeout = 0;

		if(eepromBuffer[i] == 255){
			full_time_count++;

		}else{
			if(eepromBuffer[i+1] == 0){
				duration = full_time_count * 254 + eepromBuffer[i];
				TIM1->CCR1 = 0 ; //
				TIM1->CCR2 = 0;
				TIM1->CCR3 = 0;
				delayMillis(duration);
			}else{
			frequency = getBlueJayNoteFrequency(eepromBuffer[i+1]);
			duration= (full_time_count * 254 + eepromBuffer[i])  * (float)(1000 / frequency);
			playBJNote(frequency, duration);
			}
			full_time_count = 0;
		}
	}
	allOff();                // turn all channels low again
	TIM1->PSC = 0;           // set prescaler back to 0.
	TIM1->ARR = TIMER1_MAX_ARR;
	signaltimeout = 0;
	LL_IWDG_ReloadCounter(IWDG);
}

void playStartupTune(){
	__disable_irq();

	uint8_t value = *(uint8_t*)(EEPROM_START_ADD+48);
		if(value != 0xFF){
		playBlueJayTune();
		}else{
	TIM1->ARR = TIM1_AUTORELOAD;
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
	}
	
	TIM1->ARR = TIMER1_MAX_ARR;
	__enable_irq();
}

void playBrushedStartupTune(){
	__disable_irq();
	TIM1->ARR = TIM1_AUTORELOAD;
	setCaptureCompare();
	comStep(1);       // activate a pwm channel
	TIM1->PSC = 40;        // frequency of beep
    delayMillis(300);         // duration of beep
	comStep(2);       // activate a pwm channel
	TIM1->PSC = 30;        // frequency of beep
    delayMillis(300);         // duration of beep
	comStep(3);       // activate a pwm channel
	TIM1->PSC = 25;        // frequency of beep
    delayMillis(300);         // duration of beep
    comStep(4);
	TIM1->PSC = 20;         // higher again..
	delayMillis(300);
	allOff();                // turn all channels low again
	TIM1->PSC = 0;           // set prescaler back to 0.
	signaltimeout = 0;
	TIM1->ARR = TIMER1_MAX_ARR;
	__enable_irq();
}

void playDuskingTune(){
	setCaptureCompare();
	TIM1->ARR = TIM1_AUTORELOAD;
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
	TIM1->ARR = TIMER1_MAX_ARR;
}


void playInputTune2(){
    TIM1->ARR = TIM1_AUTORELOAD;
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
	TIM1->ARR = TIMER1_MAX_ARR;
	__enable_irq();
}




void playInputTune(){
	__disable_irq();
	TIM1->ARR = TIM1_AUTORELOAD;
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
	TIM1->ARR = TIMER1_MAX_ARR;
	__enable_irq();
}

void playDefaultTone(){
	TIM1->ARR = TIM1_AUTORELOAD;
	TIM1->PSC = 50;
	setCaptureCompare();
	comStep(2);
	delayMillis(150);
	 LL_IWDG_ReloadCounter(IWDG);
	TIM1->PSC = 30;
	delayMillis(150);
	allOff();
	TIM1->PSC = 0;
	signaltimeout = 0;
	TIM1->ARR = TIMER1_MAX_ARR;

}

void playChangedTone(){
	TIM1->ARR = TIM1_AUTORELOAD;
	TIM1->PSC = 40;
	setCaptureCompare();
	comStep(2);
	delayMillis(150);
	LL_IWDG_ReloadCounter(IWDG);
	TIM1->PSC = 80;
	delayMillis(150);
	allOff();
	TIM1->PSC = 0;
	signaltimeout = 0;
	TIM1->ARR = TIMER1_MAX_ARR;

}


void playBeaconTune3(){
	TIM1->ARR = TIM1_AUTORELOAD;
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
	TIM1->ARR = TIMER1_MAX_ARR;
	__enable_irq();
}
