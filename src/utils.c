#include "../include/utils.h"

#include <stdio.h>
#include <stdint.h>

extern volatile uint32_t TickCounter;

int itoa(int n, char* str) {
    if (str == NULL) return 1;

    int i = 0;
    int is_negative = 0;

    // Handle negative numbers
    if (n < 0) {
        is_negative = 1;
        n = -n;
    }

    // Handle the case where n is 0
    if (n == 0) {
        str[i++] = '0';
    }

    // Process individual digits
    while (n != 0) {
        str[i++] = (n % 10) + '0';
        n /= 10;
    }

    // Add negative sign if needed
    if (is_negative) {
        str[i++] = '-';
    }

    // Add null terminator
    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }

    return 0;
}

/**
 * @brief  Better delay function
 *
 * @param  delay is in ms
 */

void delay(uint32_t delay) {
    volatile uint32_t initialvalue = TickCounter;

    while ((TickCounter - initialvalue) < delay);
}

uint64_t array_sum(uint64_t* array, int size) {
    uint64_t sum = 0;
    
    for(int i = 0; i < size; i++) {
        sum += array[i];
    }

    return sum;
}