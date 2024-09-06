#ifndef __LMD_I_H__
#define __LMD_I_H__

#include <stddef.h>
#include "lmd.h"

#ifdef __cplusplus
#define _LMD_I_ITFC_BEGIN extern "C" {
#define _LMD_I_ITFC_END }
#else
#define _LMD_I_ITFC_BEGIN 
#define _LMD_I_ITFC_END 
#endif

_LMD_I_ITFC_BEGIN 

/**
 * allocate heap object
 */
void*
lmd_i_alloc(
    size_t size);

/**
 * allocate heap object
 */
void*
lmd_i_alloc_1(
    unsigned int size);

/**
 * realocate heap object
 */
void*
lmd_i_realloc(
    void* obj,
    size_t size);
/**
 * free allocated object
 */
void
lmd_i_free(
    void* obj);
/**
 * memory copy
 */
void*
lmd_i_mem_copy(
    void* dst,
    const void* src,
    size_t size);
/**
 * memory copy
 */
void*
lmd_i_mem_move(
    void* dst,
    const void* src,
    size_t size);

_LMD_I_ITFC_END 

/* vi: se ts=4 sw=4 et: */
#endif
