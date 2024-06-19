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

#define DELAYVAL 9

void Delay(uint32_t delay_ammount) {
    volatile uint32_t counter;

    for(int i = 0; i < delay_ammount; i++) {
        counter = 100000;
        while(counter) {
            counter--;
        }
    }
}

int main(void) {
    LED_Init(LED1 | LED2);

    while(1) {
        Delay(DELAYVAL);
        LED_Toggle(LED1 | LED2);
    }
}