#pragma once

#include <stdint.h>

#include "macros.h"

#define LED1 BIT(2)
#define LED2 BIT(3)

void LED_Init(uint32_t leds);
void LED_On(uint32_t leds);
void LED_Off(uint32_t leds);
void LED_Toggle(uint32_t leds);
void LED_Write(uint32_t off, uint32_t on);