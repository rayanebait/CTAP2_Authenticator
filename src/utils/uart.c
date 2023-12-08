#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/sleep.h>

#include "uart.h"
#include "ring_buffer.h"


#define RX_BUFFER_SIZE 100

int UART__putchar(char, FILE*);
int UART__getchar(FILE*);

uint8_t rx_raw_buffer[RX_BUFFER_SIZE];
struct ring_buffer rx_buffer;
char UART_receive_buffer;


static FILE mystdout = FDEV_SETUP_STREAM(UART__putchar, NULL, _FDEV_SETUP_WRITE);
static FILE mystderr = FDEV_SETUP_STREAM(UART__putchar, NULL, _FDEV_SETUP_WRITE);
static FILE mystdin = FDEV_SETUP_STREAM(NULL, UART__getchar, _FDEV_SETUP_READ);

volatile uint8_t count;
volatile uint8_t no_block = 0;

ISR(TIMER0_OVF_vect){
    count++;
}

void 
set_block(){
    no_block = 0;
}

void
set_noblock(){
    no_block = 1;
}



/*UBRRH/L_VALUE are computed to take care 
of floats and floor imprecisions in util/setbaud.h
and depend on BAUD and F_CPU.*/
void
UART__init_115200(){
    #undef BAUD
    #define BAUD 115200
    
    #include <util/setbaud.h>
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    #if USE_2X
    UCSR0A |= (1 << U2X0);
    #else
    UCSR0A &= ~(1 << U2X0);
    #endif
}

void
UART__init_9600(){
    #undef BAUD
    #define BAUD 9600
    
    #include <util/setbaud.h>
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    #if USE_2X
    UCSR0A |= (1 << U2X0);
    #else
    UCSR0A &= ~(1 << U2X0);
    #endif
}


/*Set baud to:
    -BAUD_115200 for a baudrate of 
    115200
    -BAUD_9600 for a baudrate of 
    9600
if the value is different sets a baud 
rate of 115200. Doesn't support other
values for now
*/
/*The uart is based on a ring buffer 
defined in ring_buffer.h. stdout,
stdin and stderr are set so that 
the print and read utilities use 
the UART__getchar and UART__putchar
definitions.*/
void
UART__init(uint8_t baud)
{

    if(baud == BAUD_115200){
        /*Set baud rate of 9600*/
        UART__init_115200();
    } else if(baud == BAUD_9600){
        /*Set baud rate of 9600*/
        UART__init_9600();
    } else {
        /*To be modified*/
        UART__init_115200();
    }

    // enable RX, TX for UART0
    UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);  // 8 bit data format

    ring_buffer__init(&rx_buffer, rx_raw_buffer, RX_BUFFER_SIZE);

    stdout = &mystdout;
    stdin = &mystdin;
    stderr = &mystderr;
}

ISR(USART_RX_vect) {
    UART_receive_buffer = UDR0;
    ring_buffer__push(&rx_buffer, UART_receive_buffer);
}

int UART__putchar(char c, FILE* _stream) {
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}

/*Checks if the ring buffer is empty, if yes 
wait until RX interrupt if set in blocking mode
or wait for either RX interrupt or Timer0 interrupt if
set in non blocking mode.*/
int UART__getchar(FILE* _stream) {
    uint8_t data;
    if(no_block){
        TCNT0 = 0;
        count = 0;
        while ((ring_buffer__pop(&rx_buffer, &data) == 1) && (count < 2)){
            sleep_mode();
        }
    } else {
        while( ring_buffer__pop(&rx_buffer, &data) == 1){
            /* If the ring buffer is empty
            wait for a USART_RX interrupt. Stay 
            in a loop in case of another interrupt.*/
            sleep_mode();
        }
    }
    return (int) data; 
}

