#include "lmd_ui.h"
#include <stdio.h>
#include <errno.h>

/**
 * increment reference count
 */
unsigned int 
lmd_ui_retain(
    lmd_ui* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = obj->vtbl->retain(obj);
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * decrement reference count
 */
unsigned int 
lmd_ui_release(
    lmd_ui* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = obj->vtbl->release(obj);
    } else {
        errno = EINVAL;
    }
    return result;
}


/**
 * get character
 */
int 
lmd_ui_get_char(
    lmd_ui* obj)
{
    int result;
    result = EOF;
    if (obj) {
        result = obj->vtbl->get_char(obj);
    } else {
        errno = EINVAL;
    }
    return result;
}


/* vi: se ts=4 sw=4 et: */
