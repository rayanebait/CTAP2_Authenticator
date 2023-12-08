#include "ListCredentials.h"

#include <stdio.h>
#include <stdint.h>
#include <avr/eeprom.h>
#include <util/atomic.h>

#include "error.h"
#include "manage_credentials.h"



/*Assumes the number of credentials,
stored at address E2END, is correct */
status_t ListCredential(void){
    uint8_t status = 0;
    uint8_t credential_id[CREDENTIAL_ID_SIZE];
    uint8_t hashed_app_id[HASHED_APP_ID_SIZE];

    uint8_t nb_credentials = 
        eeprom_read_byte((void *) CREDENTIAL_COUNT_ADDR);

    /*Send STATUS_OK and the number 
    of credentials.*/
    fwrite(&status, 1, 1, stdout);
    fwrite(&nb_credentials, 1, 1, stdout);

    if(nb_credentials != 0){
        /*Read each credential and put it 
        in hashed_app_id and credential_id
        then write both*/
        for(uint8_t i = 0; i < nb_credentials; i++){
            eeprom_read_block(credential_id, 
                                (void *)
                                (((int )i)*CREDENTIAL_SIZE
                                        + KEY_SIZE
                                        + HASHED_APP_ID_SIZE),
                                CREDENTIAL_ID_SIZE);
            eeprom_read_block(hashed_app_id, 
                                (void *)
                                (((int )i)*CREDENTIAL_SIZE 
                                        + KEY_SIZE),
                                HASHED_APP_ID_SIZE);


            fwrite(credential_id, 1,
                CREDENTIAL_ID_SIZE, stdout);
            fwrite(hashed_app_id, 1,
                HASHED_APP_ID_SIZE, stdout);
        }
    }
    
    return STATUS_OK;
}