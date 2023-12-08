#include <stdint.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "weak_prg.h"
#include "wdt_setup.h"
#include "timer0_setup.h"

EMPTY_INTERRUPT(WDT_vect);

static uint8_t rng_type = 0;

void set_rng_type_timer0(){
    rng_type = 1;
}

void set_rng_type_tiltball(){
    rng_type = 0;
}
/*Based on reading a pin that's randomly 
1 or 0. Needs a software interrupt every 16/32ms*/
static uint8_t get_byte(){
    uint8_t byte = 0;

    /*Get a bit at every interrupt*/
    for(uint8_t i = 0; i < (uint8_t) 8; i++){
        uint8_t bit = 0;
        /*Wait interrupt*/
        sleep_mode();
        /*Get pin value at PINB5*/
        bit = (PINB >> PINB5) & 1;
        byte |= (bit<<i);
    }

    return byte;
}

/*Relies on the fact that 
the watch dog timer is not 
so precise*/
static uint8_t get_byte_timer0(){
    uint8_t byte = TCNT0;
    return byte;
}


/*
Uses a tilt ball switch joining ground and a pin:
    -Enable interrupts every 16/32ms.
    -At startup of gen_seed, shake the tilt ball
    for ~2/4seconds.
*/
void rng(){
    for(uint8_t i = 0; i < SEED_SIZE; i++){
        seed[i] = get_byte();
    }
}

/*sleep until interrupted by the watchdog timer
then read TCNT0 which runs with no prescaler.*/
void false_rng(){
    Timer0_Stop();
    for(uint8_t i = 0; i < SEED_SIZE; i++){
        Timer0_Overflow_No_Interrupt_setup(1);
        seed[i] = get_byte_timer0();
        Timer0_Stop();
        sleep_mode();
    }
}


/*by default uses tilt ball switch*/
int uECC_rng(uint8_t *dest, unsigned size){
    WDT_init();
    WDT_timeout_setup(0);


    if( rng_type == 0 ){
        DDRB &= ~_BV(DDB5);
        rng();
        DDRB |= _BV(DDB5);
    } else {
        /*Timer0 with no prescaler*/
        Timer0_Overflow_No_Interrupt_setup(1);
        false_rng();
    }

    WDT_stop();


    int score = 0;
    for(int i = 0; i < SEED_SIZE ; i++){
        score += (int) seed[i];
    }

    if(( score < ((int) SEED_SIZE/5)*255 )
                || ( (SEED_SIZE*255 - ((int) SEED_SIZE/5)*255) < score) ){
            return 0;
    }

    for(uint8_t i = 0; i < size; i++){
        dest[i] = seed[i];
    }

    return 1;
}

