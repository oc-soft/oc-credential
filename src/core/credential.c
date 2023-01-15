#include "config.h"
#include "credential.h"
#include <json-c/json.h>
#ifdef HAVE_EMSCRIPTEN_H
#include <emscripten.h>
#endif
#include "credential_desc.h"
#include "credential_i.h"
#include "credential_storage.h"
#include "wasm_i.h"


/**
 * get credential
 */
WASM_EXPORT
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
        char* password;
        password = NULL;
        result = credential_storage_find_password(cred_desc->protocol,
            cred_desc->host, cred_desc->path, cred_desc->username, &password);
        if (result == 0) {
            if (password) {
                result = credential_desc_set_password(cred_desc, password);
                credential_storage_free_object(password);
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
WASM_EXPORT
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
        result = credential_storage_remove_password(cred_desc->protocol,
            cred_desc->host, cred_desc->path, cred_desc->username);
    }
    credential_desc_free(cred_desc);

    return result;
}

/**
 * store credential
 */
WASM_EXPORT
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
        result = credential_storage_store_password(
            cred_desc->protocol, cred_desc->host,
            cred_desc->path, cred_desc->username, cred_desc->password);
    }
    credential_desc_free(cred_desc);

    return result;
}
    
/**
 * free heap object which you get from this interface.
 */
WASM_EXPORT
void
credential_free(
    void* obj)
{
    credential_i_free(obj);
}

/* vi: se ts=4 sw=4 et: */
