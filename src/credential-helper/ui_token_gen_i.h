#ifndef __UI_TOKEN_GEN_I_H__
#define __UI_TOKEN_GEN_I_H__
#include <stddef.h>
#include "ui_token_gen_types.h"
#ifdef __cplusplus
#define _UI_TOKEN_GEN_I_ITFC_BEGIN extern "C" {
#define _UI_TOKEN_GEN_I_ITFC_END }
#else
#define _UI_TOKEN_GEN_I_ITFC_BEGIN 
#define _UI_TOKEN_GEN_I_ITFC_END 
#endif

_UI_TOKEN_GEN_I_ITFC_BEGIN 

struct _ui_token_gen
{
    /**
     * reference count
     */
    unsigned int ref_count;


    /**
     * service
     */
    char* service;
};

/**
 * allocate heap object
 */
void*
ui_token_gen_i_alloc(
    size_t size);

/**
 * free heap object
 */
void
ui_token_gen_i_free(
    void* object);



_UI_TOKEN_GEN_I_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif

