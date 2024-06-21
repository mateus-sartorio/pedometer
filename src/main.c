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

#include "../include/button.h"
#include "../include/led.h"
#include "clock_efm32gg.h"
#include "em_device.h"

#define true 1
#define false 0

void button_callback(uint32_t mask) {
    if(Button_ReadReleased()) {
        if(BUTTON1 & mask) {
            LED_Toggle(LED1);
        } else if(BUTTON2 & mask) {
            LED_Toggle(LED2);
        }
    }
}

int main(void) {
    LED_Init(LED1 | LED2);

    Button_Init(BUTTON1 | BUTTON2);
    Button_SetCallback(button_callback);

    __enable_irq();

    LED_Write(0, LED1 | LED2);

    while(true) {
        __WFI();
    }
}