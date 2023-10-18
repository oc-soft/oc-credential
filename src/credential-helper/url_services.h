#ifndef __URL_SERVICES_H__
#define __URL_SERVICES_H__
#include <stddef.h>

#ifdef __cplusplus
#define _URL_SERVICES_ITFC_BEGIN extern "C" {
#define _URL_SERVICES_ITFC_END }
#else
#define _URL_SERVICES_ITFC_BEGIN 
#define _URL_SERVICES_ITFC_END
#endif

_URL_SERVICES_ITFC_BEGIN 

/**
 * get services
 */
int
url_services_get(
    const char* protocol,
    const char* host,
    const char* path,
    const char*** services,
    size_t* services_size);


_URL_SERVICES_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif
