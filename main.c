#include <avr/io.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "wdt_setup.h"
#include "timer0_setup.h"

#include "Authenticator.h"



void setup(){
    /*enables sleep modes for uart 
    and the random number generator.*/
    set_sleep_mode(SLEEP_MODE_IDLE);

    UART__init(BAUD_115200);

    /*For the button setup*/
    DDRB &= _BV(DDB0);
}


int main(void){
    setup();

    sei();

    while(1){
        uint8_t status = parse_request();
        if(status != STATUS_OK)
            fwrite(&status, 1, 1, stdout);
    }
}