#ifndef __SERVICE_URL_LMD_OAUTH_TOKEN_PARAM_H__
#define __SERVICE_URL_LMD_OAUTH_TOKEN_PARAM_H__

#include <stddef.h>
#include "lmd_oauth_token_param_creator.h"

#ifdef __cplusplus
#define _SERVICE_URL_LMD_OAUTH_TOKEN_PARAM_ITFC_BEGIN extern "C" {
#define _SERVICE_URL_LMD_OAUTH_TOKEN_PARAM_ITFC_END }
#else
#define _SERVICE_URL_LMD_OAUTH_TOKEN_PARAM_ITFC_BEGIN 
#define _SERVICE_URL_LMD_OAUTH_TOKEN_PARAM_ITFC_END
#endif

_SERVICE_URL_LMD_OAUTH_TOKEN_PARAM_ITFC_BEGIN 


/**
 * get param creator from service id and url id
 */
int
service_url_lmd_oauth_token_param_get_creator(
    const char* service_id,
    const char* url_id,
    lmd_oauth_token_param_creator** param_creator);

/**
 * free memory
 */
void
service_url_lmd_oauth_token_param_free(
    void* mem);


_SERVICE_URL_LMD_OAUTH_TOKEN_PARAM_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif

