#ifndef __TOKEN_GEN_UI_I_H__
#define __TOKEN_GEN_UI_I_H__

#include <stddef.h>

#ifdef __cplusplus
#define _TOKEN_GEN_UI_I_BEGIN extern "C" {
#define _TOKEN_GEN_UI_I_END }
#else
#define _TOKEN_GEN_UI_I_BEGIN 
#define _TOKEN_GEN_UI_I_END 
#endif

_TOKEN_GEN_UI_I_BEGIN 

/**
 * allocate heap memory
 */
void*
token_gen_ui_i_alloc(
    size_t size);

/**
 * free heap memory
 */
void
token_gen_ui_i_free(
    void* obj);

/**
 * copy memory
 */
void*
token_gen_ui_i_mem_copy(
    void* dst,
    const void* src,
    size_t size);
/**
 * move memory
 */
void*
token_gen_ui_i_mem_move(
    void* dst,
    const void* src,
    size_t size);


/**
 * reallocate memory
 */
void*
token_gen_ui_i_realloc(
    void* obj,
    size_t size);


/**
 * run token generator program
 */
int
token_gen_ui_i_run(
    const char* in_data,
    size_t in_data_size,
    const char* program,
    int argc,
    const char** argv,
    int std_in_fd,
    int std_out_fd,
    int std_err_fd,
    char** out_str,
    char** err_str);



_TOKEN_GEN_UI_I_END 
/* vi: se ts=4 sw=4 et: */
#endif
