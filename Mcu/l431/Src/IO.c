/*
 * IO.c
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */

#include "targets.h"
#include "IO.h"
#include "dshot.h"
#include "serial_telemetry.h"
#include "functions.h"
#include "common.h"

//int max_servo_deviation = 250;
//int servorawinput;
char ic_timer_prescaler = CPU_FREQUENCY_MHZ / 8 - 1;
char output_timer_prescaler;
int buffersize = 32;
int smallestnumber = 20000;
uint32_t dma_buffer[64] = {0};
char out_put = 0;
char buffer_divider = 44;
int dshot_runout_timer = 62500;
uint16_t average_signal_pulse = 0;



void changeToInput(){
	  LL_DMA_SetDataTransferDirection(DMA1, INPUT_DMA_CHANNEL, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
#ifdef USE_TIMER_15_CHANNEL_1
	  LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM15);           // de-init timer 2
	  LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM15);
	  IC_TIMER_REGISTER->CCMR1 = 0x1;
	  IC_TIMER_REGISTER->CCER = 0xa;
#endif

	  IC_TIMER_REGISTER->PSC = ic_timer_prescaler;
	  IC_TIMER_REGISTER->ARR = 0xFFFF;
	  LL_TIM_GenerateEvent_UPDATE(IC_TIMER_REGISTER);
	  out_put = 0;
}



void receiveDshotDma(){
	changeToInput();
	IC_TIMER_REGISTER->CNT = 0;
	   LL_DMA_ConfigAddresses(DMA1, INPUT_DMA_CHANNEL, (uint32_t)&IC_TIMER_REGISTER->CCR1, (uint32_t)&dma_buffer, LL_DMA_GetDataTransferDirection(DMA1, INPUT_DMA_CHANNEL));
	   LL_DMA_SetDataLength(DMA1, INPUT_DMA_CHANNEL, buffersize);
	   LL_DMA_EnableIT_TC(DMA1, INPUT_DMA_CHANNEL);
	   LL_DMA_EnableIT_TE(DMA1, INPUT_DMA_CHANNEL);
	   LL_DMA_EnableChannel(DMA1, INPUT_DMA_CHANNEL);

	   LL_TIM_EnableDMAReq_CC1(IC_TIMER_REGISTER);
	   LL_TIM_EnableDMAReq_CC1(IC_TIMER_REGISTER);

	   LL_TIM_CC_EnableChannel(IC_TIMER_REGISTER, IC_TIMER_CHANNEL);
	   LL_TIM_EnableCounter(IC_TIMER_REGISTER);
	//   TIM16->PSC = 1;

}
void changeToOutput(){
	LL_DMA_SetDataTransferDirection(DMA1, INPUT_DMA_CHANNEL, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

#ifdef USE_TIMER_15_CHANNEL_1
	  LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM15);           // de-init timer 2
	  LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM15);
	  IC_TIMER_REGISTER->CCMR1 = 0x60;
	  IC_TIMER_REGISTER->CCER = 0x3;
#endif
	  IC_TIMER_REGISTER->PSC = output_timer_prescaler;
	  IC_TIMER_REGISTER->ARR = 115;
	  out_put = 1;
	  LL_TIM_GenerateEvent_UPDATE(IC_TIMER_REGISTER);
}

void sendDshotDma(){
	changeToOutput();
	          LL_DMA_ConfigAddresses(DMA1, INPUT_DMA_CHANNEL, (uint32_t)&gcr, (uint32_t)&IC_TIMER_REGISTER->CCR1, LL_DMA_GetDataTransferDirection(DMA1, INPUT_DMA_CHANNEL));

			  LL_DMA_SetDataLength(DMA1, INPUT_DMA_CHANNEL, 30);
			  LL_DMA_EnableIT_TC(DMA1, INPUT_DMA_CHANNEL);
			  LL_DMA_EnableIT_TE(DMA1, INPUT_DMA_CHANNEL);
			  LL_DMA_EnableChannel(DMA1, INPUT_DMA_CHANNEL);


			  LL_TIM_EnableDMAReq_CC1(IC_TIMER_REGISTER);

			  LL_TIM_CC_EnableChannel(IC_TIMER_REGISTER, IC_TIMER_CHANNEL);
			  LL_TIM_EnableAllOutputs(IC_TIMER_REGISTER);
			  LL_TIM_EnableCounter(IC_TIMER_REGISTER);

}

void checkDshot(){
		if ((smallestnumber >= 0)&&(smallestnumber < 2)&& (average_signal_pulse < 3)) {
		ic_timer_prescaler= 1;
		output_timer_prescaler=0;
		dshot = 1;
		buffer_divider = 44;
		dshot_runout_timer = 65000;
		buffersize = 32;
	}
	if ((smallestnumber >= 2)&&(smallestnumber < 4)&& (average_signal_pulse < 4)) {
			ic_timer_prescaler= 3;
			output_timer_prescaler=0;
			dshot = 1;
			buffersize = 32;
		}


	if ((smallestnumber >= 4)&&(smallestnumber < 6)&& (average_signal_pulse < 8)) {
		ic_timer_prescaler= 7;
		output_timer_prescaler=0;
		dshot = 1;
		buffersize = 32;
		inputSet = 1;
	}
	if ((smallestnumber >= 8 )&&(smallestnumber < 10)&& (average_signal_pulse < 20)){
		dshot = 1;
		ic_timer_prescaler=15;
		output_timer_prescaler=1;
//		IC_TIMER_REGISTER->CNT = 0xffff;
		buffersize = 32;
		inputSet = 1;
	}
}
void checkServo(){
		if (smallestnumber > 300 && smallestnumber < 20000){
			servoPwm = 1;
			ic_timer_prescaler=CPU_FREQUENCY_MHZ - 1;
			armed_count_threshold = 35;
			buffersize = 2;
			inputSet = 1;
		}


}


void detectInput(){
	smallestnumber = 20000;
	average_signal_pulse = 0;
	int lastnumber = dma_buffer[0];

	for ( int j = 1 ; j < 31; j++){
		if(dma_buffer[j] - lastnumber > 0 ){
		if((dma_buffer[j] - lastnumber) < smallestnumber){

			smallestnumber = dma_buffer[j] - lastnumber;

	}

		average_signal_pulse += (dma_buffer[j] - lastnumber);
	}
		lastnumber = dma_buffer[j];
	}
	average_signal_pulse = average_signal_pulse/32 ;

if(dshot == 1){
 checkDshot();
}
if(servoPwm == 1){
 checkServo();
}

if(!dshot && !servoPwm){
	checkDshot();
	checkServo();
}

}
