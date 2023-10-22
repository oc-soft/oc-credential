#ifndef __LMD_OAUTH_TOKEN_PARAM_CREATOR_H__
#define __LMD_OAUTH_TOKEN_PARAM_CREATOR_H__

#include <stddef.h>

#ifdef __cplusplus
#define _LMD_OAUTH_TOKEN_PARAM_CREATOR_ITFC_BEGIN extern "C" {
#define _LMD_OAUTH_TOKEN_PARAM_CREATOR_ITFC_END }
#else
#define _LMD_OAUTH_TOKEN_PARAM_CREATOR_ITFC_BEGIN 
#define _LMD_OAUTH_TOKEN_PARAM_CREATOR_ITFC_END
#endif

_LMD_OAUTH_TOKEN_PARAM_CREATOR_ITFC_BEGIN 


/**
 * parametor creator for oauth token in limited access
 */
typedef struct _lmd_oauth_token_param_creator lmd_oauth_token_param_creator;

/**
 * parametor creator virtual function table for oauth token in limited access
 */
typedef struct _lmd_oauth_token_param_creator_vtbl
    lmd_oauth_token_param_creator_vtbl;


/**
 * parametor creator for oauth token in limited access
 */
struct _lmd_oauth_token_param_creator {
    /**
     * virtual function table
     */
    const lmd_oauth_token_param_creator_vtbl* vtbl;
};

/**
 * parametor creator virtual function table for oauth token in limited access
 */
struct _lmd_oauth_token_param_creator_vtbl {

    /**
     * increment reference count
     */
    unsigned int (*retain)(lmd_oauth_token_param_creator*);

    /**
     * decrement reference count
     */
    unsigned int (*release)(lmd_oauth_token_param_creator*);

    /**
     * create parameter
     */
    int (*create_param)(
        lmd_oauth_token_param_creator*,
        const char*, const char*, const char*, char**,
        void* (*)(size_t));
};

/**
 * increment reference count
 */
unsigned int 
lmd_oauth_token_param_creator_retain(
    lmd_oauth_token_param_creator* obj);

/**
 * decrement reference count
 */
unsigned int
lmd_oauth_token_param_creator_release(
    lmd_oauth_token_param_creator* obj);

/**
 * create parameter
 */
int
lmd_oauth_token_param_creator_create_param(
    lmd_oauth_token_param_creator* obj,
    const char* client_id,
    const char* device_code,
    const char* client_secret,
    char** param,
    void* (*mem_alloc)(size_t));

_LMD_OAUTH_TOKEN_PARAM_CREATOR_ITFC_END 

/* vi: se ts=4 sw=4 et: */
#endif
