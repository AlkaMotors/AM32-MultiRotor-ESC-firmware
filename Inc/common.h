#pragma once

extern uint8_t eepromBuffer[176];
extern uint16_t TIMER1_MAX_ARR;

extern uint32_t gcr[30];
extern uint16_t adjusted_input;
extern uint32_t dma_buffer[64];
extern uint8_t dshotcommand;
extern uint16_t armed_count_threshold;
extern char forward;
extern uint8_t running;
extern uint16_t zero_input_count;
extern uint16_t signaltimeout;
extern uint16_t input;
extern uint16_t newinput;
extern char play_tone_flag;

typedef struct PID{
	float error;
	float Kp;
	float Ki;
	float Kd;
	float integral;
	float derivative;
	float last_error;
	float pid_ouput;
	int16_t integral_limit;
	int16_t output_limit;
}PID;


