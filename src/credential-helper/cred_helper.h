#ifndef __CRED_HELPER_H__
#define __CRED_HELPER_H__

#include "cred_helper_types.h"
#include "lmd_types.h"
#include "ui_token_gen_types.h"
#include "credential_op.h"
#ifdef __cplusplus
#define _CRED_HELPER_ITFC_BEGIN extern "C" {
#define _CRED_HELPER_ITFC_END }
#else
#define _CRED_HELPER_ITFC_BEGIN 
#define _CRED_HELPER_ITFC_END 
#endif

_CRED_HELPER_ITFC_BEGIN 

/**
 * create credential helper
 */
cred_helper*
cred_helper_create();

/**
 * increment reference count
 */
unsigned int
cred_helper_retain(
    cred_helper* obj);

/**
 * decrement reference count
 */
unsigned int
cred_helper_release(
    cred_helper* obj);

/**
 * set credential operation
 */
int
cred_helper_set_credential_op(
    cred_helper* obj,
    credential_op op);

/**
 * get credential operation
 */
credential_op
cred_helper_get_credential_op(
    cred_helper* obj);


/**
 * set client id
 */
int
cred_helper_set_client_id(
    cred_helper* obj,
    const char* client_id);

/**
 * get client id 
 */
const char*
cred_helper_get_client_id_ref(
    const cred_helper* obj);

/**
 * get client secret
 */
const char*
cred_helper_get_client_secret_ref(
    const cred_helper* obj);

/**
 * set client secret
 */
int
cred_helper_set_client_secret(
    cred_helper* obj,
    const char* client_secret);

/**
 * get access token
 */
const char*
cred_helper_get_access_token_ref(
    const cred_helper* obj);

/**
 * set access token
 */
int
cred_helper_set_access_token(
    cred_helper* obj,
    const char* access_token);


/**
 * set default token generator service
 */
int
cred_helper_set_service(
    cred_helper* obj,
    const char* service);

/**
 * get default token generator service reference
 */
const char*
cred_helper_get_service_ref(
    cred_helper* obj);



/**
 * set generator mode
 */
int
cred_helper_set_generator_mode(
    cred_helper* obj,
    int mode);

/**
 * get non zero if cred_helper info is running on generator mode.
 */
int
cred_helper_is_generator_mode(
    cred_helper* obj);

/**
 * set the flag for running for gui generator.
 */
int
cred_helper_run_gui_generator(
    cred_helper* obj, 
    int running);


/**
 * get the flag for running for gui generator.
 */
int
cred_helper_dose_run_gui_generator(
    cred_helper* obj);


/**
 * set the flag for running limited deivce generator.
 */
int
cred_helper_run_limited_device(
    cred_helper* obj, 
    int running);

/**
 * get the flag for running limited deivce generator.
 */
int
cred_helper_dose_run_limited_device(
    cred_helper* obj);


/**
 * set verbose level
 */
int
cred_helper_set_verbose_level(
    cred_helper* obj,
    int verbose_level);

/**
 * get verbose level
 */
int
cred_helper_get_verbose_level(
    cred_helper* obj);


/**
 * get limited device access object
 */
lmd*
cred_helper_get_lmd(
    cred_helper* obj);

/**
 * get token generator user interface
 */
ui_token_gen*
cred_helper_get_ui_token_gen(
    cred_helper* obj);

/**
 * update access token with embeded lmd object
 */
int
cred_helper_update_access_token_with_lmd(
    cred_helper* obj);

/**
 * you get no zero if user request to get usage
 */
int
cred_helper_is_requested_usage(
    cred_helper* obj);

/**
 * set no zero if user request to get usage
 */
int
cred_helper_set_requested_usage(
    cred_helper* obj,
    int usage_request);


_CRED_HELPER_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif
