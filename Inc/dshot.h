/*
 * dshot.h
 *
 *  Created on: Apr. 22, 2020
 *      Author: Alka
 */

#include "main.h"

#ifndef INC_DSHOT_H_
#define INC_DSHOT_H_





void computeDshotDMA(void);
void make_dshot_package();

extern void playInputTune(void);
extern void playInputTune2(void);
extern uint32_t gcr[];
extern int newinput;
extern int dshot_frametime;
extern uint32_t dma_buffer[64];
extern char dshot_telemetry;
extern int dshot_badcounts;
extern int dshotcommand;
extern char armed;
extern int signaltimeout;
extern int dshot_goodcounts;
extern char dir_reversed;
extern char bi_direction;
extern int zero_input_count;
extern int forward;
extern char buffer_divider;
extern int running;
extern uint8_t last_dshot_command;
extern int k_erpm;
extern uint16_t commutation_interval;
extern uint16_t process_time;

//int e_com_time;


#endif /* INC_DSHOT_H_ */
