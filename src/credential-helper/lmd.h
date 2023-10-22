#ifndef __LMD_H__
#define __LMD_H__

#include <stddef.h>
#include <json-c/json.h>
#include "lmd_types.h"
#include "credential_op.h"
#include "lmd_oauth_token_param_creator.h"

#ifdef __cplusplus
#define _LMD_ITFC_BEGIN extern "C" {
#define _LMD_ITFC_END }
#else
#define _LMD_ITFC_BEGIN 
#define _LMD_ITFC_END 
#endif

_LMD_ITFC_BEGIN 

/**
 * create instance
 */
lmd*
lmd_create();

/**
 * increment reference count
 */
unsigned int
lmd_retain(
    lmd* obj);

/**
 * decrement reference count
 */
unsigned int
lmd_release(
    lmd* obj);

/**
 * get service
 */
const char*
lmd_get_service_ref(
    const lmd* obj);


/**
 * set service
 */
int
lmd_set_service(
    lmd* obj,
    const char* service);


/**
 * set service
 */
int
lmd_set_service_0(
    lmd* obj,
    const char* service,
    size_t service_length);


/**
 * get client id
 */
const char*
lmd_get_client_id_ref(
    const lmd* obj);


/**
 * set client id
 */
int
lmd_set_client_id(
    lmd* obj,
    const char* client_id);



/**
 * get client secret
 */
const char*
lmd_get_client_secret_ref(
    const lmd* obj);

/**
 * set client secret
 */
int
lmd_set_client_secret(
    lmd* obj,
    const char* client_secret);


/**
 * get device user code param
 */
const char*
lmd_get_device_user_code_param_ref(
    const lmd* obj);


/**
 * set device user code param
 */
int
lmd_set_device_user_code_param(
    lmd* obj,
    const char* device_user_code_param);


/**
 * get verification url
 */
const char*
lmd_get_verification_url_ref(
    const lmd* obj);


/**
 * set verification url
 */
int
lmd_set_verification_url(
    lmd* obj,
    const char* url);


/**
 * set verification url
 */
int
lmd_set_verification_url_0(
    lmd* obj,
    const char* url,
    size_t length);

/**
 * set credential operation
 */
int
lmd_set_credential_op(
    lmd* obj,
    credential_op op);

/**
 * get credential operation
 */
credential_op
lmd_get_credential_op(
    lmd* obj);


/**
 * set verbose level
 */
int
lmd_set_verbose_level(
    lmd* obj,
    int verbose_level);

/**
 * get verbose level
 */
int
lmd_get_verbose_level(
    lmd* obj);

/**
 * get token end point url
 */
const char*
lmd_get_token_endpoint_url_ref(
    lmd* obj);

/**
 * set token end point url
 */
int
lmd_set_token_endpoint_url(
    lmd* obj,
    const char* url);

/**
 * set token end point url
 */
int
lmd_set_token_endpoint_url_0(
    lmd* obj,
    const char* url,
    size_t length);

/**
 * set token endpoint request header options
 */
int
lmd_set_token_eprqhd_options(
    lmd* obj,
    const char** options,
    size_t options_size);

/**
 * get token the size of endpoint request post header options
 */
size_t
lmd_get_token_eprqhd_options_size(
    lmd* obj);

/**
 * get token endpoint request post header options
 */
const char**
lmd_get_token_eprqhd_options_ref(
    lmd* obj);


/**
 * device code
 */
const char*
lmd_get_device_code_ref(
    lmd* obj);

/**
 * device code
 */
int
lmd_set_device_code(
    lmd* obj,
    const char* device_code);

/**
 * devide code
 */
int
lmd_set_devide_code_0(
    lmd* obj,
    const char* device_code,
    size_t length);

/**
 * set oauth token url param creator
 */
int
lmd_set_oauth_token_param_creator(
    lmd* obj,
    lmd_oauth_token_param_creator* creator);

/**
 * get oauth token url param creator
 */
int
lmd_get_oauth_token_param_creator(
    lmd* obj,
    lmd_oauth_token_param_creator** creator);


/**
 * get oauth token url param creator reference
 */
lmd_oauth_token_param_creator*
lmd_get_oauth_token_param_creator_ref(
    lmd* obj);


/**
 * expiration seconds to poll for oath token
 */
int
lmd_get_expires_in(
    lmd* obj);


/**
 * get device authorization token end point url
 */
const char*
lmd_get_device_auth_token_endpoint_url_ref(
    lmd* obj);


/**
 * set device authorization token end point url
 */
int
lmd_set_device_auth_token_endpoint_url(
    lmd* obj,
    const char* url);

/**
 * set device authorization token end point url
 */
int
lmd_set_device_auth_token_endpoint_url_0(
    lmd* obj,
    const char* url,
    size_t length);


/**
 * set options for device oauth token endpoint request header
 */
int
lmd_set_dot_eprqhd_options(
    lmd* obj,
    const char** options,
    size_t options_size);


/**
 * get the size of device oauth token endpoint request header options
 */
size_t
lmd_get_dot_eprqhd_options_size(
    lmd* obj);

/**
 * get options of device oauth token endpoint request header
 */
const char**
lmd_get_dot_eprqhd_options_ref(
    lmd* obj);


/**
 * user code
 */
const char*
lmd_get_user_code_ref(
    lmd* obj);

/**
 * user code
 */
int
lmd_set_user_code(
    lmd* obj,
    const char* user_code);

/**
 * user code
 */
int
lmd_set_user_code_0(
    lmd* obj,
    const char* user_code,
    size_t length);

/**
 * device code
 */
const char*
lmd_get_device_code_ref(
    lmd* obj);

/**
 * device code
 */
int
lmd_set_device_code(
    lmd* obj,
    const char* code);

/**
 * device code
 */
int
lmd_set_device_code_0(
    lmd* obj,
    const char* code,
    size_t lenth);

/**
 * access token 
 */
const char*
lmd_get_access_token_ref(
    lmd* obj);

/**
 * access token
 */
int
lmd_set_access_token(
    lmd* obj,
    const char* token);

/**
 * access token
 */
int
lmd_set_access_token_0(
    lmd* obj,
    const char* token,
    size_t length);

/**
 * token type
 */
const char*
lmd_get_token_type_ref(
    lmd* obj);

/**
 * token type
 */
int
lmd_set_token_type(
    lmd* obj,
    const char* type);

/**
 * token type
 */
int
lmd_set_token_type_0(
    lmd* obj,
    const char* token,
    size_t length);

/**
 * refresh token
 */
const char*
lmd_get_refresh_token_ref(
    lmd* obj);

/**
 * refresh token
 */
int
lmd_set_refresh_token(
    lmd* obj,
    const char* refresh_token);

/**
 * refresh token
 */
int
lmd_set_refresh_token_0(
    lmd* obj,
    const char* refresh_token,
    size_t length);

/**
 * scope
 */
const char*
lmd_get_scope_ref(
    lmd* obj);

/**
 * scope
 */
int
lmd_set_scope(
    lmd* obj,
    const char* scope);

/**
 * scope
 */
int
lmd_set_scope_0(
    lmd* obj,
    const char* scope,
    size_t length);



/**
 * expiration seconds for oauth token
 */
int
lmd_get_expires_in(
    lmd* obj);

/**
 * expiration seconds for oauth token
 */
int
lmd_set_expires_in(
    lmd* obj,
    int expiration);


/**
 * expiration seconds to poll for oath token
 */
int
lmd_get_polling_expires_in(
    lmd* obj);

/**
 * expiration seconds to poll for auth token
 */
int
lmd_set_polling_expires_in(
    lmd* obj,
    int expiration);

/**
 * interval seconds to poll for oath token
 */
int
lmd_get_interval(
    lmd* obj);

/**
 * interval seconds to poll for auth token
 */
int
lmd_set_interval(
    lmd* obj,
    int interval);

/**
 * set tenant for microsoft oauth token
 */
int
lmd_set_ms_tenant(
    lmd* obj,
    const char* tenant);

/**
 * set tenant for microsoft oauth token
 */
int
lmd_set_ms_tenant_0(
    lmd* obj,
    const char* tenant,
    size_t length);


/**
 * get tenant for microsoft oauth token
 */
const char*
lmd_get_ms_tenant_ref(
    lmd* obj);


/**
 * set test mode to get
 */
int
lmd_set_test_mode_to_get(
    lmd* obj,
    int testing_mode);

/**
 * get test mode to get
 */
int
lmd_is_test_mode_to_get(
    lmd* obj);
    
/**
 * set generator mode
 */
int
lmd_set_generator_mode(
    lmd* obj,
    int mode);

/**
 * get non zero if limited device info is running on generator mode.
 */
int
lmd_is_generator_mode(
    lmd* obj);
    

/**
 * set oauth loader 
 */
int
lmd_set_oauth_token_loader(
    lmd* obj,
    int (*loader)(lmd*, json_object*));

/**
 * set oauth loader 
 */
int
lmd_get_oauth_token_loader(
    lmd* obj,
    int (**loader)(lmd*, json_object*));


/**
 * set oauth response error parser 
 */
int
lmd_set_oauth_response_error_parser(
    lmd* obj,
    int (*error_parser)(json_object*, int*));

/**
 * set oauth response error parser
 */
int
lmd_get_oauth_response_error_parser(
    lmd* obj,
    int (**error_parser)(json_object*, int*));


/**
 * get string representation
 */
char*
lmd_get_str_representation(
    lmd* obj);

/**
 * release heap object which is made in this interface
 */
void
lmd_free_object(
    lmd* obj,
    void* heap_obj);

_LMD_ITFC_END 

/* vi: se ts=4 sw=4 et: */
#endif
