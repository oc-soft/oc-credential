#ifndef __CREDENTIAL_OP_H__
#define __CREDENTIAL_OP_H__

#ifdef __cplusplus
#define _CREDENTIAL_OP_ITFC_BEGIN extern "C" {
#define _CREDENTIAL_OP_ITFC_END }
#else
#define _CREDENTIAL_OP_ITFC_BEGIN 
#define _CREDENTIAL_OP_ITFC_END 
#endif

_CREDENTIAL_OP_ITFC_BEGIN 

/**
 * credential option code
 */
enum _credential_op {
    /**
     * unknown operation
     */
    CDT_OP_UNKNOWN,
    /**
     * get operation
     */
    CDT_OP_GET,
    /**
     * store operation
     */
    CDT_OP_STORE,
    /**
     * erase operation
     */
    CDT_OP_ERASE
};

typedef enum _credential_op credential_op;

_CREDENTIAL_OP_ITFC_END

/* vi: se ts=4 sw=4 et: */
#endif
