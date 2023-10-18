#ifndef __LMD_REQUESTS_GOOGLE_H__
#define __LMD_REQUESTS_GOOGLE_H__

#include "lmd.h"

#ifdef __cplusplus
#define _LMD_REQUESTS_GOOGLE_ITFC_BEGIN extern "C" {
#define _LMD_REQUESTS_GOOGLE_ITFC_END }
#else
#define _LMD_REQUESTS_GOOGLE_ITFC_BEGIN 
#define _LMD_REQUESTS_GOOGLE_ITFC_END 
#endif

_LMD_REQUESTS_GOOGLE_ITFC_BEGIN 

/**
 * create parameter for oauth_token
 */
char*
lmd_requests_google_create_param_for_oauth_token(
    lmd* obj);

_LMD_REQUESTS_GOOGLE_ITFC_END 
/* vi: se ts=4 sw=4 et: */
#endif
