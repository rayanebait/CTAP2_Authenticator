#include <stdint.h>
#include <avr/io.h>
#include <util/atomic.h>

#include "timer1_setup.h"

//TIMER1/0_COMPA/B_vect
//TIMER1/0_OVF_vect

/*16 bit write is not atomic*/
void Write_Timer1( uint16_t i ){
    /*Ensures atomic write*/
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        TCNT1 = i;
    }
}

void Write_COMPA( uint16_t i ){
    /*Ensures atomic write*/
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        OCR1A = i;
    }
}


uint16_t Read_Timer1( void ){
    unsigned int i;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        /* Read TCNT1 into i */
        i = TCNT1;
    }
    return i;
}

uint16_t Read_COMPA( void ){
    unsigned int i;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        /* Read TCNT1 into i */
        i = OCR1A;
    }
    return i;
}

/*prescaler=4 for 256*/
void Timer1_overflow_setup(uint8_t prescaler){

    TIMSK1 = _BV(TOIE1);

    TCCR1A = 0;
    /*Prescaler of 1<<2*prescaler*/
    TCCR1B = _BV(prescaler);
}

void Timer1_CTCA_setup(uint16_t cmpa, uint8_t prescaler){
    Write_COMPA(cmpa);

    TCCR1B = prescaler | _BV(WGM12);
    TIMSK1 = _BV(OCIE1A);

    /*Prescaler of 1<<2*prescaler*/
}

void Timer1_Stop(){
    TCCR1B = 0x00;
}
// void Timer1_PWM_setup(uint8_t prescaler){

// }
