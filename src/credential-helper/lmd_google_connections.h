#ifndef __LMD_GOOGLE_CONNECTIONS_H__
#define __LMD_GOOGLE_CONNECTIONS_H__

#include "lmd.h"

#ifdef __cplusplus
#define _LMD_GOOGLE_CONNECTIONS_ITFC_BEGIN extern "C" {
#define _LMD_GOOGLE_CONNECTIONS_ITFC_END }
#else
#define _LMD_GOOGLE_CONNECTIONS_ITFC_BEGIN 
#define _LMD_GOOGLE_CONNECTIONS_ITFC_END 
#endif

_LMD_GOOGLE_CONNECTIONS_ITFC_BEGIN 

/**
 * load google discovery document
 */
int
lmd_google_connections_load_discovery_document(
    lmd* obj);

_LMD_GOOGLE_CONNECTIONS_ITFC_END 
/* vi: se ts=4 sw=4 et: */
#endif
