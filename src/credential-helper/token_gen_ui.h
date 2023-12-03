#ifndef __TOKEN_GEN_UI_H__
#define __TOKEN_GEN_UI_H__
#include <stddef.h>

#ifdef __cplusplus
#define _TOKEN_GEN_UI_BEGIN extern "C" {
#define _TOKEN_GEN_UI_END }
#else
#define _TOKEN_GEN_UI_BEGIN 
#define _TOKEN_GEN_UI_END 
#endif

_TOKEN_GEN_UI_BEGIN 

/**
 * run token generator and get token
 */
int
token_gen_ui_run(
    const char* in_data,
    size_t in_data_size,
    const char* service,
    char** token);

/**
 * free object from this interface
 */
void
token_gen_ui_free(
    void* obj);

_TOKEN_GEN_UI_END
/* vi: se ts=4 sw=4 et: */
#endif
