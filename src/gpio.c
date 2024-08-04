#include "../include/gpio.h"

#include <stdint.h>

void GPIO_ConfigurePins(GPIO_t gpio, uint32_t pins, uint32_t mode) {
    const uint32_t mask = 0xF;
    uint32_t mbit = 0x0001;

    for (int i = 0; i < 32; i += 4) {
        if ((pins & mbit) != 0) {
            gpio->MODEL &= ~(mask << i);
            gpio->MODEL |= mode << i;
        }
        mbit <<= 1;
    }

    for (int i = 0; i < 32; i++) {
        if ((pins & mbit) != 0) {
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