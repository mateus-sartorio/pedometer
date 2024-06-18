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

#include "em_device.h"

#define true 1
#define false 0

#define BIT(N) (1U << (N))

#define LED1 BIT(2)
#define LED2 BIT(3)

#define DELAYVAL 9

void delay(uint32_t delay_ammount) {
    volatile uint32_t counter;

    for(int i = 0; i < delay_ammount; i++) {
        counter = 100000;
        while(counter) {
            counter--;
        }
    }
}

int main(void) {
    GPIO_P_TypeDef* const GPIOE = &(GPIO->P[4]);

    CMU->HFPERCLKDIV |= CMU_HFPERCLKDIV_HFPERCLKEN;
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;

    GPIOE->MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK | _GPIO_P_MODEL_MODE3_MASK);
    GPIOE->MODEL |= (GPIO_P_MODEL_MODE2_PUSHPULL | GPIO_P_MODEL_MODE3_PUSHPULL);

    GPIOE->DOUT &= ~(LED1 | LED2);

    while(true) {
        GPIOE->DOUT |= LED1 | LED2;
        delay(DELAYVAL);

        GPIOE->DOUT &= ~(LED1 | LED2);
        delay(DELAYVAL);
    }
}
