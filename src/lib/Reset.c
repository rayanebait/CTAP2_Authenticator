#include "Reset.h"

#include <stdio.h>
#include <stdint.h>
#include <avr/eeprom.h>
#include <util/atomic.h>

#include "manage_credentials.h"
#include "get_approval.h"
#include "error.h"

/*Sets every byte at 0*/
status_t Reset(){
    /*
    Toggles the led every 500ms
    until button is pressed or 10s 
    timeout.
    */
    if(get_approval() == STATUS_ERR_APPROVAL)
        return STATUS_ERR_APPROVAL;

    /*Gets nb_credentials to rewrite only used 
    memory*/
    uint8_t nb_credentials =
             eeprom_read_byte((void *)
                 CREDENTIAL_COUNT_ADDR);

    if(nb_credentials == 0){
        fwrite(&nb_credentials, 1,1, stdout);
        return STATUS_OK;
    }
    
    uint8_t c = 0;

    for(uint16_t i = 0; i < nb_credentials*CREDENTIAL_SIZE; i++){
        eeprom_update_byte((void *) i, c);
    }
    eeprom_update_byte((void *) CREDENTIAL_COUNT_ADDR, 0);

    fwrite(&c, 1,1,stdout);

    return STATUS_OK;
}