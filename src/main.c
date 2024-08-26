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
#include "../include/clock_efm32gg2.h"
#include "../include/utils.h"
#include "../include/button.h"
#include "../include/lcd.h"

#define DELAYVAL 100
#define MOVING_AVERAGE_WINDOW_SIZE 10
#define UPPER_THRESHOLD 3240000000
#define LOWER_THRESHOLD 2890000000

enum state {
    IDLE,
    HIGH,
};

enum state current_state = IDLE;

  /**
   * @brief Tick counter
   * @note  Incremented every 1 ms
   */
volatile uint32_t TickCounter = 0;

static uint32_t steps_taken = 0;
static char lcd_print_string[MAX_INT_STR_SIZE] = "0";

static uint64_t moving_average_values[MOVING_AVERAGE_WINDOW_SIZE] = { 0 };

void button_callback(uint32_t value) {
  steps_taken = 0;
}

const int TickDivisor = 1000;

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

  LCD_WriteAlphanumericDisplay(lcd_print_string);

  I2CMaster_Init(I2C1, 100, 0);

  I2C_MyWrite(I2C1, 0x68, 0x6B, 0x00);
  I2C_MyWrite(I2C1, 0x68, 0x1C, 0x00);

  delay(1000);

  uint8_t who_am_i = I2C_MyRead(I2C1, 0x68, 0x75);
  uint8_t power = I2C_MyRead(I2C1, 0x68, 0x6B);

  int moving_average_index = 0;
  int moving_average_size = 0;
  while (true) {
    uint8_t accel_xout_h = I2C_MyRead(I2C1, 0x68, 0x3B);
    uint8_t accel_xout_l = I2C_MyRead(I2C1, 0x68, 0x3C);
    uint8_t accel_yout_h = I2C_MyRead(I2C1, 0x68, 0x3D);
    uint8_t accel_yout_l = I2C_MyRead(I2C1, 0x68, 0x3E);
    uint8_t accel_zout_h = I2C_MyRead(I2C1, 0x68, 0x3F);
    uint8_t accel_zout_l = I2C_MyRead(I2C1, 0x68, 0x40);

    int16_t ac_x = (accel_xout_h << 8) | accel_xout_l;
    int16_t ac_y = (accel_yout_h << 8) | accel_yout_l;
    int16_t ac_z = (accel_zout_h << 8) | accel_zout_l;

    uint64_t acc = ac_x*ac_x + ac_y*ac_y + ac_z*ac_z;

    moving_average_values[moving_average_index] = acc;
    moving_average_index = (moving_average_index + 1) % MOVING_AVERAGE_WINDOW_SIZE;
    
    if(moving_average_size < MOVING_AVERAGE_WINDOW_SIZE) {
      moving_average_size++;
    }
    else {
      uint64_t sum = array_sum(moving_average_values, MOVING_AVERAGE_WINDOW_SIZE);

      if(sum > UPPER_THRESHOLD && current_state == IDLE) {
        steps_taken++;
        current_state = HIGH;
      }
      else if(sum < LOWER_THRESHOLD && current_state == HIGH) {
        current_state = IDLE;
      }
    }

    itoa(steps_taken, lcd_print_string);
    LCD_WriteString(lcd_print_string);

    // Enter low power state
    __WFI();
  }
}