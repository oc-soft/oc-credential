#ifndef __CLIENT_SECRET_H__
#define __CLIENT_SECRET_H__


#ifdef __cplusplus
#define _CLIENT_SECRET_ITFC_BEGIN extern "C" {
#define _CLIENT_SECRET_ITFC_END }
#else
#define _CLIENT_SECRET_ITFC_BEGIN 
#define _CLIENT_SECRET_ITFC_END
#endif

_CLIENT_SECRET_ITFC_BEGIN 

/**
 * get client secret
 */
const char*
client_secret_get();

_CLIENT_SECRET_ITFC_END
/* vi: se ts=4 sw=4 */
#endif
