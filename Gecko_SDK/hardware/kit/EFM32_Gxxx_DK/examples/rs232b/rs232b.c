/**************************************************************************//**
 * @file
 * @brief RS232B example for EFM32_G2xx_DK and EFM32_G8xx_DK
 *     Connect a terminal application with baudrate 9600-8-N-1
 *     on serial port B of kit to
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

#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"
#include "bsp.h"
#include "bsp_trace.h"
#include "retargetserial.h"

/** LEUART input buffer size */
#define ECHOBUFSIZE    80
/** LEUART1 Input buffer */
char echoBuffer[ECHOBUFSIZE];

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  int c;
  unsigned int index;

  /* Chip revision alignment and errata fixes */
  CHIP_Init();

  /* Initialize DVK board register access */
  BSP_Init(BSP_INIT_DEFAULT);

  /* If first word of user data page is non-zero, enable eA Profiler trace */
  BSP_TraceProfilerSetup();

  /* Initialize LEUART */
  RETARGET_SerialInit();

  /* Map LF to CRLF */
  RETARGET_SerialCrLf(1);

  printf("\nEFM32 LEUART1 example\n");
  for (index = 0; index < ECHOBUFSIZE-1; index++)
  {
    echoBuffer[index] = (char)'a' + index % ((char)'~' - (char)'a' + 1);
  }

  /* Retrieve characters, print local echo and full line back */
  index = 0;
  while (1)
  {
    /* Retrieve new character */
    c = getchar();
    if (c > 0)
    {
      /* Output character - most terminals use CRLF */
      if (c == '\r')
      {
        echoBuffer[index] = '\0';
        /* Output entire line */
        printf("\n%s\n", echoBuffer);
        index = 0;
      }
      else
      {
        /* Filter non-printable characters */
        if ((c < ' ') || (c > '~'))
          continue;

        /* Enter into buffer */
        echoBuffer[index] = c;
        index++;
        if (index == ECHOBUFSIZE)
        {
          /* Flush buffer */
          index = 0;
        }
        /* Local echo */
        putchar(c);
      }
    }
    else
    {
      /* Enter EM2 when idle */
      EMU_EnterEM2(true);
    }
  }
}
