/*
 * WS2812.c
 *
 *  Created on: Sep 9, 2020
 *      Author: Alka
 */

#include "WS2812.h"

uint16_t led_Buffer[26] = {0,0,20,20,20,20,20,20,20,20,
		                  60,60,60,60,60,60,60,60,
		                  20,20,20,20,20,20,20,20};

void send_LED_DMA(){
	dma_busy = 1;
	TIM16->CNT = 0;
	  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_6, (uint32_t)&led_Buffer, (uint32_t)&TIM16->CCR1, LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_6));
	  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_6, 26);
	  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_6);
	  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_6);
			  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_6);
			  LL_TIM_EnableDMAReq_CC1(TIM16);
			  LL_TIM_CC_EnableChannel(TIM16, LL_TIM_CHANNEL_CH1);
			  LL_TIM_EnableAllOutputs(TIM16);
			  LL_TIM_EnableCounter(TIM16);
}

void send_LED_RGB(uint8_t red, uint8_t green, uint8_t blue){
if(!dma_busy){
uint32_t twenty_four_bit_color_number = green << 16 | red << 8 | blue ;


for(int i = 2; i < 26 ; i ++){
	led_Buffer[i] = (((twenty_four_bit_color_number >> (25 - i))&1) * 40) + 20;
}

send_LED_DMA();
}
}




void WS2812_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */
	 NVIC_SetPriority(DMA1_Ch4_7_DMAMUX1_OVR_IRQn, 3);
	 NVIC_EnableIRQ(DMA1_Ch4_7_DMAMUX1_OVR_IRQn);
  /* USER CODE END TIM16_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
  LL_TIM_BDTR_InitTypeDef TIM_BDTRInitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM16);

  /* TIM16 DMA Init */

  /* TIM16_CH1 Init */
  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_6, LL_DMAMUX_REQ_TIM16_CH1);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_6, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_6, LL_DMA_PRIORITY_HIGH);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_6, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_6, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_6, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_6, LL_DMA_PDATAALIGN_HALFWORD);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_6, LL_DMA_MDATAALIGN_HALFWORD);

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_DOWN;
  TIM_InitStruct.Autoreload = 79;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM16, &TIM_InitStruct);
  LL_TIM_EnableARRPreload(TIM16);
  LL_TIM_OC_EnablePreload(TIM16, LL_TIM_CHANNEL_CH1);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
  TIM_OC_InitStruct.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
  LL_TIM_OC_Init(TIM16, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM16, LL_TIM_CHANNEL_CH1);
  TIM_BDTRInitStruct.OSSRState = LL_TIM_OSSR_DISABLE;
  TIM_BDTRInitStruct.OSSIState = LL_TIM_OSSI_DISABLE;
  TIM_BDTRInitStruct.LockLevel = LL_TIM_LOCKLEVEL_OFF;
  TIM_BDTRInitStruct.DeadTime = 0;
  TIM_BDTRInitStruct.BreakState = LL_TIM_BREAK_DISABLE;
  TIM_BDTRInitStruct.BreakPolarity = LL_TIM_BREAK_POLARITY_HIGH;
  TIM_BDTRInitStruct.BreakFilter = LL_TIM_BREAK_FILTER_FDIV1;
  TIM_BDTRInitStruct.AutomaticOutput = LL_TIM_AUTOMATICOUTPUT_DISABLE;
  LL_TIM_BDTR_Init(TIM16, &TIM_BDTRInitStruct);
  /* USER CODE BEGIN TIM16_Init 2 */
//  NVIC_SetPriority(TIM16_IRQn, 0);
//  NVIC_EnableIRQ(TIM16_IRQn);
  /* USER CODE END TIM16_Init 2 */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  /**TIM16 GPIO Configuration
  PB8   ------> TIM16_CH1
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  TIM16->CCER |=  1 << 0;
}
