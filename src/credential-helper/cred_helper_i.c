#include "cred_helper_i.h"

#include <string.h>
#include <stdlib.h>

/**
 * allocate heap object
 */
void*
cred_helper_i_alloc(
    size_t size)
{
    return malloc(size);
}

/**
 * realocate heap object
 */
void*
cred_helper_i_realloc(
    void* obj,
    size_t size)
{
    return realloc(obj, size);
}

/**
 * free allocated object
 */
void
cred_helper_i_free(
    void* obj)
{
    free(obj); 
}

/**
 * memory copy
 */
void*
cred_helper_i_mem_copy(
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
cred_helper_i_mem_move(
    void* dst,
    const void* src,
    size_t size)
{
    return memmove(dst, src, size);
}

/**
 * duplicate string
 */
char*
cred_helper_i_strdup(
    const char* str)
{
    char* result;
    if (str) {
        result = strdup(str);
    } else {
        result = NULL;
    }
    return result;
}

/* vi: se ts=4 sw=4 et: */
