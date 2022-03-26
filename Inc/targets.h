

#ifndef USE_MAKE

//#define FD6288
//#define IFLIGHT
//#define MP6531
//#define TMOTOR55     // like iflight but with leds
//#define TMOTOR45
//#define HGLRC
//#define SISKIN
#define MAMBA_F60PRO
//#define WRAITH32
//#define AIKON20X20
//#define AIKONSINGLE
//#define FLYCOLOR
//#define HKMICRO
//#define AM32REF
//#define BLPWR
//#define HVFLYCOLOR

//#define G072ESC
//#define G071ENABLE
//#define G071_OPEN_DRAIN
//#define G071_OPEN_DRAIN_B
//define  G071_64K
//define  DT120_G071
#endif

//GLOBAL
//#define USE_ADC_INPUT
//#define USE_ALKAS_DEBUG_LED


/****************************      F051 Targets ************************************/
#ifdef FD6288
#define FIRMWARE_NAME           "FD6288_PA2  "
#define DEAD_TIME               45
#define HARDWARE_GROUP_F0_A
#define USE_SERIAL_TELEMETRY
#endif

#ifdef IFLIGHT
#define FIRMWARE_NAME           "IFlight_50A "
#define DEAD_TIME               45
#define HARDWARE_GROUP_F0_B
#define USE_SERIAL_TELEMETRY
#endif

#ifdef MP6531
#define FIRMWARE_NAME           "Mp6531_Dev  "
#define DEAD_TIME               45
#define HARDWARE_GROUP_F0_C
#define USE_SERIAL_TELEMETRY
#endif

#ifdef TMOTOR55
#define FIRMWARE_NAME           "T-MOTOR 55A "
#define DEAD_TIME               45
#define HARDWARE_GROUP_F0_B
#define USE_SERIAL_TELEMETRY
#endif

#ifdef TMOTOR45
#define FIRMWARE_NAME           "T-MOTOR 45A "
#define DEAD_TIME               45
#define HARDWARE_GROUP_F0_C
#define USE_SERIAL_TELEMETRY
#endif

#ifdef HGLRC
#define FIRMWARE_NAME           "HGLRC_60A   "
#define DEAD_TIME               45
#define HARDWARE_GROUP_F0_A
#define USE_SERIAL_TELEMETRY
#endif

#ifdef SISKIN
#define FIRMWARE_NAME           "SISKIN_PA2  "
#define DEAD_TIME               45
#define HARDWARE_GROUP_F0_A
#define USE_SERIAL_TELEMETRY
#endif

#ifdef DIATONE
#define FIRMWARE_NAME           "DIATONE GEN "
#define DEAD_TIME               45
#define HARDWARE_GROUP_F0_F
#define USE_SERIAL_TELEMETRY
#endif

#ifdef MAMBA_F40PRO
#define FIRMWARE_NAME           "MAMBA F40PRO"
#define DEAD_TIME               13
#define HARDWARE_GROUP_F0_F
#define USE_SERIAL_TELEMETRY
#endif

#ifdef MAMBA_F50PRO
#define FIRMWARE_NAME           "MAMBA F50PRO"
#define DEAD_TIME               14
#define HARDWARE_GROUP_F0_F
#define USE_SERIAL_TELEMETRY
#endif

#ifdef MAMBA_F60PRO
#define FIRMWARE_NAME           "MAMBA F60PRO"
#define DEAD_TIME               20
#define HARDWARE_GROUP_F0_F
#define USE_SERIAL_TELEMETRY
#endif

#ifdef WRAITH32
#define FIRMWARE_NAME           "Wraith_32   "
#define DEAD_TIME               45
#define TARGET_VOLTAGE_DIVIDER  75
#define HARDWARE_GROUP_F0_C
#define USE_SERIAL_TELEMETRY
#endif

#ifdef  AIKON20X20
#define FIRMWARE_NAME           "Aikon 20x20 "
#define DEAD_TIME               40
#define HARDWARE_GROUP_F0_C
#define USE_SERIAL_TELEMETRY
#endif

#ifdef  AIKONSINGLE
#define FIRMWARE_NAME           "Aikon AK32  "
#define DEAD_TIME               40
#define HARDWARE_GROUP_F0_B
#define USE_SERIAL_TELEMETRY
#endif

#ifdef  CRTEENSY_HILARIESC
#define FIRMWARE_NAME           "cr hilariesc"
#define DEAD_TIME               40
#define HARDWARE_GROUP_F0_G
//#define USE_SERIAL_TELEMETRY
#endif

#ifdef FLYCOLOR
#define FIRMWARE_NAME           "Flycolor Gen"
#define DEAD_TIME               45
#define HARDWARE_GROUP_F0_A
//#define USE_SERIAL_TELEMETRY
#define PA6_VOLTAGE
#endif

#ifdef HVFLYCOLOR
#define FIRMWARE_NAME           "FLYCOLOR HV "
#define DEAD_TIME               70
#define HARDWARE_GROUP_F0_H
#define USE_SERIAL_TELEMETRY
#endif

#ifdef SKYSTARS
#define FIRMWARE_NAME           "SkyStars K55"
#define DEAD_TIME               40
#define HARDWARE_GROUP_F0_B
#define USE_SERIAL_TELEMETRY
#endif

#ifdef HKMICRO
#define FIRMWARE_NAME           "HK Micro    "
#define DEAD_TIME               45
#define HARDWARE_GROUP_F0_B
#define MILLIVOLT_PER_AMP       20
#define CURRENT_OFFSET          0
#define TARGET_VOLTAGE_DIVIDER  110
#define USE_SERIAL_TELEMETRY
#define USE_RGB_LED
#endif


#ifdef BLPWR
#define FIRMWARE_NAME           "BlPwr-ESC"
#define DEAD_TIME               20
#define HARDWARE_GROUP_F0_A
#define MILLIVOLT_PER_AMP       20
#define CURRENT_OFFSET          0
#define TARGET_VOLTAGE_DIVIDER  110
#define USE_SERIAL_TELEMETRY
#endif


#ifdef AM32REF
#define FIRMWARE_NAME           "AM32 Ref-ESC"
#define DEAD_TIME               45
#define HARDWARE_GROUP_F0_B
#define MILLIVOLT_PER_AMP       65
#define CURRENT_OFFSET          0
#define TARGET_VOLTAGE_DIVIDER  110
#define PA6_VOLTAGE
#define USE_SERIAL_TELEMETRY
#define TARGET_STALL_PROTECTION_INTERVAL 9000

#endif
/*******************************   G071 Targets *********************************/

#ifdef G071_64K
#define FIRMWARE_NAME  			    "G071 64kESC "
#define DEAD_TIME               60
#define MILLIVOLT_PER_AMP       67
#define CURRENT_OFFSET          0
#define HARDWARE_GROUP_G0_A
#define USE_SERIAL_TELEMETRY
#define SIXTY_FOUR_KB_MEMORY
#endif


#ifdef  G071ESC
#define FIRMWARE_NAME  			    "G071 DevESC "
#define DEAD_TIME               60
#define MILLIVOLT_PER_AMP       15
#define CURRENT_OFFSET          0
#define HARDWARE_GROUP_G0_A
#define USE_SERIAL_TELEMETRY
#endif


#ifdef  IFLIGHT_BLITZ_G
#define FIRMWARE_NAME  			    "Blitz 55 G0 "
#define DEAD_TIME               60
#define MILLIVOLT_PER_AMP       15
#define CURRENT_OFFSET          0
#define HARDWARE_GROUP_G0_F
#define USE_SERIAL_TELEMETRY
#define SIXTY_FOUR_KB_MEMORY
#endif


#ifdef  DT120_G071
#define FIRMWARE_NAME  			    "G071 120 DT "
#define DEAD_TIME               120
#define MILLIVOLT_PER_AMP       15
#define CURRENT_OFFSET          0
#define HARDWARE_GROUP_G0_A
#define USE_SERIAL_TELEMETRY
#endif

#ifdef  DT120_G071_64K
#define FIRMWARE_NAME  			    "G071 120 64K"
#define DEAD_TIME               120
#define MILLIVOLT_PER_AMP       67
#define CURRENT_OFFSET          0
#define HARDWARE_GROUP_G0_A
#define SIXTY_FOUR_KB_MEMORY
#endif

#ifdef  G071ENABLE
#define FIRMWARE_NAME           "G071 PWM_EN "
#define DEAD_TIME               60
#define MILLIVOLT_PER_AMP       20
#define CURRENT_OFFSET          0
#define HARDWARE_GROUP_G0_B
#define USE_SERIAL_TELEMETRY
#endif

#ifdef  G071_OPEN_DRAIN
#define FIRMWARE_NAME           "G071 TEENSY "
#define DEAD_TIME               60
#define MILLIVOLT_PER_AMP       20
#define CURRENT_OFFSET          0
#define HARDWARE_GROUP_G0_C
#define USE_SERIAL_TELEMETRY
#endif

#ifdef  G071_OPEN_DRAIN_B
#define FIRMWARE_NAME           "G071 TEENSYB"
#define DEAD_TIME               60
#define MILLIVOLT_PER_AMP       20
#define CURRENT_OFFSET          0
#define HARDWARE_GROUP_G0_D
#endif

/********************************** F031 Targets ***********************************/




/********************************** defaults if not set ***************************/

#ifndef 	TARGET_VOLTAGE_DIVIDER
#define 	TARGET_VOLTAGE_DIVIDER  	110
#endif

#ifndef 	SINE_DIVIDER
#define 	SINE_DIVIDER 				2
#endif

#ifndef  	MILLIVOLT_PER_AMP
#define     MILLIVOLT_PER_AMP           20
#endif 

#ifndef 	CURRENT_OFFSET
#define     CURRENT_OFFSET              0
#endif

#ifndef TARGET_STALL_PROTECTION_INTERVAL
#define TARGET_STALL_PROTECTION_INTERVAL 6500
#endif

/************************************ F051 Hardware Groups ****************************/

#ifdef HARDWARE_GROUP_F0_A

#define MCU_F051
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


#ifdef HARDWARE_GROUP_F0_B

#define MCU_F051
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


#ifdef  HARDWARE_GROUP_F0_C

#define MCU_F051
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



#ifdef HARDWARE_GROUP_F0_E

#define MCU_F051
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


#ifdef HARDWARE_GROUP_F0_F

#define MCU_F051
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


#ifdef HARDWARE_GROUP_F0_G

#define MCU_F051
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

#define PHASE_A_COMP COMP_PA4
#define PHASE_B_COMP COMP_PA0
#define PHASE_C_COMP COMP_PA5

#define USE_INVERTED_LOW
//#define USE_INVERTED_HIGH
#define USE_OPEN_DRAIN_LOW
#define USE_OPEN_DRAIN_HIGH

#endif

#ifdef HARDWARE_GROUP_F0_H

#define MCU_F051
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
#define PHASE_B_COMP COMP_PA4
#define PHASE_C_COMP COMP_PA0

#endif



/************************************* G071 Hardware Groups **********************************/



#ifdef     HARDWARE_GROUP_G0_A

#define    MCU_G071
#define    USE_TIMER_3_CHANNEL_1
#define    INPUT_PIN               LL_GPIO_PIN_4
#define    INPUT_PIN_PORT              GPIOB
#define    IC_TIMER_CHANNEL         LL_TIM_CHANNEL_CH1
#define    IC_TIMER_REGISTER          TIM3
#define    IC_TIMER_POINTER           htim3

#define    INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_1
#define    DMA_HANDLE_TYPE_DEF     hdma_tim3_ch1
#define    IC_DMA_IRQ_NAME         DMA1_Channel1_IRQn

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

#define PHASE_A_COMP  LL_COMP_INPUT_MINUS_IO2  // pb7
#define PHASE_B_COMP  LL_COMP_INPUT_MINUS_IO1  // pb3
#define PHASE_C_COMP  LL_COMP_INPUT_MINUS_IO3  // pa2

#endif


#ifdef     HARDWARE_GROUP_G0_B

#define    MCU_G071
#define    PWM_ENABLE_BRIDGE
#define    USE_TIMER_3_CHANNEL_1

#define    INPUT_PIN               LL_GPIO_PIN_4
#define    INPUT_PIN_PORT              GPIOB
#define    IC_TIMER_CHANNEL         LL_TIM_CHANNEL_CH1
#define    IC_TIMER_REGISTER          TIM3
#define    IC_TIMER_POINTER           htim3

#define    INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_1
#define    DMA_HANDLE_TYPE_DEF     hdma_tim3_ch1
#define    IC_DMA_IRQ_NAME         DMA1_Channel1_IRQn

#define PHASE_A_GPIO_PWM          LL_GPIO_PIN_10
#define PHASE_A_GPIO_PORT_PWM         GPIOA
#define PHASE_A_GPIO_ENABLE          LL_GPIO_PIN_1
#define PHASE_A_GPIO_PORT_ENABLE         GPIOB

#define PHASE_B_GPIO_PWM          LL_GPIO_PIN_9
#define PHASE_B_GPIO_PORT_PWM         GPIOA
#define PHASE_B_GPIO_ENABLE          LL_GPIO_PIN_0
#define PHASE_B_GPIO_PORT_ENABLE         GPIOB

#define PHASE_C_GPIO_PWM          LL_GPIO_PIN_8
#define PHASE_C_GPIO_PORT_PWM         GPIOA
#define PHASE_C_GPIO_ENABLE          LL_GPIO_PIN_7
#define PHASE_C_GPIO_PORT_ENABLE         GPIOA

#define PHASE_A_COMP  LL_COMP_INPUT_MINUS_IO2  // pb7
#define PHASE_B_COMP  LL_COMP_INPUT_MINUS_IO1  // pb3
#define PHASE_C_COMP  LL_COMP_INPUT_MINUS_IO3  // pa2

#endif


#ifdef     HARDWARE_GROUP_G0_C

#define    MCU_G071
#define    OPEN_DRAIN_PWM
#define    PWM_ENABLE_BRIDGE
#define    USE_TIMER_3_CHANNEL_1

#define    INPUT_PIN               LL_GPIO_PIN_4
#define    INPUT_PIN_PORT              GPIOB
#define    IC_TIMER_CHANNEL         LL_TIM_CHANNEL_CH1
#define    IC_TIMER_REGISTER          TIM3
#define    IC_TIMER_POINTER           htim3

#define    INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_1
#define    DMA_HANDLE_TYPE_DEF     hdma_tim3_ch1
#define    IC_DMA_IRQ_NAME         DMA1_Channel1_IRQn

#define PHASE_A_GPIO_PWM          LL_GPIO_PIN_10
#define PHASE_A_GPIO_PORT_PWM         GPIOA
#define PHASE_A_GPIO_ENABLE          LL_GPIO_PIN_1
#define PHASE_A_GPIO_PORT_ENABLE         GPIOB

#define PHASE_B_GPIO_PWM          LL_GPIO_PIN_9
#define PHASE_B_GPIO_PORT_PWM         GPIOA
#define PHASE_B_GPIO_ENABLE          LL_GPIO_PIN_0
#define PHASE_B_GPIO_PORT_ENABLE         GPIOB

#define PHASE_C_GPIO_PWM          LL_GPIO_PIN_8
#define PHASE_C_GPIO_PORT_PWM         GPIOA
#define PHASE_C_GPIO_ENABLE          LL_GPIO_PIN_7
#define PHASE_C_GPIO_PORT_ENABLE         GPIOA

#define PHASE_A_COMP  LL_COMP_INPUT_MINUS_IO2  // pb7
#define PHASE_B_COMP  LL_COMP_INPUT_MINUS_IO1  // pb3
#define PHASE_C_COMP  LL_COMP_INPUT_MINUS_IO3  // pa2

#endif


#ifdef     HARDWARE_GROUP_G0_D

#define    MCU_G071
#define    OPEN_DRAIN_PWM
#define    PWM_ENABLE_BRIDGE
#define    USE_TIMER_3_CHANNEL_1

#define    INPUT_PIN               LL_GPIO_PIN_4
#define    INPUT_PIN_PORT              GPIOB
#define    IC_TIMER_CHANNEL         LL_TIM_CHANNEL_CH1
#define    IC_TIMER_REGISTER          TIM3
#define    IC_TIMER_POINTER           htim3

#define    INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_1
#define    DMA_HANDLE_TYPE_DEF     hdma_tim3_ch1
#define    IC_DMA_IRQ_NAME         DMA1_Channel1_IRQn

#define PHASE_A_GPIO_PWM          LL_GPIO_PIN_10
#define PHASE_A_GPIO_PORT_PWM         GPIOA
#define PHASE_A_GPIO_ENABLE          LL_GPIO_PIN_15
#define PHASE_A_GPIO_PORT_ENABLE         GPIOA

#define PHASE_B_GPIO_PWM          LL_GPIO_PIN_9
#define PHASE_B_GPIO_PORT_PWM         GPIOA
#define PHASE_B_GPIO_ENABLE          LL_GPIO_PIN_6
#define PHASE_B_GPIO_PORT_ENABLE         GPIOC

#define PHASE_C_GPIO_PWM          LL_GPIO_PIN_8
#define PHASE_C_GPIO_PORT_PWM         GPIOA
#define PHASE_C_GPIO_ENABLE          LL_GPIO_PIN_7
#define PHASE_C_GPIO_PORT_ENABLE         GPIOA

#define PHASE_A_COMP  LL_COMP_INPUT_MINUS_IO2  // pb7
#define PHASE_B_COMP  LL_COMP_INPUT_MINUS_IO1  // pb3
#define PHASE_C_COMP  LL_COMP_INPUT_MINUS_IO3  // pa2
#endif

#ifdef     HARDWARE_GROUP_G0_F

#define    MCU_G071
#define    USE_TIMER_3_CHANNEL_1
#define    INPUT_PIN               LL_GPIO_PIN_4
#define    INPUT_PIN_PORT              GPIOB
#define    IC_TIMER_CHANNEL         LL_TIM_CHANNEL_CH1
#define    IC_TIMER_REGISTER          TIM3
#define    IC_TIMER_POINTER           htim3

#define    INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_1
#define    DMA_HANDLE_TYPE_DEF     hdma_tim3_ch1
#define    IC_DMA_IRQ_NAME         DMA1_Channel1_IRQn

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

#define PHASE_A_COMP  LL_COMP_INPUT_MINUS_IO1  // pb3
#define PHASE_B_COMP  LL_COMP_INPUT_MINUS_IO2  // pb7
#define PHASE_C_COMP  LL_COMP_INPUT_MINUS_IO3  // pa2

#endif

/************************************ G031 Hardware Groups ************************************************/



#ifdef    HARDWARE_GROUP_F031_B

#define    USE_TIMER_2_CHANNEL_3
#define    INPUT_PIN               LL_GPIO_PIN_2
#define    IC_TIMER_CHANNEL         LL_TIM_CHANNEL_CH3
#define    IC_TIMER_REGISTER          TIM2
#define    IC_TIMER_POINTER           htim2

#define    INPUT_DMA_CHANNEL       LL_DMA_CHANNEL_1
#define    DMA_HANDLE_TYPE_DEF     hdma_tim2_ch3
#define    IC_DMA_IRQ_NAME         DMA1_Channel1_IRQn


#define    PHASE_A_EXTI_PORT          GPIOF
#define    PHASE_B_EXTI_PORT          GPIOF
#define    PHASE_C_EXTI_PORT          GPIOA

#define    PHASE_A_EXTI_PIN           LL_GPIO_PIN_0
#define    PHASE_B_EXTI_PIN           LL_GPIO_PIN_1
#define    PHASE_C_EXTI_PIN           LL_GPIO_PIN_5

#define    PHASE_A_EXTI_LINE          0
#define    PHASE_B_EXTI_LINE          1
#define    PHASE_C_EXTI_LINE          5

#define    SYSCFG_EXTI_PORTA         LL_SYSCFG_EXTI_PORTF
#define    SYSCFG_EXTI_PORTB         LL_SYSCFG_EXTI_PORTF
#define    SYSCFG_EXTI_PORTC         LL_SYSCFG_EXTI_PORTA

#define    SYSCFG_EXTI_LINEA         LL_SYSCFG_EXTI_LINE0
#define    SYSCFG_EXTI_LINEB         LL_SYSCFG_EXTI_LINE1
#define    SYSCFG_EXTI_LINEC         LL_SYSCFG_EXTI_LINE5

#define    PHASE_A_LL_EXTI_LINE      LL_EXTI_LINE_0
#define    PHASE_B_LL_EXTI_LINE      LL_EXTI_LINE_1
#define    PHASE_C_LL_EXTI_LINE      LL_EXTI_LINE_5

#define    EXTI_IRQ1_NAME          EXTI0_1_IRQn
#define    EXTI_IRQ2_NAME          EXTI4_15_IRQn

#define PHASE_A_GPIO_LOW          LL_GPIO_PIN_14
#define PHASE_A_GPIO_PORT_LOW         GPIOB
#define PHASE_A_GPIO_HIGH          LL_GPIO_PIN_9
#define PHASE_A_GPIO_PORT_HIGH         GPIOA

#define PHASE_B_GPIO_LOW          LL_GPIO_PIN_13
#define PHASE_B_GPIO_PORT_LOW         GPIOB
#define PHASE_B_GPIO_HIGH          LL_GPIO_PIN_8
#define PHASE_B_GPIO_PORT_HIGH         GPIOA

#define PHASE_C_GPIO_LOW          LL_GPIO_PIN_15
#define PHASE_C_GPIO_PORT_LOW         GPIOB
#define PHASE_C_GPIO_HIGH          LL_GPIO_PIN_10
#define PHASE_C_GPIO_PORT_HIGH         GPIOA

#define CURRENT_SENSE_ADC_PIN        LL_GPIO_PIN_3
#define VOLTAGE_SENSE_ADC_PIN        LL_GPIO_PIN_4

#define CURRENT_ADC_CHANNEL         LL_ADC_CHANNEL_3
#define VOLTAGE_ADC_CHANNEL         LL_ADC_CHANNEL_4

#endif
/************************************ MCU COMMON PERIPHERALS **********************************************/

#ifdef MCU_F051
#define CPU_FREQUENCY_MHZ   48
#define EEPROM_START_ADD  (uint32_t)0x08007C00
#define INTERVAL_TIMER     TIM2
#define TEN_KHZ_TIMER      TIM6
#define UTILITY_TIMER      TIM17
#define COM_TIMER          TIM14
#define TIM1_AUTORELOAD    1999
#define APPLICATION_ADDRESS 0x08001000
#define MAIN_COMP	COMP1
#define EXTI_LINE   LL_EXTI_LINE_21
#define TARGET_MIN_BEMF_COUNTS 6
//#define USE_SERIAL_TELEMETRY // moved to individual ESCs
#define USE_ADC
#endif

#ifdef MCU_F031
#define CPU_FREQUENCY_MHZ   48
#define EEPROM_START_ADD  (uint32_t)0x08007C00
#define INTERVAL_TIMER     TIM3
#define TEN_KHZ_TIMER      TIM16
#define UTILITY_TIMER      TIM17
#define COM_TIMER          TIM14
#define TIM1_AUTORELOAD    1999
#define APPLICATION_ADDRESS 0x08001000
#define TARGET_MIN_BEMF_COUNTS 3
//#define USE_SERIAL_TELEMETRY // moved to individual ESCs
#define USE_ADC
#endif

#ifdef MCU_G071
#define CPU_FREQUENCY_MHZ   64
#ifdef  SIXTY_FOUR_KB_MEMORY
#define EEPROM_START_ADD  (uint32_t)0x0800F800
#else
#define EEPROM_START_ADD  (uint32_t)0x0801F800
#endif
#define INTERVAL_TIMER     TIM2
#define TEN_KHZ_TIMER      TIM6
#define UTILITY_TIMER      TIM17
#define COM_TIMER          TIM14
#define TIM1_AUTORELOAD    2667
#define APPLICATION_ADDRESS 0x08001000
#define MAIN_COMP	COMP2
#define EXTI_LINE   LL_EXTI_LINE_18
#define TARGET_MIN_BEMF_COUNTS 4
#define USE_ADC
#endif












