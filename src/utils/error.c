#include "error.h"

#include <stdio.h>

char err_code[1] = {0};

void set_error(status_t err){
    switch(err){
        case STATUS_ERR_APPROVAL:
            snprintf(err_code, 1, "%d", STATUS_ERR_APPROVAL);
            break;
        case STATUS_ERR_BAD_PARAMETER:
            snprintf(err_code, 1, "%d", STATUS_ERR_BAD_PARAMETER);
            break;
        case STATUS_ERR_COMMAND_UNKOWN:
            snprintf(err_code, 1, "%d", STATUS_ERR_COMMAND_UNKOWN);
            break;
        case STATUS_ERR_CRYPTO_FAILED:
            snprintf(err_code, 1, "%d", STATUS_ERR_CRYPTO_FAILED);
            break;
        case STATUS_ERR_NOT_FOUND:;
            snprintf(err_code, 1, "%d", STATUS_ERR_NOT_FOUND);
            break;
        case STATUS_ERR_STORAGE_FULL:;
            snprintf(err_code, 1, "%d", STATUS_ERR_STORAGE_FULL);
            break;
        case STATUS_OK:
            snprintf(err_code, 1, "%d", STATUS_OK);
            break;
        default:
            printf("Shouldn't happen\r\n");
            break;
    }
}

char get_error(){
    return err_code[0];
}

void print_error(){
    fputc(err_code[0], stderr);
}
