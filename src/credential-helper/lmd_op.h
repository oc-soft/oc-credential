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
 * select service
 */
int
lmd_op_select_service(
    lmd* obj,
    const char* protocol,
    const char* host,
    const char* path);


/**
 * get oauth token
 */
int
lmd_op_get_oauth_token_with_client(
    lmd* limited_acc);

_LMD_OP_ITFC_END 

/* vi: se ts=4 sw=4 et: */
#endif
