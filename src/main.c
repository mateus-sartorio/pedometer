/** ***************************************************************************
 * @file    main.c
 * @brief   Simple UART Demo for EFM32GG_STK3700
 * @version 1.0
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
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
#include "../include/utils.h"
#include "../include/button.h"
#include "../include/lcd.h"

  /**
   * @brief Tick counter
   * @note  Incremented every 1 ms
   */
volatile uint32_t TickCounter = 0;

static uint32_t steps_taken = 0;

static uint32_t cool_value = 0;
static char count_string[MAX_INT_STR_SIZE] = "0";

#define DELAYVAL 100

/************************************************************************//**
 * @brief  Button callback
 *
 * @note   runs with disabled interrupts
 */
void button_callback(uint32_t v) {
  steps_taken = 0;
}

/*************************************************************************//**
* @brief  Sys Tick Handler
*/
const int TickDivisor = 1000; // milliseconds

/**
 * @brief  SysTick interrupt handler
 *
 * @note   Called every 1/DIVIDER seconds (1 ms)
 */

void SysTick_Handler(void) {
  TickCounter++;
}

int main(void) {
  ClockSetCoreClock(CLOCK_HFXO, 0, 0);

  GPIO_Init(GPIOC, 0, 0);
  GPIO_ConfigPins(GPIOC, BIT(4) | BIT(5), GPIO_MODE_WIREDANDPULLUP);

  /* Configure buttons */
  Button_Init(BUTTON1 | BUTTON2);
  Button_SetCallback(button_callback);

  /* Enable interrupts */
  __enable_irq();

  /* Configure SysTick */
  SysTick_Config(SystemCoreClock / TickDivisor);

  /* Configure LCD */
  LCD_Init();

  LCD_SetAll();
  delay(DELAYVAL);

  LCD_ClearAll();
  delay(DELAYVAL);


  // I2CMaster_Init(I2C1, 100, 0);

  // I2C_MyWrite(I2C1, 0x68, 0x6B, 0x00);
  // I2C_MyWrite(I2C1, 0x68, 0x1C, 0x00);

  // delay(1000);

  // uint8_t who_am_i = I2C_MyRead(I2C1, 0x68, 0x75);
  // uint8_t power = I2C_MyRead(I2C1, 0x68, 0x6B);

  // uint8_t accel_xout_h = I2C_MyRead(I2C1, 0x68, 0x3B);
  // uint8_t accel_xout_l = I2C_MyRead(I2C1, 0x68, 0x3C);
  // uint8_t accel_yout_h = I2C_MyRead(I2C1, 0x68, 0x3D);
  // uint8_t accel_yout_l = I2C_MyRead(I2C1, 0x68, 0x3E);
  // uint8_t accel_zout_h = I2C_MyRead(I2C1, 0x68, 0x3F);
  // uint8_t accel_zout_l = I2C_MyRead(I2C1, 0x68, 0x40);


  LCD_WriteAlphanumericDisplay(count_string);

  while (true) {
    // Enter low power state
    __WFI();
  }
}