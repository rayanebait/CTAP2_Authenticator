#include "GetAssertion.h"

#include <stdio.h>

#include "uECC.h"
#include "error.h"
#include "manage_credentials.h"
#include "get_approval.h"

#define SIGNATURE_SIZE 40
#define CLIENT_DATA_HASH_SIZE 20

status_t GetAssertion(uint8_t *hashed_app_id, 
                        uint8_t *client_data_hash){
    uint8_t private_key[KEY_SIZE];
    uint8_t credential_id[CREDENTIAL_ID_SIZE];

    /*Fills private_key and credential_id 
    if an entry for hashed_app_id exists. 
    Otherwise returns an error*/
    if(get_credential(private_key, hashed_app_id,
                credential_id) == STATUS_ERR_NOT_FOUND)
        return STATUS_ERR_NOT_FOUND;

    /*Toggles the led every 500ms until button 
    is pressed or until 10s timeout*/
    if(get_approval() == STATUS_ERR_APPROVAL)
        return STATUS_ERR_APPROVAL;

    /*Signs client_data_hash using the private_key 
    associated to hashed_app_id. Then fills signature 
    with the signature.*/
    uint8_t signature[SIGNATURE_SIZE];
    if(!uECC_sign(private_key, client_data_hash,
         CLIENT_DATA_HASH_SIZE, signature, uECC_secp160r1()))
            return STATUS_ERR_CRYPTO_FAILED;


    uint8_t status = STATUS_OK;

    /*Sends STATUS_OK then credential_id
    then the signature of the challenges.*/
    fwrite(&status, 1,1, stdout);
    fwrite(credential_id, 1, 
        CREDENTIAL_ID_SIZE, stdout);
    fwrite(signature, 1,
            SIGNATURE_SIZE, stdout);

    return STATUS_OK;
}