#ifndef __CREDENTIAL_H__
#define __CREDENTIAL_H__

#include <stddef.h>

#ifdef __cplusplus
#define _CREDENTIAL_ITFC_BEGIN extern "C" {
#define _CREDENTIAL_ITFC_END }
#else
#define _CREDENTIAL_ITFC_BEGIN 
#define _CREDENTIAL_ITFC_END
#endif

/**
 * get credential
 */
int
credential_get(
    const char* descriptor,
    size_t descriptor_length,
    char** descriptor_res,
    size_t* descriptor_res_length);

/**
 * erase credential
 */
int
credential_erase(
    const char* descriptor,
    size_t descriptor_length);

/**
 * store credential
 */
int
credential_store(
    const char* descriptor,
    size_t descriptor_length);

/**
 * free heap object which you get from this interface.
 */
void
credential_free(
    void* obj);
    
/* vi: se ts=4 sw=4 et: */
#endif
