/*
 * ADC.c
 *
 *  Created on: May 20, 2020
 *      Author: Alka
 */
#include "ADC.h"
#include "targets.h"


#ifdef USE_ADC_INPUT
uint16_t ADCDataDMA[4];
#else
uint16_t ADCDataDMA[3];
#endif


extern uint16_t ADC_raw_temp;
extern uint16_t ADC_raw_volts;
extern uint16_t ADC_raw_current;
extern uint16_t ADC_raw_input;

#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES  (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 64)
//
//char marker = 0;
//
//
void ADC_DMA_Callback(){  // read dma buffer and set extern variables

#ifdef USE_ADC_INPUT
	ADC_raw_temp =    ADCDataDMA[3];
	ADC_raw_volts  = ADCDataDMA[1]/2;
	ADC_raw_current =ADCDataDMA[2];
	ADC_raw_input = ADCDataDMA[0];


#else
ADC_raw_temp =    ADCDataDMA[2];
ADC_raw_volts  =  ADCDataDMA[1];
ADC_raw_current = ADCDataDMA[0];
#endif
}

void ADC_Init(void)
{       // gpio A3 mapped to channel 8, A6 to 11, temp on channel 17.
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_ANALOG);

//  NVIC_SetPriority(ADC1_2_IRQn, 0); /* ADC IRQ greater priority than DMA IRQ */
//  NVIC_EnableIRQ(ADC1_2_IRQn);

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC);

  if(__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
  {
    LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_SYNC_PCLK_DIV1);
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_TEMPSENSOR);
  }

  if ((LL_ADC_IsEnabled(ADC1) == 0)               ||
      (LL_ADC_REG_IsConversionOngoing(ADC1) == 0)   )
  {
    LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);

    // LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);

    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);

    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);

    LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN);

    LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_ENABLE_3RANKS);


    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_8);
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_11);
     LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_3, LL_ADC_CHANNEL_17);
  }

  if ((LL_ADC_IsEnabled(ADC1) == 0)                    ||
      ((LL_ADC_REG_IsConversionOngoing(ADC1) == 0) &&
       (LL_ADC_INJ_IsConversionOngoing(ADC1) == 0)   )   )
  {
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_8, LL_ADC_SAMPLINGTIME_47CYCLES_5);
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_11, LL_ADC_SAMPLINGTIME_47CYCLES_5);
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_17, LL_ADC_SAMPLINGTIME_47CYCLES_5);

  }

  LL_ADC_EnableIT_OVR(ADC1);


}


void activateADC(void)
{
  __IO uint32_t wait_loop_index = 0U;

  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    LL_ADC_DisableDeepPowerDown(ADC1);
    LL_ADC_EnableInternalRegulator(ADC1);

    wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }
      LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);
      while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0)
      {

      }
      wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1);
      while(wait_loop_index != 0)
      {
        wait_loop_index--;
      }
    LL_ADC_Enable(ADC1);
    while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0)
    {

    }

  }

}

void enableADC_DMA(void)
{

  NVIC_SetPriority(DMA1_Channel1_IRQn, 1); /* DMA IRQ lower priority than ADC IRQ */
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  LL_DMA_ConfigTransfer(DMA1,
                        LL_DMA_CHANNEL_1,
                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
                        LL_DMA_MODE_CIRCULAR              |
                        LL_DMA_PERIPH_NOINCREMENT         |
                        LL_DMA_MEMORY_INCREMENT           |
                        LL_DMA_PDATAALIGN_HALFWORD        |
                        LL_DMA_MDATAALIGN_HALFWORD        |
                        LL_DMA_PRIORITY_HIGH               );


  LL_DMA_SetPeriphRequest(DMA1,
                          LL_DMA_CHANNEL_1,
                          LL_DMA_REQUEST_0);


  LL_DMA_ConfigAddresses(DMA1,
                         LL_DMA_CHANNEL_1,
                         LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
                         (uint32_t)&ADCDataDMA,
                         LL_DMA_DIRECTION_PERIPH_TO_MEMORY);


  LL_DMA_SetDataLength(DMA1,
                       LL_DMA_CHANNEL_1,
                       3);

  LL_DMA_EnableIT_TC(DMA1,
                     LL_DMA_CHANNEL_1);

  LL_DMA_EnableIT_HT(DMA1,
                     LL_DMA_CHANNEL_1);


  LL_DMA_EnableIT_TE(DMA1,
                     LL_DMA_CHANNEL_1);

  //## Activation of DMA #####################################################
  LL_DMA_EnableChannel(DMA1,
                       LL_DMA_CHANNEL_1);
}
