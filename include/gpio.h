#pragma once

#include "em_device.h"

typedef GPIO_P_TypeDef *GPIO_t;

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

static const GPIO_t const GPIOA = &(GPIO->P[0]);
static const GPIO_t const GPIOB = &(GPIO->P[1]);
static const GPIO_t const GPIOC = &(GPIO->P[2]);
static const GPIO_t const GPIOD = &(GPIO->P[3]);
static const GPIO_t const GPIOE = &(GPIO->P[4]);
static const GPIO_t const GPIOF = &(GPIO->P[5]);

void GPIO_ConfigurePins(GPIO_t gpio, uint32_t pins, uint32_t mode);

void GPIO_Init(GPIO_t gpio, uint32_t inputs, uint32_t outputs);

inline static void GPIO_WritePins(GPIO_t gpio, uint32_t zeroes, uint32_t ones) {
    gpio->DOUTCLR = zeroes;
    gpio->DOUTSET = ones;
}

inline static void GPIO_TogglePins(GPIO_t gpio, uint32_t pins) {
    gpio->DOUTTGL = pins;
}

inline static uint32_t GPIO_ReadPins(GPIO_t gpio) { return gpio->DIN; }