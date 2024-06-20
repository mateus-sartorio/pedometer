#include "../include/button.h"

#include "../include/gpio.h"

static uint32_t last_read = 0;
static uint32_t input_pins = 0;

void Button_Init(uint32_t buttons) {
    if(buttons & BUTTON1) {
        GPIOB->MODEH &= ~_GPIO_P_MODEL_MODE1_MASK;
        GPIOB->MODEH |= GPIO_P_MODEL_MODE1_INPUT;
        input_pins |= BUTTON1;
    }

    if(buttons & BUTTON2) {
        GPIOB->MODEH &= ~_GPIO_P_MODEL_MODE2_MASK;
        GPIOB->MODEH |= GPIO_P_MODEL_MODE2_INPUT;
        input_pins |= BUTTON2;
    }

    last_read = GPIOB->DIN;
}

uint32_t Button_Read(void) {
    last_read = GPIOB->DIN;
    return last_read & input_pins;
}

uint32_t Button_ReadChanges(void) {
    uint32_t new_read;
    uint32_t changes;

    new_read = GPIOB->DIN;
    changes = new_read ^ last_read;
    last_read = new_read;

    return changes & input_pins;
}

uint32_t Button_ReadPressed(void) {
    uint32_t new_read;
    uint32_t changes;

    new_read = GPIOB->DIN;
    changes = ~new_read & last_read;
    last_read = new_read;

    return changes & input_pins;
}

uint32_t Button_ReadReleased(void) {
    uint32_t new_read;
    uint32_t changes;

    new_read = GPIOB->DIN;
    changes = new_read & ~last_read;
    last_read = new_read;

    return changes & input_pins;
}