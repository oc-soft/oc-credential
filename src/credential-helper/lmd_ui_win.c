#include "lmd_ui.h"
#include <stdio.h>
#include <errno.h>
#include <conio.h>

#include "lmd_i.h"

/**
 * windows limted device user interface
 */
typedef struct _lmd_ui_win lmd_ui_win;

/**
 * windows limted device user interface
 */
struct _lmd_ui_win  {
    /**
     * virtual function table
     */
    const lmd_ui_vtbl* vtbl;


    /**
     * reference count
     */
    unsigned int ref_count;

};

/**
 * increment reference count
 */
static unsigned int 
lmd_ui_win_retain(
    lmd_ui_win* obj);

/**
 * decrement reference count
 */
static unsigned int 
lmd_ui_win_release(
    lmd_ui_win* obj);

/**
 * get character
 */
static int 
lmd_ui_win_get_char(
    lmd_ui* obj);


const static lmd_ui_vtbl vtbl = {
    (unsigned int (*)(lmd_ui*))lmd_ui_win_retain,
    (unsigned int (*)(lmd_ui*))lmd_ui_win_release,
    (int  (*)(lmd_ui))lmd_ui_win_get_char,
};
 
lmd_ui*
lmd_ui_create()
{
    lmd_ui_win* result;
    int state;
    result = NULL;
    state = 0;

    if (state == 0) {
        result = (lmd_ui_win*)lmd_i_alloc(sizeof(lmd_ui_win));
        state = result ? 0 : -1;
    }

    if (state == 0) {
        result->vtbl = &vtbl;
        result->ref_count = 1;
    }
    return (lmd_ui*)result;
}


/**
 * increment reference count
 */
static unsigned int 
lmd_ui_win_retain(
    lmd_ui_win* obj)
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
lmd_ui_win_release(
    lmd_ui_win* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = --obj->ref_count
        if (result == 0) {
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
lmd_ui_win_get_char(
    lmd_ui* obj);
{
    int result;
    result = EOF;
    if (obj) {
        result = _getche(obj->term_in);
    } else {
        errno = EINVAL;
    }
    return result;
}
/* vi: se ts=4 sw=4 et: */
