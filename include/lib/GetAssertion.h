#ifndef GET_ASSERTION_H
#define GET_ASSERTION_H

#include <stdint.h>

#include "error.h"

status_t GetAssertion(uint8_t * hashed_app_id, uint8_t *client_data_hash);


#endif