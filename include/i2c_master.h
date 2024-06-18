// #pragma once

// #include <efm32gg_i2c.h>
// #include <stdint.h>

// /**
//  * @brief
//  * SDA: Data line
//  * SCL: Clock line
//  *
//  */
// void I2C_master_init(I2C_TypeDef* unit, uint32_t speed, uint8_t location);

// void I2C_master_configure_speed(I2C_TypeDef* unit, uint32_t speed);

// int I2C_master_send(I2C_TypeDef* unit, uint16_t address, uint8_t* data, uint32_t size);

// int I2C_master_send_start(I2C_TypeDef* unit, uint16_t address, uint8_t* data, uint32_t size);
// int I2C_master_send_get_status(I2C_TypeDef* unit);

// unsigned I2C_master_receive(uint16_t address, const char* area, unsigned n);