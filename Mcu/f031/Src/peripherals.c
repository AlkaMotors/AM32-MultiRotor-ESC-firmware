/*
 * peripherals.c
 *
 *  Created on: Sep. 26, 2020
 *      Author: Alka
 */


// PERIPHERAL SETUP

#include "peripherals.h"
#include "targets.h"
#include "serial_telemetry.h"

void initCorePeripherals(void){

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  SystemClock_Config();

 FLASH->ACR |= FLASH_ACR_PRFTBE;   //// prefetch buffer enable
  UN_GPIO_Init();
  MX_DMA_Init();
  MX_TIM14_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM17_Init();
  TEN_KHZ_Timer_Init();
  UN_TIM_Init();
  #ifdef USE_SERIAL_TELEMETRY
  telem_UART_Init();
#endif
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



void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
//  Error_Handler();
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
}


void MX_IWDG_Init(void)
{

  LL_IWDG_Enable(IWDG);
  LL_IWDG_EnableWriteAccess(IWDG);
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_8);
  LL_IWDG_SetReloadCounter(IWDG, 4095);
  while (LL_IWDG_IsReady(IWDG) != 1)
  {
  }

  LL_IWDG_SetWindow(IWDG, 4095);
  LL_IWDG_ReloadCounter(IWDG);

}


void MX_TIM1_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
  LL_TIM_BDTR_InitTypeDef TIM_BDTRInitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM1);

  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 1999;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM1, &TIM_InitStruct);
  LL_TIM_EnableARRPreload(TIM1);
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
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**TIM1 GPIO Configuration
  PB13   ------> TIM1_CH1N
  PB14   ------> TIM1_CH2N
  PB15   ------> TIM1_CH3N
  PA8   ------> TIM1_CH1
  PA9   ------> TIM1_CH2
  PA10   ------> TIM1_CH3
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_14;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
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

void MX_TIM3_Init(void)
{

  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

  TIM_InitStruct.Prescaler = 23;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM3, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM3);
  LL_TIM_SetClockSource(TIM3, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM3);

}

void MX_TIM14_Init(void)
{

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM14);

  /* TIM14 interrupt Init */
  NVIC_SetPriority(TIM14_IRQn, 0);
  NVIC_EnableIRQ(TIM14_IRQn);
  TIM_InitStruct.Prescaler = 23;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0xFFFF;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM14, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM14);

}

void MX_TIM17_Init(void)
{

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM17);
  TIM_InitStruct.Prescaler = 47;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0xFFFF;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM17, &TIM_InitStruct);
  LL_TIM_EnableARRPreload(TIM17);

}

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* DMA interrupt init */
  /* DMA1_Channel2_3_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1);
  NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
  /* DMA1_Channel4_5_IRQn interrupt configuration */
  NVIC_SetPriority(IC_DMA_IRQ_NAME, 1);
  NVIC_EnableIRQ(IC_DMA_IRQ_NAME);

}


void TEN_KHZ_Timer_Init(){

#ifdef USE_TIMER_16
	  LL_TIM_InitTypeDef TIM_InitStruct = {0};
	  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	  /* Peripheral clock enable */
	  TIM_InitStruct.Prescaler = 47;
	  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	  TIM_InitStruct.Autoreload = 100;
	  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	  TIM_InitStruct.RepetitionCounter = 0;
	  LL_TIM_Init(TIM2, &TIM_InitStruct);
	  LL_TIM_EnableARRPreload(TIM2);
	  NVIC_SetPriority(TIM2_IRQn, 2);
	  NVIC_EnableIRQ(TIM2_IRQn);
#else
	  LL_TIM_InitTypeDef TIM_InitStruct = {0};
	  /* Peripheral clock enable */
	  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM16);
	  TIM_InitStruct.Prescaler = 47;
	  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	  TIM_InitStruct.Autoreload = 100;
	  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	  TIM_InitStruct.RepetitionCounter = 0;
	  LL_TIM_Init(TIM16, &TIM_InitStruct);
	  LL_TIM_EnableARRPreload(TIM16);
	  NVIC_SetPriority(TIM16_IRQn, 2);
	  NVIC_EnableIRQ(TIM16_IRQn);


#endif


}


void UN_TIM_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
#ifdef USE_TIMER_16
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM16);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**TIM16 GPIO Configuration
  PA6   ------> TIM16_CH1
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);


 #else

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**TIM16 GPIO Configuration
  PA6   ------> TIM16_CH1
  */
  GPIO_InitStruct.Pin = INPUT_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
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

#ifdef USE_TIMER_16
//  NVIC_SetPriority(TIM16_IRQn, 1);
//  NVIC_EnableIRQ(TIM16_IRQn);
#else
//  NVIC_SetPriority(TIM2_IRQn, 1);
 // NVIC_EnableIRQ(TIM2_IRQn);

#endif

  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 63;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(IC_TIMER_REGISTER, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(IC_TIMER_REGISTER);
//  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
//  LL_TIM_DisableMasterSlaveMode(TIM2);

  LL_TIM_IC_SetActiveInput(IC_TIMER_REGISTER, IC_TIMER_CHANNEL, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(IC_TIMER_REGISTER, IC_TIMER_CHANNEL, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter(IC_TIMER_REGISTER, IC_TIMER_CHANNEL, LL_TIM_IC_FILTER_FDIV1);
  LL_TIM_IC_SetPolarity(IC_TIMER_REGISTER, IC_TIMER_CHANNEL, LL_TIM_IC_POLARITY_BOTHEDGE);


}

void UN_GPIO_Init(void)
{
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_11);

  /**/
  LL_GPIO_ResetOutputPin(GPIOF, LL_GPIO_PIN_6);

  /**/
  LL_GPIO_ResetOutputPin(GPIOF, LL_GPIO_PIN_7);

  /**/
  LL_SYSCFG_SetEXTISource(SYSCFG_EXTI_PORTB, SYSCFG_EXTI_LINEB);

  /**/
  LL_SYSCFG_SetEXTISource(SYSCFG_EXTI_PORTA, SYSCFG_EXTI_LINEA);

  /**/
  LL_SYSCFG_SetEXTISource(SYSCFG_EXTI_PORTC, SYSCFG_EXTI_LINEC);


  LL_GPIO_SetPinPull(PHASE_B_EXTI_PORT, PHASE_B_EXTI_PIN, LL_GPIO_PULL_NO);

  /**/
  LL_GPIO_SetPinPull(PHASE_A_EXTI_PORT, PHASE_A_EXTI_PIN, LL_GPIO_PULL_NO);

  /**/
  LL_GPIO_SetPinPull(PHASE_C_EXTI_PORT, PHASE_C_EXTI_PIN, LL_GPIO_PULL_NO);

  /**/
  LL_GPIO_SetPinMode(PHASE_B_EXTI_PORT, PHASE_B_EXTI_PIN, LL_GPIO_MODE_INPUT);

  /**/
  LL_GPIO_SetPinMode(PHASE_A_EXTI_PORT, PHASE_A_EXTI_PIN, LL_GPIO_MODE_INPUT);

  /**/
  LL_GPIO_SetPinMode(PHASE_C_EXTI_PORT, PHASE_C_EXTI_PIN, LL_GPIO_MODE_INPUT);

  /**/
  EXTI_InitStruct.Line_0_31 = PHASE_B_LL_EXTI_LINE;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  EXTI_InitStruct.Line_0_31 = PHASE_A_LL_EXTI_LINE;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  EXTI_InitStruct.Line_0_31 = PHASE_C_LL_EXTI_LINE;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOF, &GPIO_InitStruct);


#ifdef   USE_HALL_SENSOR

  GPIO_InitStruct.Pin = HALL_A_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(HALL_A_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = HALL_B_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(HALL_B_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = HALL_C_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(HALL_C_PORT, &GPIO_InitStruct);


  #endif

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI_IRQ1_NAME, 0);
  NVIC_EnableIRQ(EXTI_IRQ1_NAME);
  NVIC_SetPriority(EXTI_IRQ2_NAME, 0);
  NVIC_EnableIRQ(EXTI_IRQ2_NAME);

}




void Error_Handler(void)
{
// do absolutely nothing
}
