/*
 * sounds.c
 *
 *  Created on: May 13, 2020
 *      Author: Alka
 */

#include "main.h"
#include "sounds.h"
#include "phaseouts.h"
#include "functions.h"
#include "eeprom.h"
#include "targets.h"


uint8_t beep_volume;

uint8_t blueJayTuneBuffer[128] = {};

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
	if(freq < 523){
		TIM1->PSC = 92*CPU_FREQUENCY_MHZ/48;
		timerOne_reload = map(freq, 261, 523, TIM1_AUTORELOAD, TIM1_AUTORELOAD/2);
	}
	if(freq > 523 && freq < 1046){
		TIM1->PSC = 46*CPU_FREQUENCY_MHZ/48;
		timerOne_reload = map(freq, 523, 1046, TIM1_AUTORELOAD, TIM1_AUTORELOAD/2);
	}
	if(freq > 1046){
		TIM1->PSC = 23*CPU_FREQUENCY_MHZ/48;
		timerOne_reload = map(freq, 1046, 4186, TIM1_AUTORELOAD, TIM1_AUTORELOAD/4);
	}

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
	uint16_t frequency;
	comStep(3);

	//FIXME
	read_flash_bin(blueJayTuneBuffer , EEPROM_START_ADDRESS + 48);

	for(int i = 4 ; i < 128 ; i+=2){
		LL_IWDG_ReloadCounter(IWDG);
		input_signal_missing = 0;

		if(blueJayTuneBuffer[i] == 255){
			full_time_count++;

		}else{
			if(blueJayTuneBuffer[i+1] == 0){
				duration = full_time_count * 254 + blueJayTuneBuffer[i];
				TIM1->CCR1 = 0 ; //
				TIM1->CCR2 = 0;
				TIM1->CCR3 = 0;
				delayMillis(duration);
			}else{
			frequency = getBlueJayNoteFrequency(blueJayTuneBuffer[i+1]);
			duration= (full_time_count * 254 + blueJayTuneBuffer[i])  * (float)(1000 / frequency);
			playBJNote(frequency, duration);
			}
			full_time_count = 0;
		}
	}
	allOff();                // turn all channels low again
	TIM1->PSC = 0;           // set prescaler back to 0.
	TIM1->ARR = TIM1_AUTORELOAD;
	input_signal_missing = 0;
	LL_IWDG_ReloadCounter(IWDG);
}

void playStartupTune(){
	__disable_irq();
   //FIXME
	uint8_t value = ((uint8_t*)EEPROM_START_ADDRESS)[48];
		if(value != 0xFF){
		playBlueJayTune();
		}else{

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
	input_signal_missing = 0;
	}
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
	input_signal_missing = 0;
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
	input_signal_missing = 0;
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
	input_signal_missing = 0;
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
	input_signal_missing = 0;

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
	input_signal_missing = 0;

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
	input_signal_missing = 0;
	__enable_irq();
}
