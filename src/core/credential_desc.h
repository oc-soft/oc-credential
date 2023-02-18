#ifndef __CREDENTIAL_DESC_H__
#define __CREDENTIAL_DESC_H__

#include <stddef.h>
#include <stdio.h>

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

    /**
     * quit
     */
    int quit;
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
 * decode from file stream
 */
credential_desc*
credential_desc_decode_from_file(
    FILE* file_stream);

/**
 * encode into file stream
 */
int
credential_desc_encode_into_file(
    credential_desc* obj,
    FILE* file_stream);


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
 * set quit option
 */
int
credential_desc_set_quit(
    credential_desc* obj,
    int state);

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
