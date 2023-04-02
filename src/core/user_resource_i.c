#include "user_resource.h"
#include <stdlib.h>

/**
 * allocate heap memory
 */
void*
user_resource_i_alloc(
    size_t size)
{
    void* result;
    result = malloc(size);
    
    return result;
}

/**
 * free heap memory
 */
void
user_resource_i_free(
    void* obj)
{
    free(obj);
}

/* vi: se ts=4 sw=4 et: */
