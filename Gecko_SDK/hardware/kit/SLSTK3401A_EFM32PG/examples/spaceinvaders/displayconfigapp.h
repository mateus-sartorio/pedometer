/***************************************************************************//**
 * @file displayconfigapp.h
 * @brief Display application specific configuration file.
 * @version 5.0.0
 *******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#ifndef __DISPLAYCONFIGAPP_H__
#define __DISPLAYCONFIGAPP_H__

/* Exclude pixel matrix allocation support which is not used by the
   spaceinvaders game. The game allocates it's own static framebuffer.
*/
#undef PIXEL_MATRIX_ALLOC_SUPPORT

#endif /* __DISPLAYCONFIGAPP_H__ */
