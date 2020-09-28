/*
 * functions.c
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */

#include "functions.h"

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	if (x < in_min){
		x = in_min;
	}
	if (x > in_max){
		x = in_max;
	}
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

}


int getAbsDif(int number1, int number2){
	int result = number1 - number2;
	if (result < 0) {
	    result = -result;
	}
	return result;
}


void delayMicros(uint32_t micros){
	TIM17->CNT = 0;
	while (TIM17->CNT < micros){

	}
}

void delayMillis(uint32_t millis){
	TIM17->CNT = 0;
	TIM17->PSC = 47999;
	LL_TIM_GenerateEvent_UPDATE(TIM17);
	while (TIM17->CNT < millis){

	}
	TIM17->PSC = 47;
	LL_TIM_GenerateEvent_UPDATE(TIM17);
}
