#ifndef __CREDENTIAL_DESC_H__
#define __CREDENTIAL_DESC_H__

#include <stddef.h>

#ifdef __cplusplus
#define _CREDENTIAL_DESC_ITFC_BEGIN extern "C" {
#define _CREDENTIAL_DESC_ITFC_END }
#else
#define _CREDENTIAL_DESC_ITFC_BEGIN 
#define _CREDENTIAL_DESC_ITFC_END
#endif

_CREDENTIAL_DESC_ITFC_BEGIN 

/**
 * credential descritor
 */
typedef struct _credential_desc credential_desc;


/**
 * credential descritor
 */
struct _credential_desc {
    /**
     * protocol
     */
    char* protocol;
    /**
     * host
     */
    char* host;
    /**
     * path 
     */
    char* path;
    /**
     * username
     */
    char* username;
    /**
     * password
     */
    char* password;
};


/**
 * create credential_desc from git descriptor format
 */
credential_desc*
credential_desc_decode(
    const char* descriptor,
    size_t descriptor_length);

/**
 * encode credential_desc into git descriptor format
 */
int
credential_desc_encode(
    const credential_desc* obj,
    char** descriptor,
    size_t* descriptor_length);

/**
 * set username
 */
int
credential_desc_set_username(
    credential_desc* obj,
    const char* username);

/**
 * set password
 */
int
credential_desc_set_password(
    credential_desc* obj,
    const char* password);

/**
 * free credential descriptor
 */
void
credential_desc_free(
    credential_desc* desc);

/**
 * free heap object from which you get in this interface.
 */
void
credential_desc_free_object(
    void* obj);

_CREDENTIAL_DESC_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif
