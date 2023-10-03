#include "token_gen_ui_i.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "fd_io.h"
#include "logging.h"

/**
 * allocate heap memory
 */
void*
token_gen_ui_i_alloc(
    size_t size)
{
    return malloc(size);
}

/**
 * free heap memory
 */
void
token_gen_ui_i_free(
    void* obj)
{
    free(obj);
}

/**
 * copy memory
 */
void*
token_gen_ui_i_mem_copy(
    void* dst,
    const void* src,
    size_t size)
{
    return memcpy(dst, src, size);
}

/**
 * move memory
 */
void*
token_gen_ui_i_mem_move(
    void* dst,
    const void* src,
    size_t size)
{
    return memmove(dst, src, size);
}


/**
 * reallocate memory
 */
void*
token_gen_ui_i_realloc(
    void* obj,
    size_t size)
{
    return realloc(obj, size);
}




/* vi: se ts=4 sw=4 et: */
