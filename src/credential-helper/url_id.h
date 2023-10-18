#ifndef __URL_ID_H__
#define __URL_ID_H__

#include <stddef.h>

#ifdef __cplusplus
#define _URL_ID_ITFC_BEGIN extern "C" {
#define _URL_ID_ITFC_END }
#else
#define _URL_ID_ITFC_BEGIN 
#define _URL_ID_ITFC_END 
#endif

_URL_ID_ITFC_BEGIN 

/**
 * get url id
 */
int
url_id_get_id(
    const char* protocol,
    const char* host,
    const char* path,
    char** id,
    void *(*mem_alloc)(size_t));


/**
 * free memory block
 */
void
url_id_free(
    void* obj);

_URL_ID_ITFC_END 
/* vi: se ts=4 sw=4 et: */
#endif
