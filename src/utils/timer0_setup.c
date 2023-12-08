#include <avr/io.h>

#include "timer0_setup.h"
//TIFR0 to check interrupt flags

/*
    A compare match output A setup, sets:
        -Top=
*/
void Timer0_CTCA_setup(uint8_t prescaler, char OC0A_at_compare_match){
    if(prescaler > 5){
        /*no prescaler*/
        Timer0_CTCA_setup(1, OC0A_at_compare_match);
    }
    switch(OC0A_at_compare_match){
        case 'c':
        /*clear*/
            TCCR0A = _BV(COM0A1);
            break;
        case 's':
        /*set*/
            TCCR0A = _BV(COM0A0) | _BV(COM0A1);
            break;
        case 't':
        /*toggle*/
            TCCR0A = _BV(COM0A0);
            break;
        default:
            Timer0_CTCA_setup(prescaler, 't');
    }

    /*prescaler of 1<<2*prescaler*/
    TCCR0B = (prescaler);

    TIMSK0 = _BV(OCIE0A);
}

void Timer0_Overflow_setup(uint8_t prescaler){
    if(prescaler > 5){
        /*no prescaler*/
        Timer0_Overflow_setup(1);
    }
    TCCR0A = 0x00;

    /*prescaler of 1<<2*prescaler*/
    TCCR0B = (prescaler);

    TIMSK0 = _BV(TOIE0);
}

void Timer0_Overflow_No_Interrupt_setup(uint8_t prescaler){
    if(prescaler > 5){
        /*no prescaler*/
        Timer0_Overflow_No_Interrupt_setup(1);
    }
    TCCR0A = 0x00;

    /*prescaler of 1<<2*prescaler*/
    TCCR0B = (prescaler);
}

/*A PWM setup for the Timer0, prescaler=0 stops the timer*/
/*
    A Timer0 fast PWM setup, sets:
        -(COM0A1) OC0A set on compare match A
         (readable on PD6(pin 6 on arduino atmega328p))
        -top='m' : (WGM01 + WGM00) Top=Oxff=MAX, TOV Flag set at MAX
        -top='c': adds WGM02, Top=OCR0A, TOV Flag set at Top
        -prescaler of 1<<prescaler.
    (OC0A = readable as OCR0A at pin PD6=6)
*/
void Timer0_Fast_PWM_setup(uint8_t prescaler, char top){
    if(prescaler > 5){
        /*no prescaler*/
        Timer0_Fast_PWM_setup(1, top);
    }

   switch(top){
        /*Max*/
        case 'm':
            TCCR0A = _BV(WGM01) | _BV(WGM00)
                     | _BV(COM0A1);
            break;
        /*compare, OCR0A*/
        case 'c':
            TCCR0A = _BV(WGM02) | _BV(WGM01)
                     | _BV(WGM00) | _BV(COM0A1);
            break;
        default:
            Timer0_Fast_PWM_setup(prescaler, 'm');
   }

    /*Set a prescaler of 1<<2*prescaler*/
    TCCR0B = (prescaler);

    /*Overflow interrupt enable*/
    TIMSK0 = _BV(TOIE0);
}

void Timer0_Stop(){
    TCCR0B = 0x00;
}