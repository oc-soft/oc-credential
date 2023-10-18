#ifndef __SERVICE_URL_CLIENT_H__
#define __SERVICE_URL_CLIENT_H__

#include <stddef.h>

#ifdef __cplusplus
#define _SERVICE_URL_CLIENT_ITFC_BEGIN extern "C" {
#define _SERVICE_URL_CLIENT_ITFC_END }
#else
#define _SERVICE_URL_CLIENT_ITFC_BEGIN 
#define _SERVICE_URL_CLIENT_ITFC_END
#endif

_SERVICE_URL_CLIENT_ITFC_BEGIN 

/**
 * get client from sevice id and url id
 */
int
service_url_client_get_client(
    const char* service_id,
    const char* url_id,
    char** client,
    void* (*mem_alloc)(size_t));


/**
 * free memory
 */
void
service_url_client_free(
    void* mem);

_SERVICE_URL_CLIENT_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif
