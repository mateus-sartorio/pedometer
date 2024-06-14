/***************************************************************************//**
 * @file app-config.h
 * @brief Range Test application configurations.
 *
 * @version 5.0.0
 *******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/


#ifndef APP_CONFIGURATION_H_
#define APP_CONFIGURATION_H_

#if (defined APP_USE_GENERATED_CONFIGURATION)
/* Include the generated radio configuration */
#include "app-config.h"
#else
/* Include the default radio configuration for the board */
#if   ( (defined SL_WSTK6200A) || (defined SL_WSTK6220A) )
#include "app-config_R60-868MHz-13dBm.h"
#elif ( (defined SL_WSTK6202A) || (defined SL_WSTK6222A) )
#include "app-config_R63-915MHz-20dBm.h"
#elif (defined SL_WSTK6223A)
#include "app-config_R63-490MHz-20dBm.h"
#elif (defined SL_WSTK6224A)
#include "app-config_R63-169MHz-20dBm.h"
#else
#error No application configuration is defined! Create your own application configuration or define your kit properly!
#endif
#endif

#endif /* APP_CONFIGURATION_H_ */
