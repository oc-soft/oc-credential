#include "ui_token_gen_i.h"
#include <stdlib.h>

/**
 * allocate heap object
 */
void*
ui_token_gen_i_alloc(
    size_t size)
{
    return malloc(size);
}

/**
 * free heap object
 */
void
ui_token_gen_i_free(
    void* object)
{
    free(object);
}
/* vi: se ts=4 sw=4 et: */
