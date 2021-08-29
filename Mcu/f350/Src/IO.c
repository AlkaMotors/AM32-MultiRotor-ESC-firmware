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

char ic_timer_prescaler = 1;
char output_timer_prescaler;
//int servorawinput;
int buffersize = 32;
int smallestnumber = 20000;
uint32_t dma_buffer[64];
char out_put = 0;
char buffer_divider = 44;
int dshot_runout_timer = 62500;

void changeToOutput(){

	LL_DMA_SetDataTransferDirection(DMA1, INPUT_DMA_CHANNEL, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

#ifdef USE_TIMER_2_CHANNEL_3

	  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM2);           // de-init timer 2
	  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM2);
	  IC_TIMER_REGISTER->CCMR2 = 0x60;
	  IC_TIMER_REGISTER->CCER = 0x200;
#endif

#ifdef USE_TIMER_3_CHANNEL_1
	  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM3);           // de-init timer 2
	  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM3);
	  IC_TIMER_REGISTER->CCMR1 = 0x60;
	  IC_TIMER_REGISTER->CCER = 0x3;
#endif
#ifdef USE_TIMER_15_CHANNEL_1
	  LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM15);
	  LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM15);
	  LL_TIM_OC_SetIdleState(TIM15, LL_TIM_CHANNEL_CH1, LL_TIM_OCIDLESTATE_HIGH);
	  IC_TIMER_REGISTER->CCMR1 = 0x60;                         // channel 1 tim 15
	  IC_TIMER_REGISTER->CCER = 0x3;
	//  LL_TIM_OC_SetPolarity(TIM15, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_LOW);

#endif

	  IC_TIMER_REGISTER->PSC = output_timer_prescaler;
	  IC_TIMER_REGISTER->ARR = 80;
	  out_put = 1;
	  LL_TIM_GenerateEvent_UPDATE(IC_TIMER_REGISTER);
}

void changeToInput(){
	  LL_DMA_SetDataTransferDirection(DMA1, INPUT_DMA_CHANNEL, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

#ifdef USE_TIMER_2_CHANNEL_3
	  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM2);           // de-init timer 2
	  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM2);
	  IC_TIMER_REGISTER->CCMR2 = 0x1;
	  IC_TIMER_REGISTER->CCER = 0xa00;
#endif

#ifdef USE_TIMER_3_CHANNEL_1
	  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM3);           // de-init timer 2
	  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM3);
	  IC_TIMER_REGISTER->CCMR1 = 0x1;
	  IC_TIMER_REGISTER->CCER = 0xa;
#endif
#ifdef USE_TIMER_15_CHANNEL_1
	  LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM15);
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
#ifdef USE_TIMER_2_CHANNEL_4
	   LL_DMA_ConfigAddresses(DMA1, INPUT_DMA_CHANNEL, (uint32_t)&IC_TIMER_REGISTER->CCR4, (uint32_t)&dma_buffer, LL_DMA_GetDataTransferDirection(DMA1, INPUT_DMA_CHANNEL));
#endif
#ifdef USE_TIMER_3_CHANNEL_1
	   LL_DMA_ConfigAddresses(DMA1, INPUT_DMA_CHANNEL, (uint32_t)&IC_TIMER_REGISTER->CCR1, (uint32_t)&dma_buffer, LL_DMA_GetDataTransferDirection(DMA1, INPUT_DMA_CHANNEL));
#endif
#ifdef USE_TIMER_15_CHANNEL_1
	   LL_DMA_ConfigAddresses(DMA1, INPUT_DMA_CHANNEL, (uint32_t)&IC_TIMER_REGISTER->CCR1, (uint32_t)&dma_buffer, LL_DMA_GetDataTransferDirection(DMA1, INPUT_DMA_CHANNEL));
#endif
	   LL_DMA_SetDataLength(DMA1, INPUT_DMA_CHANNEL, buffersize);
	   LL_DMA_EnableIT_TC(DMA1, INPUT_DMA_CHANNEL);
	   LL_DMA_EnableIT_TE(DMA1, INPUT_DMA_CHANNEL);
	   LL_DMA_EnableChannel(DMA1, INPUT_DMA_CHANNEL);
#ifdef USE_TIMER_2_CHANNEL_4
	   LL_TIM_EnableDMAReq_CC4(IC_TIMER_REGISTER);
	   LL_TIM_EnableDMAReq_CC4(IC_TIMER_REGISTER);
#endif
#ifdef USE_TIMER_3_CHANNEL_1
	   LL_TIM_EnableDMAReq_CC1(IC_TIMER_REGISTER);
	   LL_TIM_EnableDMAReq_CC1(IC_TIMER_REGISTER);
#endif
#ifdef USE_TIMER_15_CHANNEL_1
	   LL_TIM_EnableDMAReq_CC1(IC_TIMER_REGISTER);
	   LL_TIM_EnableDMAReq_CC1(IC_TIMER_REGISTER);
#endif
	   LL_TIM_CC_EnableChannel(IC_TIMER_REGISTER, IC_TIMER_CHANNEL);
	   LL_TIM_EnableCounter(IC_TIMER_REGISTER);
	//   TIM16->PSC = 1;

}

void sendDshotDma(){


	changeToOutput();


#ifdef USE_TIMER_2_CHANNEL_4
	          LL_DMA_ConfigAddresses(DMA1, INPUT_DMA_CHANNEL, (uint32_t)&gcr, (uint32_t)&IC_TIMER_REGISTER->CCR4, LL_DMA_GetDataTransferDirection(DMA1, INPUT_DMA_CHANNEL));
#endif
#ifdef USE_TIMER_3_CHANNEL_1
	          LL_DMA_ConfigAddresses(DMA1, INPUT_DMA_CHANNEL, (uint32_t)&gcr, (uint32_t)&IC_TIMER_REGISTER->CCR1, LL_DMA_GetDataTransferDirection(DMA1, INPUT_DMA_CHANNEL));
#endif
#ifdef USE_TIMER_15_CHANNEL_1
			  LL_DMA_ConfigAddresses(DMA1, INPUT_DMA_CHANNEL, (uint32_t)&gcr, (uint32_t)&IC_TIMER_REGISTER->CCR1, LL_DMA_GetDataTransferDirection(DMA1, INPUT_DMA_CHANNEL));
#endif
			  LL_DMA_SetDataLength(DMA1, INPUT_DMA_CHANNEL, 30);
			  LL_DMA_EnableIT_TC(DMA1, INPUT_DMA_CHANNEL);
			  LL_DMA_EnableIT_TE(DMA1, INPUT_DMA_CHANNEL);
			  LL_DMA_EnableChannel(DMA1, INPUT_DMA_CHANNEL);
#ifdef USE_TIMER_2_CHANNEL_4
			  LL_TIM_EnableDMAReq_CC4(IC_TIMER_REGISTER);
#endif
#ifdef USE_TIMER_3_CHANNEL_1
			  LL_TIM_EnableDMAReq_CC1(IC_TIMER_REGISTER);
#endif
#ifdef USE_TIMER_15_CHANNEL_1
			  LL_TIM_EnableDMAReq_CC1(IC_TIMER_REGISTER);
#endif
			  LL_TIM_CC_EnableChannel(IC_TIMER_REGISTER, IC_TIMER_CHANNEL);
			  LL_TIM_EnableAllOutputs(IC_TIMER_REGISTER);
			  LL_TIM_EnableCounter(IC_TIMER_REGISTER);
}


void detectInput(){
	smallestnumber = 20000;
	dshot = 0;
	servoPwm = 0;
	int lastnumber = dma_buffer[0];
	for ( int j = 1 ; j < 31; j++){

		if((dma_buffer[j] - lastnumber) < smallestnumber){ // blank space
			smallestnumber = dma_buffer[j] - lastnumber;

		}
		lastnumber = dma_buffer[j];
	}

	if ((smallestnumber > 3)&&(smallestnumber < 32)){
		ic_timer_prescaler= 0;
		output_timer_prescaler=0;
		dshot = 1;
		buffer_divider = 65;
		dshot_runout_timer = 65000;
		armed_count_threshold = 10000;
		buffersize = 32;
	}
	if ((smallestnumber > 32 )&&(smallestnumber < 80)){
		dshot = 1;
		ic_timer_prescaler=1;
		output_timer_prescaler=1;
		IC_TIMER_REGISTER->CNT = 0xffff;
		buffer_divider = 65;
		dshot_runout_timer = 65000;
		armed_count_threshold = 10000;
		buffersize = 32;
	}
	if (smallestnumber > 300 ){
		servoPwm = 1;
		ic_timer_prescaler=63;
		armed_count_threshold = 100;
		buffersize = 4;
	}

	if (smallestnumber == 20000 ){
		inputSet = 0;
	}else{

		inputSet = 1;
	}

}


//void computeMSInput(){

	//int lastnumber = dma_buffer[0];
	//for ( int j = 1 ; j < 2; j++){

		//if(((dma_buffer[j] - lastnumber) < 1500) && ((dma_buffer[j] - lastnumber) > 0)){ // blank space

			//newinput = map((dma_buffer[j] - lastnumber),243,1200, 0, 2000);
			//break;
		//}
		//lastnumber = dma_buffer[j];
	//}
//}


//void computeServoInput(){

	//int lastnumber = dma_buffer[0];
	//for ( int j = 1 ; j < 3; j++){

		//if(((dma_buffer[j] - lastnumber) >1000 ) && ((dma_buffer[j] - lastnumber) < 2010)){ // blank space

			//if(bi_direction){
				//if(dma_buffer[j] - lastnumber <= servo_neutral){
				//servorawinput = map((dma_buffer[j] - lastnumber), servo_low_threshold, servo_neutral, 0, 1000);
				//}else{
				//servorawinput = map((dma_buffer[j] - lastnumber), servo_neutral+1, servo_high_threshold, 1001, 2000);
				//}
			//}else{
			//servorawinput = map((dma_buffer[j] - lastnumber), servo_low_threshold, servo_high_threshold, 0, 2000);
			//}
			//break;
		//}
		//lastnumber = dma_buffer[j];
	//}
	//if (servorawinput - newinput > max_servo_deviation){
		//newinput += max_servo_deviation;
	//}else if(newinput - servorawinput > max_servo_deviation){
		//newinput -= max_servo_deviation;
	//}else{
		//newinput = servorawinput;
	//}

//}

//void transfercomplete(){
	//if(armed && dshot_telemetry){
	    //if(out_put){


	  	//receiveDshotDma();
	   	//return;
	    //}else{

			//sendDshotDma();
			//make_dshot_package();
			//computeDshotDMA();
	    //return;
	    //}
	//}

	  //if (inputSet == 0){
	 	 //detectInput();
	 	//receiveDshotDma();
	 	//return;
	  //}

	//if (inputSet == 1){
	//if(!armed){
		//signaltimeout = 0;
		//if (input < 0){
			  						//input = 0;
			  					//}
		 		 //if (input == 0){                       // note this in input..not newinput so it will be adjusted be main loop
		 		 			//zero_input_count++;
		 		 		//}else{
		 		 			//zero_input_count = 0;
		 		 		//}
		//}

//if(dshot_telemetry){
    //if(out_put){
////    	TIM17->CNT = 0;
    	//make_dshot_package();          // this takes around 10us !!
  	//computeDshotDMA();             //this is slow too..
  	//receiveDshotDma();             //holy smokes.. reverse the line and set up dma again
   	//return;
    //}else{
		//sendDshotDma();
    //return;
    //}
//}else{

		//if (dshot == 1){
			//computeDshotDMA();
			//if(send_telemetry){
////			  makeTelemPackage(degrees_celsius,
////					  ADC_raw_volts,
////			  					  1000,
////			  					  200,
////			  					  k_erpm);
////			  send_telem_DMA();
			//}
			//receiveDshotDma();
		//}


		//if  (servoPwm == 1){
			//computeServoInput();
			//IC_TIMER_REGISTER->CNT = 0;
			//signaltimeout = 0;
			//receiveDshotDma();
		//}

	//}
	//}
//}




















