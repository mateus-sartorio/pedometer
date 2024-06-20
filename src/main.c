/**
 * @file    main.c
 * @brief   Pedometro with EFM32GG_STK3700
 * @version 1.0
 *
 * @note    Just blinks the LEDs of the STK3700
 *
 * @note    LEDs are on pins 2 and 3 of GPIO Port E
 *
 * @note    It uses a primitive delay mechanism. Do not use it.
 *
 * @author  Mateus Sartorio
 * @date    01/09/2018
 */
#include <stdint.h>

#include "../include/led.h"
#include "em_device.h"

/**
 * @brief  SysTick is called every 1 ms
 */
#define DIVIDER 1000

#define DELAYVAL 1000

volatile uint32_t tick_counter = 0;

void SysTick_Handler(void) { tick_counter++; }

void Delay(uint32_t delay_ammount) {
    volatile uint32_t initial_value = tick_counter;
    while((tick_counter - initial_value) < delay_ammount);
}

void DelayPulses(uint32_t delay) {
    SysTick->LOAD = delay;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;

    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
    SysTick->CTRL = 0;
}

int main(void) {
    LED_Init(LED1 | LED2);

    SysTick_Config(SystemCoreClock / DIVIDER);

    __enable_irq();

    while(1) {
        Delay(DELAYVAL);
        LED_Toggle(LED1 | LED2);
    }
}