/**************************************************************************//**
 * @file
 * @brief 16 x 28 Font Definition
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

#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 16 x 28 pixel font */
extern const struct
{
  unsigned int width;      /* Image width */
  unsigned int height;     /* Image height */
  unsigned int c_width;    /* Character width */
  unsigned int c_height;   /* Character height */
  uint16_t     data[1504 * 28];
} font16x28;

#ifdef __cplusplus
}
#endif

#endif

