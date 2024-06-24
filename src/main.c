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
#include <ctype.h>
#include <stdint.h>

#include "../include/button.h"
#include "../include/lcd.h"
#include "../include/led.h"
#include "../include/uart.h"
#include "clock_efm32gg_ext.h"
#include "em_device.h"

#define DELAYVAL 100

void Delay(uint32_t delay) {
    volatile uint32_t counter;
    int i;

    for(i = 0; i < delay; i++) {
        counter = 10000;
        while(counter) counter--;
    }
}

void WriteMultiple(uint8_t ch) {
    int pos;

    for(pos = 1; pos <= 11; pos++) LCD_WriteChar(ch, pos);
}

int main(void) {
    static char* string =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<>:,.;"
        "abcdefghijklmnopqrstuvwzyz!@#$%*()[]";

    static char* numstring = "0123456789";

    char* s = string;
    char* n = numstring;

    /* Configure Pins in GPIOE */
    LED_Init(LED1 | LED2);

    /* Configure LCD */
    LCD_Init();

    LCD_SetAll();
    Delay(DELAYVAL);

    LCD_ClearAll();
    Delay(DELAYVAL);

    /* Blink loop */
    while(1) {
        Delay(DELAYVAL);
        LED_Toggle(LED1);  // Toggle LED1
        LCD_WriteAlphanumericDisplay(s++);
        if(*s == '\0')
            s = string;
        LCD_WriteNumericDisplay(n++);
        if(*n == '\0')
            n = numstring;

        Delay(DELAYVAL);
        LED_Toggle(LED2);  // Toggle LED2

        Delay(DELAYVAL);
        LED_Write(0, LED1 | LED2);  // Turn On All LEDs
    }
}