#include "credential_i.h"
#include <string.h>
#include <stdlib.h>
/**
 * duplicate string
 */
char*
credential_i_str_dup_0(
    const char* src,
    size_t length)
{
    char* result;
    result = credential_i_alloc(length + 1);

    if (result) {
        memcpy(result, src, length);
        result[length] = '\0';
    }
    return result;
}


/**
 * allocate heap memory 
 */
void*
credential_i_alloc(
    size_t size)
{
    return malloc(size);
}

/**
 * allocate heap memory 
 */
void*
credential_i_alloc_1(
    unsigned int size)
{
    return credential_i_alloc(size);
}

/**
 * realocate heap object
 */
void*
crendential_i_realloc(
    void* obj,
    size_t size)
{
    return realloc(obj, size);
}


/**
 * free heap memory
 */
void
credential_i_free(
    void* obj)
{
    free(obj);
}

/**
 * memory copy
 */
void*
credential_i_mem_copy(
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
credential_i_mem_move(
    void* dst,
    const void* src,
    size_t size)
{
    return memmove(dst, src, size);
}


/* vi: se ts=4 sw=4 et: */
