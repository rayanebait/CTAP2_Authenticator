#ifndef MANAGE_KEYS_H
#define MANAGE_KEYS_H

#include <stdint.h>
#include "error.h"

/*Data:
    -1024 bytes of EEPROM
    -priv_key+sha1(app_id)+credential_id = 57 bytes
    -1024/57 = 17.964...
So at most 17 keys
*/

/*Store the number of keys stored at the last eeprom byte*/
#define CREDENTIAL_COUNT_ADDR E2END

/*key is 21 bytes, sha1(app_id) is 20 bytes, credential_id is 16 bytes*/
#define CREDENTIAL_SIZE 57 
#define KEY_SIZE 21
#define HASHED_APP_ID_SIZE 20
#define CREDENTIAL_ID_SIZE 16

int eeprom_is_full();

uint8_t id_exists(uint8_t *hashed_app_id);

status_t add_credential(uint8_t *private_key, uint8_t 
                    *hashed_app_id, uint8_t *credential_id);

status_t get_credential(uint8_t *private_key, uint8_t
                    *hashed_app_id, uint8_t *credential_id);

#endif