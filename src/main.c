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
#include "../include/i2cmaster.h"
#include "../include/lcd.h"
#include "../include/led.h"
#include "../include/utils.h"
#include "em_device.h"

#define DELAYVAL 100
#define MPU_ADDR 0x68
#define MPU_WHO_AM_I 0x75

int main(void) {
    int a = 2;

    return 0;
}