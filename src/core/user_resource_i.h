#ifndef __USER_RESOURCE_I_H__
#define __USER_RESOURCE_I_H__

#include <stddef.h>

#ifdef __cplusplus
#define _USER_RESOURCE_I_ITFC_BEGIN extern "C" {
#define _USER_RESOURCE_I_ITFC_END }
#else
#define _USER_RESOURCE_I_ITFC_BEGIN 
#define _USER_RESOURCE_I_ITFC_END
#endif

_USER_RESOURCE_I_ITFC_BEGIN 
/**
 * allocate heap memory
 */
void*
user_resource_i_alloc(
    size_t size);

/**
 * free heap memory
 */
void
user_resource_i_free(
    void* obj);


_USER_RESOURCE_I_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif
