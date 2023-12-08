#ifndef TIMER0_SETUP_H
#define TIMER0_SETUP_H

#include <stdint.h>
#include <avr/io.h>

void Timer0_CTCA_setup(uint8_t prescaler, char OC0A_at_compare_match);
void Timer0_Overflow_setup(uint8_t prescaler);
void Timer0_Overflow_No_Interrupt_setup(uint8_t prescaler);

void Timer0_Fast_PWM_setup(uint8_t prescaler, char top);
void Timer0_Stop(void);

#endif /*TIMER0_SETUP_H*/