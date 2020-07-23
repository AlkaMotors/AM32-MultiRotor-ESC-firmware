/* ALKA32- multirotor brushless controller firmware for the stm32f051 */

#include "main.h"

#include "targets.h"
#include "dshot.h"
#include "phaseouts.h"
#include "eeprom.h"
#include "sounds.h"
#include "ADC.h"
#include "serial_telemetry.h"

uint8_t version_major = 1;
uint8_t version_minor = 53;

#define APPLICATION_ADDRESS 0x08001000

uint16_t DEAD_TIME = 45;

int checkcount = 0;

typedef enum
{
  GPIO_PIN_RESET = 0U,
  GPIO_PIN_SET
}GPIO_PinState;

char dir_reversed = 0;
char bi_direction = 0;
char use_sin_start = 0;
char low_rpm_throttle_limit = 1;
char VARIABLE_PWM = 1;
char lowkv = 0;
char comp_pwm = 1;
int min_startup_duty = 180;
int sin_mode_min_s_d = 120;
char bemf_timeout = 10;
char crawler_mode = 0;
char advance_level = 2;                // 7.5 degree increments 0 , 7.5, 15, 22.5)
char stuck_rotor_protection = 1;

uint16_t pwm_shift_hysterisis = 10;
uint16_t pwm_dither_amount = 200;

char low_kv_filter_level = 20;

char stall_protection = 0;
int duty_cycle_maximum = 1999;
int low_rpm_level  = 20;        // thousand erpm
int high_rpm_level = 70;      //
int throttle_max_at_low_rpm  = 500;
int throttle_max_at_high_rpm = 1999;

uint16_t commutation_intervals[6] = {0};
uint32_t average_interval = 0;
uint32_t last_average_interval;
int e_com_time;

uint16_t ADC_smoothed_input = 0;
uint8_t degrees_celsius;
uint16_t ADC_raw_temp;
uint16_t ADC_raw_volts;
uint16_t ADC_raw_current;
uint16_t ADC_raw_input;
int adc_counter = 0;
char send_telemetry = 0;
char prop_brake_active = 0;

char out_put = 0;
int int_count = 0;

uint16_t filter_delay = 10;

uint8_t eepromBuffer[48] ={0};

uint32_t gcr[23] =  {0,0,0,0,64,0,0,0,0,64,0,0,0,0,64,0,0,0,64,64,0,64,0};
uint8_t gcr_size;


uint16_t process_time = 0;

uint16_t last_input_at_commutation = 0;
uint16_t current_input;

char dshot_telemetry = 0;
char coasting = 0;
char output = 0;
int dshot_frametime = 0;

uint16_t phase_a_interval = 0;
uint16_t phase_b_interval = 0;
uint16_t phase_c_interval = 0;
uint32_t current_EXTI_LINE;

char buffer_divider = 44;
int transfercount = 0;
int dshot_goodcounts = 0;
int dshot_badcounts = 0;
uint8_t last_dshot_command = 0;

int stop_time = 0;
char old_routine = 0;

int current_state= 0;
uint16_t Current_GPIO_Pin;
GPIO_TypeDef* current_gpio_port;


int adjusted_input;
int filter_level_up = 8;
int filter_level_down = 8;
int dshot_runout_timer = 62500;

#define TEMP30_CAL_VALUE            ((uint16_t*)((uint32_t)0x1FFFF7B8))
#define TEMP110_CAL_VALUE           ((uint16_t*)((uint32_t)0x1FFFF7C2))

int temp110cal;
int temp30cal;
int smoothedinput = 0;
int voltageraw;

const int numReadings = 100;     // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;
int readings[100];
int tempraw = 0;
int temp_degrees = 0;

int bemf_timout_happened = 0;
int timeout_count = 0;
int bemf_timeout_threshold = 10;

int changeover_step = 5;

int max_change;
int offset;

int ticks = 0;
int filter_level = 5;
int toggled = 0;
int running = 0;
int advance = 0;
int advancedivisor = 3;
int blanktime;
int START_ARR=800;
int rising = 1;
int count = 0;


const int pwmSin[] = {180,183,186,189,193,196,199,202,
		205,208,211,214,217,220,224,227,
		230,233,236,239,242,245,247,250,
		253,256,259,262,265,267,270,273,
		275,278,281,283,286,288,291,293,
		296,298,300,303,305,307,309,312,
		314,316,318,320,322,324,326,327,
		329,331,333,334,336,337,339,340,
		342,343,344,346,347,348,349,350,
		351,352,353,354,355,355,356,357,
		357,358,358,359,359,359,360,360,
		360,360,360,360,360,360,360,359,
		359,359,358,358,357,357,356,355,
		355,354,353,352,351,350,349,348,
		347,346,344,343,342,340,339,337,
		336,334,333,331,329,327,326,324,
		322,320,318,316,314,312,309,307,
		305,303,300,298,296,293,291,288,
		286,283,281,278,275,273,270,267,
		265,262,259,256,253,250,247,245,
		242,239,236,233,230,227,224,220,
		217,214,211,208,205,202,199,196,
		193,189,186,183,180,177,174,171,
		167,164,161,158,155,152,149,146,
		143,140,136,133,130,127,124,121,
		118,115,113,110,107,104,101,98,
		95,93,90,87,85,82,79,77,
		74,72,69,67,64,62,60,57,
		55,53,51,48,46,44,42,40,
		38,36,34,33,31,29,27,26,
		24,23,21,20,18,17,16,14,
		13,12,11,10,9,8,7,6,
		5,5,4,3,3,2,2,1,
		1,1,0,0,0,0,0,0,
		0,0,0,1,1,1,2,2,
		3,3,4,5,5,6,7,8,
		9,10,11,12,13,14,16,17,
		18,20,21,23,24,26,27,29,
		31,33,34,36,38,40,42,44,
		46,48,51,53,55,57,60,62,
		64,67,69,72,74,77,79,82,
		85,87,90,93,95,98,101,104,
		107,110,113,115,118,121,124,127,
		130,133,136,140,143,146,149,152,
		155,158,161,164,167,171,174,177};


int sin_divider = 2;
int phase_A_position;
int phase_B_position;
int phase_C_position;
int step_delay  = 100;
char stepper_sine = 0;
int forward = 1;
int gate_drive_offset = 60;

int stuckcounter = 0;
int duty_cycle_limit = 0;

int looptime;
int k_erpm;

char reversed_direction = 0;

char ic_timer_prescaler;
char output_timer_prescaler;

uint16_t adjusted_duty_cycle;
uint16_t tim1_arr = 1999;

int threshold_up = 20;
int threshold_down = 20;
int bad_count = 0;

int ADCtimer= 30;
int demagtime = 50;

int whatstepisthis = 0 ;       // for debugging
int myneutral = 1300; // for debugging
int myneutralup = 1500;


int buffersize = 32;
int smallestnumber = 20000;
uint32_t dma_buffer[64];
int propulse[4] = {0,0,0,0};

int calcCRC;
int checkCRC;
int dshotcommand;
int max_servo_deviation = 150;
int servorawinput;
int armed_count_threshold = 1000;

char armed = 0;
int zero_input_count = 0;

int input = 0;
int newinput =0;
char inputSet = 0;
char dshot = 0;
char proshot = 0;
char multishot = 0;
char oneshot42 = 0;
char oneshot125 = 0;
char servoPwm = 0;
int zero_crosses;


int integral = 0;
int threshold = 850;
int zcfound = 0;
int difference;
int bemfcounter;
int min_bemf_counts_up = 7;
int min_bemf_counts_down = 7;
int zcs = 0;

int adc_timer = 600;
int ROC = 1;

int lastzctime;
uint16_t thiszctime;
int upthiszctime;
int uplastzctime;
int wait_time;

int phase = 1;

int tim2_start_arr= 616;

uint32_t last_adc_channel;

int duty_cycle = 0;
uint32_t ADC1ConvertedValues[2] = {0,0};
int bemf_rising = 1;

int step = 1;
int pot = 1000;

uint16_t commutation_interval = 12500;
int pwm = 1;
int floating =2;
int lowside = 3;
int zero_cross_offset_up= 1;  //  up and down offset adc read
int zero_cross_offset_down= 30;
int sensorless = 1;
int waitTime = 0;
int threshhold = 5;

int signaltimeout = 0;

uint8_t ubAnalogWatchdogStatus = RESET;

uint16_t dshot_raw_input;
uint16_t max_dshot_deviation = 1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC_Init(void);
static void MX_COMP1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_IWDG_Init(void);
static void MX_TIM16_Init(void);
static void MX_TIM14_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM17_Init(void);
static void MX_USART1_UART_Init(void);

static void UN_TIM_Init(void);
static void LED_GPIO_init();

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


void loadEEpromSettings(){
	   read_flash_bin( eepromBuffer , 0x08007c00 , 48);
	   if(eepromBuffer[18] == 0x01){
	 	  bi_direction = 1;
	   }
	   if(eepromBuffer[17] == 0x01){
	 	  dir_reversed =  1;
	   }


	   if(eepromBuffer[20] == 0x01){
	  	  comp_pwm = 1;
	    }else{
	    	comp_pwm = 0;
	    }
	   if(eepromBuffer[21] == 0x01){
		   VARIABLE_PWM = 1;
	    }else{
	    	VARIABLE_PWM = 0;
	    }

	   if(eepromBuffer[22] == 0x01){
		   stuck_rotor_protection = 1;
	    }else{
	    	stuck_rotor_protection = 0;
	    }

	   if(eepromBuffer[23] < 4){
		   advance_level = eepromBuffer[23];
	    }else{
	    	advance_level = 2;  // * 7.5 increments
	    }

	   if(eepromBuffer[24] < 49 && eepromBuffer[24] > 23){
		   tim1_arr = map (eepromBuffer[24], 24, 48, 1999 ,999);
		   TIM1->ARR = tim1_arr;
	    }else{
	    	tim1_arr = 1999;
	    	TIM1->ARR = tim1_arr;
	    }

	   if(eepromBuffer[25] < 151 && eepromBuffer[25] > 49){
		   min_startup_duty = eepromBuffer[25] + 50;
	    }else{
	    	min_startup_duty = 150;
	    }

	   if(eepromBuffer[19] == 0x01){
	 	  use_sin_start = 1;
	 	 min_startup_duty = sin_mode_min_s_d;
	   }



	if(!comp_pwm){
		bi_direction = 0;
	}

}

void saveEEpromSettings(){

}


void initAfterJump(){
    volatile uint32_t *VectorTable = (volatile uint32_t *)0x20000000;
    uint32_t vector_index = 0;
    for(vector_index  = 0; vector_index  < 48; vector_index++)
    {
      VectorTable[vector_index ] = *(__IO uint32_t*)(APPLICATION_ADDRESS + (vector_index <<2));         // no VTOR on cortex-MO so need to copy vector table
    }
  //	  /* Enable the SYSCFG peripheral clock*/
    do {
       volatile uint32_t tmpreg;
       ((((RCC_TypeDef *) ((((uint32_t)0x40000000U) + 0x00020000) + 0x00001000))->APB2ENR) |= ((0x1U << (0U))));
           /* Delay after an RCC peripheral clock enabling */
      tmpreg = ((((RCC_TypeDef *) ((((uint32_t)0x40000000U) + 0x00020000) + 0x00001000))->APB2ENR) & ((0x1U << (0U))));
         ((void)(tmpreg));
          } while(0U);
  //	  /* Remap SRAM at 0x00000000 */
    do {((SYSCFG_TypeDef *) (((uint32_t)0x40000000U) + 0x00010000))->CFGR1 &= ~((0x3U << (0U)));
        ((SYSCFG_TypeDef *) (((uint32_t)0x40000000U) + 0x00010000))->CFGR1 |= ((0x1U << (0U)) | (0x2U << (0U)));
      }while(0);

    if (SysTick_Config(SystemCoreClock / 1000))
     {
       /* Capture error */
       while (1)
       {
       }
     }
    __enable_irq();
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
	  LL_APB1_GRP2_ForceReset(LL_APB1_GRP2_PERIPH_TIM15);
	  LL_APB1_GRP2_ReleaseReset(LL_APB1_GRP2_PERIPH_TIM15);
	  IC_TIMER_REGISTER->CCMR1 = 0x60;                         // channel 1 tim 15
	  IC_TIMER_REGISTER->CCER = 0x3;
#endif

	  IC_TIMER_REGISTER->PSC = output_timer_prescaler;
	  IC_TIMER_REGISTER->ARR = 61;
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
	  LL_APB1_GRP2_ForceReset(LL_APB1_GRP2_PERIPH_TIM15);
	  LL_APB1_GRP2_ReleaseReset(LL_APB1_GRP2_PERIPH_TIM15);
	  IC_TIMER_REGISTER->CCMR1 = 0x1;
	  IC_TIMER_REGISTER->CCER = 0xa;
#endif
	  IC_TIMER_REGISTER->PSC = ic_timer_prescaler;
	  IC_TIMER_REGISTER->ARR = 0xFFFF;
	  LL_TIM_GenerateEvent_UPDATE(IC_TIMER_REGISTER);
	  out_put = 0;

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

			  LL_DMA_SetDataLength(DMA1, INPUT_DMA_CHANNEL, 23);
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

void getSmoothedInput() {

		total = total - readings[readIndex];
		// read from the sensor:
		readings[readIndex] = tempraw;
		// add the reading to the total:
		total = total + readings[readIndex];
		// advance to the next position in the array:
		readIndex = readIndex + 1;
		// if we're at the end of the array...
		if (readIndex >= numReadings) {
			// ...wrap around to the beginning:
			readIndex = 0;
		}
	// calculate the average:
		smoothedinput = total / numReadings;


}

void detectInput(){
	smallestnumber = 20000;
	dshot = 0;
	proshot = 0;
	multishot = 0;
	oneshot42 = 0;
	oneshot125 = 0;
	servoPwm = 0;
	int lastnumber = dma_buffer[0];
	for ( int j = 1 ; j < 31; j++){

		if((dma_buffer[j] - lastnumber) < smallestnumber){ // blank space
			smallestnumber = dma_buffer[j] - lastnumber;

		}
		lastnumber = dma_buffer[j];
	}

	if ((smallestnumber > 3)&&(smallestnumber < 39)){
		ic_timer_prescaler= 0;
		output_timer_prescaler=0;
		dshot = 1;
		buffer_divider = 44;
		dshot_runout_timer = 65000;
		armed_count_threshold = 10000;
		buffersize = 32;
	}
	if ((smallestnumber > 40 )&&(smallestnumber < 80)){
		dshot = 1;
		ic_timer_prescaler=1;
		output_timer_prescaler=1;
		IC_TIMER_REGISTER->CNT = 0xffff;
		buffer_divider = 44;
		dshot_runout_timer = 65000;
		armed_count_threshold = 10000;
		buffersize = 32;
	}
	if ((smallestnumber > 100 )&&(smallestnumber < 400)){
		multishot = 1;
		armed_count_threshold = 1000;
		buffersize = 4;
	}
//	if ((smallestnumber > 2000 )&&(smallestnumber < 3000)){
//		oneshot42 = 1;
//	}
////	if ((smallestnumber > 3000 )&&(smallestnumber < 7000)){
////		oneshot125 = 1;
////	}
	if (smallestnumber > 6000){
		servoPwm = 1;
		ic_timer_prescaler=47;
		armed_count_threshold = 100;
		buffersize = 4;
	}

	if (smallestnumber == 0){
		inputSet = 0;
	}else{

		inputSet = 1;
	}

}


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

	int lastnumber = dma_buffer[0];
	for ( int j = 1 ; j < 3; j++){

		if(((dma_buffer[j] - lastnumber) >1000 ) && ((dma_buffer[j] - lastnumber) < 2010)){ // blank space

			servorawinput = map((dma_buffer[j] - lastnumber), 1090, 2000, 0, 2000);

			break;
		}
		lastnumber = dma_buffer[j];
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

	  if (inputSet == 0){
	 	 detectInput();
	 	receiveDshotDma();
	 	return;
	  }

	if (inputSet == 1){
	if(!armed){
		signaltimeout = 0;
		if (input < 0){
			  						input = 0;
			  					}
		 		 if (input == 0){                       // note this in input..not newinput so it will be adjusted be main loop
		 		 			zero_input_count++;
		 		 		}else{
		 		 			zero_input_count = 0;
		 		 		}
		}

if(dshot_telemetry){
    if(out_put){
//    	TIM17->CNT = 0;

    	 make_dshot_package();          // this takes around 10us !!
    	 computeDshotDMA();             //this is slow too..

    //	TIM17->CNT = 0;
    	receiveDshotDma();             //holy smokes.. reverse the line and set up dma again

    	return;
    }else{
 //   TIM17->CNT = 0;
		if(send_telemetry){
		  makeTelemPackage(degrees_celsius,
		  		              ADC_raw_volts,
		  					  1250,
		  					  200,
		  					  k_erpm);
		  send_telem_DMA();
		}

    sendDshotDma();                        // reverse lines again!
  //  process_time = TIM17->CNT;
    return;
    }
}else{

		if (dshot == 1){
			computeDshotDMA();
			if(send_telemetry){
			  makeTelemPackage(degrees_celsius,
					  ADC_raw_volts,
			  					  1000,
			  					  200,
			  					  k_erpm);
			  send_telem_DMA();
			}
			receiveDshotDma();
		}
		if (proshot == 1){
		//	computeProshotDMA();
		//	HAL_TIM_IC_Start_DMA(&IC_TIMER_POINTER, IC_TIMER_CHANNEL, dma_buffer , 16);
		}

		if  (servoPwm == 1){
			computeServoInput();
			IC_TIMER_REGISTER->CNT = 0;
			signaltimeout = 0;
			receiveDshotDma();
		}
//		if  (multishot){
//			computeMSInput();
//			HAL_TIM_IC_Start_DMA(&htim2, timer2_input_channel, dma_buffer , 3);
//
//		}
//		if  (oneshot125){
//			computeOS125Input();
//			HAL_TIM_IC_Start_DMA(&htim2, timer2_input_channel, dma_buffer , 3);
//
//		}
//		if  (oneshot42){
//			computeOS42Input();
//			HAL_TIM_IC_Start_DMA(&htim2, timer2_input_channel, dma_buffer , 3);
//
//		}

	}
	}
}


void getBemfState(){

    if (rising){
    	if (LL_COMP_ReadOutputLevel(COMP1) == LL_COMP_OUTPUT_LEVEL_LOW){
    		bemfcounter++;
    		}else{
    	//	bemfcounter = 0;
    			bad_count++;
    			if(bad_count > 2){
    				bemfcounter = 0;
    			}
   	}
    }else{
    	if(LL_COMP_ReadOutputLevel(COMP1) == LL_COMP_OUTPUT_LEVEL_HIGH){
    		bemfcounter++;
    	}else{
    //		bemfcounter = 0;
    		bad_count++;
    	    			if(bad_count > 2){
    	    				bemfcounter = 0;
    	    			}
    	}
    }
}

void maskPhaseInterrupts(){
	EXTI->IMR &= (0 << 21);
}

void changeCompInput() {
//	TIM3->CNT = 0;
//	HAL_COMP_Stop_IT(&hcomp1);            // done in comparator interrupt routine

	if (step == 1 || step == 4) {   // c floating
	//	hcomp1.Init.InvertingInput = COMP_INVERTINGINPUT_IO1;
#ifdef use_A0_B4_C5_comp_order
		COMP->CSR = 0b1010001;        // pa5

#else
		COMP->CSR = 0b1100001;         // pa0
#endif


	}

	if (step == 2 || step == 5) {     // a floating
#ifdef MP6531
		//COMP->CSR = 0x40;
	//	hcomp1.Init.InvertingInput = COMP_INVERTINGINPUT_DAC1;
		COMP->CSR = 0b1000001;                        /// if f051k6  step 2 , 5 is dac 1 ( swap comp input)
#endif

#ifdef FD6288
	//hcomp1.Init.InvertingInput = COMP_INVERTINGINPUT_DAC2;      // PA5
		COMP->CSR = 0b1010001;
#endif
#ifdef use_A0_B4_C5_comp_order
	                                                              // PA0
		COMP->CSR = 0b1100001;
#endif


	}

	if (step == 3 || step == 6) {      // b floating
#ifdef MP6531
		//COMP->CSR = 0x50
	//	hcomp1.Init.InvertingInput = COMP_INVERTINGINPUT_DAC2;
		COMP->CSR = 0b1010001;
#endif
#if defined (FD6288) || defined (use_A0_B4_C5_comp_order)
	//	hcomp1.Init.InvertingInput = COMP_INVERTINGINPUT_DAC1;       // PA4
		COMP->CSR = 0b1000001;
#endif
	}
	if (rising){

		EXTI->RTSR = 0x0;
	EXTI->FTSR = 0x200000;

	//	hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_FALLING;   // polarity of comp output reversed
	}else{                          // falling bemf

	EXTI->FTSR = 0x0;
	EXTI->RTSR = 0x200000;
	//	hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_RISING;
	}


}

void commutate(){
	commutation_intervals[step-1] = thiszctime;
	e_com_time = (commutation_intervals[0] + commutation_intervals[1] + commutation_intervals[2] + commutation_intervals[3] + commutation_intervals[4] +commutation_intervals[5]) >> 1;  // COMMUTATION INTERVAL IS 0.5US INCREMENTS
	average_interval = e_com_time / 3;
	if(step==6){       // desync check
		if(getAbsDif(average_interval,last_average_interval) > average_interval >> 1){
			zero_crosses = 0;
running = 0;
old_routine = 1;
		}
		last_average_interval = average_interval;
	}

	TIM14->CNT = 0;
	if (forward == 1){
		step++;
		if (step > 6) {
			step = 1;
		}
		if (step == 1 || step == 3 || step == 5) {
			rising = 1;                                // is back emf rising or falling
		}
		if (step == 2 || step == 4 || step == 6) {
			rising = 0;
		}
	}
	if (forward == 0){
		step--;
		if (step < 1) {
			step = 6;
		}
		if (step == 1 || step == 3 || step == 5) {
			rising = 0;
		}
		if (step == 2 || step == 4 || step == 6) {
			rising = 1;
		}
	}


	comStep(step);
	changeCompInput();
if(commutation_interval > 4000 && crawler_mode){
	old_routine = 1;
}

	bemfcounter = 0;
	zcfound = 0;
	timeout_count = 0;
//	stop_time = TIM14->CNT;

}

void interruptRoutine(){

/// todo move all desync error checking to main loop, doesn't need to be done every interrupt.

if ((TIM2->CNT < 125) && (duty_cycle < 600) && (zero_crosses < 500)){    //should be impossible, desync?exit anyway

	return;
}
thiszctime = TIM2->CNT;

	if (commutation_interval > 50){
	//	delayMicros(filter_delay);
stuckcounter++;             // stuck at 100 interrupts before the main loop happens again.
if (stuckcounter > 100){
	maskPhaseInterrupts();
	zero_crosses = 0;
	return;
}
			if (rising){

				for (int i = 0; i < filter_level; i++){
					if(LL_COMP_ReadOutputLevel(COMP1) == LL_COMP_OUTPUT_LEVEL_HIGH){
											return;
										}
				}
			}else{
				for (int i = 0; i < filter_level; i++){
					if(LL_COMP_ReadOutputLevel(COMP1) == LL_COMP_OUTPUT_LEVEL_LOW){
											return;
										}
				}
			}
	}
							maskPhaseInterrupts();
							LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_21);

			TIM2->CNT = 0;
			TIM14->CNT = 0;
			TIM14->ARR = waitTime;
			TIM14->SR = 0x00;
			TIM14->DIER |= (0x1UL << (0U));             // enable TIM14 interrupt
}


void advanceincrement(){

if (!forward){
	phase_A_position ++;
    if (phase_A_position > 359){
	   phase_A_position = 0 ;
    }

	    phase_B_position ++;
	     if (phase_B_position > 359){
		phase_B_position = 0 ;
	}
	    phase_C_position ++;
	     if (phase_C_position > 359){
		phase_C_position = 0 ;
	}
}else{
	phase_A_position --;
	    if (phase_A_position < 0){
		   phase_A_position = 359 ;
	    }

		    phase_B_position --;
		     if (phase_B_position < 0){
			phase_B_position = 359;
		}
		    phase_C_position --;
		     if (phase_C_position < 0){
			phase_C_position = 359 ;
		}
}

#ifdef MP6531
TIM1->CCR1 = (pwmSin[phase_C_position])+gate_drive_offset;
    TIM1->CCR2 = (pwmSin[phase_B_position])+gate_drive_offset;
    TIM1->CCR3 = (pwmSin[phase_A_position])+gate_drive_offset;

#else
    TIM1->CCR1 = (pwmSin[phase_A_position]/2)+gate_drive_offset;
    TIM1->CCR2 = (pwmSin[phase_B_position]/2)+gate_drive_offset;
    TIM1->CCR3 = (pwmSin[phase_C_position]/2)+gate_drive_offset;
#endif
}


void PeriodElapsedCallback(){
			count++;
			TIM14->DIER &= ~((0x1UL << (0U)));             // disable interrupt
			commutate();
			commutation_interval = ((3 *commutation_interval) + thiszctime)>>2;
            advance = (commutation_interval>>3) * advance_level;   // 60 divde 8 7.5 degree increments
			waitTime = (commutation_interval >>1)  - advance;
			//	blanktime = commutation_interval / 8; // no blanktime /demag , if the motor is accelerating fast the next zc can happen right after commutation
			if(!old_routine){
			EXTI->IMR |= (1 << 21);     // enable comp interrupt
			}
			zero_crosses++;
}

void startMotor() {
	if (running == 0){
	commutate();
	commutation_interval = 10000;
	TIM2->CNT = 5000;
	running = 1;
	}
	EXTI->IMR |= (1 << 21);
	sensorless = 1;
}


void zcfoundroutine(){
	thiszctime = TIM2->CNT;
	TIM2->CNT = 0;
	commutation_interval = (thiszctime + (3*commutation_interval)) / 4;
	advance = commutation_interval / advancedivisor;
	waitTime = commutation_interval /2  - advance;
	blanktime = commutation_interval / 4;
	while (TIM2->CNT - thiszctime < waitTime - advance){

	}
	commutate();
    bemfcounter = 0;
    bad_count = 0;

    zero_crosses++;
    if(crawler_mode){
    	 if (zero_crosses >= 100 && adjusted_input > 400) {
    	    	old_routine = 0;
    	    	EXTI->IMR |= (1 << 21);          // enable interrupt
    	 }
    }else{
    if (zero_crosses >= 20) {
    	old_routine = 0;
    	EXTI->IMR |= (1 << 21);          // enable interrupt
    }
    }

}


int main(void)
{
 initAfterJump();
 LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
 LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_COMP1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();

  MX_TIM16_Init();
  MX_TIM14_Init();
  MX_TIM6_Init();
  MX_TIM17_Init();

  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1N);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2N);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3N);

  /* Enable counter */
  LL_TIM_EnableCounter(TIM1);
  LL_TIM_EnableAllOutputs(TIM1);
  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM1);
#ifdef USE_ADC_INPUT

#else
  UN_TIM_Init();
  LL_TIM_CC_EnableChannel(IC_TIMER_REGISTER, IC_TIMER_CHANNEL);  // input capture and output compare
  LL_TIM_EnableCounter(IC_TIMER_REGISTER);
#endif

#ifdef tmotor55
  LED_GPIO_init();
  GPIOB->BSRR = LL_GPIO_PIN_3; // turn on red
#endif

   LL_TIM_EnableCounter(TIM14);               // commutation_timer priority 0
   LL_TIM_GenerateEvent_UPDATE(TIM14);
   LL_TIM_EnableIT_UPDATE(TIM14);
   TIM14->DIER &= ~((0x1UL << (0U)));         // disable for now.

   LL_TIM_EnableCounter(TIM17);                 // delay timer
   LL_TIM_GenerateEvent_UPDATE(TIM17);

   LL_TIM_EnableCounter(TIM2);                 // delay timer
   LL_TIM_GenerateEvent_UPDATE(TIM2);

   __IO uint32_t wait_loop_index = 0;

   /* Enable comparator */
   LL_COMP_Enable(COMP1);

   wait_loop_index = ((LL_COMP_DELAY_STARTUP_US * (SystemCoreClock / (100000 * 2))) / 10);
   while(wait_loop_index != 0)
   {
     wait_loop_index--;
   }

   playStartupTune();
  // playDuskingTune();
   ADC_Init();
   enableADC_DMA();
   activateADC();


   telem_UART_Init();

   MX_IWDG_Init();
   LL_IWDG_ReloadCounter(IWDG);
#ifdef USE_ADC_INPUT
   armed_count_threshold = 5000;
   inputSet = 1;
   if(use_sin_start){
   min_startup_duty = sin_mode_min_s_d;
   }
#else
  receiveDshotDma();
  loadEEpromSettings();

	if (dir_reversed == 1){
			forward = 0;
		}else{
			forward = 1;
		}


#endif
//TODO, timeouts are based on counts of the main loop, which currently takes about 30us. This is not good and the control loop should be done on timed interval.


  while (1)
  {
	  adc_counter++;
	  if(adc_counter>50){   // for testing adc and telemetry
		  degrees_celsius = __LL_ADC_CALC_TEMPERATURE(3300,  ADC_raw_temp, LL_ADC_RESOLUTION_12B);

		  LL_ADC_REG_StartConversion(ADC1);

		  adc_counter = 0;
#ifdef USE_ADC_INPUT
		  if(ADC_raw_input < 10){
			  zero_input_count++;
		  }else{
			  zero_input_count=0;
		  }
#endif
	  }
#ifdef USE_ADC_INPUT

signaltimeout = 0;
ADC_smoothed_input = (((10*ADC_smoothed_input) + ADC_raw_input)/11);
newinput = ADC_smoothed_input / 2;
if(newinput > 2000){
	newinput = 2000;
}
#endif
	  stuckcounter = 0;
	  LL_IWDG_ReloadCounter(IWDG);
  		  if (zero_input_count > armed_count_threshold && !armed){
  			  armed = 1;
#ifdef tmotor55
  			GPIOB->BRR = LL_GPIO_PIN_3;    // turn off red
  			GPIOA->BSRR = LL_GPIO_PIN_15;   // turn on green
#endif
  			  playInputTune();
  		  }
  		  if (bi_direction == 1 && proshot == 0 && dshot == 0){
				if (newinput > 1100) {
					if (forward == dir_reversed) {
				adjusted_input = 0;

				forward = 1 - dir_reversed;
				}
				adjusted_input = (newinput - 1099) * 3;

				}
				if (newinput < 760) {
				if (forward == (1 - dir_reversed)) {

				adjusted_input = 0;
				forward = dir_reversed;
					}
			adjusted_input = ((760 - newinput) * 3);
								}
									if (newinput >= 760 && newinput < 1100) {
										adjusted_input = 0;
									}

 		}else if ((proshot || dshot) && bi_direction) {
  					if (newinput > 1047) {

  						if (forward == dir_reversed) {
  							if(commutation_interval > 1500 || stepper_sine){
  							forward = 1 - dir_reversed;
  							zero_crosses = 0;
  							old_routine = 1;
  							}else{
  								newinput = 0;

  							}
  						}
  						adjusted_input = (newinput - 1047) * 2 + 47;

  					}
  					if (newinput <= 1047 && newinput > 0) {
  					//	startcount++;

  						if (forward == (1 - dir_reversed)) {
  							if(commutation_interval > 1500 || stepper_sine){
  							zero_crosses = 0;
  							old_routine = 1;
  							forward = dir_reversed;
  							}else{
  								newinput = 0;

  							}
 						}
  						adjusted_input = (newinput - 47) * 2 + 47;
  					}
  					if ( newinput < 48) {
  						adjusted_input = 0;
  					}


  	  				}else{
  		 adjusted_input = newinput;
  	 }
	 	 if ((zero_crosses > 1000) || (adjusted_input == 0)){
 	 		bemf_timout_happened = 0;
#ifdef tmotor55
 	 		if(adjusted_input == 0 && armed){
			  GPIOA->BSRR = LL_GPIO_PIN_15; // on green
			  GPIOB->BRR = LL_GPIO_PIN_5;  // off blue
			  GPIOB->BRR = LL_GPIO_PIN_3;  //off red
 	 		}
#endif
 	 	 }

 	 	 if(crawler_mode){
 	 		if (adjusted_input < 400){
 	 			bemf_timout_happened = 0;
 	 		}
 	 	 }
	  if(bemf_timout_happened > bemf_timeout * ( 1 + (crawler_mode*100))&& stuck_rotor_protection){
	 		 allOff();
	 		 maskPhaseInterrupts();
	 		 input = 0;
#ifdef tmotor55
			  GPIOA->BRR = LL_GPIO_PIN_15; // off green
			  GPIOB->BRR = LL_GPIO_PIN_5;  // off blue
			  GPIOB->BSRR = LL_GPIO_PIN_3;
#endif
	 	  }else{
	 		 input=adjusted_input;

	 	  }

 		signaltimeout++;              // yeah this is silly
  		if (signaltimeout > 30000){
  			armed = 0;
  			input = 0;
  			inputSet = 0;
  			zero_input_count = 0;
  			smallestnumber = 20000;
  			TIM1->CCR1 = 0;
  		    TIM1->CCR2 = 0;
  			TIM1->CCR3 = 0;
  			IC_TIMER_REGISTER->PSC = 0;
  			IC_TIMER_REGISTER->CNT = 0;
  		//	HAL_TIM_IC_Stop_DMA(&IC_TIMER_POINTER,IC_TIMER_CHANNEL);
  			for(int i = 0; i < 64; i++){
  				dma_buffer[i] = 0;
  			}

  			while(1){  // reset esc by forceing watchdog timer

  			}
  		}
		  if ( stepper_sine == 0){

	  if (input >= 47 +(90*stepper_sine) && armed){
		  if (running == 0){
			  if(!old_routine){
			 startMotor();
			  }
			  running = 1;
#ifdef tmotor55
			  GPIOB->BRR = LL_GPIO_PIN_3;  // off red
			  GPIOA->BRR = LL_GPIO_PIN_15; // off green
			  GPIOB->BSRR = LL_GPIO_PIN_5;  // on blue
#endif

		  }
		  coasting = 0;
	 //	 running = 1;
	 	 duty_cycle = map(input, 47, 2047, 84, 2000) - (40*use_sin_start);
	  }
	  if (input < 47 + (90*stepper_sine)){

  		  if(!comp_pwm){
  			duty_cycle = 0;
  			if(!running){
  				old_routine = 1;
  				zero_crosses = 0;
  			}
  		  }else{
		  if (running == 1){
		  coasting = 0;
		  }
		  old_routine = 1;
	 //	 running = 0;
	 	 duty_cycle = 0;
	 	  phase_A_position = 0;
	 	  phase_B_position = 119;
	 	  phase_C_position = 239;
	 	  if(use_sin_start == 1){
	 	 stepper_sine = 1;
	 	  }
	 	zero_crosses = 0;
 		  }
		  }

   if (zero_crosses < 40){
	   if (duty_cycle < min_startup_duty){
	   duty_cycle = min_startup_duty;

	   }
	   if (duty_cycle > 400){
		   duty_cycle = 400;
	   }
   }

  if (duty_cycle < 160 && running){
  	 if(duty_cycle < 84){
	  duty_cycle = 84;
  	 }
  	 if(stall_protection && zero_crosses > 50){  // this boosts throttle as the rpm gets lower, for crawlers and rc cars only, do not use for multirotors.
	 if(commutation_interval > 7000){
		    	 duty_cycle = duty_cycle + map(commutation_interval, 7000, 9000, 1, 80);
		     }
  	 }
   }

   k_erpm = ((1000000/ e_com_time) * 60) / 1000; // ecom time is time for one electrical revolution in microseconds
   if(low_rpm_throttle_limit){     // some hardware doesn't need this, its on by default to keep hardware / motors protected but can slow down the response in the very low end a little.

  duty_cycle_maximum = map(k_erpm, low_rpm_level, high_rpm_level, throttle_max_at_low_rpm, throttle_max_at_high_rpm);   // for more performance lower the high_rpm_level, set to a consvervative number in source.
   }

	 if (duty_cycle > duty_cycle_maximum){
		 duty_cycle = duty_cycle_maximum;
	 }

if (armed && running && (input > 47)){
		if(VARIABLE_PWM){
			//throttle_adjust_pwm_shift = (step % 2 ) * pwm_dither_amount;
		if(((average_interval < 330)&&(average_interval > 323)) || ((average_interval <= 246)&&(average_interval > 238))) {  // when the commutation frequency is close to pwm harmonics
		advance_level = 1 + (step%2) + (commutation_interval%2);  // jiggle timing around a bit around a bit so it doesn't lock to it, i might remove this.
						    		}else{
		tim1_arr = map(commutation_interval, 96, 200, 1000, 2000);
        advance_level = eepromBuffer[23];
		}
		TIM1->ARR = tim1_arr;
		}
	    adjusted_duty_cycle = (duty_cycle * tim1_arr)/2000;

		TIM1->CCR1 = adjusted_duty_cycle;
		TIM1->CCR2 = adjusted_duty_cycle;
	    TIM1->CCR3 = adjusted_duty_cycle;

	  }else{           // not armed

		  TIM1->CCR1 = 0;
		  TIM1->CCR2 = 0;
		  TIM1->CCR3 = 0;
	  }

if (zero_crosses < 150 || commutation_interval > 900 || duty_cycle < 400) {
		advancedivisor = 4;
		filter_level = 12;
		filter_delay = 0;
	} else {
		advancedivisor = 4;         // 15 degree advance
		filter_level = 6;
		filter_delay = 0;

	}
	if (duty_cycle > 900 && zero_crosses > 100 && commutation_interval < 900){
		filter_level = 4;
		filter_delay = 0;
	}

	if (commutation_interval < 90 && duty_cycle > 800){
		filter_level = 3;
		filter_delay = 0;
	}

if(lowkv){

	filter_level = low_kv_filter_level;
}

/**************** old routine*********************/
if (old_routine == 1){
	//old_routine = 1;
	maskPhaseInterrupts();
	 		 getBemfState();                                      // uncomment to make run !!!!!!!!!!!!!!!
	 	  if (!zcfound){
	 		  if (rising){
	 		 if (bemfcounter > min_bemf_counts_up){

	 			 zcfound = 1;
	 		//	 bemfcounter = 0;
	 			 zcfoundroutine();
	 		//	 break;

	 		}
	 		  }else{
	 			  if (bemfcounter > min_bemf_counts_down){

	 			  			 zcfound = 1;
	 			//  			 bemfcounter = 0;
	 			  			 zcfoundroutine();
	 			  //			 break;

	 			  		}
	 		  }
	 	  }
//
}
	 	  if (TIM2->CNT > 40000 && running == 1){
	 		//  TIM3->CNT = commutation_interval / 2;
	 		  timeout_count++;
	 		  if (timeout_count > bemf_timeout_threshold){
bemf_timout_happened ++;
	 		  zcfoundroutine();
	 		  maskPhaseInterrupts();
	 //		  old_routine = 1;
	 		   running = 0;
	 		   zero_crosses = 0;
	 		  timeout_count = 0;

	 		  }
	 	  }
	 	  }else{            // stepper sine

if(input > 60 && armed){
	 		  if (input > 60 && input < 140){// sine wave stepper
	 			 maskPhaseInterrupts();
	 			 allpwm();
	 		 advanceincrement();

		 	//  count = 0;
             step_delay = map (input, 60, 140, 500, 100);
	 		 delayMicros(step_delay);

	 		  }else{
	 			 advanceincrement();
	 			  if(input > 200){
	 				 phase_A_position = 0;
	 				 step_delay = 80;
	 			  }

	 			 delayMicros(step_delay);
	 			  if (phase_A_position == 0){
	 			  stepper_sine = 0;
	 			  running = 1;
	 			 zero_crosses = 0;
	 			  step = changeover_step;                    // rising bemf on a same as position 0.
	 			LL_TIM_GenerateEvent_UPDATE(TIM1);
	 			  zcfoundroutine();
	 			  }
	 		  }
}else{
	TIM1->CCR1 = 0;												// set duty cycle to 50 out of 768 to start.
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 0;
	fullBrake();
}
	 	  }

  }

}

// PERIPHERAL SETUP

void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI14_Enable();

   /* Wait till HSI14 is ready */
  while(LL_RCC_HSI14_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI14_SetCalibTrimming(16);
  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while(LL_RCC_LSI_IsReady() != 1)
  {
    
  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_12);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {
    
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  
  }
  LL_Init1msTick(48000000);
  LL_SetSystemCoreClock(48000000);
  LL_RCC_HSI14_EnableADCControl();
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
}


static void MX_COMP1_Init(void)
{

LL_COMP_InitTypeDef COMP_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**COMP1 GPIO Configuration  
  PA1   ------> COMP1_INP
  PA5   ------> COMP1_INM 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  NVIC_SetPriority(ADC1_COMP_IRQn, 0);
  NVIC_EnableIRQ(ADC1_COMP_IRQn);

  COMP_InitStruct.PowerMode = LL_COMP_POWERMODE_HIGHSPEED;
  COMP_InitStruct.InputPlus = LL_COMP_INPUT_PLUS_IO1;
  COMP_InitStruct.InputMinus = LL_COMP_INPUT_MINUS_DAC1_CH2;
  COMP_InitStruct.InputHysteresis = LL_COMP_HYSTERESIS_NONE;
  COMP_InitStruct.OutputSelection = LL_COMP_OUTPUT_NONE;
  COMP_InitStruct.OutputPolarity = LL_COMP_OUTPUTPOL_NONINVERTED;
  LL_COMP_Init(COMP1, &COMP_InitStruct);

}


static void MX_IWDG_Init(void)
{

  LL_IWDG_Enable(IWDG);
  LL_IWDG_EnableWriteAccess(IWDG);
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_16);
  LL_IWDG_SetReloadCounter(IWDG, 4000);
  while (LL_IWDG_IsReady(IWDG) != 1)
  {
  }

  LL_IWDG_SetWindow(IWDG, 4095);
  LL_IWDG_ReloadCounter(IWDG);


}


static void MX_TIM1_Init(void)
{


  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
  LL_TIM_BDTR_InitTypeDef TIM_BDTRInitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM1);


  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 1999;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM1, &TIM_InitStruct);
  LL_TIM_EnableARRPreload(TIM1);
  LL_TIM_SetClockSource(TIM1, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH1);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
  TIM_OC_InitStruct.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM1, LL_TIM_CHANNEL_CH1);
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH2);
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH2, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM1, LL_TIM_CHANNEL_CH2);
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH3);
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH3, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM1, LL_TIM_CHANNEL_CH3);
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH4);
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH4, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM1, LL_TIM_CHANNEL_CH4);
  LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM1);
  TIM_BDTRInitStruct.OSSRState = LL_TIM_OSSR_DISABLE;
  TIM_BDTRInitStruct.OSSIState = LL_TIM_OSSI_DISABLE;
  TIM_BDTRInitStruct.LockLevel = LL_TIM_LOCKLEVEL_OFF;
  TIM_BDTRInitStruct.DeadTime = DEAD_TIME;
  TIM_BDTRInitStruct.BreakState = LL_TIM_BREAK_DISABLE;
  TIM_BDTRInitStruct.BreakPolarity = LL_TIM_BREAK_POLARITY_HIGH;
  TIM_BDTRInitStruct.AutomaticOutput = LL_TIM_AUTOMATICOUTPUT_DISABLE;
  LL_TIM_BDTR_Init(TIM1, &TIM_BDTRInitStruct);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**TIM1 GPIO Configuration  
  PA7   ------> TIM1_CH1N
  PB0   ------> TIM1_CH2N
  PB1   ------> TIM1_CH3N
  PA8   ------> TIM1_CH1
  PA9   ------> TIM1_CH2
  PA10   ------> TIM1_CH3 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}


static void MX_TIM2_Init(void)
{

 LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
  TIM_InitStruct.Prescaler = 23;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0xFFFF;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM2);

}


static void MX_TIM6_Init(void)
{

  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);

  NVIC_SetPriority(TIM6_DAC_IRQn, 2);
  NVIC_EnableIRQ(TIM6_DAC_IRQn);

  TIM_InitStruct.Prescaler = 23;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  LL_TIM_Init(TIM6, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM6);
  LL_TIM_SetTriggerOutput(TIM6, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM6);


}


static void MX_TIM14_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM14);

  NVIC_SetPriority(TIM14_IRQn, 0);
  NVIC_EnableIRQ(TIM14_IRQn);
  TIM_InitStruct.Prescaler = 23;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 4000;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM14, &TIM_InitStruct);
  LL_TIM_EnableARRPreload(TIM14);

}


static void MX_TIM16_Init(void)
{

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM16);

  NVIC_SetPriority(TIM16_IRQn, 2);
  NVIC_EnableIRQ(TIM16_IRQn);

  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 9000;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM16, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM16);

}


static void MX_TIM17_Init(void)
{
 LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM17);

  TIM_InitStruct.Prescaler = 47;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM17, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM17);


}


static void MX_DMA_Init(void) 
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* DMA interrupt init */
  /* DMA1_Channel2_3_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1);
  NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
  /* DMA1_Channel4_5_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel4_5_IRQn, 1);
  NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);

}

static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_15);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}


static void UN_TIM_Init(void)
{



  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
#ifdef USE_TIMER_15_CHANNEL_1
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM15);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**TIM16 GPIO Configuration
  PA6   ------> TIM16_CH1
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif

#ifdef USE_TIMER_3_CHANNEL_1

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**TIM16 GPIO Configuration
  PA6   ------> TIM16_CH1
  */
  GPIO_InitStruct.Pin = INPUT_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif

 /* TIM16 DMA Init */

  /* TIM16_CH1_UP Init */
  LL_DMA_SetDataTransferDirection(DMA1, INPUT_DMA_CHANNEL, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetChannelPriorityLevel(DMA1, INPUT_DMA_CHANNEL, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, INPUT_DMA_CHANNEL, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, INPUT_DMA_CHANNEL, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, INPUT_DMA_CHANNEL, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, INPUT_DMA_CHANNEL, LL_DMA_PDATAALIGN_HALFWORD);

  LL_DMA_SetMemorySize(DMA1, INPUT_DMA_CHANNEL, LL_DMA_MDATAALIGN_WORD);

  /* TIM16 interrupt Init */

#ifdef USE_TIMER_15_CHANNEL_1
  NVIC_SetPriority(TIM15_IRQn, 0);
  NVIC_EnableIRQ(TIM15_IRQn);
#endif
#ifdef USE_TIMER_3_CHANNEL_1
  NVIC_SetPriority(TIM3_IRQn, 0);
   NVIC_EnableIRQ(TIM3_IRQn);
   NVIC_SetPriority(IC_DMA_IRQ_NAME, 1);
   NVIC_EnableIRQ(IC_DMA_IRQ_NAME);
#endif

  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 63;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(IC_TIMER_REGISTER, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(IC_TIMER_REGISTER);
  LL_TIM_IC_SetActiveInput(IC_TIMER_REGISTER, IC_TIMER_CHANNEL, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(IC_TIMER_REGISTER, IC_TIMER_CHANNEL, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter(IC_TIMER_REGISTER, IC_TIMER_CHANNEL, LL_TIM_IC_FILTER_FDIV1);
  LL_TIM_IC_SetPolarity(IC_TIMER_REGISTER, IC_TIMER_CHANNEL, LL_TIM_IC_POLARITY_BOTHEDGE);


}


void LED_GPIO_init(){
	  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */

	  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_15);
      LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
	  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);

	  GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void Error_Handler(void)
{

}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


