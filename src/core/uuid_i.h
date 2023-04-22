#ifndef _UUID_I_H_
#define _UUID_I_H_

#include <stddef.h>

#ifdef __cplusplus
#define _UUID_I_ITFC_BEGIN extern "C" {
#define _UUID_I_ITFC_END }
#else
#define _UUID_I_ITFC_BEGIN 
#define _UUID_I_ITFC_END
#endif

_UUID_I_ITFC_BEGIN 

/**
 * generate uuid
 */
int
uuid_i_generate(
    char** uuid,
    void* (*alloc)(size_t));


_UUID_I_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif
