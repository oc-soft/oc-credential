#include "lmd_ui.h"
#include <stdio.h>
#include <errno.h>

#include "lmd_i.h"
#include "fd_io.h"

/**
 * posix limted device user interface
 */
typedef struct _lmd_ui_posix lmd_ui_posix;

/**
 * posix limted device user interface
 */
struct _lmd_ui_posix  {
    /**
     * virtual function table
     */
    const lmd_ui_vtbl* vtbl;


    /**
     * reference count
     */
    unsigned int ref_count;

    /**
     * terminal input
     */
    FILE* terminal_in;
    

    /**
     * the flag to close terminal input
     */
    int close_input;
};

/**
 * close input
 */
static int
lmd_ui_posix_close_input(
    lmd_ui_posix* obj);

/**
 * increment reference count
 */
static unsigned int 
lmd_ui_posix_retain(
    lmd_ui_posix* obj);

/**
 * decrement reference count
 */
static unsigned int 
lmd_ui_posix_release(
    lmd_ui_posix* obj);

/**
 * get character
 */
static int 
lmd_ui_posix_get_char(
    lmd_ui_posix* obj);


const static lmd_ui_vtbl vtbl = {
    (unsigned int (*)(lmd_ui*))lmd_ui_posix_retain,
    (unsigned int (*)(lmd_ui*))lmd_ui_posix_release,
    (int (*)(lmd_ui*))lmd_ui_posix_get_char,
};
 
lmd_ui*
lmd_ui_create()
{
    lmd_ui_posix* result;
    FILE* term_in;
    int close_fs;
    int state;
    result = NULL;
    term_in = NULL;
    close_fs = 0;
    state = 0;

    if (fd_io_isatty(fd_io_fileno(stdin))) {
        term_in = stdin;
        close_fs = 0;
    } else {
        term_in = fopen("/dev/tty", "r");
        state = term_in ? 0 : -1;
        if (state == 0) {
            close_fs = 1;
        }
    }
    if (state == 0) {
        result = (lmd_ui_posix*)lmd_i_alloc(sizeof(lmd_ui_posix));
        state = result ? 0 : -1;
    }

    if (state == 0) {
        result->vtbl = &vtbl;
        result->ref_count = 1;
        result->terminal_in = term_in;
        result->close_input = close_fs;
        term_in = NULL;
    }
    if (term_in && close_fs) {
        fclose(term_in);
    }
    return (lmd_ui*)result;
}


/**
 * increment reference count
 */
static unsigned int 
lmd_ui_posix_retain(
    lmd_ui_posix* obj)
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
static unsigned int 
lmd_ui_posix_release(
    lmd_ui_posix* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = --obj->ref_count;
        if (result == 0) {
            lmd_ui_posix_close_input(obj);
            lmd_i_free(obj);
        }
    } else {
        errno = EINVAL;
    }
    return result;
}


/**
 * get character
 */
static int 
lmd_ui_posix_get_char(
    lmd_ui_posix* obj)
{
    int result;
    result = EOF;
    if (obj) {
        result = getc(obj->terminal_in);
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * close input
 */
static int
lmd_ui_posix_close_input(
    lmd_ui_posix* obj)
{
    int result;
    result = 0;
    if (obj) {
        if (obj->close_input && obj->terminal_in) {
            fclose(obj->terminal_in);
            obj->terminal_in = NULL;
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}
/* vi: se ts=4 sw=4 et: */
