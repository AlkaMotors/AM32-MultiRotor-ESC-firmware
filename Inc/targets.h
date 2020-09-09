//#define FD6288
//#define TTRC4IN1              // or iflight target
//#define MP6531
#define tmotor55     // like iflight but with leds
//#define tmotor45
//#define hglrc

//GLOBAL
//#define USE_ADC_INPUT




#ifdef FD6288

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

#endif


#ifdef TTRC4IN1

#define     USE_TIMER_3_CHANNEL_1

#define    INPUT_PIN               LL_GPIO_PIN_4
#define    INPUT_PIN_PORT              GPIOB
#define    IC_TIMER_CHANNEL         LL_TIM_CHANNEL_CH1
#define    IC_TIMER_REGISTER          TIM3
#define    IC_TIMER_POINTER           htim3

#define    INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_4
#define    DMA_HANDLE_TYPE_DEF     hdma_tim3_ch1
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

#define use_A0_B4_C5_comp_order


#endif


#ifdef  MP6531


#define    USE_TIMER_15_CHANNEL_1
#define    INPUT_PIN               LL_GPIO_PIN_2
#define    INPUT_PIN_PORT              GPIOA
#define    IC_TIMER_CHANNEL         LL_TIM_CHANNEL_CH1
#define    IC_TIMER_REGISTER          TIM15
#define    IC_TIMER_POINTER           htim15

#define    INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_5
#define    DMA_HANDLE_TYPE_DEF     hdma_tim15_ch1
#define    IC_DMA_IRQ_NAME         DMA1_Channel4_5_IRQn


#define PHASE_A_GPIO_LOW          LL_GPIO_PIN_0
#define PHASE_A_GPIO_PORT_LOW         GPIOB
#define PHASE_A_GPIO_HIGH          LL_GPIO_PIN_9
#define PHASE_A_GPIO_PORT_HIGH         GPIOA


#define PHASE_B_GPIO_LOW          LL_GPIO_PIN_7
#define PHASE_B_GPIO_PORT_LOW         GPIOA
#define PHASE_B_GPIO_HIGH          LL_GPIO_PIN_8
#define PHASE_B_GPIO_PORT_HIGH         GPIOA


#define PHASE_C_GPIO_LOW          LL_GPIO_PIN_1
#define PHASE_C_GPIO_PORT_LOW         GPIOB
#define PHASE_C_GPIO_HIGH          LL_GPIO_PIN_10
#define PHASE_C_GPIO_PORT_HIGH         GPIOA




#endif


#ifdef tmotor55

#define     USE_TIMER_3_CHANNEL_1

#define    INPUT_PIN               LL_GPIO_PIN_4
#define    INPUT_PIN_PORT              GPIOB
#define    IC_TIMER_CHANNEL         LL_TIM_CHANNEL_CH1
#define    IC_TIMER_REGISTER          TIM3
#define    IC_TIMER_POINTER           htim3

#define    INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_4
#define    DMA_HANDLE_TYPE_DEF     hdma_tim3_ch1
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

#define use_A0_B4_C5_comp_order
#endif

#ifdef tmotor45

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

#define use_A0_B4_C5_comp_order
#endif


#ifdef hglrc

#define    USE_TIMER_15_CHANNEL_1
#define    INPUT_PIN               LL_GPIO_PIN_2
#define    INPUT_PIN_PORT              GPIOA
#define    IC_TIMER_CHANNEL         LL_TIM_CHANNEL_CH1
#define    IC_TIMER_REGISTER          TIM15
#define    IC_TIMER_POINTER           htim15

#define    INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_5
#define    DMA_HANDLE_TYPE_DEF     hdma_tim15_ch1
#define    IC_DMA_IRQ_NAME         DMA1_Channel4_5_IRQn


#define PHASE_A_GPIO_LOW          LL_GPIO_PIN_7
#define PHASE_A_GPIO_PORT_LOW         GPIOA
#define PHASE_A_GPIO_HIGH          LL_GPIO_PIN_8
#define PHASE_A_GPIO_PORT_HIGH         GPIOA



#define PHASE_B_GPIO_LOW          LL_GPIO_PIN_0
#define PHASE_B_GPIO_PORT_LOW         GPIOB
#define PHASE_B_GPIO_HIGH          LL_GPIO_PIN_9
#define PHASE_B_GPIO_PORT_HIGH         GPIOA



#define PHASE_C_GPIO_LOW          LL_GPIO_PIN_1
#define PHASE_C_GPIO_PORT_LOW         GPIOB
#define PHASE_C_GPIO_HIGH          LL_GPIO_PIN_10
#define PHASE_C_GPIO_PORT_HIGH         GPIOA

#define use_A0_B4_C5_comp_order
#endif

















