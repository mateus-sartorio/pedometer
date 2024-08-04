#include "../include/button.h"

#include "../include/gpio.h"

#define BUTTON_INT_LEVEL 3

static uint32_t last_read = 0;
static uint32_t newest_read = 0;
static uint32_t input_pins = 0;
static void (*callback)(uint32_t param) = NULL;

void GPIO_EVEN_IRQHandler(void) {
    uint32_t new_read;

    const uint32_t mask = BUTTON2;

    if (GPIO->IF & mask) {
        last_read = (last_read & ~mask) | (newest_read & mask);
        new_read = GPIOB->DIN & mask;
        newest_read = (newest_read & ~mask) | new_read;
    }

    GPIO->IFC = 0x5555;

    if (callback) {
        callback(mask);
    }
}

void GPIO_ODD_IRQHandler(void) {
    uint32_t new_read;

    const uint32_t mask = BUTTON1;

    if (GPIO->IF & mask) {
        last_read = (last_read & ~mask) | (newest_read & mask);
        new_read = GPIOB->DIN & mask;
        newest_read = (newest_read & ~mask) | new_read;
    }

    GPIO->IFC = 0xAAAA;

    if (callback) {
        callback(mask);
    }
}

void Button_Init(uint32_t buttons) {
    if (buttons & BUTTON1) {
        GPIOB->MODEH &= ~_GPIO_P_MODEL_MODE1_MASK;
        GPIOB->MODEH |= GPIO_P_MODEL_MODE1_INPUT;
        input_pins |= BUTTON1;

        GPIO->EXTIPSELH =
            (GPIO->EXTIPSELH & ~(_GPIO_EXTIPSELH_EXTIPSEL9_MASK)) |
            GPIO_EXTIPSELH_EXTIPSEL9_PORTB;
        GPIO->EXTIRISE |= BUTTON1;
        GPIO->EXTIFALL |= BUTTON1;
        GPIO->IEN |= BUTTON1;
    }

    if (buttons & BUTTON2) {
        GPIOB->MODEH &= ~_GPIO_P_MODEL_MODE2_MASK;
        GPIOB->MODEH |= GPIO_P_MODEL_MODE2_INPUT;
        input_pins |= BUTTON2;

        GPIO->EXTIPSELH =
            (GPIO->EXTIPSELH & ~(_GPIO_EXTIPSELH_EXTIPSEL10_MASK)) |
            GPIO_EXTIPSELH_EXTIPSEL10_PORTB;
        GPIO->EXTIRISE |= BUTTON2;
        GPIO->EXTIFALL |= BUTTON2;
        GPIO->IEN |= BUTTON2;
    }

    last_read = GPIOB->DIN & input_pins;

    GPIO->IFC = 0xFFFF;

    NVIC_SetPriority(GPIO_EVEN_IRQn, BUTTON_INT_LEVEL);
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);

    NVIC_SetPriority(GPIO_ODD_IRQn, BUTTON_INT_LEVEL);
    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

uint32_t Button_Read(void) { return newest_read & input_pins; }

uint32_t Button_ReadChanges(void) {
    uint32_t changes = newest_read ^ last_read;
    last_read = newest_read;

    return changes & input_pins;
}

uint32_t Button_ReadPressed(void) {
    uint32_t changes = ~newest_read & last_read;
    last_read = newest_read;

    return changes & input_pins;
}

uint32_t Button_ReadReleased(void) {
    uint32_t changes = newest_read & ~last_read;
    last_read = newest_read;

    return changes & input_pins;
}

void Button_SetCallback(void (*proc)(uint32_t param)) { callback = proc; }