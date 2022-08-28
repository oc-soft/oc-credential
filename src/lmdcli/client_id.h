#ifndef __CLIENT_ID_H__
#define __CLIENT_ID_H__


#ifdef __cplusplus
#define _CLIENT_ID_ITFC_BEGIN extern "C" {
#define _CLIENT_ID_ITFC_END }
#else
#define _CLIENT_ID_ITFC_BEGIN 
#define _CLIENT_ID_ITFC_END
#endif

_CLIENT_ID_ITFC_BEGIN 

/**
 * get client secret
 */
const char*
client_id_get();

_CLIENT_ID_ITFC_END
/* vi: se ts=4 sw=4 */
#endif
