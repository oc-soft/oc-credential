#ifndef __CRED_HELPER_OP_H__
#define __CRED_HELPER_OP_H__

#include "cred_helper_types.h"
#include "credential_op.h"
#ifdef __cplusplus
#define _CRED_HELPER_OP_ITFC_BEGIN extern "C" {
#define _CRED_HELPER_OP_ITFC_END }
#else
#define _CRED_HELPER_OP_ITFC_BEGIN 
#define _CRED_HELPER_OP_ITFC_END 
#endif

_CRED_HELPER_OP_ITFC_BEGIN 

/**
 * run credential operation
 */
int
cred_helper_op_run(
    cred_helper* obj);


_CRED_HELPER_OP_ITFC_END

/* vi: se ts=4 sw=4 et: */
#endif
