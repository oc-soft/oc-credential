#ifndef __PATH_H__
#define __PATH_H__

#include <unistd.h>
#include <stddef.h>

#ifdef __cplusplus
#define _PATH_ITFC_BEGIN extern "C" {
#define _PATH_ITFC_END }
#else
#define _PATH_ITFC_BEGIN 
#define _PATH_ITFC_END 
#endif

_PATH_ITFC_BEGIN 

/**
 * join path
 */
char*
path_join(
    const char* path_1,
    const char* path_2,
    void* (*alloc_mem)(size_t),
    void (*free_mem)(void*));

/**
 * free heap object
 */
void
path_free(
    void* obj);

_PATH_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif
