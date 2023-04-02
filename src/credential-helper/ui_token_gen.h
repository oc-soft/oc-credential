#ifndef __UI_TOKEN_GEN_H__
#define __UI_TOKEN_GEN_H__

#include <stddef.h>
#include "ui_token_gen_types.h"

#ifdef __cplusplus
#define _UI_TOKEN_GEN_ITFC_BEGIN extern "C" {
#define _UI_TOKEN_GEN_ITFC_END }
#else
#define _UI_TOKEN_GEN_ITFC_BEGIN 
#define _UI_TOKEN_GEN_ITFC_END 
#endif

_UI_TOKEN_GEN_ITFC_BEGIN 

/**
 * create token generator user interface
 */
ui_token_gen*
ui_token_gen_create();

/**
 * increment reference count
 */
unsigned int
ui_token_gen_retain(
    ui_token_gen* obj);

/**
 * decrement reference count
 */
unsigned int
ui_token_gen_release(
    ui_token_gen* obj);


/**
 * create token
 */
int
ui_token_gen_create_token(
    ui_token_gen* obj,
    const char* descreptor,
    size_t descptor_length,
    char** response,
    size_t* descriptor_length);


/**
 * free object created in this interface.
 */
void
ui_token_gen_free(
    void* obj);

_UI_TOKEN_GEN_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif
