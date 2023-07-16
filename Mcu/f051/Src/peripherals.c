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
#include "common.h"



void initCorePeripherals(void){
  SystemClock_Config();
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  FLASH->ACR |= FLASH_ACR_PRFTBE;   //// prefetch buffer enable
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_COMP1_Init();
  MX_TIM14_Init();
  MX_TIM6_Init();
  MX_TIM17_Init();	 
  UN_TIM_Init();
  #ifdef USE_SERIAL_TELEMETRY
    telem_UART_Init();
  #endif
#ifdef USE_CRSF_INPUT
  crsf_UART_Init();
  receiveCRSF();
#endif
}

void initAfterJump(void){
    volatile uint32_t *VectorTable = (volatile uint32_t *)0x20000000;
    uint32_t vector_index = 0;
    for(vector_index  = 0; vector_index  < 48; vector_index++)
    {
      VectorTable[vector_index ] = *(__IO uint32_t*)(APPLICATION_ADDRESS + (vector_index <<2));         // no VTOR on cortex-MO so need to copy vector table
    }
  	  /* Enable the SYSCFG peripheral clock*/
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
 // Error_Handler();
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


void MX_COMP1_Init(void)
{

//LL_COMP_InitTypeDef COMP_InitStruct = {0};

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

  // COMP_InitStruct.PowerMode = LL_COMP_POWERMODE_HIGHSPEED;
  // COMP_InitStruct.InputPlus = LL_COMP_INPUT_PLUS_IO1;
  // COMP_InitStruct.InputMinus = LL_COMP_INPUT_MINUS_DAC1_CH2;
  // COMP_InitStruct.InputHysteresis = LL_COMP_HYSTERESIS_NONE;
  // COMP_InitStruct.OutputSelection = LL_COMP_OUTPUT_NONE;
  // COMP_InitStruct.OutputPolarity = LL_COMP_OUTPUTPOL_NONINVERTED;
  // LL_COMP_Init(COMP1, &COMP_InitStruct);

}


void MX_IWDG_Init(void)
{

  IWDG->KR = 0x0000CCCCU;
  IWDG->KR = 0x00005555U;
  IWDG->PR = LL_IWDG_PRESCALER_16;
  IWDG->RLR = 4000;
  LL_IWDG_ReloadCounter(IWDG);
}


void MX_TIM1_Init(void)
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
  #ifdef USE_SWAPPED_OUPUT
    TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM2;
  #else
    TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  #endif
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  #ifdef USE_INVERTED_HIGH
    TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_LOW;
    TIM_OC_InitStruct.OCIdleState = LL_TIM_OCIDLESTATE_HIGH;
  #else
    TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
    TIM_OC_InitStruct.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
  #endif
  #ifdef USE_INVERTED_LOW
    TIM_OC_InitStruct.OCNPolarity = LL_TIM_OCPOLARITY_LOW;
    TIM_OC_InitStruct.OCNIdleState = LL_TIM_OCIDLESTATE_HIGH;
  #else
    TIM_OC_InitStruct.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
    TIM_OC_InitStruct.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
  #endif
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
  #ifdef USE_OPEN_DRAIN_LOW
    #pragma message ("using open drain low side")
    #define LOW_OUTPUT_TYPE LL_GPIO_OUTPUT_OPENDRAIN
  #else
    #define LOW_OUTPUT_TYPE LL_GPIO_OUTPUT_PUSHPULL
  #endif
  #ifdef USE_OPEN_DRAIN_HIGH
    #pragma message ("using open drain high side")
    #define HIGH_OUTPUT_TYPE LL_GPIO_OUTPUT_OPENDRAIN
  #else
    #define HIGH_OUTPUT_TYPE LL_GPIO_OUTPUT_PUSHPULL
  #endif
  GPIO_InitStruct.Pin = PHASE_A_GPIO_LOW;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LOW_OUTPUT_TYPE;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(PHASE_A_GPIO_PORT_LOW, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PHASE_B_GPIO_LOW;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LOW_OUTPUT_TYPE;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(PHASE_B_GPIO_PORT_LOW, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PHASE_C_GPIO_LOW;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LOW_OUTPUT_TYPE;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(PHASE_C_GPIO_PORT_LOW, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PHASE_A_GPIO_HIGH;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = HIGH_OUTPUT_TYPE;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(PHASE_A_GPIO_PORT_HIGH, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PHASE_B_GPIO_HIGH;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = HIGH_OUTPUT_TYPE;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(PHASE_B_GPIO_PORT_HIGH, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PHASE_C_GPIO_HIGH;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = HIGH_OUTPUT_TYPE;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(PHASE_C_GPIO_PORT_HIGH, &GPIO_InitStruct);

  NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 2);
  NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);

}


void MX_TIM2_Init(void)
{
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
  TIM2->PSC = 23;
  TIM2->ARR = 0xFFFF;
}


void MX_TIM6_Init(void)
{

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);

  NVIC_SetPriority(TIM6_DAC_IRQn, 2);
  NVIC_EnableIRQ(TIM6_DAC_IRQn);
TIM6->PSC = 47;
TIM6->ARR = 100;

}


void MX_TIM14_Init(void)
{
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM14);
TIM14->PSC = 23;
TIM14->ARR = 4000;
  NVIC_SetPriority(TIM14_IRQn, 0);
  NVIC_EnableIRQ(TIM14_IRQn);
  LL_TIM_EnableARRPreload(TIM14);

}


void MX_TIM16_Init(void)
{
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM16);
 NVIC_SetPriority(TIM16_IRQn, 2);
  NVIC_EnableIRQ(TIM16_IRQn);
TIM16->PSC = 0;
TIM16->ARR = 9000;
  LL_TIM_DisableARRPreload(TIM16);
}


void MX_TIM17_Init(void)
{
   LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM17);
TIM17->PSC = 47;
TIM17->ARR = 0XFFFF;
  LL_TIM_DisableARRPreload(TIM17);
}


void MX_DMA_Init(void)
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
 /* DMA interrupt init */
  /* DMA1_Channel2_3_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1);
  NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
  /* DMA1_Channel4_5_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel4_5_IRQn, 1);
  NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);

}

void MX_GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_15);

  /**/
  #ifdef USE_ALKAS_DEBUG_LED
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  #endif
}


void UN_TIM_Init(void)
{
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

#ifdef USE_TIMER_15_CHANNEL_1
  NVIC_SetPriority(TIM15_IRQn, 0);
  NVIC_EnableIRQ(TIM15_IRQn);
#endif
#ifdef USE_TIMER_3_CHANNEL_1
   NVIC_SetPriority(IC_DMA_IRQ_NAME, 1);
   NVIC_EnableIRQ(IC_DMA_IRQ_NAME);
#endif

IC_TIMER_REGISTER->PSC = 0;
IC_TIMER_REGISTER->ARR = 63;

}

#ifdef USE_RGB_LED              // has 3 color led
void LED_GPIO_init(){
	  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */
	  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8);
      LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
	  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);

	  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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

#endif

#ifdef USE_CRSF_INPUT
void crsf_UART_Init(void)
{

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**USART1 GPIO Configuration

  PB6   ------> USART1_TX
  */

  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USART1 DMA Init */

  /* USART1_TX Init */
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PRIORITY_LOW);
  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MODE_CIRCULAR);
  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PERIPH_NOINCREMENT);
  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MEMORY_INCREMENT);
  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PDATAALIGN_BYTE);
  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MDATAALIGN_BYTE);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, 2);
  NVIC_EnableIRQ(USART1_IRQn);

  USART_InitStruct.BaudRate = 420000;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_DisableIT_CTS(USART1);
  USART1->CR3 |= (1 << 3);  // half duplex
  LL_USART_Enable(USART1);

// set dma address
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_3,
                         LL_USART_DMA_GetRegAddr(USART1, LL_USART_DMA_REG_DATA_RECEIVE),
                         (uint32_t)&crsf_buffer,
                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3));
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, 64);
  USART1->CR1 |= (1<<4); // idle line interrupt
}

void receiveCRSF(){
	 LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, 64);
	 LL_USART_EnableDMAReq_RX(USART1);
	 LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
}

#endif
