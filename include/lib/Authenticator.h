#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include "MakeCredential.h"
#include "GetAssertion.h"
#include "ListCredentials.h"
#include "Reset.h"

#include "error.h"


typedef enum {
    COMMAND_LIST_CREDENTIALS=0,
    COMMAND_MAKE_CREDENTIAL,
    COMMAND_GET_ASSERTION,
    COMMAND_RESET
}command_t;

status_t get_approval();

status_t parse_request(void);




#endif /*AUTHENTICATOR_H*/