#ifndef I2CMASTER_H
#define I2CMASTER_H
/**
 * @file    i2cmaster.c
 * @brief   I2C HAL for EFM32GG (master mode)
 *
 * | Signal     | #0  | #1  | #2  | #3  | #4  | #5  | #6  |                                        |
 * |------------|-----|-----|-----|-----|-----|-----|-----|----------------------------------------|
 * | I2C0_SCL   | PA1 | PD7 | PC7 | PD15| PC1 | PF1 | PE13| I2C0 Serial Clock Line input / output  |
 * | I2C0_SDA   | PA0 | PD6 | PC6 | PD14| PC0 | PF0 | PE12| I2C0 Serial Data input / output        |
 * | I2C1_SCL   | PC5 | PB12| PE1 |     |     |     |     | I2C1 Serial Clock Line input / output  |
 * | I2C1_SDA   | PC4 | PB11| PE0 |     |     |     |     | I2C1 Serial Data input / output        |
 *
 * @version 1.0
 * @author  Hans
 * @data    16/09/2023
 */

#include "em_device.h"
#include <stdint.h>

 /**
  * @brief  Send routines
  */
  ///@{
int I2CMaster_Init(I2C_TypeDef* unit, uint32_t speed, uint8_t loc);
int I2CMaster_ConfigureSpeed(I2C_TypeDef* unit, uint32_t speed);
uint8_t I2C_MyRead(I2C_TypeDef* i2c, uint8_t address, uint8_t reg);
int I2C_MyWrite(I2C_TypeDef* i2c, uint8_t address, uint8_t reg, uint8_t value);

#endif // I2CMASTER_H