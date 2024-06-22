#pragma once

#include "macros.h"

#define UART_TXREADY BIT(6)
#define UART_RXDATAV BIT(7)
#define UART_RXFULL BIT(8)
#define UART_TXENS BIT(1)
#define UART_RXENS BIT(0)

void UART_Init(void);
unsigned UART_GetStatus(void);
void UART_SendChar(char c);
void UART_SendString(char* s);

unsigned UART_GetChar(void);
unsigned UART_GetChahrNoWait(void);
void UART_GetString(char* s, int n);