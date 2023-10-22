#ifndef __SERVICE_URL_DEVICE_USER_CODE_PARAM_H__
#define __SERVICE_URL_DEVICE_USER_CODE_PARAM_H__

#include <stddef.h>

#ifdef __cplusplus
#define _SERVICE_URL_DEVICE_USER_CODE_PARAM_ITFC_BEGIN extern "C" {
#define _SERVICE_URL_DEVICE_USER_CODE_PARAM_ITFC_END }
#else
#define _SERVICE_URL_DEVICE_USER_CODE_PARAM_ITFC_BEGIN 
#define _SERVICE_URL_DEVICE_USER_CODE_PARAM_ITFC_END
#endif

_SERVICE_URL_DEVICE_USER_CODE_PARAM_ITFC_BEGIN 

/**
 * get secret from sevice id and url id
 */
int
service_url_device_user_code_param_create(
    const char* service_id,
    const char* url_id,
    const char* client_id,
    char** param,
    void* (*mem_alloc)(size_t));

/**
 * free memory
 */
void
service_url_device_user_code_param_free(
    void* mem);


_SERVICE_URL_DEVICE_USER_CODE_PARAM_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif
