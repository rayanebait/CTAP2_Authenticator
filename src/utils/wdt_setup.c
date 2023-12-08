#include <avr/io.h>
#include <avr/wdt.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "wdt_setup.h"

/*=_BV(WDIE)*/
#define WDIE_BIT 0b01000000


void WDT_init(void){
    wdt_disable(); 
}

/*For timeouts only, no system resets,
prescaler value=n implies 
(16)*2^(1+n)ms*/
void WDT_timeout_setup(uint8_t prescaler){
    if(prescaler > 9){
        /*Timed sequence, enable WDE and WDCE*/
        WDTCSR = _BV(WDCE) | _BV(WDE);
        /* put interrupt enable with no prescaler*/
        WDTCSR = WDIE_BIT;
    } else {
        /*_BV(WDIE) | prescaler*/
        prescaler = WDIE_BIT | prescaler;
        /*Timed sequence, enable WDE and WDCE*/
        WDTCSR |= _BV(WDCE) | _BV(WDE);
        /* put interrupt enable with prescaler of 1<<2^(1+prescaler)*/
        WDTCSR = prescaler;
    }
}

void WDT_stop(void){
    MCUSR &= ~_BV(WDRF);                 // Clear the WDT reset flag

    WDTCSR |= (_BV(WDCE) | _BV(WDE));   // Enable the WD Change Bit
    WDTCSR = 0x00;                      // Disable the WDT

}
