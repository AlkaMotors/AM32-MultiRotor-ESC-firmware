/*
 * functions.c
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */

#include "functions.h"
#include "targets.h"

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
	UTILITY_TIMER->CNT = 0;
	while (UTILITY_TIMER->CNT < micros){

	}
}

void delayMillis(uint32_t millis){
	UTILITY_TIMER->CNT = 0;
	UTILITY_TIMER->PSC = CPU_FREQUENCY_MHZ*1000;
	LL_TIM_GenerateEvent_UPDATE(UTILITY_TIMER);
	while (UTILITY_TIMER->CNT < millis){

	}
	UTILITY_TIMER->PSC = CPU_FREQUENCY_MHZ;      // back to micros
	LL_TIM_GenerateEvent_UPDATE(UTILITY_TIMER);
}