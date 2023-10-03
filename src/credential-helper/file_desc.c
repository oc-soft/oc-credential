#include "file_desc.h"

/**
 * increment reference count
 */
unsigned int 
file_desc_retain(
    file_desc* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = obj->vtbl->retain(obj);
    }
    return result;
}

/**
 * decrement reference count
 */
unsigned int 
file_desc_release(
    file_desc* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = obj->vtbl->release(obj);
    }
    return result;
}


/**
 * get file descriptor
 */
int 
file_desc_get_desc(
    file_desc* obj)
{
    int result;
    result = -1;
    if (obj) {
        result = obj->vtbl->get_desc(obj);
    }
    return result;
}

/**
 * close file descriptor
 */
int 
file_desc_close(
    file_desc* obj)
{
    int result;
    result = -1;
    if (obj) {
        result = obj->vtbl->close(obj);
    }
    return result;
}

/* vi: se ts=4 sw=4 et: */
