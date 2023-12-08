#ifndef TIMER1_SETUP_H
#define TIMER1_SETUP_H

#include <avr/io.h>
#include <stdint.h>


void Write_Timer1(uint16_t);
void Write_COMPA(uint16_t);

uint16_t Read_Timer1(void);
uint16_t Read_COMPA(void);

void Timer1_overflow_setup(uint8_t prescaler);
void Timer1_CTCA_setup(uint16_t cmpa, uint8_t prescaler);

void Timer1_Stop(void);

#endif /*TIMER1_SETUP_H*/