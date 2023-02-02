#ifndef __LMD_OP_H__
#define __LMD_OP_H__

#include <stddef.h>
#include "lmd_types.h"

#ifdef __cplusplus
#define _LMD_OP_ITFC_BEGIN extern "C" {
#define _LMD_OP_ITFC_END }
#else
#define _LMD_OP_ITFC_BEGIN 
#define _LMD_OP_ITFC_END 
#endif

_LMD_OP_ITFC_BEGIN 

/**
 * get oauth token
 */
int
lmd_get_oauth_token_with_client(
    lmd* limited_acc,
    const char* client_id,
    const char* client_secret);

_LMD_OP_ITFC_END 

/* vi: se ts=4 sw=4 et: */
#endif
