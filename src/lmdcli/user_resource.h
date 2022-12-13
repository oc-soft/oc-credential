#ifndef __USER_RESOURCE_H__
#define __USER_RESOURCE_H__

#ifdef __cplusplus
#define _USER_RESOURCE_ITFC_BEGIN extern "C" {
#define _USER_RESOURCE_ID_ITFC_END }
#else
#define _USER_RESOURCE_ID_ITFC_BEGIN 
#define _USER_RESOURCE_ID_ITFC_END
#endif

_USER_RESOURCE_ID_ITFC_BEGIN 

/**
 * get credential data directory 
 */
char*
user_resource_get_credential_data_directory();

/**
 * get credential data path
 */
char*
user_resource_get_credentail_data_path();

/**
 * free object
 */
void
user_resource_free(
    void* obj);
_USER_RESOURCE_ID_ITFC_END

/* vi: se ts=4 sw=4 et: */
#endif
