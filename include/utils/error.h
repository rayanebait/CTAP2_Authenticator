#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

typedef enum{
    /*ascii code for '0' is 48*/
    STATUS_OK=0,
    STATUS_ERR_COMMAND_UNKOWN,
    STATUS_ERR_CRYPTO_FAILED,
    STATUS_ERR_BAD_PARAMETER,
    STATUS_ERR_NOT_FOUND,
    STATUS_ERR_STORAGE_FULL,
    STATUS_ERR_APPROVAL
}status_t;


void set_error(status_t);

char get_error(void);

void print_error();


#endif /*ERROR_H*/
