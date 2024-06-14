/***************************************************************************//**
 * @file
 * @brief uC/OS-III example - Board Support Package (BSP) header
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

#ifndef  __BSPOS_PRESENT_H
#define  __BSPOS_PRESENT_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *****************************   INCLUDE FILES   *******************************
 ******************************************************************************/


/*******************************************************************************
 **************************   FUNCTION PROTOTYPES   ****************************
 ******************************************************************************/
void BSPOS_Init(void);
CPU_INT32U OS_CPU_SysTickClkFreq (void);

#ifdef __cplusplus
}
#endif

#endif  /* end of __BSPOS_PRESENT_H */
