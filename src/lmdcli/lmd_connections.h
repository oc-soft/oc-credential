#ifndef __LMD_CONNECTIONS_H__
#define __LMD_CONNECTIONS_H__

#include "lmd.h"

#ifdef __cplusplus
#define _LMD_CONNECTIONS_ITFC_BEGIN extern "C" {
#define _LMD_CONNECTIONS_ITFC_END }
#else
#define _LMD_CONNECTIONS_ITFC_BEGIN 
#define _LMD_CONNECTIONS_ITFC_END 
#endif

_LMD_CONNECTIONS_ITFC_BEGIN 

/**
 * load discovery document
 */
int
lmd_connections_load_discovery_document(
    lmd* obj);


/**
 * request device and user code
 */
int
lmd_requests_device_and_user_code(
    lmd* obj);

_LMD_CONNECTIONS_ITFC_END 
/* vi: se ts=4 sw=4 et: */
#endif
