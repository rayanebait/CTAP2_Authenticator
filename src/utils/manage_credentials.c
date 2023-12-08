#include "manage_credentials.h"

#include <stdint.h>
#include <string.h>
#include <avr/eeprom.h>
#include <util/atomic.h>

#include "error.h"

#define ID_DOESNT_EXIST 255
/*Simply reads at address 
CREDENTIAL_COUNT_ADDR=E2END
if the credential count is the max*/
int eeprom_is_full(){
    if(eeprom_read_byte( (void *)
                        CREDENTIAL_COUNT_ADDR) >= 17)
        return 1;
    return 0;
}


uint8_t id_exists(uint8_t *hashed_app_id){
    /*
    Get the number of credentials 
    to search only through written 
    eeprom.
    */
    uint8_t nb_credentials = eeprom_read_byte((void *)
                                     CREDENTIAL_COUNT_ADDR);
    
    /*
    EEPROM read into buf then compare 
    with hashed_app_id. Loop only
    on existing credentials.
    */
    uint8_t buf[HASHED_APP_ID_SIZE];
    for(uint8_t i = 0; i < nb_credentials; i++){
        eeprom_read_block(buf, (void *) 
                            (i*CREDENTIAL_SIZE
                                + KEY_SIZE),
                            HASHED_APP_ID_SIZE);
        if(memcmp(buf, hashed_app_id, HASHED_APP_ID_SIZE) == 0)
                    return i;
    }
    return ID_DOESNT_EXIST;
}

/*Searches an entry for hashed_app_id then*/
status_t add_credential(uint8_t *private_key, uint8_t 
                    *hashed_app_id, uint8_t *credential_id){

    /*get the number of credentials currently stored*/
    uint8_t nb_credentials = eeprom_read_byte((void *)
                                     CREDENTIAL_COUNT_ADDR);

    /*
    Checks if the id exists by simply going trough every stored id 
    and comparing them with hashed_app_id
    */
    uint8_t id_exists_at = id_exists(hashed_app_id);
        
    /*Checks if eeprom is full:
        -if eeprom is full, checks if the id to add
        is new, if yes, throw an error. If not 
        simply replace the old id credentials

        -if eeprom is not full, checks if id is new.
        If yes, write credentials after the last 
        credential entry.*/
    if(eeprom_is_full() && (id_exists_at == ID_DOESNT_EXIST)){
        return STATUS_ERR_STORAGE_FULL;
    } else if(id_exists_at == ID_DOESNT_EXIST){
        id_exists_at = nb_credentials;
        /*Update the credential count only if 
        id doesn't exist*/
        eeprom_write_byte((void *) CREDENTIAL_COUNT_ADDR,
                                                nb_credentials + 1);
    }

    /*Write new credential at the first unused adress
    or update an old credential at the old address*/
    eeprom_write_block(private_key, 
                        (void *)
                        (((int) id_exists_at)*CREDENTIAL_SIZE ),
                        KEY_SIZE);

    /*If hashed_app_id exists, doesn't overwrite the 
    old entry.*/
    eeprom_update_block(hashed_app_id, 
                        (void *)
                        (((int) id_exists_at)*CREDENTIAL_SIZE
                                + KEY_SIZE ),
                            HASHED_APP_ID_SIZE);

    eeprom_write_block(credential_id, 
                        (void *)
                        (((int) id_exists_at)*CREDENTIAL_SIZE
                                + KEY_SIZE
                                + HASHED_APP_ID_SIZE),
                            CREDENTIAL_ID_SIZE);

    return STATUS_OK;
}

/*Searches for hashed_app_id in the eeprom.
    -Returns STATUS_ERR_NOT_FOUND if hashed_app_id
    doesn't exist.

    -Fills private_key with the private_key corresponding
    to hashed_app_id in the eeprom.
    Same for credential_id.
    -Returns STATUS_OK if everything went right
*/
status_t get_credential(uint8_t *private_key,
         uint8_t *hashed_app_id, uint8_t *credential_id){

    uint8_t id_exists_at = id_exists(hashed_app_id);
    if(id_exists_at == ID_DOESNT_EXIST){
        return STATUS_ERR_NOT_FOUND;
    }

    eeprom_read_block(private_key, 
                        (void *)
                        ( id_exists_at*CREDENTIAL_SIZE ),
                        KEY_SIZE);

    eeprom_read_block(credential_id, 
                        (void *)
                        ( id_exists_at*CREDENTIAL_SIZE
                                + KEY_SIZE
                                + HASHED_APP_ID_SIZE),
                            CREDENTIAL_ID_SIZE);

    return STATUS_OK;
}