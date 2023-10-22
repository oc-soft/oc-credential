#include "lmd_oauth_token_param_creator.h"
#include <errno.h>

/**
 * increment reference count
 */
unsigned int 
lmd_oauth_token_param_creator_retain(
    lmd_oauth_token_param_creator* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = obj->vtbl->retain(obj);
    } else {
        errno = EINVAL;
    }
    return result;  
}

/**
 * decrement reference count
 */
unsigned int
lmd_oauth_token_param_creator_release(
    lmd_oauth_token_param_creator* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = obj->vtbl->release(obj);
    } else {
        errno = EINVAL;
    }
    return result;  
}

/**
 * create parameter
 */
int
lmd_oauth_token_param_creator_create_param(
    lmd_oauth_token_param_creator* obj,
    const char* client_id,
    const char* device_code,
    const char* client_secret,
    char** param,
    void* (*mem_alloc)(size_t))
{
    int result;
    result = 0;
    if (obj) {
        result = obj->vtbl->create_param(obj, 
            client_id, device_code, client_secret, param, mem_alloc);
    } else {
        errno = EINVAL;
        result = -1;
    }
    return result;
}
/* vi: se ts=4 sw=4 et: */
