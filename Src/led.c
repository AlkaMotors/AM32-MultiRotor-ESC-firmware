/*
 * led.c
 *
 *  Created on: July 9, 2023
 *      Author: frank26080115
 */

#include "led.h"

#include "peripherals.h"
#include "targets.h"

#ifdef USE_LED_STRIP
#include "WS2812.h"
#endif

static inline void led_init_gpio(GPIO_TypeDef *GPIOx, uint32_t Pin, bool opendrain)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    #if defined(GPIOA)
    if (GPIOx == GPIOA) {
        #ifdef LL_AHB1_GRP1_PERIPH_GPIOA
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
        #endif
        #ifdef LL_IOP_GRP1_PERIPH_GPIOA
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
        #endif
    }
    #endif
    #if defined(GPIOB)
    if (GPIOx == GPIOB) {
        #ifdef LL_AHB1_GRP1_PERIPH_GPIOB
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
        #endif
        #ifdef LL_IOP_GRP1_PERIPH_GPIOB
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
        #endif
    }
    #endif
    #if defined(GPIOC)
    if (GPIOx == GPIOC) {
        #ifdef LL_AHB1_GRP1_PERIPH_GPIOC
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
        #endif
        #ifdef LL_IOP_GRP1_PERIPH_GPIOC
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
        #endif
    }
    #endif
    #if defined(GPIOD)
    if (GPIOx == GPIOD) {
        #ifdef LL_AHB1_GRP1_PERIPH_GPIOD
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
        #endif
        #ifdef LL_IOP_GRP1_PERIPH_GPIOD
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOD);
        #endif
    }
    #endif
    #if defined(GPIOE)
    if (GPIOx == GPIOE) {
        #ifdef LL_AHB1_GRP1_PERIPH_GPIOE
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
        #endif
        #ifdef LL_IOP_GRP1_PERIPH_GPIOE
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOE);
        #endif
    }
    #endif
    #if defined(GPIOF)
    if (GPIOx == GPIOF) {
        #ifdef LL_AHB1_GRP1_PERIPH_GPIOF
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
        #endif
        #ifdef LL_IOP_GRP1_PERIPH_GPIOF
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
        #endif
    }
    #endif

    // TODO: add more clock initialization when required

    GPIO_InitStruct.Pin        = Pin;
    GPIO_InitStruct.Mode       = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = opendrain ? LL_GPIO_OUTPUT_OPENDRAIN : LL_GPIO_OUTPUT_PUSHPULL; // TODO: is there any benefit to not using push-pull all the time? switch back if issues arise
    GPIO_InitStruct.Pull       = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOx, &GPIO_InitStruct);
    if (opendrain) {
        LL_GPIO_SetOutputPin(GPIOx, Pin);
    }
    else {
        LL_GPIO_ResetOutputPin(GPIOx, Pin);
    }
}

static inline void led_set(GPIO_TypeDef *GPIOx, uint32_t Pin, bool ison, bool opendrain)
{
    if ((ison && opendrain) || (!ison && !opendrain)) {
        LL_GPIO_ResetOutputPin(GPIOx, Pin);
    }
    else {
        LL_GPIO_SetOutputPin(GPIOx, Pin);
    }
}

#include "blink_patterns.h"

uint16_t blink_idx;                       // index of the current frame in the pattern
uint8_t* blink_primary_pattern   = NULL;  // primary pattern is used to indicate current status, and loops forever
uint8_t* blink_secondary_pattern = NULL;  // secondary pattern is used to indicate events, and will end, switching back to primary pattern
uint32_t blink_tmr = 0;                   // forever running timer
uint8_t  blink_span = 0;                  // the time span of the current time frame
uint32_t blink_last_time = 0;             // the last time that the frame advanced

void led_init()
{
#ifdef USE_LED_STRIP
    WS2812_Init();
    send_LED_RGB(255, 0, 0);
#endif

#ifdef USE_RGB_LED
    led_init_gpio(RGBLED_RED_GPIOx  , RGBLED_RED_PIN  , LED_IS_OPENDRAIN);
    led_init_gpio(RGBLED_GREEN_GPIOx, RGBLED_GREEN_PIN, LED_IS_OPENDRAIN);
    led_init_gpio(RGBLED_BLUE_GPIOx , RGBLED_BLUE_PIN , LED_IS_OPENDRAIN);
#elif defined(LED_GPIOx) && defined(LED_PIN)
    led_init_gpio(LED_GPIOx, LED_PIN, LED_IS_OPENDRAIN);
#endif

    led_set_unarmed();
}

void led_blink_set(uint8_t x)
{
    #ifdef USE_RGB_LED
    led_set(RGBLED_RED_GPIOx  , RGBLED_RED_PIN  , BLINK_IS_RED(x)  , LED_IS_OPENDRAIN);
    led_set(RGBLED_GREEN_GPIOx, RGBLED_GREEN_PIN, BLINK_IS_GREEN(x), LED_IS_OPENDRAIN);
    led_set(RGBLED_BLUE_GPIOx , RGBLED_BLUE_PIN , BLINK_IS_BLUE(x) , LED_IS_OPENDRAIN);
    #elif defined(LED_GPIOx) && defined(LED_PIN)
    led_set(LED_GPIOx, LED_PIN, BLINK_IS_ON(x), LED_IS_OPENDRAIN);
    #endif
    #ifdef USE_LED_STRIP
    send_LED_RGB(BLINK_IS_RED(x) ? 255 : 0, BLINK_IS_GREEN(x) ? 255 : 0, BLINK_IS_BLUE(x) ? 255 : 0);
    #endif
    blink_span = BLINK_GET_TIME(x);
}

void led_blink_100ms(void) // call this every 100ms
{
    blink_tmr++; // this will increment forever, 32 bits means 4971 days of operation without overflow

    uint8_t* active_pattern = (blink_secondary_pattern != NULL) ? blink_secondary_pattern : blink_primary_pattern;
    if (active_pattern == NULL) {
        return;
    }

    if ((blink_tmr - blink_last_time) >= blink_span) // time span has passed
    {
        blink_last_time = blink_tmr;
        blink_idx++; // advance to the next time slice in the pattern
        uint8_t x = active_pattern[blink_idx];
        if (x == BLINK_STOP)
        {
            // loop around
            blink_idx = 0;
            if (blink_secondary_pattern != NULL) // we are in the secondary pattern, so stop the pattern and switch back to primary pattern
            {
                blink_secondary_pattern = NULL;
                active_pattern = blink_primary_pattern;
            }
            x = active_pattern[blink_idx];
        }
        led_blink_set(x);
    }
}

void led_set_pattern_primary(const uint8_t* pattern)
{
    if (pattern != blink_primary_pattern) // prevent restarting the same pattern
    {
        blink_primary_pattern = (uint8_t*)pattern;

        // only start pattern if not interrupting the secondary pattern
        if (blink_secondary_pattern == NULL)
        {
            blink_idx  = 0;
            blink_span = 0;
            led_blink_set(pattern[blink_idx]);
        }
    }
}

void led_set_pattern_secondary(const uint8_t* pattern)
{
    if (pattern != blink_secondary_pattern) // prevent restarting the same pattern
    {
        blink_secondary_pattern = (uint8_t*)pattern;
        // start the pattern
        blink_idx  = 0;
        blink_span = 0;
        led_blink_set(pattern[blink_idx]);
    }
}

void led_set_unarmed()
{
    led_set_pattern_primary(blinkpattern_unarmed);
}

void led_set_armed()
{
    led_set_pattern_primary(blinkpattern_armed);
}

void led_set_armstate(bool x)
{
    led_set_pattern_primary(x ? blinkpattern_armed : blinkpattern_unarmed);
}

void led_set_running()
{
    led_set_pattern_primary(blinkpattern_running);
}

void led_sig_currentlimited()
{
    led_set_pattern_secondary(blinkpattern_currentlimited);
}

void led_sig_stalled()
{
    led_set_pattern_secondary(blinkpattern_stalled);
}

void led_sig_lowbatt()
{
    led_set_pattern_secondary(blinkpattern_lowbatt);
}

void led_sig_desync()
{
    led_set_pattern_secondary(blinkpattern_desync);
}

void led_set_red()
{
    static const uint8_t pattern[] = {
        BLINK_RGB_TIME(1, 0, 0, 1),
        BLINK_LOOP,
    };
    led_set_pattern_secondary(pattern);
}

void led_set_green()
{
    static const uint8_t pattern[] = {
        BLINK_RGB_TIME(0, 1, 0, 1),
        BLINK_LOOP,
    };
    led_set_pattern_secondary(pattern);
}

void led_set_blue()
{
    static const uint8_t pattern[] = {
        BLINK_RGB_TIME(0, 0, 1, 1),
        BLINK_LOOP,
    };
    led_set_pattern_secondary(pattern);
}
