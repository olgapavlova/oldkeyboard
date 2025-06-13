#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "adc.h"

void uart_init(void);
void uart_send_char(char);
void uart_send_string(const char *);

#endif
