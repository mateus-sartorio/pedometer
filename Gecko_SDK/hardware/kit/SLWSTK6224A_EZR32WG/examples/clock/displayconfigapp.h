/***************************************************************************//**
 * @file displayconfigapp.h
 * @brief Display application specific configuration file.
 * @version 5.0.0
 ******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#ifndef _DISPLAY_CONFIG_APP_H_
#define _DISPLAY_CONFIG_APP_H_

/* Include TEXTDISPLAY support because this example prints text to the
   display. */
#define INCLUDE_TEXTDISPLAY_SUPPORT

/* Select one of the fonts listed below:
   TEXTDISPLAY_FONT_6x8
   TEXTDISPLAY_FONT_8x8
   TEXTDISPLAY_NUMBER_FONT_16x20
*/
#define  TEXTDISPLAY_NUMBER_FONT_16x20

/* Enable or disable scroll mode on the text display. */
#define RETARGETTEXTDISPLAY_SCROLL_MODE  (false)

/* Enable or disable adding Carriage Return (CR) to Line Feed (LF) characters
   on the text display. */
#define RETARGETTEXTDISPLAY_LINE_FEED_MODE  (true)

/* Include pixel matrix allocation support. */
#define PIXEL_MATRIX_ALLOC_SUPPORT

/* Enable allocation of pixel matrices from the static pixel matrix pool.
   NOTE:
   The allocator does not support free'ing pixel matrices. It allocates
   continuosly from the static pool without keeping track of the sizes of
   old allocations. I.e. this is a one-shot allocator, and the  user should
   allocate buffers once at the beginning of the program.
*/
#define USE_STATIC_PIXEL_MATRIX_POOL

/* Specify the size of the static pixel matrix pool. For the clock demo
   we need one pixel matrix (framebuffer) covering the whole display.
*/
#define PIXEL_MATRIX_POOL_SIZE   ( (DISPLAY0_HEIGHT * DISPLAY0_WIDTH/8) + \
                                   (FONT_HEIGHT * DISPLAY0_WIDTH/8) )

/* On EFM32ZG_STK3200, the DISPLAY driver Platform Abstraction Layer (PAL)
   uses the RTC to time and toggle the EXTCOMIN pin of the Sharp memory
   LCD per default. However, the clock example wants to use the RTC to
   keep track of time, i.e. generate interrupt every second. Therefore
   the clock example implements a function (RtcIntCallbackRegister)) that
   enables the PAL to register the callback function that needs to be called in
   order to toggle the EXTCOMIN pin.
   I.e we need to undefine the INCLUDE_PAL_GPIO_PIN_AUTO_TOGGLE (which
   is defined by default and uses the RTC) and we need to define the
   PAL_TIMER_REPEAT_FUNCTION. */
#undef INCLUDE_PAL_GPIO_PIN_AUTO_TOGGLE
#define PAL_TIMER_REPEAT_FUNCTION  (RtcIntCallbackRegister)
extern int RtcIntCallbackRegister (void(*pFunction)(void*),
                                   void* argument,
                                   unsigned int frequency);

#endif /* _DISPLAY_CONFIG_APP_H_ */
