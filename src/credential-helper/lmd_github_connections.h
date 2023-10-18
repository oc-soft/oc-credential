#ifndef __LMD_GITHUB_CONNECTIONS_H__
#define __LMD_GITHUB_CONNECTIONS_H__

#include "lmd.h"

#ifdef __cplusplus
#define _LMD_GITHUB_CONNECTIONS_ITFC_BEGIN extern "C" {
#define _LMD_GITHUB_CONNECTIONS_ITFC_END }
#else
#define _LMD_GITHUB_CONNECTIONS_ITFC_BEGIN 
#define _LMD_GITHUB_CONNECTIONS_ITFC_END 
#endif

_LMD_GITHUB_CONNECTIONS_ITFC_BEGIN 

/**
 * load github oauth related endpoint urls
 */
int
lmd_github_connections_load_oauth_endpoint_urls(
    lmd* obj);

_LMD_GITHUB_CONNECTIONS_ITFC_END 
/* vi: se ts=4 sw=4 et: */
#endif
