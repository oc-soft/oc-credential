#include "lmd_i.h"

#include <string.h>
#include <stdlib.h>

/**
 * allocate heap object
 */
void*
lmd_i_alloc(
    size_t size)
{
    return malloc(size);
}

/**
 * realocate heap object
 */
void*
lmd_i_realloc(
    void* obj,
    size_t size)
{
    return realloc(obj, size);
}

/**
 * free allocated object
 */
void
lmd_i_free(
    void* obj)
{
    free(obj); 
}

/**
 * memory copy
 */
void*
lmd_i_mem_copy(
    void* dst,
    const void* src,
    size_t size)
{
    return memcpy(dst, src, size);
}
/**
 * memory copy
 */
void*
lmd_i_mem_move(
    void* dst,
    const void* src,
    size_t size)
{
    return memmove(dst, src, size);
}

/* vi: se ts=4 sw=4 et: */
