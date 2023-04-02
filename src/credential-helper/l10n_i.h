#ifndef __L10N_I_H__
#define __L10N_I_H__

#include <stddef.h>
#include <stdio.h>
#ifdef __cplusplus
#define _L10N_I_BEGIN extern "C" {
#define _L10N_I_END }
#else
#define _L10N_I_BEGIN 
#define _L10N_I_END 
#endif

_L10N_I_BEGIN 

/**
 * get file path for locale path
 */
char* 
l10n_i_get_locpath_file_path();

/**
 * read locale path from file
 */
char*
l10n_i_read_locpath_from_file(
    const char* file);

/**
 * you get locale path string if the path string is not empty according to 
 * l10n locpath specification.
 */
int
l10n_i_find_path_range(
    const char* path_str,
    size_t* start,
    size_t* end);

/**
 * allocate memory
 */
void*
l10n_i_alloc(
    size_t size);

/**
 * free heap memory allocated by this api.
 */
void
l10n_i_free(
    void* obj);


/**
 * open file stream
 */
FILE*
l10n_i_open_fs(
    const char* file_path,
    const char* mode);

/**
 * close file stream
 */
int
l10n_i_close_fs(
    FILE* fs);

/**
 * get line from file stream
 */
char*
l10n_i_gets(
    char* buffer,
    int buffer_size,
    FILE* fs);


_L10N_I_END 
/* vi: se ts=4 sw=4 et: */
#endif
