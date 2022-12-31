#ifndef __CREDENTIAL_STORAGE_H__
#define __CREDENTIAL_STORAGE_H__

#ifdef __cplusplus
#define _CREDENTIAL_STORAGE_ITFC_BEGIN extern "C" {
#define _CREDENTIAL_STORAGE_ITFC_END }
#else
#define _CREDENTIAL_STORAGE_ITFC_BEGIN 
#define _CREDENTIAL_STORAGE_ITFC_END 
#endif

_CREDENTIAL_STORAGE_ITFC_BEGIN 


/**
 * find password 
 */
int
credential_storage_find_password(
    const char* protocol,
    const char* host,
    const char* path, 
    const char* username,
    char** password);

/**
 * store password 
 */
int
credential_storage_store_password(
    const char* protocol,
    const char* host,
    const char* path,
    const char* username,
    const char* password);


/**
 * remove password
 */
int
credential_storage_remove_password(
    const char* protocol,
    const char* host,
    const char* path,
    const char* username);



/**
 * free heap object from this module
 */
void
credential_storage_free_object(
    void* obj);


/**
 * initialize this module 
 */
int
credential_storage_start();

/**
 * tear down this module 
 */
int
credential_storage_stop();


_CREDENTIAL_STORAGE_ITFC_END

/* vi: se ts=4 sw=4 et: */
#endif
