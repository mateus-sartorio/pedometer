/**************************************************************************//**
 * @file
 * @brief LCD demo and test, prototypes and definitions
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


#ifndef __LCDTEST_H
#define __LCDTEST_H

#ifdef __cplusplus
extern "C" {
#endif

/** Timer used for bringing the system back to EM0. */
extern RTCDRV_TimerID_t xTimerForWakeUp;


/* Segment LCD test prototypes */
void ScrollText(char *scrolltext);
void Test(void);
void BlinkTest(void);
void AnimateTest(void);

/* Various test functions */
void GPIO_IRQInit(void);
void RtcTrigger(RTCDRV_TimerID_t id, void *user);
void EM1Sleep(uint32_t msec);
void EM2Sleep(uint32_t msec);
void EM3Sleep(void);
void EM4Sleep(void);

#ifdef __cplusplus
}
#endif

#endif
