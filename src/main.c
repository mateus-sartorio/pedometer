/**
 * @file    main.c
 * @brief   Pedometro with EFM32GG_STK3700
 * @version 1.0
 *
 * @note    Just blinks the LEDs of the STK3700
 *
 * @note    LEDs are on pins 2 and 3 of GPIO Port E
 *
 * @note    It uses a primitive delay mechanism. Do not use it.
 *
 * @author  Mateus Sartorio
 * @date    01/09/2018
 */
#include <ctype.h>
#include <stdint.h>

#include "../include/button.h"
#include "../include/lcd.h"
#include "../include/led.h"
#include "em_device.h"
#include "../include/i2cmaster.h"

#define DELAYVAL 100
#define MPU_ADDR 0x68

void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char *itoa(int num, char *str, int base)
{
    int i = 0;
    int isNegative = 0;

    // Handle 0 explicitly, otherwise empty string is printed for 0
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are only handled if base is 10
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If number is negative, append '-'
    if (isNegative)
    {
        str[i++] = '-';
    }

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}

void Delay(uint32_t delay)
{
    volatile uint32_t counter;
    int i;

    for (i = 0; i < delay; i++)
    {
        counter = 10000;
        while (counter)
            counter--;
    }
}

void WriteMultiple(uint8_t ch)
{
    int pos;

    for (pos = 1; pos <= 11; pos++)
        LCD_WriteChar(ch, pos);
}

int main(void)
{
    uint8_t data[4];
    data[0] = 0x12;
    data[1] = 0xFE;

    /* Configure Pins in GPIOE */
    LED_Init(LED1 | LED2);

    /* Configure LCD */
    LCD_Init();

    LCD_SetAll();
    Delay(DELAYVAL);

    LCD_ClearAll();
    Delay(DELAYVAL);

    int rc = I2CMaster_Init(I2C1, 100, 0);
    rc = I2CMaster_Send(I2C1, MPU_ADDR, data, 2);

    char string[10];
    itoa(rc, string, 10);
    char *s = string;

    /* Blink loop */
    while (1)
    {
        Delay(DELAYVAL);
        LED_Toggle(LED1); // Toggle LED1
        LCD_WriteAlphanumericDisplay(s++);

        if (*s == '\0')
        {
            s = string;
        }

        Delay(DELAYVAL);
        LED_Toggle(LED2); // Toggle LED2

        Delay(DELAYVAL);
        LED_Write(0, LED1 | LED2); // Turn On All LEDs

        // rc = I2CMaster_Send(I2C1, VALIDADDR, data, 1);
        // if(rc < 0) {
        //     fprintf(stderr,"Detecting valid I2C device with address %d failed
        //     (%d)\n",VALIDADDR,rc);
        // } else {
        //     fprintf(stderr,"Detected I2C device with address %d
        //     successful\n",VALIDADDR);
        // }

        // rc = I2CMaster_Send(I2C1, NOTVALIDADDR, data, 1);
        // if(rc < 0) {
        //     fprintf(stderr,"Detected an I2c device with address %d
        //     \n",NOTVALIDADDR);
        // } else {
        //     fprintf(stderr,"Detected an I2c device with address"
        //                    " %d but the address should be
        //                    invalid\n",NOTVALIDADDR);
        // }
    }
}