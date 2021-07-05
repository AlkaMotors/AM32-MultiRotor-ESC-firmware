/*
 * IO.c
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */

#include "targets.h"
#include "signal.h"
#include  "IO.h"
#include "dshot.h"
#include "serial_telemetry.h"
#include "functions.h"

int max_servo_deviation = 250;
int servorawinput;

void computeMSInput(){

	int lastnumber = dma_buffer[0];
	for ( int j = 1 ; j < 2; j++){

		if(((dma_buffer[j] - lastnumber) < 1500) && ((dma_buffer[j] - lastnumber) > 0)){ // blank space

			newinput = map((dma_buffer[j] - lastnumber),243,1200, 0, 2000);
			break;
		}
		lastnumber = dma_buffer[j];
	}
}


void computeServoInput(){


		if(((dma_buffer[1] - dma_buffer[0]) >900 ) && ((dma_buffer[1] - dma_buffer[0]) < 2150)){

			if(bi_direction){
				if(dma_buffer[1] - dma_buffer[0] <= servo_neutral){
				servorawinput = map((dma_buffer[1] - dma_buffer[0]), servo_low_threshold, servo_neutral, 0, 1000);
				}else{
				servorawinput = map((dma_buffer[1] - dma_buffer[0]), servo_neutral+1, servo_high_threshold, 1001, 2000);
				}
			}else{
			servorawinput = map((dma_buffer[1] - dma_buffer[0]), servo_low_threshold, servo_high_threshold, 0, 2000);
			}
			signaltimeout = 0;

		}else{
			zero_input_count = 0;      // reset if out of range
		}

	if (servorawinput - newinput > max_servo_deviation){
		newinput += max_servo_deviation;
	}else if(newinput - servorawinput > max_servo_deviation){
		newinput -= max_servo_deviation;
	}else{
		newinput = servorawinput;
	}

}

void transfercomplete(){
	if(armed && dshot_telemetry){
	    if(out_put){


	  	receiveDshotDma();
	   	return;
	    }else{

			sendDshotDma();
			make_dshot_package();
			computeDshotDMA();
	    return;
	    }
	}

	  if (inputSet == 0){
	 	 detectInput();
	 	receiveDshotDma();
	 return;
	  }

	if (inputSet == 1){



if(dshot_telemetry){
    if(out_put){
//    	TIM17->CNT = 0;
    	make_dshot_package();          // this takes around 10us !!
  	computeDshotDMA();             //this is slow too..
  	receiveDshotDma();             //holy smokes.. reverse the line and set up dma again
   	return;
    }else{
		sendDshotDma();
    return;
    }
}else{

		if (dshot == 1){
			computeDshotDMA();
			if(send_telemetry){
            // done in 10khz routine
			}
			receiveDshotDma();
		}
		if  (servoPwm == 1){
			computeServoInput();
			LL_TIM_IC_SetPolarity(IC_TIMER_REGISTER, IC_TIMER_CHANNEL, LL_TIM_IC_POLARITY_RISING); // setup rising pin trigger.
     		receiveDshotDma();
     	    LL_DMA_EnableIT_HT(DMA1, INPUT_DMA_CHANNEL);
		}

	}
if(!armed){
	if (adjusted_input < 0){
		adjusted_input = 0;
		}
	 if (adjusted_input == 0){                       // note this in input..not newinput so it will be adjusted be main loop
	 	zero_input_count++;
	 		}else{
	 	zero_input_count = 0;
	 	}
	}
	}
}





















