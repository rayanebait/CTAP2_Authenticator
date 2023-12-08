#include "Authenticator.h"

#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

#include "get_approval.h"
#include "error.h"
#include "timer0_setup.h"
#include "uart.h"

#define MAX_REQUEST_SIZE 40
#define MAKE_CREDENTIAL_REQUEST_SIZE 20
#define GET_ASSERTION_REQUEST_SIZE 40


status_t parse_request(void){
    // char c = fgetc(stdin);
    uint8_t command;
    uint8_t request[MAX_REQUEST_SIZE];

    /*Set blocking read, */
    set_block();
    fread(&command, 1, 1, stdin);

    /*Set non-blocking read, as explained under*/
    set_noblock();

    switch(command){
        case COMMAND_MAKE_CREDENTIAL: /*MakeCredential*/

        /*Start non-blocking read, since UART__getc
        is blocking by default. In case some 
        bytes never arrive.*/
            Timer0_Overflow_setup(5);
            if(fread(request, 1,
                 MAKE_CREDENTIAL_REQUEST_SIZE, stdin)
                     != MAKE_CREDENTIAL_REQUEST_SIZE ){
                Timer0_Stop();
                return STATUS_ERR_BAD_PARAMETER;
            }
            Timer0_Stop();
            return MakeCredential(request);
            break;

        case COMMAND_GET_ASSERTION: /*GetAssertion*/

        /*Start non-blocking read, since UART__getc
        is blocking by default. In case some 
        bytes never arrive.*/
            Timer0_Overflow_setup(5);
            if( fread(request, 1,
                    GET_ASSERTION_REQUEST_SIZE, stdin)
                        != GET_ASSERTION_REQUEST_SIZE ){
                Timer0_Stop();
                return STATUS_ERR_BAD_PARAMETER;
            }

            Timer0_Stop();
            return GetAssertion(request, request + 20);
            break;

        case COMMAND_LIST_CREDENTIALS:/*ListCredential*/
            return ListCredential();
            break;
        case COMMAND_RESET:
            return Reset();
            break;
        default:
            return STATUS_ERR_COMMAND_UNKOWN;
            break;
    }

}

