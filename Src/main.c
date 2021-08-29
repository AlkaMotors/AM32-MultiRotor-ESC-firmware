/* AM32- multi-purpose brushless controller firmware for the stm32f051 */

//===========================================================================
//=============================== Changelog =================================
//===========================================================================
/*
 * 1.54 Changelog;
 * --Added firmware name to targets and firmware version to main
 * --added two more dshot to beacons 1-3 currently working
 * --added KV option to firmware, low rpm power protection is based on KV
 * --start power now controls minimum idle power as well as startup strength.
 * --change default timing to 22.5
 * --Lowered default minimum idle setting to 1.5 percent duty cycle, slider range from 1-2.
 * --Added dshot commands to save settings and reset ESC.
 *
 *1.56 Changelog.
 * -- added check to stall protection to wait until after 40 zero crosses to fix high startup throttle hiccup.
 * -- added TIMER 1 update interrupt and PWM changes are done once per pwm period
 * -- reduce commutation interval averaging length
 * -- reduce false positive filter level to 2 and eliminate threshold where filter is stopped.
 * -- disable interrupt before sounds
 * -- disable TIM1 interrupt during stepper sinusoidal mode
 * -- add 28us delay for dshot300
 * -- report 0 rpm until the first 10 successful steps.
 * -- move serial ADC telemetry calculations and desync check to 10Khz interrupt.
 *
 * 1.57
 * -- remove spurious commutations and rpm data at startup by polling for longer interval on startup
 *
 * 1.58
 * -- move signal timeout to 10khz routine and set armed timeout to one quarter second 2500 / 10000
 * 1.59
 * -- moved comp order definitions to target.h
 * -- fixed update version number if older than new version
 * -- cleanup, moved all input and output to IO.c
 * -- moved comparator functions to comparator.c
 * -- removed ALOT of useless variables
 * -- added siskin target
 * -- moved pwm changes to 10khz routine
 * -- moved basic functions to functions.c
 * -- moved peripherals setup to periherals.c
 * -- added crawler mode settings
 *
 * 1.60
 * -- added sine mode hysteresis
 * -- increased power in stall protection and lowered start rpm for crawlers
 * -- removed onehot125 from crawler mode
 * -- reduced maximum startup power from 400 to 350
 * -- change minimum duty cycle to DEAD_TIME
 * -- version and name moved to permanent spot in FLASH memory, thanks mikeller
 *
 * 1.61
 * -- moved duty cycle calculation to 10khz and added max change option.
 * -- decreased maximum interval change to 25%
 * -- reduce wait time on fast acceleration (fast_accel)
 * -- added check in interrupt for early zero cross
 *
 * 1.62
 * --moved control to 10khz loop
 * --changed condition for low rpm filter for duty cycle from || to &&
 * --introduced max deceleration and set it to 20ms to go from 100 to 0
 * --added configurable servo throttle ranges
 *
 *
 *1.63
 *-- increase time for zero cross error detection below 250us commutation interval
 *-- increase max change a low rpm x10
 *-- set low limit of throttle ramp to a lower point and increase upper range
 *-- change desync event from full restart to just lower throttle.

 *1.64
 * --added startup check for continuous high signal, reboot to enter bootloader.
 *-- added brake on stop from eeprom
 *-- added stall protection from eeprom
 *-- added motor pole divider for sinusoidal and low rpm power protection
 *-- fixed dshot commands, added confirmation beeps and removed blocking behavior
 *--
 *1.65
 *-- Added 32 millisecond telemetry output
 *-- added low voltage cutoff , divider value and cutoff voltage needs to be added to eeprom
 *-- added beep to indicate cell count if low voltage active
 *-- added current reading on pa3 , conversion factor needs to be added to eeprom
 *-- fixed servo input capture to only read positive pulse to handle higher refresh rates.
 *-- disabled oneshot 125.
 *-- extended servo range to match full output range of receivers
 *-- added RC CAR style reverse, proportional brake on first reverse , double tap to change direction
 *-- added brushed motor control mode
 *-- added settings to EEPROM version 1
 *-- add gimbal control option.
 *--
 *1.66
 *-- move idwg init to after input tune
 *-- remove reset after save command -- dshot
 *-- added wraith32 target
 *-- added average pulse check for signal detection
 *--
 *1.67
 *-- Rework file structure for multiple MCU support
 *-- Add g071 mcu
 *--
 *1.68
 *--increased allowed average pulse length to avoid double startup
 *1.69
 *--removed line re-enabling comparator after disabling.
 *1.70 fix dshot for Kiss FC
 *1.71 fix dshot for Ardupilot / Px4 FC
 *1.72 Fix telemetry output and add 1 second arming.
 *1.73 Fix false arming if no signal. Remove low rpm throttle protection below 300kv
 *1.74 Add Sine Mode range and drake brake strength adjustment
 *1.75 Disable brake on stop for PWM_ENABLE_BRIDGE 
	   Removed automatic brake on stop on neutral for RC car proportional brake.
	   Adjust sine speed and stall protection speed to more closely match
	   makefile fixes from Cruwaller 
	   Removed gd32 build, until firmware is functional
 */
#include <stdint.h>
#include "main.h"
#include "targets.h"
#include "signal.h"
#include "dshot.h"
#include "phaseouts.h"
#include "eeprom.h"
#include "sounds.h"
#include "ADC.h"
#include "serial_telemetry.h"
#include "IO.h"
#include "comparator.h"
#include "functions.h"
#include "peripherals.h"

//===========================================================================
//============================= EEPROM Defaults =============================
//===========================================================================

#define VERSION_MAJOR 1
#define VERSION_MINOR 75
char dir_reversed = 0;
char comp_pwm = 1;
char VARIABLE_PWM = 1;
char bi_direction = 0;
char stuck_rotor_protection = 1;	// Turn off for Crawlers
char brake_on_stop = 0;
char stall_protection = 0;
char use_sin_start = 0;
char THIRTY_TWO_MS_TLM = 0;

char advance_level = 2;			// 7.5 degree increments 0 , 7.5, 15, 22.5)
uint16_t motor_kv = 2000;
char motor_poles = 14;
//add Startup Power
//Add PWM Frequency
//Add Beep Volume
char drag_brake_strength = 10;		// Drag Brake Power
char sine_mode_changeover_thottle_level = 5;	// Sine Startup Range

char USE_HALL_SENSOR = 0;

//============================= Servo Settings ==============================
uint16_t servo_low_threshold = 1100;	// anything below this point considered 0
uint16_t servo_high_threshold = 1900;	// anything above this point considered 2000 (max)
uint16_t servo_neutral = 1500;
uint8_t servo_dead_band = 100;

//========================= Battery Cuttoff Settings ========================
char LOW_VOLTAGE_CUTOFF = 0;		// Turn Low Voltage CUTOFF on or off
uint16_t low_cell_volt_cutoff = 330;	// 3.3volts per cell

//Add Car/basher mode

//=========================== END EEPROM Defaults ===========================

typedef struct __attribute__((packed)) {
  uint8_t version_major;
  uint8_t version_minor;
  char device_name[12];
} firmware_info_s;

firmware_info_s __attribute__ ((section(".firmware_info"))) firmware_info = {
  version_major: VERSION_MAJOR,
  version_minor: VERSION_MINOR,
  device_name: FIRMWARE_NAME
};

uint8_t EEPROM_VERSION;

//firmware build options
char BRUSHED_MODE = 0;         // overrides everything else
char RC_CAR_REVERSE = 0;         // have to set bidirectional, comp_pwm off and stall protection off, no sinusoidal startup
char GIMBAL_MODE = 0;     // also sinusoidal_startup needs to be on.
//move these to targets folder or peripherals for each mcu


uint16_t current_angle = 90;
uint16_t desired_angle = 90;

uint32_t MCU_Id = 0;
uint32_t REV_Id = 0;

uint16_t armed_timeout_count;

uint8_t desync_happened = 0;
char maximum_throttle_change_ramp = 1;
  
char crawler_mode = 0;  // no longer used //
uint16_t velocity_count = 0;
uint16_t velocity_count_threshold = 100;

char low_rpm_throttle_limit = 1;

uint16_t low_voltage_count = 0;


uint16_t thirty_two_ms_count;

char VOLTAGE_DIVIDER = TARGET_VOLTAGE_DIVIDER;     // 100k upper and 10k lower resistor in divider

uint16_t battery_voltage;  // scale in volts * 10.  1260 is a battery voltage of 12.60

char cell_count = 0;

char brushed_direction_set = 0;

uint16_t consumption_timer = 0;

float consumed_current = 0;
uint16_t smoothed_raw_current = 0;
uint16_t actual_current = 0;

char lowkv = 0;

int min_startup_duty = 120;
int sin_mode_min_s_d = 120;
char bemf_timeout = 10;

char startup_boost = 35;
char reversing_dead_band = 1;

int checkcount = 0;
uint16_t low_pin_count = 0;

uint8_t max_duty_cycle_change = 2;
char fast_accel = 1;
uint16_t last_duty_cycle = 0;
char play_tone_flag = 0;

typedef enum
{
  GPIO_PIN_RESET = 0U,
  GPIO_PIN_SET
}GPIO_PinState;

uint16_t minimum_duty_cycle = DEAD_TIME;
char desync_check = 0;
char low_kv_filter_level = 20;

uint16_t tim1_arr = TIM1_AUTORELOAD;         // current auto reset value
uint16_t TIMER1_MAX_ARR = TIM1_AUTORELOAD;      // maximum auto reset register value
int duty_cycle_maximum = TIM1_AUTORELOAD;     //restricted by temperature or low rpm throttle protect
int low_rpm_level  = 20;        // thousand erpm used to set range for throttle resrictions
int high_rpm_level = 70;      //
int throttle_max_at_low_rpm  = 400;
int throttle_max_at_high_rpm = TIM1_AUTORELOAD;

uint16_t commutation_intervals[6] = {0};
uint32_t average_interval = 0;
uint32_t last_average_interval;
int e_com_time;

uint16_t ADC_smoothed_input = 0;
uint8_t degrees_celsius;
uint16_t converted_degrees;
uint8_t temperature_offset;
uint16_t ADC_raw_temp;
uint16_t ADC_raw_volts;
uint16_t ADC_raw_current;
uint16_t ADC_raw_input;
int adc_counter = 0;
char send_telemetry = 0;
char telemetry_done = 0;
char prop_brake_active = 0;

uint8_t eepromBuffer[48] ={0};
uint32_t gcr[30] =  {0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,0,64,0,0,0,0,64,0,0,0,64,64,0,64,0};
uint8_t gcr_size;
uint16_t process_time = 0;

char dshot_telemetry = 0;
char output = 0;
int dshot_frametime = 0;

uint16_t phase_a_interval = 0;
uint16_t phase_b_interval = 0;
uint16_t phase_c_interval = 0;
uint32_t current_EXTI_LINE;

int dshot_goodcounts = 0;
int dshot_badcounts = 0;
uint8_t last_dshot_command = 0;
char old_routine = 0;
int adjusted_input;

#define TEMP30_CAL_VALUE            ((uint16_t*)((uint32_t)0x1FFFF7B8))
#define TEMP110_CAL_VALUE           ((uint16_t*)((uint32_t)0x1FFFF7C2))
//
//uint32_t temp110cal = 1;
//uint32_t temp30cal = 1;

int smoothedinput = 0;
int voltageraw;

const int numReadings = 30;     // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;
int readings[30];
int tempraw = 0;
int temp_degrees = 0;

int bemf_timout_happened = 0;
int timeout_count = 0;
int bemf_timeout_threshold = 10;

int changeover_step = 5;
int filter_level = 5;
int running = 0;
int advance = 0;
int advancedivisor = 6;
char rising = 1;

////Space Vector PWM ////////////////
//const int pwmSin[] ={128, 132, 136, 140, 143, 147, 151, 155, 159, 162, 166, 170, 174, 178, 181, 185, 189, 192, 196, 200, 203, 207, 211, 214, 218, 221, 225, 228, 232, 235, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 235, 232, 228, 225, 221, 218, 214, 211, 207, 203, 200, 196, 192, 189, 185, 181, 178, 174, 170, 166, 162, 159, 155, 151, 147, 143, 140, 136, 132, 128, 124, 120, 116, 113, 109, 105, 101, 97, 94, 90, 86, 82, 78, 75, 71, 67, 64, 60, 56, 53, 49, 45, 42, 38, 35, 31, 28, 24, 21, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 21, 24, 28, 31, 35, 38, 42, 45, 49, 53, 56, 60, 64, 67, 71, 75, 78, 82, 86, 90, 94, 97, 101, 105, 109, 113, 116, 120, 124};


////Sine Wave PWM ///////////////////
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


//int sin_divider = 2;
int phase_A_position;
int phase_B_position;
int phase_C_position;
int step_delay  = 100;
char stepper_sine = 0;
int forward = 1;
int gate_drive_offset = 60;

int stuckcounter = 0;
int k_erpm;
uint16_t e_rpm;      // electrical revolution /100 so,  123 is 12300 erpm

uint16_t adjusted_duty_cycle;

int bad_count = 0;
int dshotcommand;
int armed_count_threshold = 1000;

char armed = 0;
int zero_input_count = 0;

int input = 0;
int newinput =0;
char inputSet = 0;
char dshot = 0;
char servoPwm = 0;
int zero_crosses;

int zcfound = 0;

int bemfcounter;
int min_bemf_counts_up = 7;
int min_bemf_counts_down = 7;
int adc_timer = 600;
int lastzctime;
uint16_t thiszctime;
int phase = 1;
int duty_cycle = 0;
char step = 1;
uint16_t commutation_interval = 12500;
int pwm = 1;
int floating =2;
int lowside = 3;
int sensorless = 1;
uint32_t waitTime = 0;
int signaltimeout = 0;

uint8_t ubAnalogWatchdogStatus = RESET;


void checkForHighSignal(){
changeToInput();
LL_GPIO_SetPinPull(INPUT_PIN_PORT, INPUT_PIN, LL_GPIO_PULL_DOWN);
delayMicros(1000);
for(int i = 0 ; i < 1000; i ++){
	 if( !(INPUT_PIN_PORT->IDR & INPUT_PIN)){  // if the pin is low for 5 checks out of 100 in  100ms or more its either no signal or signal. jump to application
		 low_pin_count++;
	 }
     delayMicros(10);
}
LL_GPIO_SetPinPull(INPUT_PIN_PORT, INPUT_PIN, LL_GPIO_PULL_NO);
	 if(low_pin_count > 5){
		 return;      // its either a signal or a disconnected pin
	 }else{
		allOff();
		NVIC_SystemReset();
	 }
}

void loadEEpromSettings(){
	   read_flash_bin( eepromBuffer , EEPROM_START_ADD , 48);

	   if(eepromBuffer[17] == 0x01){
	 	  dir_reversed =  1;
	   }else{
		   dir_reversed = 0;
	   }
	   if(eepromBuffer[18] == 0x01){
	 	  bi_direction = 1;
	   }else{
		  bi_direction = 0;
	   }
	   if(eepromBuffer[19] == 0x01){
	 	  use_sin_start = 1;
	 //	 min_startup_duty = sin_mode_min_s_d;
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
		   TIMER1_MAX_ARR = map (eepromBuffer[24], 24, 48, TIM1_AUTORELOAD ,TIM1_AUTORELOAD/2);
		   TIM1->ARR = TIMER1_MAX_ARR;
		   
	    }else{
	    	tim1_arr = TIM1_AUTORELOAD;
	    	TIM1->ARR = tim1_arr;
	    }

	   if(eepromBuffer[25] < 151 && eepromBuffer[25] > 49){
		   min_startup_duty = eepromBuffer[25]/ 2 + 10 + startup_boost;
		   minimum_duty_cycle = eepromBuffer[25]/ 2 + DEAD_TIME/3;
//		   if (use_sin_start){
//			   min_startup_duty = eepromBuffer[25];
//			   minimum_duty_cycle = eepromBuffer[25]/ 4;
//		   }
	    }else{
	    	min_startup_duty = 150;
	    	minimum_duty_cycle = (min_startup_duty / 2) + 10;
	    }

       motor_kv = (eepromBuffer[26] * 40) + 20;
       motor_poles = eepromBuffer[27];

	   if(eepromBuffer[28] == 0x01){
		   brake_on_stop = 1;
	    }else{
	    	brake_on_stop = 0;
	    }
	   if(eepromBuffer[29] == 0x01){
		   stall_protection = 1;
	    }else{
	    	stall_protection = 0;
	    }
	   setVolume(5);
	   if(eepromBuffer[1] > 0){             // these commands weren't introduced until eeprom version 1.

		   if(eepromBuffer[30] > 11){
			   setVolume(5);
		   }else{
			   setVolume(eepromBuffer[30]);
		   }
		   if(eepromBuffer[31] == 0x01){
			   THIRTY_TWO_MS_TLM = 1;
		   }else{
			   THIRTY_TWO_MS_TLM = 0;
		   }
		   servo_low_threshold = (eepromBuffer[32]*2) + 750; // anything below this point considered 0
		   servo_high_threshold = (eepromBuffer[33]*2) + 1750;;  // anything above this point considered 2000 (max)
		   servo_neutral = (eepromBuffer[34]) + 1374;
		   servo_dead_band = eepromBuffer[35];

		   if(eepromBuffer[36] == 0x01){
			   LOW_VOLTAGE_CUTOFF = 1;
		   }else{
			   LOW_VOLTAGE_CUTOFF = 0;
		   }

		   low_cell_volt_cutoff = eepromBuffer[37] + 250; // 2.5 to 3.5 volts per cell range
		   if(eepromBuffer[38] == 0x01){
			   RC_CAR_REVERSE = 1;
		   }else{
			   RC_CAR_REVERSE = 0;
		   }
		   if(eepromBuffer[39] == 0x01){
#ifdef HAS_HALL_SENSORS
			   USE_HALL_SENSOR = 1;
#else
			   USE_HALL_SENSOR = 0;
#endif
		   }else{
			   USE_HALL_SENSOR = 0;
		   }
	   if(eepromBuffer[40] > 4 && eepromBuffer[40] < 26){            // sine mode changeover 5-25 percent throttle
       sine_mode_changeover_thottle_level = eepromBuffer[40];
	   }
	   if(eepromBuffer[41] > 0 && eepromBuffer[41] < 11){        // drag brake 0-10
       drag_brake_strength = eepromBuffer[41];
	   }
	   }


	   if(motor_kv < 300){
		   low_rpm_throttle_limit = 0;
	   }
	   low_rpm_level  = motor_kv / 200 / (16 / motor_poles);
	   high_rpm_level = (40 + (motor_kv / 100)) / (16/motor_poles);

	if(!comp_pwm){
		bi_direction = 0;
	}



}

void saveEEpromSettings(){


   if(dir_reversed == 1){
	   eepromBuffer[17] = 0x01;
   }else{
	   eepromBuffer[17] = 0x00;
   }
   if(bi_direction == 1){
	   eepromBuffer[18] = 0x01;
      }else{
    	  eepromBuffer[18] = 0x00;
      }
   if(use_sin_start == 1){
	   eepromBuffer[19] = 0x01;
      }else{
    	  eepromBuffer[19] = 0x00;
      }

   if(comp_pwm == 1){
	   eepromBuffer[20] = 0x01;
      }else{
    	  eepromBuffer[20] = 0x00;
      }
   if(VARIABLE_PWM == 1){
	   eepromBuffer[21] = 0x01;
      }else{
    	  eepromBuffer[21] = 0x00;
      }
   if(stuck_rotor_protection == 1){
	   eepromBuffer[22] = 0x01;
      }else{
    	  eepromBuffer[22] = 0x00;
      }


   eepromBuffer[23] = advance_level;

   save_flash_nolib(eepromBuffer, 48, EEPROM_START_ADD);
}





void getSmoothedInput() {

		total = total - readings[readIndex];
		readings[readIndex] = commutation_interval;
		total = total + readings[readIndex];
		readIndex = readIndex + 1;
		if (readIndex >= numReadings) {
			readIndex = 0;
		}
		smoothedinput = total / numReadings;


}

void getBemfState(){

    if (rising){
    	if (LL_COMP_ReadOutputLevel(MAIN_COMP) == LL_COMP_OUTPUT_LEVEL_LOW){
    		bemfcounter++;
    		}else{
    		bad_count++;
    		if(bad_count > 2){
    		bemfcounter = 0;
    		}
   	}
    }else{
    	if(LL_COMP_ReadOutputLevel(MAIN_COMP) == LL_COMP_OUTPUT_LEVEL_HIGH){
    		bemfcounter++;
    	}else{
    		bad_count++;
    	    if(bad_count > 2){
    	    bemfcounter = 0;
    	  }
    	}
    }
}

void commutate(){
	commutation_intervals[step-1] = commutation_interval;
	e_com_time = (commutation_intervals[0] + commutation_intervals[1] + commutation_intervals[2] + commutation_intervals[3] + commutation_intervals[4] +commutation_intervals[5]) >> 1;  // COMMUTATION INTERVAL IS 0.5US INCREMENTS

//	COM_TIMER->CNT = 0;
	if (forward == 1){
		step++;
		if (step > 6) {
			step = 1;
			desync_check = 1;
		}
		rising = step % 2;
	}else{
		step--;
		if (step < 1) {
			step = 6;
			desync_check = 1;
		}
		rising = !(step % 2);
	}
	if(!prop_brake_active){
	comStep(step);
	}

	changeCompInput();

if(average_interval > 2000 && (stall_protection || RC_CAR_REVERSE)){
	old_routine = 1;
}
	bemfcounter = 0;
	zcfound = 0;
	timeout_count = 0;
}

void PeriodElapsedCallback(){

			COM_TIMER->DIER &= ~((0x1UL << (0U)));             // disable interrupt
			commutation_interval = (( 3*commutation_interval) + thiszctime)>>2;
			commutate();
			advance = (commutation_interval>>3) * advance_level;   // 60 divde 8 7.5 degree increments
			waitTime = (commutation_interval >>1)  - advance;
			if(!old_routine){
			enableCompInterrupts();     // enable comp interrupt
			}
			if(zero_crosses<10000){
			zero_crosses++;
			}
		//	UTILITY_TIMER->CNT = 0;

}


void interruptRoutine(){
	if (average_interval > 125){
if ((INTERVAL_TIMER->CNT < 125) && (duty_cycle < 600) && (zero_crosses < 500)){    //should be impossible, desync?exit anyway
	return;
}

if (INTERVAL_TIMER->CNT < (commutation_interval >> 1)){
	return;
}



stuckcounter++;             // stuck at 100 interrupts before the main loop happens again.
if (stuckcounter > 100){
	maskPhaseInterrupts();
	zero_crosses = 0;
	return;
}
	}
thiszctime = INTERVAL_TIMER->CNT;
			if (rising){
			for (int i = 0; i < filter_level; i++){
				if(LL_COMP_ReadOutputLevel(MAIN_COMP) == LL_COMP_OUTPUT_LEVEL_HIGH){
							return;
					}

				}
			}else{
				for (int i = 0; i < filter_level; i++){
				if(LL_COMP_ReadOutputLevel(MAIN_COMP) == LL_COMP_OUTPUT_LEVEL_LOW){
							return;
			    }
				}
			}
							maskPhaseInterrupts();
							INTERVAL_TIMER->CNT = 0 ;


		   waitTime = waitTime >> fast_accel;


            COM_TIMER->CNT = 0;
            COM_TIMER->ARR = waitTime;
			COM_TIMER->SR = 0x00;
			COM_TIMER->DIER |= (0x1UL << (0U));             // enable COM_TIMER interrupt
}

void startMotor() {
	if (running == 0){
	commutate();
	commutation_interval = 10000;
	INTERVAL_TIMER->CNT = 5000;
	running = 1;
	}
	enableCompInterrupts();
	sensorless = 1;
}




void tenKhzRoutine(){
	consumption_timer++;
	if(consumption_timer > 10000){      // 1s sample interval
		consumed_current = (float)actual_current/3600 + consumed_current;
		consumption_timer = 0;
	}
if(!armed){
	if(inputSet){
		if(adjusted_input == 0){
			armed_timeout_count++;
			if(armed_timeout_count > 10000){    // one second
				if(zero_input_count > 30){
				armed = 1;
				#ifdef tmotor55
				  			GPIOB->BRR = LL_GPIO_PIN_3;    // turn off red
				  			GPIOA->BSRR = LL_GPIO_PIN_15;   // turn on green
				#endif
				  			if(cell_count == 0 && LOW_VOLTAGE_CUTOFF){
				  			  cell_count = battery_voltage / 370;
				  			  for (int i = 0 ; i < cell_count; i++){
				  			  playInputTune();
				  			  delayMillis(100);
				  			 LL_IWDG_ReloadCounter(IWDG);
				  			  }
				  			  }else{
				  			  playInputTune();
				  			  }
				  			if(!servoPwm){
				  				RC_CAR_REVERSE = 0;
				  			}
			}else{
				inputSet = 0;
				armed_timeout_count =0;
			}
			}
		}else{
			armed_timeout_count =0;
		}
	}
}

	if(THIRTY_TWO_MS_TLM){
		thirty_two_ms_count++;
		if(thirty_two_ms_count>320){
			send_telemetry = 1;
			thirty_two_ms_count = 0;
		}
	}

	if(!stepper_sine && !BRUSHED_MODE){
	  if (input >= 47 +(80*use_sin_start) && armed){
		  if (running == 0){
			  allOff();
			  if(!old_routine){
			 startMotor();
			  }
			  running = 1;
			  last_duty_cycle = min_startup_duty;
#ifdef tmotor55
			  GPIOB->BRR = LL_GPIO_PIN_3;  // off red
			  GPIOA->BRR = LL_GPIO_PIN_15; // off green
			  GPIOB->BSRR = LL_GPIO_PIN_5;  // on blue
#endif

		  }
	  if(use_sin_start){
		duty_cycle = map(input, 127, 2047, minimum_duty_cycle, TIMER1_MAX_ARR);
  	  }else{
	 	 duty_cycle = map(input, 47, 2047, minimum_duty_cycle, TIMER1_MAX_ARR);
	  }
	  if(!RC_CAR_REVERSE){
		  prop_brake_active = 0;
	  }
	  }
	  if (input < 47 + (80*use_sin_start)){
		if(play_tone_flag != 0){
			if(play_tone_flag == 1){
				playDefaultTone();

			}if(play_tone_flag == 2){
				playChangedTone();
			}
			play_tone_flag = 0;
		}

		  if(!comp_pwm){
			duty_cycle = 0;
			if(!running){
				old_routine = 1;
				zero_crosses = 0;
				  if(brake_on_stop){
					  fullBrake();
				  }else{
					  allOff();
				  }
			}
			if (RC_CAR_REVERSE && prop_brake_active) {
#ifndef PWM_ENABLE_BRIDGE
					duty_cycle = getAbsDif(1000, newinput) + 1000;
				    proportionalBrake();
#endif
			}


		  }else{
		  if (!running){
			  duty_cycle = 0;
		  old_routine = 1;
		  zero_crosses = 0;
		  bad_count = 0;
		  if(brake_on_stop){
			if(!use_sin_start){
#ifndef PWM_ENABLE_BRIDGE				
			duty_cycle = (TIMER1_MAX_ARR-19) + drag_brake_strength*2;
			proportionalBrake();
	        prop_brake_active = 1;
#else
	//todo add proportional braking for pwm/enable style bridge.
#endif
			}
		  }else{
         allOff();
         duty_cycle = 0;
		  }
		  }
		 	  phase_A_position = 0;
		 	  phase_B_position = 119;
		 	  phase_C_position = 239;
		 	  if(use_sin_start == 1){
		    	 stepper_sine = 1;
		 	  }

		  }
		  }
if(!prop_brake_active){
 if (zero_crosses < (20 >> stall_protection)){
	   if (duty_cycle < min_startup_duty){
	   duty_cycle = min_startup_duty;

	   }
	   if (duty_cycle > 200<<stall_protection){
		   duty_cycle = 200<<stall_protection;
	   }
 }
if (running){
	 if(stall_protection){  // this boosts throttle as the rpm gets lower, for crawlers and rc cars only, do not use for multirotors.
		 min_startup_duty = eepromBuffer[25];
		 velocity_count++;
				 if (velocity_count > velocity_count_threshold){
					 if(commutation_interval > 9000){
						    	// duty_cycle = duty_cycle + map(commutation_interval, 10000, 12000, 1, 100);
						    	 minimum_duty_cycle ++;
						     }else{
						    	 minimum_duty_cycle--;
						     }
					 if(minimum_duty_cycle > 200){
						 minimum_duty_cycle = 200;
					 }
					 if(minimum_duty_cycle < (DEAD_TIME/2) + (eepromBuffer[25]/2)){        // boost minimum duty cycle by a small amount permanently too
						 minimum_duty_cycle= (DEAD_TIME/2) + (eepromBuffer[25]/2);
					 }

				velocity_count = 0;

				 }
	 }
}
	 if (duty_cycle > duty_cycle_maximum){
		 duty_cycle = duty_cycle_maximum;
	 }

		if(maximum_throttle_change_ramp){
	//	max_duty_cycle_change = map(k_erpm, low_rpm_level, high_rpm_level, 1, 40);
			if(average_interval > 500){
				max_duty_cycle_change = 10;
			}else{
				max_duty_cycle_change = 30;
			}

	 if ((duty_cycle - last_duty_cycle) > max_duty_cycle_change){
		duty_cycle = last_duty_cycle + max_duty_cycle_change;
		if(commutation_interval > 500){
			fast_accel = 1;
		}else{
			fast_accel = 0;
		}

	}else if ((last_duty_cycle - duty_cycle) > max_duty_cycle_change){
		duty_cycle = last_duty_cycle - max_duty_cycle_change;
		fast_accel = 0;
	}else{
		fast_accel = 0;
	}
		}
}
		if (armed && running && (input > 47)){
		if(VARIABLE_PWM){
	    tim1_arr = map(commutation_interval, 96, 200, TIMER1_MAX_ARR/2, TIMER1_MAX_ARR);
		advance_level = eepromBuffer[23];
		}
	    adjusted_duty_cycle = ((duty_cycle * tim1_arr)/TIMER1_MAX_ARR)+1;
		}else{
				if(prop_brake_active){
					adjusted_duty_cycle = TIMER1_MAX_ARR - ((duty_cycle * tim1_arr)/TIMER1_MAX_ARR)+1;
				}else{
				adjusted_duty_cycle = 0;
				}
	    }
		last_duty_cycle = duty_cycle;


	TIM1->ARR = tim1_arr;

	TIM1->CCR1 = adjusted_duty_cycle;
	TIM1->CCR2 = adjusted_duty_cycle;
	TIM1->CCR3 = adjusted_duty_cycle;
	}
average_interval = e_com_time / 3;
if(desync_check && zero_crosses > 10){
//	if(average_interval < last_average_interval){
//
//	}
		if((getAbsDif(last_average_interval,average_interval) > average_interval>>1) && (average_interval < 1000)){ //throttle resitricted before zc 20.
		zero_crosses = 10;
		desync_happened ++;
//running = 0;
//old_routine = 1;
		last_duty_cycle = min_startup_duty/2;
		}

		desync_check = 0;
//	}
	last_average_interval = average_interval;
	}
	if(send_telemetry){
#ifdef	USE_SERIAL_TELEMETRY
	  makeTelemPackage(degrees_celsius,
			           battery_voltage,
					   actual_current,
	  				   (uint16_t)consumed_current/10,
	  					e_rpm);
	  send_telem_DMA();
	  send_telemetry = 0;
#endif
	}

	if(commutation_interval > 400){
		   NVIC_SetPriority(IC_DMA_IRQ_NAME, 0);
		   NVIC_SetPriority(ADC1_COMP_IRQn, 1);
	}else{
		NVIC_SetPriority(IC_DMA_IRQ_NAME, 1);
		NVIC_SetPriority(ADC1_COMP_IRQn, 0);
	}

		signaltimeout++;
		if(signaltimeout > 2500 * (servoPwm+1)) { // quarter second timeout when armed half second for servo;
			if(armed){
				allOff();
				armed = 0;
				input = 0;
				inputSet = 0;
				zero_input_count = 0;
				TIM1->CCR1 = 0;
			    TIM1->CCR2 = 0;
				TIM1->CCR3 = 0;
				IC_TIMER_REGISTER->PSC = 0;
				IC_TIMER_REGISTER->CNT = 0;
				for(int i = 0; i < 64; i++){
					dma_buffer[i] = 0;
				}
				NVIC_SystemReset();
			}

		if (signaltimeout > 25000){     // 2.5 second
			allOff();
			armed = 0;
			input = 0;
			inputSet = 0;
			zero_input_count = 0;
			TIM1->CCR1 = 0;
		    TIM1->CCR2 = 0;
			TIM1->CCR3 = 0;
			IC_TIMER_REGISTER->PSC = 0;
			IC_TIMER_REGISTER->CNT = 0;
			for(int i = 0; i < 64; i++){
				dma_buffer[i] = 0;
			}
			NVIC_SystemReset();
		}
			}
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
    if(GIMBAL_MODE){
    TIM1->CCR1 = ((2*pwmSin[phase_A_position])+gate_drive_offset)*TIM1_AUTORELOAD/2000;
    TIM1->CCR2 = ((2*pwmSin[phase_B_position])+gate_drive_offset)*TIM1_AUTORELOAD/2000;
    TIM1->CCR3 = ((2*pwmSin[phase_C_position])+gate_drive_offset)*TIM1_AUTORELOAD/2000;
    }else{
    TIM1->CCR1 = ((2*pwmSin[phase_A_position]/SINE_DIVIDER)+gate_drive_offset)*TIM1_AUTORELOAD/2000;
    TIM1->CCR2 = ((2*pwmSin[phase_B_position]/SINE_DIVIDER)+gate_drive_offset)*TIM1_AUTORELOAD/2000;
    TIM1->CCR3 = ((2*pwmSin[phase_C_position]/SINE_DIVIDER)+gate_drive_offset)*TIM1_AUTORELOAD/2000;
    }
}


void zcfoundroutine(){   // only used in polling mode, blocking routine.
	thiszctime = INTERVAL_TIMER->CNT;
	INTERVAL_TIMER->CNT = 0;
	commutation_interval = (thiszctime + (3*commutation_interval)) / 4;
	advance = commutation_interval / advancedivisor;
	waitTime = commutation_interval /2  - advance;
//	blanktime = commutation_interval / 4;
	while (INTERVAL_TIMER->CNT - thiszctime < waitTime - advance){

	}
	commutate();
    bemfcounter = 0;
    bad_count = 0;

    zero_crosses++;
    if(stall_protection || RC_CAR_REVERSE){
   	 if (zero_crosses >= 100 && commutation_interval <= 2000) {
   	    	old_routine = 0;
   	    	enableCompInterrupts();          // enable interrupt

    	 }
    }else{
    	if(zero_crosses > 30){
    	old_routine = 0;
    	enableCompInterrupts();          // enable interrupt

    }
    }

}


int main(void)
{
 initAfterJump();

 initCorePeripherals();

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
 // LL_TIM_EnableIT_UPDATE(TIM1);
#ifdef USE_ADC_INPUT

#else
//
  LL_TIM_CC_EnableChannel(IC_TIMER_REGISTER, IC_TIMER_CHANNEL);  // input capture and output compare
  LL_TIM_EnableCounter(IC_TIMER_REGISTER);
#endif

#ifdef tmotor55
  LED_GPIO_init();
  GPIOB->BSRR = LL_GPIO_PIN_3; // turn on red
#endif


   LL_TIM_EnableCounter(COM_TIMER);               // commutation_timer priority 0
   LL_TIM_GenerateEvent_UPDATE(COM_TIMER);
   LL_TIM_EnableIT_UPDATE(COM_TIMER);
   COM_TIMER->DIER &= ~((0x1UL << (0U)));         // disable for now.
//
   LL_TIM_EnableCounter(UTILITY_TIMER);
   LL_TIM_GenerateEvent_UPDATE(UTILITY_TIMER);
//
   LL_TIM_EnableCounter(INTERVAL_TIMER);
   LL_TIM_GenerateEvent_UPDATE(INTERVAL_TIMER);

   LL_TIM_EnableCounter(TEN_KHZ_TIMER);                 // 10khz timer
   LL_TIM_GenerateEvent_UPDATE(TEN_KHZ_TIMER);
   TEN_KHZ_TIMER->DIER |= (0x1UL << (0U));  // enable interrupt


  //RCC->APB2ENR  &= ~(1 << 22);  // turn debug off
#ifdef USE_ADC
   ADC_Init();
   enableADC_DMA();
   activateADC();
#endif


   __IO uint32_t wait_loop_index = 0;
    /* Enable comparator */

   LL_COMP_Enable(MAIN_COMP);

   wait_loop_index = ((LL_COMP_DELAY_STARTUP_US * (SystemCoreClock / (100000 * 2))) / 10);
   while(wait_loop_index != 0)
   {
     wait_loop_index--;
   }


  loadEEpromSettings();
//  EEPROM_VERSION = *(uint8_t*)(0x08000FFC);
  if(firmware_info.version_major != eepromBuffer[3] || firmware_info.version_minor != eepromBuffer[4]){
	  eepromBuffer[3] = firmware_info.version_major;
	  eepromBuffer[4] = firmware_info.version_minor;
	  for(int i = 0; i < 12 ; i ++){
		  eepromBuffer[5+i] = firmware_info.device_name[i];
	  }
	  saveEEpromSettings();
  }
//  if(EEPROM_VERSION != eepromBuffer[2]){
//	  eepromBuffer[2] = EEPROM_VERSION;
//	  saveEEpromSettings();
//  }


  if(use_sin_start){
  min_startup_duty = sin_mode_min_s_d;
  }
	if (dir_reversed == 1){
			forward = 0;
		}else{
			forward = 1;
		}
	tim1_arr = TIMER1_MAX_ARR;
	if(!comp_pwm){
		use_sin_start = 0;
	}

	if (RC_CAR_REVERSE) {         // overrides a whole lot of things!
		throttle_max_at_low_rpm = 1000;
		bi_direction = 1;
		use_sin_start = 0;
		low_rpm_throttle_limit = 1;
		VARIABLE_PWM = 0;
		stall_protection = 1;
		comp_pwm = 0;
      	stuck_rotor_protection = 0;
		minimum_duty_cycle = 100;
		min_startup_duty = 180;

	}

	if(BRUSHED_MODE){bi_direction = 1;
	 	 	 	 	  commutation_interval = 5000;}

	   if(BRUSHED_MODE){
		   playBrushedStartupTune();
	   }else{
		   playStartupTune();
	   }
	   zero_input_count = 0;
	   MX_IWDG_Init();
	   LL_IWDG_ReloadCounter(IWDG);


if (GIMBAL_MODE){
	bi_direction = 1;
	use_sin_start = 1;
}

#ifdef USE_ADC_INPUT
   armed_count_threshold = 5000;
   inputSet = 1;

#else
 checkForHighSignal();     // will reboot if signal line is high for 10ms
 receiveDshotDma();
#endif

#ifdef MCU_F051
 MCU_Id = DBGMCU->IDCODE &= 0xFFF;
 REV_Id = DBGMCU->IDCODE >> 16;

 if(REV_Id >= 4096){
	 temperature_offset = 0;
 }else{
	 temperature_offset = 230;
 }
#endif
 while (1)
  {

LL_IWDG_ReloadCounter(IWDG);

	  adc_counter++;
	  if(adc_counter>100){   // for testing adc and telemetry
		  ADC_raw_temp = ADC_raw_temp - (temperature_offset);
		  converted_degrees =__LL_ADC_CALC_TEMPERATURE(3300,  ADC_raw_temp, LL_ADC_RESOLUTION_12B);
		  degrees_celsius =((7 * degrees_celsius) + converted_degrees) >> 3;

          battery_voltage = ((7 * battery_voltage) + ((ADC_raw_volts * 3300 / 4095 * VOLTAGE_DIVIDER)/100)) >> 3;
          smoothed_raw_current = ((7*smoothed_raw_current + (ADC_raw_current) )>> 3);
          actual_current = ((smoothed_raw_current * 3300/4095) * MILLIVOLT_PER_AMP )/10  + CURRENT_OFFSET;

		  LL_ADC_REG_StartConversion(ADC1);
		  if(LOW_VOLTAGE_CUTOFF){
			  if(battery_voltage < (cell_count * low_cell_volt_cutoff)){
				  low_voltage_count++;
				  if(low_voltage_count > 1000){
				  input = 0;
				  allOff();
				  maskPhaseInterrupts();
				  running = 0;
				  zero_input_count = 0;
				  armed = 0;

				  }
			  }else{
				  low_voltage_count = 0;
			  }
		  }
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

  		  if (bi_direction == 1 && dshot == 0){
  			  if(RC_CAR_REVERSE){
  				  if (newinput > (1000 + (servo_dead_band<<1))) {
  					  if (forward == dir_reversed) {
  						  adjusted_input = 0;
  						  if(running){
  							  prop_brake_active = 1;
  						  }else{
  							  forward = 1 - dir_reversed;
  						  }
  					  }
  					  if (prop_brake_active == 0) {
  						  adjusted_input = map(newinput, 1000 + (servo_dead_band<<1), 2000, 47, 2047);
  					  }
  				  }
  				  if (newinput < (1000 -(servo_dead_band<<1))) {
  					  if (forward == (1 - dir_reversed)) {
  						  if(running){
  							  prop_brake_active = 1;
  						  }else{
  							  forward = dir_reversed;
  						  }
  						  adjusted_input = 0;

  					  }
  					  if (prop_brake_active == 0) {
  						  adjusted_input = map(newinput, 0, 1000-(servo_dead_band<<1), 2047, 47);
  					  }
  				  }


  				  if (newinput >= (1000 - (servo_dead_band << 1)) && newinput <= (1000 + (servo_dead_band <<1))) {
  					  adjusted_input = 0;
  					  prop_brake_active = 0;
  				  }
  			  }else{
  				  if (newinput > (1000 + (servo_dead_band<<1))) {
  					  if (forward == dir_reversed) {
  						  if(commutation_interval > 1500 || stepper_sine){
  							  forward = 1 - dir_reversed;
  							  zero_crosses = 0;
  							  old_routine = 1;
  							  maskPhaseInterrupts();
  							brushed_direction_set = 0;
  						  }else{
  							  newinput = 1000;
  						  }
  					  }
  					  adjusted_input = map(newinput, 1000 + (servo_dead_band<<1), 2000, 47, 2047);
  				  }
  				  if (newinput < (1000 -(servo_dead_band<<1))) {
  					  if (forward == (1 - dir_reversed)) {
  						  if(commutation_interval > 1500 || stepper_sine){
  							  zero_crosses = 0;
  							  old_routine = 1;
  							  forward = dir_reversed;
  							  maskPhaseInterrupts();
  							brushed_direction_set = 0;
  						  }else{
  							  newinput = 1000;

  						  }
  					  }
  					  adjusted_input = map(newinput, 0, 1000-(servo_dead_band<<1), 2047, 47);
  				  }

 				  if (newinput >= (1000 - (servo_dead_band << 1)) && newinput <= (1000 + (servo_dead_band <<1))) {
  					  adjusted_input = 0;
  					brushed_direction_set = 0;
  				  }
  			  }
 		  }else if (dshot && bi_direction) {
  			  if (newinput > 1047) {

  				  if (forward == dir_reversed) {
  					  if(commutation_interval > 1500 || stepper_sine){
  						  forward = 1 - dir_reversed;
  						  zero_crosses = 0;
  						  old_routine = 1;
  						  maskPhaseInterrupts();
  						brushed_direction_set = 0;
  					  }else{
  						  newinput = 0;

  					  }
  				  }
  				  adjusted_input = ((newinput - 1048) * 2 + 47) - reversing_dead_band;

  			  }
  			  if (newinput <= 1047  && newinput > 47) {
  				  //	startcount++;

  				  if (forward == (1 - dir_reversed)) {
  					  if(commutation_interval > 1500 || stepper_sine){
  						  zero_crosses = 0;
  						  old_routine = 1;
  						  forward = dir_reversed;
  						  maskPhaseInterrupts();
  						brushed_direction_set = 0;
  					  }else{
  						  newinput = 0;

  					  }
  				  }
  				  adjusted_input = ((newinput - 48) * 2 + 47) - reversing_dead_band;
  			  }
  			  if ( newinput < 48) {
  				  adjusted_input = 0;
  				brushed_direction_set = 0;
  			  }


  		  }else{
  			  adjusted_input = newinput;
  		  }
  		if(BRUSHED_MODE){
  			if(brushed_direction_set == 0 && adjusted_input > 48){
  				if(forward){
  					allOff();
  					delayMicros(10);
  					comStep(6);
  				}else{
  					allOff();
  					delayMicros(10);
  					comStep(3);
  				}
  				brushed_direction_set = 1;
  			}
  			if(adjusted_input > 1900){
  				adjusted_input = 1900;
  			}
  			input = map(adjusted_input, 48, 2047, 0, TIMER1_MAX_ARR);

  			if(input > 0 && armed){
  				TIM1->CCR1 = input;												// set duty cycle to 50 out of 768 to start.
  				TIM1->CCR2 = input;
  				TIM1->CCR3 = input;
  			}else{
  				TIM1->CCR1 = 0;												// set duty cycle to 50 out of 768 to start.
  				TIM1->CCR2 = 0;
  				TIM1->CCR3 = 0;
  			//	fullBrake();
  			}



  		}else{




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
	 	 if(zero_crosses > 100 && adjusted_input < 200){
	 		bemf_timout_happened = 0;
	 	 }
	 	 if(use_sin_start && adjusted_input < 160){
	 		bemf_timout_happened = 0;
	 	 }

 	 	 if(crawler_mode){
 	 		if (adjusted_input < 400){
 	 			bemf_timout_happened = 0;
 	 		}
 	 	 }else{
 	 		if (adjusted_input < 150){              // startup duty cycle should be low enough to not burn motor
 	 			bemf_timeout = 100;
 	 	 	 }else{
 	 	 		bemf_timeout = 10;
 	 	 	 }
 	 	 }
	  if(bemf_timout_happened > bemf_timeout * ( 1 + (crawler_mode*100))&& stuck_rotor_protection){
	 		 allOff();
	 		 maskPhaseInterrupts();
	 		 input = 0;
	 		bemf_timout_happened = 102;
#ifdef tmotor55
			  GPIOA->BRR = LL_GPIO_PIN_15; // off green
			  GPIOB->BRR = LL_GPIO_PIN_5;  // off blue
			  GPIOB->BSRR = LL_GPIO_PIN_3;
#endif
	 	  }else{
	  	  	if(use_sin_start){
  				if(adjusted_input < 30){           // dead band ?
  					input= 0;
  					}

  					if(adjusted_input > 30 && adjusted_input < (sine_mode_changeover_thottle_level * 20)){
  					input= map(adjusted_input, 30 , (sine_mode_changeover_thottle_level * 20) , 47 ,160);
  					}
  					if(adjusted_input >= (sine_mode_changeover_thottle_level * 20)){
  					input = map(adjusted_input , (sine_mode_changeover_thottle_level * 20) ,2000 , 160, 2000);
  					}
  				}else{
  		   			input = adjusted_input;
  				}
	 	  }
		  if ( stepper_sine == 0){

  e_rpm = running * (100000/ e_com_time) * 6;
  k_erpm =  e_rpm / 10; // ecom time is time for one electrical revolution in microseconds
   if(low_rpm_throttle_limit){     // some hardware doesn't need this, its on by default to keep hardware / motors protected but can slow down the response in the very low end a little.

  duty_cycle_maximum = map(k_erpm, low_rpm_level, high_rpm_level, throttle_max_at_low_rpm, throttle_max_at_high_rpm);   // for more performance lower the high_rpm_level, set to a consvervative number in source.
   }


if (zero_crosses < 100 || commutation_interval > 500) {

		filter_level = 12;

	} else {

		filter_level = map(average_interval, 100 , 500, 3 , 8);


	}
	if (commutation_interval < 100){
		filter_level = 2;
	}




if(lowkv){

	filter_level = low_kv_filter_level;
}

/**************** old routine*********************/
if (old_routine && running){
	maskPhaseInterrupts();
	 		 getBemfState();
	 	  if (!zcfound){
	 		  if (rising){
	 		 if (bemfcounter > min_bemf_counts_up){
	 			 zcfound = 1;
	 			 zcfoundroutine();
	 		}
	 		  }else{
	 			  if (bemfcounter > min_bemf_counts_down){
 			  			 zcfound = 1;
	 		  			 zcfoundroutine();
	 			  		}
	 		  }
	 	  }
}
	 	  if (INTERVAL_TIMER->CNT > 45000 && running == 1){
              bemf_timout_happened ++;
	 		  zcfoundroutine();
	 		  maskPhaseInterrupts();
	 		  old_routine = 1;
	 		   running = 0;
	 		   zero_crosses = 0;
	 		   if(crawler_mode&&stall_protection){
	 			   min_startup_duty = 110;
	 				 minimum_duty_cycle = minimum_duty_cycle + 10;
	 				 if(minimum_duty_cycle > 80){
	 					 minimum_duty_cycle = 80;
	 				 }
	 		   }
	 	  }
	 	  }else{            // stepper sine

	 			if(GIMBAL_MODE){
	 				step_delay = 300;
	 				maskPhaseInterrupts();
	 				allpwm();
	 				if(newinput>1000){
	 					desired_angle = map(newinput, 1000, 2000, 180, 360);
	 				}else{
	 					desired_angle = map(newinput, 0, 1000, 0, 180);
	 				}
	 				if(current_angle > desired_angle){
	 					forward = 1;
	 					advanceincrement();
	 					delayMicros(step_delay);
	 					current_angle--;
	 				}
	 				if(current_angle < desired_angle){
	 					forward = 0;
	 					advanceincrement();
	 					delayMicros(step_delay);
	 					current_angle++;
	 				}
	 			}else{



if(input > 48 && armed){

	 		  if (input > 48 && input < 137){// sine wave stepper

	 			 maskPhaseInterrupts();
	 			 allpwm();
	 		 advanceincrement();
             step_delay = map (input, 48, 137, 7000/motor_poles, 810/motor_poles);
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
				  old_routine = 1;
		 		  commutation_interval = 9000;
		 		  average_interval = 9000;
				  last_average_interval = average_interval;
		 		//  minimum_duty_cycle = ;
		 		  INTERVAL_TIMER->CNT = 9000;
				  zero_crosses = 0;
				  prop_brake_active = 0;
	 			  step = changeover_step;                    // rising bemf on a same as position 0.
		 		 comStep(step);// rising bemf on a same as position 0.
	 			LL_TIM_GenerateEvent_UPDATE(TIM1);
	 			  zcfoundroutine();
	 			  }
	 		  }

}else{
	if(brake_on_stop){
	#ifndef PWM_ENABLE_BRIDGE
	duty_cycle = (TIMER1_MAX_ARR-19) + drag_brake_strength*2;
	adjusted_duty_cycle = TIMER1_MAX_ARR - ((duty_cycle * tim1_arr)/TIMER1_MAX_ARR)+1;
	TIM1->CCR1 = adjusted_duty_cycle;
	TIM1->CCR2 = adjusted_duty_cycle;
	TIM1->CCR3 = adjusted_duty_cycle;
	proportionalBrake();
	prop_brake_active = 1;
	#else
		// todo add braking for PWM /enable style bridges.
	#endif 
	}else{
	TIM1->CCR1 = 0;
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 0;
	allOff();
	}


}

	 			}
	 	  }
  }// end of brushed mode override
  }
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
