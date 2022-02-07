/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "targets.h"
#include "ADC.h"
#include "common.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern void sendDshotDma(void);
extern void receiveDshotDma(void);
extern void transfercomplete(void);
extern void interruptRoutine();
extern void PeriodElapsedCallback();
extern void tenKhzRoutine();
extern char servoPwm;

//extern uint32_t gcr[];
//extern uint8_t gcr_size;

int update_interupt = 0;
uint8_t update_count = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel 2 and 3 interrupts.
  */
void DMA1_Channel2_3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 0 */
	  /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */
		if(LL_DMA_IsActiveFlag_HT3(DMA1)){
			if(servoPwm){
			LL_TIM_IC_SetPolarity(IC_TIMER_REGISTER, IC_TIMER_CHANNEL, LL_TIM_IC_POLARITY_FALLING);
			 LL_DMA_ClearFlag_HT3(DMA1);
			}
		}
		  if(LL_DMA_IsActiveFlag_TC3(DMA1) == 1)
		  {
		    LL_DMA_ClearFlag_GI3(DMA1);

		    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);


		    transfercomplete();

		  }
		  else if(LL_DMA_IsActiveFlag_TE3(DMA1) == 1)
		  {
			  LL_DMA_ClearFlag_GI3(DMA1);
		  }
  /* USER CODE END DMA1_Channel2_3_IRQn 0 */
		  if(LL_DMA_IsActiveFlag_TC2(DMA1) == 1)
		  {
		    /* Clear flag DMA global interrupt */
		    /* (global interrupt flag: half transfer and transfer complete flags) */
		    LL_DMA_ClearFlag_GI2(DMA1);
		    ADC_DMA_Callback();
		    /* Call interruption treatment function */
		 //   AdcDmaTransferComplete_Callback();
		  }

		  /* Check whether DMA transfer error caused the DMA interruption */
		  if(LL_DMA_IsActiveFlag_TE2(DMA1) == 1)
		  {
		    /* Clear flag DMA transfer error */
		    LL_DMA_ClearFlag_TE2(DMA1);

		    /* Call interruption treatment function */
		  }
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 1 */

  /* USER CODE END DMA1_Channel2_3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 4 and 5 interrupts.
  */
void DMA1_Channel4_5_IRQHandler(void)
{
#ifdef     USE_TIMER_2_CHANNEL_4
	if(LL_DMA_IsActiveFlag_HT4(DMA1)){
		if(servoPwm){
		LL_TIM_IC_SetPolarity(IC_TIMER_REGISTER, IC_TIMER_CHANNEL, LL_TIM_IC_POLARITY_FALLING);
		 LL_DMA_ClearFlag_HT4(DMA1);
		}
	}
	  if(LL_DMA_IsActiveFlag_TC4(DMA1) == 1)
	  {
	    LL_DMA_ClearFlag_GI4(DMA1);

	    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);


	    transfercomplete();

	  }
	  else if(LL_DMA_IsActiveFlag_TE4(DMA1) == 1)
	  {
		  LL_DMA_ClearFlag_GI4(DMA1);
	  }
#endif
#ifdef     USE_TIMER_2_CHANNEL_1
		if(LL_DMA_IsActiveFlag_HT5(DMA1)){
			if(servoPwm){
			LL_TIM_IC_SetPolarity(IC_TIMER_REGISTER, IC_TIMER_CHANNEL, LL_TIM_IC_POLARITY_FALLING);
			 LL_DMA_ClearFlag_HT5(DMA1);
			}
		}
		  if(LL_DMA_IsActiveFlag_TC5(DMA1) == 1)
		  {
		    LL_DMA_ClearFlag_GI5(DMA1);

		    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);


		    transfercomplete();

		  }
		  else if(LL_DMA_IsActiveFlag_TE5(DMA1) == 1)
		  {
			  LL_DMA_ClearFlag_GI5(DMA1);
		  }
#endif
  /* USER CODE BEGIN DMA1_Channel4_5_IRQn 1 */

  /* USER CODE END DMA1_Channel4_5_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{

	  if(LL_TIM_IsActiveFlag_CC4(TIM2) == 1)
	  {

	    LL_TIM_ClearFlag_CC4(TIM2);
	  }

	  if(LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1)
	  {
		  LL_TIM_ClearFlag_UPDATE(TIM2);
		  update_interupt++;

	  }

	  if(LL_TIM_IsActiveFlag_CC1(TIM2) == 1)
	  {

	    LL_TIM_ClearFlag_CC1(TIM2);
	  }


}

/**
  * @brief This function handles TIM14 global interrupt.
  */
void TIM14_IRQHandler(void)
{
  /* USER CODE BEGIN TIM14_IRQn 0 */
	  if(LL_TIM_IsActiveFlag_UPDATE(TIM14) == 1)
	  {
		PeriodElapsedCallback();
	    LL_TIM_ClearFlag_UPDATE(TIM14);
	  }
  /* USER CODE END TIM14_IRQn 0 */
  /* USER CODE BEGIN TIM14_IRQn 1 */

  /* USER CODE END TIM14_IRQn 1 */
}

/**
  * @brief This function handles TIM16 global interrupt.
  */
void TIM16_IRQHandler(void)
{

#ifdef USE_TIM_16
	  if(LL_TIM_IsActiveFlag_CC1(TIM16) == 1)
	  {

	    LL_TIM_ClearFlag_CC1(TIM16);
	  }

	  if(LL_TIM_IsActiveFlag_UPDATE(TIM16) == 1)
	  {
		  LL_TIM_ClearFlag_UPDATE(TIM16);
		  update_interupt++;

	  }
#else
	  if(LL_TIM_IsActiveFlag_UPDATE(TIM16) == 1)
	 	  {

	 	    LL_TIM_ClearFlag_UPDATE(TIM16);
	 	    tenKhzRoutine();

	 	  }
#endif


}

//void EXTI0_1_IRQHandler(void){
// if (LL_EXTI_IsActiveFlag_0_31(PHASE_B_LL_EXTI_LINE) != RESET){
//    LL_EXTI_ClearFlag_0_31(PHASE_B_LL_EXTI_LINE);
//
//    interruptRoutine();
//
//  }
//  if (LL_EXTI_IsActiveFlag_0_31(PHASE_A_LL_EXTI_LINE) != RESET)
//  {
//    LL_EXTI_ClearFlag_0_31(PHASE_A_LL_EXTI_LINE);
//
//    interruptRoutine();
//
//  }
//}

void EXTI4_15_IRQHandler(void){
  if (LL_EXTI_IsActiveFlag_0_31(PHASE_C_LL_EXTI_LINE) != RESET) {
    LL_EXTI_ClearFlag_0_31(PHASE_C_LL_EXTI_LINE);

    interruptRoutine();
  }

}


//#ifdef BLUE_BOARD
void EXTI0_1_IRQHandler(void)
{
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
    interruptRoutine();
  }
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
    interruptRoutine();
  }
}

void EXTI2_3_IRQHandler(void)
{
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
    interruptRoutine();
  }
}


//#endif
void DMA1_Channel1_IRQHandler(void)
{
		if(LL_DMA_IsActiveFlag_HT1(DMA1)){
			if(servoPwm){
			LL_TIM_IC_SetPolarity(IC_TIMER_REGISTER, IC_TIMER_CHANNEL, LL_TIM_IC_POLARITY_FALLING);
			 LL_DMA_ClearFlag_HT1(DMA1);
			}
		}
		  if(LL_DMA_IsActiveFlag_TC1(DMA1) == 1)
		  {
		    LL_DMA_ClearFlag_GI1(DMA1);

		    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);


		    transfercomplete();

		  }
		  else if(LL_DMA_IsActiveFlag_TE1(DMA1) == 1)
		  {
			  LL_DMA_ClearFlag_GI1(DMA1);
		  }

}


/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
