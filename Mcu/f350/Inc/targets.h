#ifndef USE_MAKE
#define GD32TEST
#endif


#define CPU_FREQUENCY_MHZ   64
#define EEPROM_START_ADD  (uint32_t)0x0800F800
#define INTERVAL_TIMER     TIM2
#define TEN_KHZ_TIMER      TIM6
#define UTILITY_TIMER      TIM17
#define COM_TIMER          TIM3
#define TIM1_AUTORELOAD    2999
#define APPLICATION_ADDRESS 0x08001000
#define MAIN_COMP	COMP1
#define EXTI_LINE   LL_EXTI_LINE_21
//#define USE_SERIAL_TELEMETRY
//#define USE_ADC
#define MCU_GD32
#define TARGET_VOLTAGE_DIVIDER  110
#define SINE_DIVIDER 3




#ifdef GD32TEST

#define    DEAD_TIME 45
#define    FIRMWARE_NAME  "FD6288_PA2  "
#define    USE_TIMER_15_CHANNEL_1
#define    INPUT_PIN               LL_GPIO_PIN_2
#define    INPUT_PIN_PORT              GPIOA
#define    IC_TIMER_CHANNEL         LL_TIM_CHANNEL_CH1
#define    IC_TIMER_REGISTER          TIM15
#define    IC_TIMER_POINTER           htim15

#define    INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_5
#define    DMA_HANDLE_TYPE_DEF     hdma_tim15_ch1
#define    IC_DMA_IRQ_NAME         DMA1_Channel4_5_IRQn

#define PHASE_A_GPIO_LOW          LL_GPIO_PIN_1
#define PHASE_A_GPIO_PORT_LOW         GPIOB
#define PHASE_A_GPIO_HIGH          LL_GPIO_PIN_10
#define PHASE_A_GPIO_PORT_HIGH         GPIOA

#define PHASE_B_GPIO_LOW          LL_GPIO_PIN_0
#define PHASE_B_GPIO_PORT_LOW         GPIOB
#define PHASE_B_GPIO_HIGH          LL_GPIO_PIN_9
#define PHASE_B_GPIO_PORT_HIGH         GPIOA

#define PHASE_C_GPIO_LOW          LL_GPIO_PIN_7
#define PHASE_C_GPIO_PORT_LOW         GPIOA
#define PHASE_C_GPIO_HIGH          LL_GPIO_PIN_8
#define PHASE_C_GPIO_PORT_HIGH         GPIOA


#define PHASE_A_COMP COMP_PA5
#define PHASE_B_COMP COMP_PA4
#define PHASE_C_COMP COMP_PA0

#endif


