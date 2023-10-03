#include "std_file_desc.h"
#include <stdlib.h>
#include "fd_io.h"


/**
 * standard file descriptor object
 */
typedef struct _std_file_desc std_file_desc;


/**
 * standard file descriptor object
 */
struct _std_file_desc 
{
    /**
     * function table
     */
    const file_desc_vtbl* vtbl;
    /**
     * reference count
     */
    unsigned int ref_count;

    /**
     * file descriptor
     */
    int fd;

    /**
     * closed status. it is available when file descriptor is closed.
     */
    int closed_status;

    /**
     * flag whether close or not
     */
    int closed; 
};

/**
 * increment reference count
 */
static unsigned int 
std_file_desc_retain(
    std_file_desc* obj);

/**
 * decrement reference count
 */
static unsigned int 
std_file_desc_release(
    std_file_desc* obj);


/**
 * get file descriptor
 */
static int 
std_file_desc_get_desc(
    std_file_desc* obj);

/**
 * close file descriptor
 */
static int
std_file_desc_close(
    std_file_desc* obj);

/**
 * file descritor virtual function table
 */
const static file_desc_vtbl vtbl = {
    (unsigned int (*)(file_desc*)) std_file_desc_retain,
    (unsigned int (*)(file_desc*)) std_file_desc_release,
    (int (*)(file_desc*)) std_file_desc_get_desc,
    (int (*)(file_desc*)) std_file_desc_close
};

/**
 * create file descritor object
 */
file_desc*
std_file_desc_create(
    int fd)
{
    std_file_desc* result;
    result = (std_file_desc*)malloc(sizeof(std_file_desc));
    if (result) {
        result->vtbl = &vtbl;
        result->ref_count = 1;
        result->fd = fd;
        result->closed_status = -1;
        result->closed = 0;
    }
    return (file_desc*)result;
}

/**
 * increment reference count
 */
static unsigned int 
std_file_desc_retain(
    std_file_desc* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = ++obj->ref_count;
    }
    return result;
}

/**
 * decrement reference count
 */
static unsigned int 
std_file_desc_release(
    std_file_desc* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = --obj->ref_count;
        if (result == 0) {
            if (!obj->closed) {
                std_file_desc_close(obj);
            }
            free(obj);
        }
    }
    return result;
}


/**
 * get file descriptor
 */
static int 
std_file_desc_get_desc(
    std_file_desc* obj)
{
    int result;
    result = -1;
    if (obj) {
        result = obj->fd;
    }
    return result;
}


/**
 * close file descriptor
 */
static int
std_file_desc_close(
    std_file_desc* obj)
{
    int result;
    result = -1;
    if (obj) {
        if (!obj->closed) {
            obj->closed = 1;

            result = fd_io_close(obj->fd);
            obj->closed_status = result;
        } else {
            result = obj->closed_status;
        }
    }
    return result;
}


/* vi: se ts=4 sw=4 et: */
