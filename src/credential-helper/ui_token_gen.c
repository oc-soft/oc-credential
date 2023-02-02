#include "ui_token_gen.h"
#include "ui_token_gen_i.h"
#include <errno.h>

/**
 * increment reference count
 */
unsigned int
ui_token_gen_retain(
    ui_token_gen* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = ++obj->ref_count;
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * decrement reference count
 */
unsigned int
ui_token_gen_release(
    ui_token_gen* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = --obj->ref_count; 
    } else {
        errno = EINVAL;
    }
    return result;
}


/**
 * free object created in this interface.
 */
void
ui_token_gen_free(
    void* obj)
{
    ui_token_gen_i_free(obj);
}

/* vi: se ts=4 sw=4 et: */
