#ifndef __LMD_REQUESTS_H__
#define __LMD_REQUESTS_H__

#include "lmd.h"

#ifdef __cplusplus
#define _LMD_REQUESTS_ITFC_BEGIN extern "C" {
#define _LMD_REQUESTS_ITFC_END }
#else
#define _LMD_REQUESTS_ITFC_BEGIN 
#define _LMD_REQUESTS_ITFC_END 
#endif

_LMD_REQUESTS_ITFC_BEGIN 


/**
 * request device and user code
 */
int
lmd_requests_load_device_and_user_code(
	lmd* obj);

/**
 * poll oauth token
 */
int
lmd_requests_poll_oauth_token(
    lmd* obj,
    int (*progress)(void*, int, lmd*),
    void* user_data);

_LMD_REQUESTS_ITFC_END 

/* vi: se ts=4 sw=4 et: */
#endif
