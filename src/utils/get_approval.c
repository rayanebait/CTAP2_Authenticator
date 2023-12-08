#include "get_approval.h"


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "timer1_setup.h"
#include "timer0_setup.h"
#include "error.h"

volatile uint8_t button_pushed = 0;
volatile uint8_t timer_half_seconds = 0;

uint8_t is_from_make_credential = 0;

void
set_from_make_credential(){
    is_from_make_credential = 1;
}

void
unset_from_make_credential(){
    is_from_make_credential = 0;
}


ISR(TIMER1_COMPA_vect){
    timer_half_seconds++;
    PORTB ^= _BV(PORTB5);
}

ISR(PCINT0_vect){
    PCMSK0 |= _BV(PCINT0);
    button_pushed = 1;
}

/*Engages in a loop which */
status_t get_approval(void){
    button_pushed = 0;
    /*enable on pin PB0=PCINT0*/
    PCMSK0 |= _BV(PCINT0);
    /*Pin change interrupt enable*/
    PCICR |= _BV(PCIE0);

    /*PORTB5 as output to be able to 
    toggle the led.*/
    DDRB |= _BV(PORTB5);

    /*COMPA = 0x7a12 and prescaler of 256 -> 0.5s delay */
    Timer1_CTCA_setup(0x7a12, 4);
    Write_Timer1(0);

    timer_half_seconds = 0;

    if(is_from_make_credential){
        /*Timer0 is setup for the random number 
        generator*/
        Timer0_Overflow_No_Interrupt_setup(1);
        while(( timer_half_seconds < 20) && !( button_pushed ));
    } else {
        while(( timer_half_seconds < 20) && !( button_pushed )){
            sleep_mode();
        }
    }
    
    

    PCMSK0 |= _BV(PCINT0);

    Timer1_Stop();
    timer_half_seconds = 0;

    if(button_pushed){
        return STATUS_OK;
    }

    return STATUS_ERR_APPROVAL;
}