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
#include "../include/led.h"
#include "../include/uart.h"
#include "clock_efm32gg_ext.h"
#include "em_device.h"

#define true 1
#define false 0

const int TickDivisor = 1000;

void SysTick_Handler(void) {
    static int counter = 0;

    if(counter == 0) {
        counter = TickDivisor;
        LED_Toggle(LED1);
    }

    counter--;
}

int main(void) {
    ClockConfiguration_t clock_config;

    LED_Init(LED1 | LED2);

    (void)SystemCoreClockSet(CLOCK_HFXO, 1, 1);
    ClockGetConfiguration(&clock_config);

    LED_Write(0, LED1 | LED2);

    SysTick_Config(SystemCoreClock / TickDivisor);

    UART_Init();

    UART_SendString("\r\n\n\nSPACE toggle change case\n\r");

    int cntchar = 0;
    unsigned ch = '*';
    int changecase = 0;

    while(true) {
        // if((ch = UART_GetCharNoWait()) != 0) {
        //     LED_Toggle(LED2);
        // } else {
        //     continue;
        // }

        // if(ch == ' ') {
        //     changecase = !changecase;
        // }

        // if(changecase) {
        //     if(isupper(ch)) {
        //         ch = tolower(ch);
        //     } else if(islower(ch)) {
        //         ch = toupper(ch);
        //     }
        // }

        // if((cntchar++ % 80) == 0) {
        //     UART_SendChar('\n');
        //     UART_SendChar('\r');
        // }

        UART_SendChar(ch);
    }
}