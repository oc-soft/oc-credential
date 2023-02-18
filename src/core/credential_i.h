#ifndef __CREDENTIAL_I_H__
#define __CREDENTIAL_I_H__
#include <stddef.h>
#ifdef __cplusplus
#define _CREDENTIAL_I_ITFC_BEGIN extern "C" {
#define _CREDENTIAL_I_ITFC_END }
#else
#define _CREDENTIAL_I_ITFC_BEGIN 
#define _CREDENTIAL_I_ITFC_END
#endif

_CREDENTIAL_I_ITFC_BEGIN 


/**
 * allocate heap memory 
 */
void*
credential_i_alloc(
    size_t size);

/**
 * allocate heap memory 
 */
void*
credential_i_alloc_1(
    unsigned int size);


/**
 * free heap memory
 */
void
credential_i_free(
    void* obj);

/**
 * realocate heap object
 */
void*
credential_i_realloc(
    void* obj,
    size_t size);

/**
 * memory copy
 */
void*
credential_i_mem_copy(
    void* dst,
    const void* src,
    size_t size);

/**
 * memory copy
 */
void*
credential_i_mem_move(
    void* dst,
    const void* src,
    size_t size);

/**
 * duplicate string
 */
char*
credential_i_str_dup_0(
    const char* src,
    size_t length);

_CREDENTIAL_I_ITFC_BEGIN 

/* vi: se ts=4 sw=4 et: */
#endif
