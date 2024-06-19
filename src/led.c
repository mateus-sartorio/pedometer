#include "../include/led.h"

#include "../include/gpio.h"

void LED_Init(uint32_t leds) {
    GPIO_Init(GPIOE, 0, leds);
    GPIO_WritePins(GPIOE, leds, 0);
}

void LED_On(uint32_t leds) { GPIO_WritePins(GPIOE, 0, leds); }

void LED_Off(uint32_t leds) { GPIO_WritePins(GPIOE, leds, 0); }

void LED_Toggle(uint32_t leds) { GPIO_TogglePins(GPIOE, leds); }

void LED_Write(uint32_t off, uint32_t on) { GPIO_WritePins(GPIOE, off, on); }