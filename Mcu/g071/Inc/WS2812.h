/*
 * WS2812.h
 *
 *  Created on: Sep 9, 2020
 *      Author: Alka
 */

#ifndef INC_WS2812_H_
#define INC_WS2812_H_

#include "main.h"




void send_LED_DMA();
void WS2812_Init(void);
void send_LED_RGB(uint8_t red, uint8_t green, uint8_t blue);

extern char dma_busy;


#endif /* INC_WS2812_H_ */