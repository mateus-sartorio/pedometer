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

typedef GPIO_P_TypeDef* GPIO_t;

/**
 * @brief   mode options
 */
//@{
static const uint32_t GPIO_MODE_DISABLE = 0x0;
static const uint32_t GPIO_MODE_INPUT = 0x1;
static const uint32_t GPIO_MODE_INPUTPULL = 0x2;
static const uint32_t GPIO_MODE_INPUTPULLFILTER = 0x3;
static const uint32_t GPIO_MODE_PUSHPULL = 0x4;
static const uint32_t GPIO_MODE_PUSHPULLDRIVE = 0x5;
static const uint32_t GPIO_MODE_WIREDOR = 0x6;
static const uint32_t GPIO_MODE_WIREDORPULLDOWN = 0x7;
static const uint32_t GPIO_MODE_WIREDAND = 0x8;
static const uint32_t GPIO_MODE_WIREDANDFILTER = 0x9;
static const uint32_t GPIO_MODE_WIREDANDPULLUP = 0xA;
static const uint32_t GPIO_MODE_WIREDANDPULLUPFILTER = 0xB;
static const uint32_t GPIO_MODE_WIREDANDDRIVE = 0xC;
static const uint32_t GPIO_MODE_WIREDANDDRIVEFILTER = 0xD;
static const uint32_t GPIO_MODE_WIREDANDDRIVEPULLUP = 0xE;
static const uint32_t GPIO_MODE_WIREDANDDRIVEPULLUPFILTER = 0xF;
//@}

void Delay(uint32_t delay_ammount) {
    volatile uint32_t counter;

    for(int i = 0; i < delay_ammount; i++) {
        counter = 100000;
        while(counter) {
            counter--;
        }
    }
}

void GPIO_ConfigurePins(GPIO_t gpio, uint32_t pins, uint32_t mode) {
    const uint32_t mask = 0xF;
    uint32_t mbit = 0x0001;

    for(int i = 0; i < 32; i += 4) {
        if((pins & mbit) != 0) {
            gpio->MODEL &= ~(mask << i);
            gpio->MODEL |= mode << i;
        }
        mbit <<= 1;
    }

    for(int i = 0; i < 32; i++) {
        if((pins & mbit) != 0) {
            gpio->MODEH &= ~(mask << i);
            gpio->MODEH |= mode << i;
        }
        mbit <<= 1;
    }
}

void GPIO_Init(GPIO_t gpio, uint32_t inputs, uint32_t outputs) {
    CMU->HFPERCLKDIV |= CMU_HFPERCLKDIV_HFPERCLKEN;
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;

    GPIO_ConfigurePins(gpio, outputs, GPIO_MODE_PUSHPULL);
    GPIO_ConfigurePins(gpio, inputs, GPIO_MODE_INPUT);
}

inline static void GPIO_WritePins(GPIO_t gpio, uint32_t zeroes, uint32_t ones) {
    gpio->DOUTCLR = zeroes;
    gpio->DOUTSET = ones;
}

inline static void GPIO_TogglePins(GPIO_t gpio, uint32_t pins) {
    gpio->DOUTTGL = pins;
}

inline static uint32_t GPIO_ReadPins(GPIO_t gpio) { return gpio->DIN; }

int main(void) {
    GPIO_P_TypeDef* const GPIOE = &(GPIO->P[4]);

    GPIO_Init(GPIOE, 0, LED1 | LED2);
    GPIO_WritePins(GPIOE, LED1 | LED2, 0);

    while(true) {
        Delay(DELAYVAL);
        GPIO_TogglePins(GPIOE, LED1);

        Delay(DELAYVAL);
        GPIO_TogglePins(GPIOE, LED2);

        Delay(DELAYVAL);
        GPIO_TogglePins(GPIOE, LED1 | LED2);
    }
}