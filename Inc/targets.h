#ifndef USE_MAKE
//#define FD6288
#define IFLIGHT
//#define MP6531
//#define TMOTOR55     // like iflight but with leds
//#define TMOTOR45
//#define HGLRC
//#define SISKIN
//#define MAMBA_F50PRO
#endif

//GLOBAL
//#define USE_ADC_INPUT

#ifdef FD6288

#define FIRMWARE_NAME           "FD6288_PA2  "
#define DEAD_TIME               45
#define USE_TIMER_15_CHANNEL_1
#define INPUT_PIN               LL_GPIO_PIN_2
#define INPUT_PIN_PORT          GPIOA
#define IC_TIMER_CHANNEL        LL_TIM_CHANNEL_CH1
#define IC_TIMER_REGISTER       TIM15
#define IC_TIMER_POINTER        htim15
#define INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_5
#define DMA_HANDLE_TYPE_DEF     hdma_tim15_ch1
#define IC_DMA_IRQ_NAME         DMA1_Channel4_5_IRQn

#define PHASE_A_GPIO_LOW        LL_GPIO_PIN_1
#define PHASE_A_GPIO_PORT_LOW   GPIOB
#define PHASE_A_GPIO_HIGH       LL_GPIO_PIN_10
#define PHASE_A_GPIO_PORT_HIGH  GPIOA

#define PHASE_B_GPIO_LOW        LL_GPIO_PIN_0
#define PHASE_B_GPIO_PORT_LOW   GPIOB
#define PHASE_B_GPIO_HIGH       LL_GPIO_PIN_9
#define PHASE_B_GPIO_PORT_HIGH  GPIOA

#define PHASE_C_GPIO_LOW        LL_GPIO_PIN_7
#define PHASE_C_GPIO_PORT_LOW   GPIOA
#define PHASE_C_GPIO_HIGH       LL_GPIO_PIN_8
#define PHASE_C_GPIO_PORT_HIGH  GPIOA

#define PHASE_A_COMP COMP_PA5
#define PHASE_B_COMP COMP_PA4
#define PHASE_C_COMP COMP_PA0

#endif


#ifdef IFLIGHT

#define FIRMWARE_NAME           "IFlight_50A "
#define DEAD_TIME               45
#define USE_TIMER_3_CHANNEL_1
#define INPUT_PIN               LL_GPIO_PIN_4
#define INPUT_PIN_PORT          GPIOB
#define IC_TIMER_CHANNEL        LL_TIM_CHANNEL_CH1
#define IC_TIMER_REGISTER       TIM3
#define IC_TIMER_POINTER        htim3
#define INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_4
#define DMA_HANDLE_TYPE_DEF     hdma_tim3_ch1
#define IC_DMA_IRQ_NAME         DMA1_Channel4_5_IRQn

#define PHASE_A_GPIO_LOW        LL_GPIO_PIN_1
#define PHASE_A_GPIO_PORT_LOW   GPIOB
#define PHASE_A_GPIO_HIGH       LL_GPIO_PIN_10
#define PHASE_A_GPIO_PORT_HIGH  GPIOA

#define PHASE_B_GPIO_LOW        LL_GPIO_PIN_0
#define PHASE_B_GPIO_PORT_LOW   GPIOB
#define PHASE_B_GPIO_HIGH       LL_GPIO_PIN_9
#define PHASE_B_GPIO_PORT_HIGH  GPIOA

#define PHASE_C_GPIO_LOW        LL_GPIO_PIN_7
#define PHASE_C_GPIO_PORT_LOW   GPIOA
#define PHASE_C_GPIO_HIGH       LL_GPIO_PIN_8
#define PHASE_C_GPIO_PORT_HIGH  GPIOA

#define PHASE_A_COMP COMP_PA0
#define PHASE_B_COMP COMP_PA4
#define PHASE_C_COMP COMP_PA5

#endif


#ifdef  MP6531

#define FIRMWARE_NAME           "Mp6531_Dev  "
#define DEAD_TIME               45
#define USE_TIMER_15_CHANNEL_1
#define INPUT_PIN               LL_GPIO_PIN_2
#define INPUT_PIN_PORT          GPIOA
#define IC_TIMER_CHANNEL        LL_TIM_CHANNEL_CH1
#define IC_TIMER_REGISTER       TIM15
#define IC_TIMER_POINTER        htim15
#define INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_5
#define DMA_HANDLE_TYPE_DEF     hdma_tim15_ch1
#define IC_DMA_IRQ_NAME         DMA1_Channel4_5_IRQn

#define PHASE_A_GPIO_LOW        LL_GPIO_PIN_0
#define PHASE_A_GPIO_PORT_LOW   GPIOB
#define PHASE_A_GPIO_HIGH       LL_GPIO_PIN_9
#define PHASE_A_GPIO_PORT_HIGH  GPIOA

#define PHASE_B_GPIO_LOW        LL_GPIO_PIN_7
#define PHASE_B_GPIO_PORT_LOW   GPIOA
#define PHASE_B_GPIO_HIGH       LL_GPIO_PIN_8
#define PHASE_B_GPIO_PORT_HIGH  GPIOA

#define PHASE_C_GPIO_LOW        LL_GPIO_PIN_1
#define PHASE_C_GPIO_PORT_LOW   GPIOB
#define PHASE_C_GPIO_HIGH       LL_GPIO_PIN_10
#define PHASE_C_GPIO_PORT_HIGH  GPIOA

#define PHASE_A_COMP COMP_PA4
#define PHASE_B_COMP COMP_PA5
#define PHASE_C_COMP COMP_PA0

#endif


#ifdef TMOTOR55

#define FIRMWARE_NAME           "T-MOTOR 55A "
#define DEAD_TIME               45
#define USE_TIMER_3_CHANNEL_1
#define INPUT_PIN               LL_GPIO_PIN_4
#define INPUT_PIN_PORT          GPIOB
#define IC_TIMER_CHANNEL        LL_TIM_CHANNEL_CH1
#define IC_TIMER_REGISTER       TIM3
#define IC_TIMER_POINTER        htim3
#define INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_4
#define DMA_HANDLE_TYPE_DEF     hdma_tim3_ch1
#define IC_DMA_IRQ_NAME         DMA1_Channel4_5_IRQn

#define PHASE_A_GPIO_LOW        LL_GPIO_PIN_1
#define PHASE_A_GPIO_PORT_LOW   GPIOB
#define PHASE_A_GPIO_HIGH       LL_GPIO_PIN_10
#define PHASE_A_GPIO_PORT_HIGH  GPIOA

#define PHASE_B_GPIO_LOW        LL_GPIO_PIN_0
#define PHASE_B_GPIO_PORT_LOW   GPIOB
#define PHASE_B_GPIO_HIGH       LL_GPIO_PIN_9
#define PHASE_B_GPIO_PORT_HIGH  GPIOA

#define PHASE_C_GPIO_LOW        LL_GPIO_PIN_7
#define PHASE_C_GPIO_PORT_LOW   GPIOA
#define PHASE_C_GPIO_HIGH       LL_GPIO_PIN_8
#define PHASE_C_GPIO_PORT_HIGH  GPIOA

#define PHASE_A_COMP COMP_PA0
#define PHASE_B_COMP COMP_PA4
#define PHASE_C_COMP COMP_PA5

#endif


#ifdef TMOTOR45

#define FIRMWARE_NAME           "T-MOTOR 45A "
#define DEAD_TIME               45
#define USE_TIMER_15_CHANNEL_1
#define INPUT_PIN               LL_GPIO_PIN_2
#define INPUT_PIN_PORT          GPIOA
#define IC_TIMER_CHANNEL        LL_TIM_CHANNEL_CH1
#define IC_TIMER_REGISTER       TIM15
#define IC_TIMER_POINTER        htim15
#define INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_5
#define DMA_HANDLE_TYPE_DEF     hdma_tim15_ch1
#define IC_DMA_IRQ_NAME         DMA1_Channel4_5_IRQn

#define PHASE_A_GPIO_LOW        LL_GPIO_PIN_1
#define PHASE_A_GPIO_PORT_LOW   GPIOB
#define PHASE_A_GPIO_HIGH       LL_GPIO_PIN_10
#define PHASE_A_GPIO_PORT_HIGH  GPIOA

#define PHASE_B_GPIO_LOW        LL_GPIO_PIN_0
#define PHASE_B_GPIO_PORT_LOW   GPIOB
#define PHASE_B_GPIO_HIGH       LL_GPIO_PIN_9
#define PHASE_B_GPIO_PORT_HIGH  GPIOA

#define PHASE_C_GPIO_LOW        LL_GPIO_PIN_7
#define PHASE_C_GPIO_PORT_LOW   GPIOA
#define PHASE_C_GPIO_HIGH       LL_GPIO_PIN_8
#define PHASE_C_GPIO_PORT_HIGH  GPIOA

#define PHASE_A_COMP COMP_PA0
#define PHASE_B_COMP COMP_PA4
#define PHASE_C_COMP COMP_PA5

#endif


#ifdef HGLRC

#define FIRMWARE_NAME           "HGLRC_60A   "
#define DEAD_TIME               45
#define USE_TIMER_15_CHANNEL_1
#define INPUT_PIN               LL_GPIO_PIN_2
#define INPUT_PIN_PORT          GPIOA
#define IC_TIMER_CHANNEL        LL_TIM_CHANNEL_CH1
#define IC_TIMER_REGISTER       TIM15
#define IC_TIMER_POINTER        htim15
#define INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_5
#define DMA_HANDLE_TYPE_DEF     hdma_tim15_ch1
#define IC_DMA_IRQ_NAME         DMA1_Channel4_5_IRQn

#define PHASE_A_GPIO_LOW        LL_GPIO_PIN_7
#define PHASE_A_GPIO_PORT_LOW   GPIOA
#define PHASE_A_GPIO_HIGH       LL_GPIO_PIN_8
#define PHASE_A_GPIO_PORT_HIGH  GPIOA

#define PHASE_B_GPIO_LOW        LL_GPIO_PIN_0
#define PHASE_B_GPIO_PORT_LOW   GPIOB
#define PHASE_B_GPIO_HIGH       LL_GPIO_PIN_9
#define PHASE_B_GPIO_PORT_HIGH  GPIOA

#define PHASE_C_GPIO_LOW        LL_GPIO_PIN_1
#define PHASE_C_GPIO_PORT_LOW   GPIOB
#define PHASE_C_GPIO_HIGH       LL_GPIO_PIN_10
#define PHASE_C_GPIO_PORT_HIGH  GPIOA

#define PHASE_A_COMP COMP_PA0
#define PHASE_B_COMP COMP_PA4
#define PHASE_C_COMP COMP_PA5

#endif


#ifdef SISKIN

#define FIRMWARE_NAME           "SISKIN_PA2  "
#define DEAD_TIME               45
#define USE_TIMER_15_CHANNEL_1
#define INPUT_PIN               LL_GPIO_PIN_2
#define INPUT_PIN_PORT          GPIOA
#define IC_TIMER_CHANNEL        LL_TIM_CHANNEL_CH1
#define IC_TIMER_REGISTER       TIM15
#define IC_TIMER_POINTER        htim15
#define INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_5
#define DMA_HANDLE_TYPE_DEF     hdma_tim15_ch1
#define IC_DMA_IRQ_NAME         DMA1_Channel4_5_IRQn

#define PHASE_A_GPIO_LOW        LL_GPIO_PIN_1
#define PHASE_A_GPIO_PORT_LOW   GPIOB
#define PHASE_A_GPIO_HIGH       LL_GPIO_PIN_10
#define PHASE_A_GPIO_PORT_HIGH  GPIOA

#define PHASE_B_GPIO_LOW        LL_GPIO_PIN_0
#define PHASE_B_GPIO_PORT_LOW   GPIOB
#define PHASE_B_GPIO_HIGH       LL_GPIO_PIN_9
#define PHASE_B_GPIO_PORT_HIGH  GPIOA

#define PHASE_C_GPIO_LOW        LL_GPIO_PIN_7
#define PHASE_C_GPIO_PORT_LOW   GPIOA
#define PHASE_C_GPIO_HIGH       LL_GPIO_PIN_8
#define PHASE_C_GPIO_PORT_HIGH  GPIOA

#define PHASE_A_COMP COMP_PA5
#define PHASE_B_COMP COMP_PA4
#define PHASE_C_COMP COMP_PA0

#endif

#ifdef DIATONE

#define FIRMWARE_NAME           "DIATONE gen "
#define DEAD_TIME               45
#define USE_TIMER_3_CHANNEL_1
#define INPUT_PIN               LL_GPIO_PIN_4
#define INPUT_PIN_PORT          GPIOB
#define IC_TIMER_CHANNEL        LL_TIM_CHANNEL_CH1
#define IC_TIMER_REGISTER       TIM3
#define IC_TIMER_POINTER        htim3
#define INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_4
#define DMA_HANDLE_TYPE_DEF     hdma_tim3_ch1
#define IC_DMA_IRQ_NAME         DMA1_Channel4_5_IRQn

#define PHASE_A_GPIO_LOW        LL_GPIO_PIN_1
#define PHASE_A_GPIO_PORT_LOW   GPIOB
#define PHASE_A_GPIO_HIGH       LL_GPIO_PIN_10
#define PHASE_A_GPIO_PORT_HIGH  GPIOA

#define PHASE_B_GPIO_LOW        LL_GPIO_PIN_0
#define PHASE_B_GPIO_PORT_LOW   GPIOB
#define PHASE_B_GPIO_HIGH       LL_GPIO_PIN_9
#define PHASE_B_GPIO_PORT_HIGH  GPIOA

#define PHASE_C_GPIO_LOW        LL_GPIO_PIN_7
#define PHASE_C_GPIO_PORT_LOW   GPIOA
#define PHASE_C_GPIO_HIGH       LL_GPIO_PIN_8
#define PHASE_C_GPIO_PORT_HIGH  GPIOA

#define PHASE_A_COMP COMP_PA5
#define PHASE_B_COMP COMP_PA0
#define PHASE_C_COMP COMP_PA4

#endif

#ifdef MAMBA_F40PRO

#define FIRMWARE_NAME           "MAMBA F40PRO"
#define DEAD_TIME               13
#define USE_TIMER_3_CHANNEL_1
#define INPUT_PIN               LL_GPIO_PIN_4
#define INPUT_PIN_PORT          GPIOB
#define IC_TIMER_CHANNEL        LL_TIM_CHANNEL_CH1
#define IC_TIMER_REGISTER       TIM3
#define IC_TIMER_POINTER        htim3
#define INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_4
#define DMA_HANDLE_TYPE_DEF     hdma_tim3_ch1
#define IC_DMA_IRQ_NAME         DMA1_Channel4_5_IRQn

#define PHASE_A_GPIO_LOW        LL_GPIO_PIN_1
#define PHASE_A_GPIO_PORT_LOW   GPIOB
#define PHASE_A_GPIO_HIGH       LL_GPIO_PIN_10
#define PHASE_A_GPIO_PORT_HIGH  GPIOA

#define PHASE_B_GPIO_LOW        LL_GPIO_PIN_0
#define PHASE_B_GPIO_PORT_LOW   GPIOB
#define PHASE_B_GPIO_HIGH       LL_GPIO_PIN_9
#define PHASE_B_GPIO_PORT_HIGH  GPIOA

#define PHASE_C_GPIO_LOW        LL_GPIO_PIN_7
#define PHASE_C_GPIO_PORT_LOW   GPIOA
#define PHASE_C_GPIO_HIGH       LL_GPIO_PIN_8
#define PHASE_C_GPIO_PORT_HIGH  GPIOA

#define PHASE_A_COMP COMP_PA5
#define PHASE_B_COMP COMP_PA0
#define PHASE_C_COMP COMP_PA4

#endif

#ifdef MAMBA_F50PRO

#define FIRMWARE_NAME           "MAMBA F50PRO"
#define DEAD_TIME               14
#define USE_TIMER_3_CHANNEL_1
#define INPUT_PIN               LL_GPIO_PIN_4
#define INPUT_PIN_PORT          GPIOB
#define IC_TIMER_CHANNEL        LL_TIM_CHANNEL_CH1
#define IC_TIMER_REGISTER       TIM3
#define IC_TIMER_POINTER        htim3
#define INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_4
#define DMA_HANDLE_TYPE_DEF     hdma_tim3_ch1
#define IC_DMA_IRQ_NAME         DMA1_Channel4_5_IRQn

#define PHASE_A_GPIO_LOW        LL_GPIO_PIN_1
#define PHASE_A_GPIO_PORT_LOW   GPIOB
#define PHASE_A_GPIO_HIGH       LL_GPIO_PIN_10
#define PHASE_A_GPIO_PORT_HIGH  GPIOA

#define PHASE_B_GPIO_LOW        LL_GPIO_PIN_0
#define PHASE_B_GPIO_PORT_LOW   GPIOB
#define PHASE_B_GPIO_HIGH       LL_GPIO_PIN_9
#define PHASE_B_GPIO_PORT_HIGH  GPIOA

#define PHASE_C_GPIO_LOW        LL_GPIO_PIN_7
#define PHASE_C_GPIO_PORT_LOW   GPIOA
#define PHASE_C_GPIO_HIGH       LL_GPIO_PIN_8
#define PHASE_C_GPIO_PORT_HIGH  GPIOA

#define PHASE_A_COMP COMP_PA5
#define PHASE_B_COMP COMP_PA0
#define PHASE_C_COMP COMP_PA4

#endif

#ifdef MAMBA_F60PRO

#define FIRMWARE_NAME           "MAMBA F60PRO"
#define DEAD_TIME               14
#define USE_TIMER_3_CHANNEL_1
#define INPUT_PIN               LL_GPIO_PIN_4
#define INPUT_PIN_PORT          GPIOB
#define IC_TIMER_CHANNEL        LL_TIM_CHANNEL_CH1
#define IC_TIMER_REGISTER       TIM3
#define IC_TIMER_POINTER        htim3
#define INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_4
#define DMA_HANDLE_TYPE_DEF     hdma_tim3_ch1
#define IC_DMA_IRQ_NAME         DMA1_Channel4_5_IRQn

#define PHASE_A_GPIO_LOW        LL_GPIO_PIN_1
#define PHASE_A_GPIO_PORT_LOW   GPIOB
#define PHASE_A_GPIO_HIGH       LL_GPIO_PIN_10
#define PHASE_A_GPIO_PORT_HIGH  GPIOA

#define PHASE_B_GPIO_LOW        LL_GPIO_PIN_0
#define PHASE_B_GPIO_PORT_LOW   GPIOB
#define PHASE_B_GPIO_HIGH       LL_GPIO_PIN_9
#define PHASE_B_GPIO_PORT_HIGH  GPIOA

#define PHASE_C_GPIO_LOW        LL_GPIO_PIN_7
#define PHASE_C_GPIO_PORT_LOW   GPIOA
#define PHASE_C_GPIO_HIGH       LL_GPIO_PIN_8
#define PHASE_C_GPIO_PORT_HIGH  GPIOA

#define PHASE_A_COMP COMP_PA5
#define PHASE_B_COMP COMP_PA0
#define PHASE_C_COMP COMP_PA4

#endif
