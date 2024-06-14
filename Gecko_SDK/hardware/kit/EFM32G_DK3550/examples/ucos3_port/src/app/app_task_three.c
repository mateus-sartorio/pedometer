/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2010; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                         uC/OS-III example code
*                                          Application task three
*
*                                   Silicon Labs EFM32 (EFM32G890F128)
*                                              with the
*                            Silicon Labs EFM32G890F128-DK Development Board
*
* @file   app_task_three.c
* @brief
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
#include <includes.h>


/*
*********************************************************************************************************
*                                             DEFINITIONS
*********************************************************************************************************
*/
/* Width of LCD screen on plugin board */
#define APPDEF_LCD_TXT_SIZE  7


/*
*********************************************************************************************************
*                                         APP_TaskThree()
* @brief      The Three task.
*
* @param[in]  p_arg       Argument passed to 'APP_TaskThree()' by 'OSTaskCreate()'.
* @exception  none
* @return     none.
*/
/* Notes      :(1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*
*
*********************************************************************************************************
*/
void APP_TaskThree(void *p_arg)
{
  OS_ERR err = OS_ERR_NONE;
  char indxChar;
  char MsgSize;
  int  msgContent;
  static char taskStringBuffer[APPDEF_LCD_TXT_SIZE+1U] = {'u','C','/','O','S','-','3','\0'};
  static int  ringPos = 0;


  (void)p_arg; /* Note(1) */

  while (1)
  { /* Task body, always written as an infinite loop  */

    /* Turn previous ring segment off                 */
    SegmentLCD_ARing(ringPos, 0);

    /* Increase ring position variable                */
    if (8u == ++ringPos)
    {
      ringPos = 0; /* 3bit overflow */
    }

    /* Turn updated ring segment on                   */
    SegmentLCD_ARing(ringPos, 1);

    /* Non-blocking reception of a message */
    msgContent = (int)OSQPend((OS_Q         *) pSerialQueObj,
                              (OS_TICK       ) 0U,
                              (OS_OPT        ) OS_OPT_PEND_NON_BLOCKING,
                              (OS_MSG_SIZE  *)&MsgSize,
                              (CPU_TS       *) 0U,
                              (OS_ERR       *)&err);

    /* If a valid message was received... */
    if (err == OS_ERR_NONE)
    {
      /* ...shift left the whole string by one... */
      for (indxChar = 0; indxChar < APPDEF_LCD_TXT_SIZE; indxChar++)
      {
        taskStringBuffer[indxChar] = taskStringBuffer[indxChar+1];
      }

      /* ...and concatenate the new character to the end. */
      taskStringBuffer[APPDEF_LCD_TXT_SIZE-1] = (char)msgContent;

      /* Write the string on serial port (USART0) */
      printf("\nBuffer: %s", taskStringBuffer);

      /* Write the string on LCD */
      SegmentLCD_Write(taskStringBuffer);
    }

    /* Delay task for 1 system tick (uC/OS-II suspends this task and executes
     * the next most important task) */
    OSTimeDly(1U, OS_OPT_TIME_DLY, &err);
  }
}
