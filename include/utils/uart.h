
#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>


#define BAUD_115200 1
#define BAUD_9600 0

void
set_block();

void
set_noblock();

void
UART__init_115200();
void
UART__init_9600();

void
UART__init(uint8_t baud);

int
UART__putchar(char c, FILE* _stream);

int
UART__getchar(FILE* _stream);

#endif /*UART_H*/