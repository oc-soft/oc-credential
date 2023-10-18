#ifndef __LMD_UI_H__
#define __LMD_UI_H__

#include <stddef.h>
#include "lmd_types.h"

#ifdef __cplusplus
#define _LMD_UI_ITFC_BEGIN extern "C" {
#define _LMD_UI_ITFC_END }
#else
#define _LMD_UI_ITFC_BEGIN 
#define _LMD_UI_ITFC_END 
#endif

_LMD_UI_ITFC_BEGIN 

/**
 * user interface for limited device
 */
typedef struct _lmd_ui lmd_ui;

/**
 * user interface virtual function table for limited device
 */
typedef struct _lmd_ui_vtbl lmd_ui_vtbl;


/**
 * user interface virtual function table for limited device
 */
struct _lmd_ui_vtbl {
    /**
     * increment reference count
     */
    unsigned int (*retain)(lmd_ui*);

    /**
     * decrement reference count
     */
    unsigned int (*release)(lmd_ui*);

    /**
     * get character
     */
    int (*get_char)(lmd_ui*);

};

/**
 * user interface for limited device
 */
struct _lmd_ui {
    /**
     * virtual function table
     */
    const lmd_ui_vtbl* vtbl;
};

/**
 * create limted device user interface.
 */
lmd_ui*
lmd_ui_create();

/**
 * increment reference count
 */
unsigned int 
lmd_ui_retain(
    lmd_ui* obj);

/**
 * decrement reference count
 */
unsigned int 
lmd_ui_release(
    lmd_ui* obj);

/**
 * get character
 */
int 
lmd_ui_get_char(
    lmd_ui* obj);

_LMD_UI_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif

