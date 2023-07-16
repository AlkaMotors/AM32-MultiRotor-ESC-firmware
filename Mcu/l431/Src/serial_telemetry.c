/*
 * serial_telemetry.c
 *
 *  Created on: May 13, 2020
 *      Author: Alka
 */


#include "serial_telemetry.h"


uint8_t aTxBuffer[10];
uint8_t nbDataToTransmit = sizeof(aTxBuffer);



void telem_UART_Init()
{


  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK2);

  /* Peripheral clock enable */
  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//  NVIC_SetPriority(USART1_IRQn, 3);
//  NVIC_EnableIRQ(USART1_IRQn);


  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_4, LL_DMA_REQUEST_2);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_4, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MDATAALIGN_BYTE);


  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigHalfDuplexMode(USART1);


  LL_USART_Enable(USART1);
  while((!(LL_USART_IsActiveFlag_TEACK(USART1))) || (!(LL_USART_IsActiveFlag_REACK(USART1))))
  {
  }

  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_4,
                         (uint32_t)aTxBuffer,
                         LL_USART_DMA_GetRegAddr(USART1, LL_USART_DMA_REG_DATA_TRANSMIT),
                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_4));
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, nbDataToTransmit);

  /* (5) Enable DMA transfer complete/error interrupts  */
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_4);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_4);






}

void send_telem_DMA(){   // set data length and enable channel to start transfer
	  LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_TX);
	//  GPIOB->OTYPER &= 0 << 6;
	  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, nbDataToTransmit);
	  LL_USART_EnableDMAReq_TX(USART1);

	  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
	  LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_RX);
}



uint8_t update_crc8(uint8_t crc, uint8_t crc_seed){
uint8_t crc_u, i;
crc_u = crc;
crc_u ^= crc_seed;
for ( i=0; i<8; i++) crc_u = ( crc_u & 0x80 ) ? 0x7 ^ ( crc_u << 1 ) : ( crc_u << 1 );
return (crc_u);
}

uint8_t get_crc8(uint8_t *Buf, uint8_t BufLen){
uint8_t crc = 0, i;
for( i=0; i<BufLen; i++) crc = update_crc8(Buf[i], crc);
return (crc);
}


void makeTelemPackage(uint8_t temp, uint16_t voltage, uint16_t current, uint16_t consumption, uint16_t e_rpm){

  aTxBuffer[0] = temp; // temperature

  aTxBuffer[1] = (voltage >> 8) & 0xFF; // voltage hB
  aTxBuffer[2] = voltage & 0xFF; // voltage   lowB

  aTxBuffer[3] = (current >> 8) & 0xFF; // current
  aTxBuffer[4] = current & 0xFF; // divide by 10 for Amps

  aTxBuffer[5] = (consumption >> 8) & 0xFF; // consumption
  aTxBuffer[6] = consumption & 0xFF; //  in mah

  aTxBuffer[7] = (e_rpm >> 8) & 0xFF; //
  aTxBuffer[8] = e_rpm & 0xFF; // eRpM *100

  aTxBuffer[9] = get_crc8(aTxBuffer,9);
}

