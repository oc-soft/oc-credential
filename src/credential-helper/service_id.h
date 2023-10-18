#ifndef __SERVICE_ID_H__
#define __SERVICE_ID_H__

#include <stddef.h>
#ifdef __cplusplus
#define _SERVICE_ID_ITFC_BEGIN extern "C" {
#define _SERVICE_ID_ITFC_END }
#else
#define _SERVICE_ID_ITFC_BEGIN 
#define _SERVICE_ID_ITFC_END
#endif

_SERVICE_ID_ITFC_BEGIN 

/**
 * get service names
 */
int
service_id_get_names(
    size_t* key_size,
    const char*** service_names);

/**
 * get service id
 */
int
service_id_get_id(
    const char* service,
    char ** id,
    void* (*mem_alloc)(size_t));


_SERVICE_ID_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif

