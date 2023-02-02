#ifndef __CRED_HELPER_I_H__
#define __CRED_HELPER_I_H__

#include <stddef.h>

#ifdef __cplusplus
#define _CRED_HELPER_I_ITFC_BEGIN extern "C" {
#define _CRED_HELPER_I_ITFC_END }
#else
#define _CRED_HELPER_I_ITFC_BEGIN 
#define _CRED_HELPER_I_ITFC_END 
#endif

_CRED_HELPER_I_ITFC_BEGIN 

/**
 * allocate heap object
 */
void*
cred_helper_i_alloc(
    size_t size);

/**
 * realocate heap object
 */
void*
cred_helper_i_realloc(
    void* obj,
    size_t size);
/**
 * free allocated object
 */
void
cred_helper_i_free(
    void* obj);
/**
 * memory copy
 */
void*
cred_helper_i_mem_copy(
    void* dst,
    const void* src,
    size_t size);
/**
 * memory copy
 */
void*
cred_helper_i_mem_move(
    void* dst,
    const void* src,
    size_t size);


/**
 * duplicate string
 */
char*
cred_helper_i_strdup(
    const char* src);

_CRED_HELPER_I_ITFC_END 

/* vi: se ts=4 sw=4 et: */
#endif
