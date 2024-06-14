/**************************************************************************//**
 * @file lcd_setup.c
 * @brief Setup LCD for energy mode demo
 * @version 5.0.0
 ******************************************************************************
 * @section License
 * <b>Copyright 2016 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include "lcd_setup.h"
#include "segmentlcd.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_assert.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define TIMEOUT    4000                    // Time (ms) to wait before energy mode starts.
#define TIME_DELAY 100                     // Time (ms) delay for screen update.
#define PB0_PORT   gpioPortB               // Pushbutton 0 port.
#define PB0_PIN    9                       // Pushbutton 0 pin.
#define PB1_PORT   gpioPortB               // Pushbutton 1 port.
#define PB1_PIN    10                      // Pushbutton 1 pin.

static volatile Energy_Mode_TypeDef eMode; // Selected energy mode.
static volatile int msCountDown;           // Time left before entering energy mode.
volatile uint32_t msTicks;                 // Counts 1ms timeTicks.

// Local function.
static void Delay(uint32_t dlyTicks);
static void gpioSetup(void);

/**************************************************************************//**
 * @brief  Selects eMode for state using the LCD and buttons.
 *
 * @return Energy_Mode_TypeDef selected eMode.
 *****************************************************************************/
Energy_Mode_TypeDef LCD_SelectMode(void)
{
  // Configure push button interrupts.
  gpioSetup();

  // Setup SysTick Timer for 1 msec interrupts.
  if (SysTick_Config(SystemCoreClockGet() / 1000))
  {
    EFM_ASSERT(false);
  }

  // Initialize LCD controller.
  SegmentLCD_Init(false);

  // Run countdown for user to select energy mode.
  msCountDown = TIMEOUT; // milliseconds.
  while (msCountDown > 0)
  {
    switch (eMode)
    {
      case EM0_HFXO_32MHZ:
        SegmentLCD_Write("EM0 32M");
        break;
      case EM0_HFRCO_28MHZ:
        SegmentLCD_Write("EM0 28M");
        break;
      case EM0_HFRCO_21MHZ:
        SegmentLCD_Write("EM0 21M");
        break;
      case EM0_HFRCO_14MHZ:
        SegmentLCD_Write("EM0 14M");
        break;
      case EM0_HFRCO_11MHZ:
        SegmentLCD_Write("EM0 11M");
        break;
      case EM0_HFRCO_7MHZ:
        SegmentLCD_Write("EM0 07M");
        break;
      case EM0_HFRCO_1MHZ:
        SegmentLCD_Write("EM0 01M");
        break;
      case EM1_HFXO_32MHZ:
        SegmentLCD_Write("EM1 32M");
        break;
      case EM1_HFRCO_28MHZ:
        SegmentLCD_Write("EM1 28M");
        break;
      case EM1_HFRCO_21MHZ:
        SegmentLCD_Write("EM1 21M");
        break;
      case EM1_HFRCO_14MHZ:
        SegmentLCD_Write("EM1 14M");
        break;
      case EM1_HFRCO_11MHZ:
        SegmentLCD_Write("EM1 11M");
        break;
      case EM1_HFRCO_7MHZ:
        SegmentLCD_Write("EM1 07M");
        break;
      case EM1_HFRCO_1MHZ:
        SegmentLCD_Write("EM1 01M");
        break;
      case EM2_LFRCO_RTC:
        SegmentLCD_Write("EM2 RTC");
        break;
      case EM3:
        SegmentLCD_Write("EM3");
        break;
      case EM4:
        SegmentLCD_Write("EM4");
        break;
      // Default case should not be reached.
      default:
        EFM_ASSERT(false);
        break;
    }
    SegmentLCD_Number(msCountDown);
    Delay(TIME_DELAY);
    msCountDown -= TIME_DELAY;
  }

  // Get ready to start the energy mode test. Turn off everything we do not need.

  // Clear LCD display.
  SegmentLCD_Write("\f");
  SegmentLCD_Disable();

  // Disable GPIO.
  NVIC_DisableIRQ(GPIO_ODD_IRQn);
  NVIC_DisableIRQ(GPIO_EVEN_IRQn);
  GPIO_PinModeSet(PB0_PORT, PB0_PIN, gpioModeDisabled, 1);
  GPIO_PinModeSet(PB1_PORT, PB1_PIN, gpioModeDisabled, 1);

  // Disable systick timer.
  SysTick ->CTRL = 0;

  // Return desired energy mode.
  return eMode;
}

/**************************************************************************//**
* @brief SysTick_Handler.
* Interrupt Service Routine for system tick counter.
*****************************************************************************/
void SysTick_Handler(void)
{
  msTicks++; // increment counter necessary in Delay().
}

/**************************************************************************//**
* @brief Delays number of msTick Systicks (typically 1 ms).
* @param dlyTicks Number of ticks to delay.
*****************************************************************************/
void Delay(uint32_t dlyTicks)
{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

/**************************************************************************//**
* @brief GPIO Interrupt handler (Pushbutton 0).
*****************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
  // Acknowledge interrupt.
  GPIO_IntClear(1 << 9);

  if ((int)eMode > 0)
  {
    eMode = (Energy_Mode_TypeDef)((int)eMode - 1);
    msCountDown = TIMEOUT;
  }
}

/**************************************************************************//**
* @brief GPIO Interrupt handler (Pushbutton 1).
*****************************************************************************/
void GPIO_EVEN_IRQHandler(void)
{
  // Acknowledge interrupt.
  GPIO_IntClear(1 << 10);

  if ((int)eMode < ((int)NUM_EMODES - 1))
  {
    eMode = (Energy_Mode_TypeDef)((int)eMode + 1);
    msCountDown = TIMEOUT;
  }
}

/**************************************************************************//**
* @brief Setup GPIO interrupt to change demo mode.
*****************************************************************************/
void gpioSetup(void)
{
  // Enable GPIO in CMU.
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Configure PB9 as input and enable interrupt.
  GPIO_PinModeSet(PB0_PORT, PB0_PIN, gpioModeInputPull, 1);
  GPIO_IntConfig(PB0_PORT, PB0_PIN, false, true, true);

  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);

  // Configure PB10 as input and enable interrupt.
  GPIO_PinModeSet(PB1_PORT, PB1_PIN, gpioModeInputPull, 1);
  GPIO_IntConfig(PB1_PORT, PB1_PIN, false, true, true);

  NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}
