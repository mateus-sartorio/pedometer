#pragma once

#include <efm32gg_gpio.h>

struct uart {
};

struct uart* UART_init(unsigned speed, unsigned size, unsigned parity, unsigned stop_bits);
unsigned UART_get_status(void);
void UART_send(unsigned value);
void UART_send_char(char c);
void UART_send_string(const char* const string);
int UART_flush(void);
void UART_receive(GPIO_TypeDef* port);
void UART_receive_no_wait(GPIO_TypeDef* port);