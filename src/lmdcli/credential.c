#include "credential.h"
#include <json-c/json.h>
#include "credential_desc.h"
#include "credential_i.h"
#include "credential_storage.h"

/**
 * get credential
 */
int
credential_get(
    const char* descriptor,
    size_t descriptor_length,
    char** descriptor_res,
    size_t* descriptor_res_length)
{
    int result;
    credential_desc* cred_desc;
    result = 0;

    cred_desc = credential_desc_decode(descriptor, descriptor_length);
    result = cred_desc ? 0 : -1;
    if (result == 0) {
        char* token;
        token = NULL;
        result = credential_storage_find_token(cred_desc->protocol,
            cred_desc->host, cred_desc->path, cred_desc->username, &token);
        if (result == 0) {
            if (token) {
                result = credential_desc_set_password(cred_desc, token);
                credential_storage_free_object(token);
            } 
        }
        if (result == 0) {
            result = credential_desc_encode(
                cred_desc, descriptor_res, descriptor_res_length);
        }
    }
    credential_desc_free(cred_desc);

    return result;
}

/**
 * erase credential
 */
int
credential_erase(
    const char* descriptor,
    size_t descriptor_length)
{
    int result;
    credential_desc* cred_desc;
    result = 0;

    cred_desc = credential_desc_decode(descriptor, descriptor_length);
    result = cred_desc ? 0 : -1;
    if (result == 0) {
        result = credential_storage_remove_token(cred_desc->protocol,
            cred_desc->host, cred_desc->path, cred_desc->username);
    }
    credential_desc_free(cred_desc);

    return result;
}

/**
 * store credential
 */
int
credential_store(
    const char* descriptor,
    size_t descriptor_length)
{
    int result;
    credential_desc* cred_desc;
    result = 0;

    cred_desc = credential_desc_decode(descriptor, descriptor_length);
    result = cred_desc ? 0 : -1;
    if (result == 0) {
        result = credential_storage_store_token(
            cred_desc->protocol, cred_desc->host,
            cred_desc->path, cred_desc->username, cred_desc->password);
    }
    credential_desc_free(cred_desc);

    return result;
}
    
/**
 * free heap object which you get from this interface.
 */
void
credential_free(
    void* obj)
{
    credential_i_free(obj);
}

/* vi: se ts=4 sw=4 et: */
