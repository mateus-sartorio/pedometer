/** ***************************************************************************
 * @file    main.c
 * @brief   Simple UART Demo for EFM32GG_STK3700
 * @version 1.0
 ******************************************************************************/

#include <stdint.h>
/*
 * Including this file, it is possible to define which processor using command line
 * E.g. -DEFM32GG995F1024
 * The alternative is to include the processor specific file directly
 * #include "efm32gg995f1024.h"
 */
#include "../include/em_device.h"

#include "../include/gpio.h"
#include "../include/i2cmaster.h"
#include "../include/led.h"
#include "../include/clock_efm32gg2.h"

int main(void) {
    ClockSetCoreClock(CLOCK_HFXO, 0, 0);

    __enable_irq();

    GPIO_Init(GPIOC, 0, 0);
    GPIO_ConfigPins(GPIOC, BIT(4) | BIT(5), GPIO_MODE_WIREDANDPULLUP);

    I2CMaster_Init(I2C1, 100, 0);
    
    I2C_MyWrite(I2C1, 0x68, 0x6B, 0x00);
    I2C_MyWrite(I2C1, 0x68, 0x1C, 0x00);

    uint8_t who_am_i = I2C_MyRead(I2C1, 0x68, 0x75);
    uint8_t power = I2C_MyRead(I2C1, 0x68, 0x6B);

    uint8_t ACCEL_XOUT_H = I2C_MyRead(I2C1, 0x68, 0x3B);
    uint8_t ACCEL_XOUT_L = I2C_MyRead(I2C1, 0x68, 0x3C);
    uint8_t ACCEL_YOUT_H = I2C_MyRead(I2C1, 0x68, 0x3D);
    uint8_t ACCEL_YOUT_L = I2C_MyRead(I2C1, 0x68, 0x3E);
    uint8_t ACCEL_ZOUT_H = I2C_MyRead(I2C1, 0x68, 0x3F);
    uint8_t ACCEL_ZOUT_L = I2C_MyRead(I2C1, 0x68, 0x40);

    
    return 0;
}