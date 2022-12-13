#ifndef __LMD_CREDENTIAL_OP_H__
#define __LMD_CREDENTIAL_OP_H__

#include "lmd_types.h"
#include "credential_op.h"
#ifdef __cplusplus
#define _LMD_CREDENTIAL_OP_ITFC_BEGIN extern "C" {
#define _LMD_CREDENTIAL_OP_ITFC_END }
#else
#define _LMD_CREDENTIAL_OP_ITFC_BEGIN 
#define _LMD_CREDENTIAL_OP_ITFC_END 
#endif

_LMD_CREDENTIAL_OP_ITFC_BEGIN 

/**
 * run credential operation
 */
int
lmd_credential_op_run(
    lmd* obj);


_LMD_CREDENTIAL_OP_ITFC_END

/* vi: se ts=4 sw=4 et: */
#endif
