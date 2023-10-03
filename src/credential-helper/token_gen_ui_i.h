#ifndef __TOKEN_GEN_UI_I_H__
#define __TOKEN_GEN_UI_I_H__

#include <stddef.h>
#include "buffer/char_buffer.h"
#include "file_desc.h"
#ifdef _WIN32
#include <windows.h>
#endif

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
    file_desc* std_in_fd,
    file_desc* std_out_fd,
    file_desc* std_err_fd,
    char** out_str,
    char** err_str);


/** 
 * read fd into buffer
 */
int
token_gen_ui_i_read_fd_into_buffer(
    int fd,
    char* tmp_buffer,
    size_t tmp_buffer_size,
    buffer_char_buffer* buffer); 

_TOKEN_GEN_UI_I_END 
/* vi: se ts=4 sw=4 et: */
#endif
