#ifndef __SERVICE_SECRETS_H__
#define __SERVICE_SECRETS_H__

#ifdef __cplusplus
#define _SERVICE_SECRETS_ITFC_BEGIN extern "C" {
#define _SERVICE_SECRETS_ITFC_END }
#else
#define _SERVICE_SECRETS_ITFC_BEGIN 
#define _SERVICE_SECRETS_ITFC_END
#endif

_SERVICE_SECRETS_ITFC_BEGIN 

/**
 * get non zero if the service need client secret id
 */
int
service_secret_need_secret(
    const char* service);


_SERVICE_SECRETS_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif

