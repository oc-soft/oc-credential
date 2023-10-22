#include "lmd.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


struct _lmd {
    /**
     * reference count
     */
    unsigned int ref_count;

    /**
     * service
     */
    char* service;

    /**
     * client id
     */
    char* client_id;

    /**
     * device user code param
     */
    char* device_user_code_param;

    /**
     * client secret
     */
    char* client_secret;

    /**
     * verification url
     */
    char* verification_url;

    /**
     * token endpoint url
     */
    char* token_endpoint_url;

    /**
     * device authorization token ednpoint url
     */
    char* device_auth_token_endpoint_url;

    /**
     * device code
     */
    char* device_code;

    /**
     * ouath token url parameter creator
     */
    lmd_oauth_token_param_creator* oauth_token_param_creator;


    /**
     * oauth token loader
     */
    int (*oauth_token_loader)(lmd*, json_object* json);

    /**
     * user code
     */
    char* user_code;


    /**
     * access token
     */
    char* access_token;

    /**
     * token type
     */
    char* token_type;

    /**
     * refresh token
     */
    char* refresh_token;

    /**
     * scope
     */
    char* scope;

    /**
     * options for token endpoint request header
     */
    char** token_eprqhd_options;

    /**
     * the size of token endpoint request header options
     */
    size_t token_eprqhd_options_size;

    /**
     * options for device oauth token endpoint request header
     */
    char** dot_eprqhd_options;

    /**
     * the size of device oauth token endpoint request header options
     */
    size_t dot_eprqhd_options_size;


    /**
     * tenant for microsoft oauth token
     */
    char* ms_tenant;

    /**
     * expires in
     */
    int expires_in;

    /**
     * expiration seconds  to poll for oauth token
     */
    int polling_expires_in;


    /**
     * interval seconds to poll for oauth token
     */
    int interval;


    /**
     * credential operation from git 
     */
    credential_op credential_operation;
    
    /* verbose level */
    int verbose_level;


    /* generator mode */
    int generator_mode:1;
};

/**
 * create instance
 */
lmd*
lmd_create()
{
    lmd* result;
    result = (lmd*)malloc(sizeof(lmd));
    if (result) {
        
        result->ref_count = 1;
        result->service = NULL;
        result->client_id = NULL;
        result->client_secret = NULL;
        result->device_user_code_param = NULL;
        result->verification_url = NULL;
        result->token_endpoint_url = NULL;
        result->device_auth_token_endpoint_url = NULL;
        result->device_code = NULL;
        result->user_code = NULL;
        result->oauth_token_param_creator = NULL;
        result->oauth_token_loader = NULL;
        result->expires_in = 0;
        result->access_token = NULL;
        result->token_type = NULL;
        result->refresh_token = NULL;
        result->scope = NULL;
        result->token_eprqhd_options = NULL;
        result->token_eprqhd_options_size = 0;
        result->dot_eprqhd_options = NULL;
        result->dot_eprqhd_options_size = 0;
        result->ms_tenant = NULL;
        result->polling_expires_in = 0;
        result->interval = 0;
        result->credential_operation = CDT_OP_UNKNOWN;
        result->verbose_level = 0;
    }
    return result;
}

/**
 * increment reference count
 */
unsigned int
lmd_retain(
    lmd* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = ++obj->ref_count;
    }
    return result;
}

/**
 * decrement reference count
 */
unsigned int
lmd_release(
    lmd* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = --obj->ref_count;

        if (result == 0) {
            lmd_set_service(obj, NULL);
            lmd_set_client_id(obj, NULL);
            lmd_set_client_secret(obj, NULL);
            lmd_set_device_user_code_param(obj, NULL);
            lmd_set_device_auth_token_endpoint_url(obj, NULL);
            lmd_set_token_endpoint_url(obj, NULL);
            lmd_set_user_code(obj, NULL);
            lmd_set_device_code(obj, NULL);
            lmd_set_oauth_token_param_creator(obj, NULL);
            lmd_set_token_eprqhd_options(obj, NULL, 0);
            lmd_set_dot_eprqhd_options(obj, NULL, 0);
            lmd_set_access_token(obj, NULL);
            lmd_set_token_type(obj, NULL);
            lmd_set_scope(obj, NULL);
            lmd_set_ms_tenant(obj, NULL);
            lmd_set_refresh_token(obj, NULL);
            free(obj);
        }
    }
    return result;
}

/**
 * get service
 */
const char*
lmd_get_service_ref(
    const lmd* obj)
{
    const char* result;
    result = NULL;
    if (obj) {
        result = obj->service;
    } else {
        errno = EINVAL;
    }
    return result;
}


/**
 * set service
 */
int
lmd_set_service(
    lmd* obj,
    const char* service)
{
    int result;
    result = 0;
    if (obj) {
        size_t len;
        if (service) {
            len = strlen(service);
        } else {
            len = 0;
        }
        result = lmd_set_service_0(obj, service, len);
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * set service
 */
int
lmd_set_service_0(
    lmd* obj,
    const char* service,
    size_t service_length)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = obj->service != service;
        if (obj->service) {
            free(obj->service);
            obj->service = NULL;
        }
        if (service) {
            char* str_obj;
            str_obj = (char*)malloc(service_length + 1); 
            if (str_obj) {
                memcpy(str_obj, service, service_length + 1);
                obj->service = str_obj;
            } else {
                result = -1;
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}



/**
 * get client id
 */
const char*
lmd_get_client_id_ref(
    const lmd* obj)
{
    const char* result;
    result = NULL;
    if (obj) {
        result = obj->client_id;
    } else {
        errno = EINVAL;
    }
    return result;
}


/**
 * set client id
 */
int
lmd_set_client_id(
    lmd* obj,
    const char* client_id)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = obj->client_id != client_id;
        if (obj->client_id) {
            free(obj->client_id);
            obj->client_id = NULL;
        }
        if (client_id) {
            char* str_obj;
            size_t len;
            str_obj = NULL;
            len = strlen(client_id);
            str_obj = (char*)malloc(len + 1); 
            if (str_obj) {
                memcpy(str_obj, client_id, len + 1);
                obj->client_id = str_obj;
            } else {
                result = -1;
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}


/**
 * set oauth token url param creator
 */
int
lmd_set_oauth_token_param_creator(
    lmd* obj,
    lmd_oauth_token_param_creator* creator)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = obj->oauth_token_param_creator != creator;
        if (do_set) {
            if (creator) {
                lmd_oauth_token_param_creator_retain(creator);
            }
            if (obj->oauth_token_param_creator) {
                lmd_oauth_token_param_creator_release(
                    obj->oauth_token_param_creator);
            }
            obj->oauth_token_param_creator = creator;
        }
    } else {
        errno = EINVAL;
        result = -1;
    }
    return result;
}

/**
 * get oauth token url param creator
 */
int
lmd_get_oauth_token_param_creator(
    lmd* obj,
    lmd_oauth_token_param_creator** creator)
{
    int result;
    result = 0;
    if (obj && creator) {
        if (obj->oauth_token_param_creator) {
            lmd_oauth_token_param_creator_retain(
                obj->oauth_token_param_creator);
        }
        *creator = obj->oauth_token_param_creator;
    } else {
        errno = EINVAL;
        result = -1;
    }
    return result;
}


/**
 * get oauth token url param creator reference
 */
lmd_oauth_token_param_creator*
lmd_get_oauth_token_param_creator_ref(
    lmd* obj)
{
    lmd_oauth_token_param_creator* result;
    result = NULL;
    if (obj) {
        result = obj->oauth_token_param_creator;
    } else {
        errno = EINVAL;
    }
    return result;
}


/**
 * get device user code param
 */
const char*
lmd_get_device_user_code_param_ref(
    const lmd* obj)
{
    const char* result;
    result = NULL;
    if (obj) {
        result = obj->device_user_code_param;
    } else {
        errno = EINVAL;
    }
    return result;
}


/**
 * set device user code param
 */
int
lmd_set_device_user_code_param(
    lmd* obj,
    const char* device_user_code_param)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = obj->device_user_code_param != device_user_code_param;
        if (obj->device_user_code_param) {
            free(obj->device_user_code_param);
            obj->device_user_code_param = NULL;
        }
        if (device_user_code_param) {
            char* str_obj;
            size_t len;
            str_obj = NULL;
            len = strlen(device_user_code_param);
            str_obj = (char*)malloc(len + 1); 
            if (str_obj) {
                memcpy(str_obj, device_user_code_param, len + 1);
                obj->device_user_code_param = str_obj;
            } else {
                result = -1;
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}


/**
 * get client secret
 */
const char*
lmd_get_client_secret_ref(
    const lmd* obj)
{
    const char* result;
    result = NULL;
    if (obj) {
        result = obj->client_secret;
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * set client secret
 */
int
lmd_set_client_secret(
    lmd* obj,
    const char* client_secret)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = obj->client_secret != client_secret;
        if (obj->client_secret) {
            free(obj->client_secret);
            obj->client_secret = NULL;
        }
        if (client_secret) {
            char* str_obj;
            size_t len;
            str_obj = NULL;
            len = strlen(client_secret);
            str_obj = (char*)malloc(len + 1); 
            if (str_obj) {
                memcpy(str_obj, client_secret, len + 1);
                obj->client_secret = str_obj;
            } else {
                result = -1;
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}


/**
 * get verification url
 */
const char*
lmd_get_verification_url_ref(
    const lmd* obj)
{
    const char* result;
    result = NULL;
    if (obj) {
        result = obj->verification_url;
    } else {
        errno = EINVAL;
    }
    return result;
}


/**
 * set verification url
 */
int
lmd_set_verification_url(
    lmd* obj,
    const char* verification_url)
{
    int result;
    result = 0;
    if (obj) {
        size_t length;
        length = 0;
        if (verification_url) {
            length = strlen(verification_url);
        }
        lmd_set_verification_url_0(obj, verification_url, length);
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * set verification url
 */
int
lmd_set_verification_url_0(
    lmd* obj,
    const char* verification_url,
    size_t length)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = obj->verification_url != verification_url;
        if (obj->verification_url) {
            free(obj->verification_url);
            obj->verification_url = NULL;
        }
        if (verification_url) {
            char* str_obj;
            str_obj = NULL;
            if (length) {
                str_obj = (char*)malloc(length + 1); 
            }
            if (str_obj) {
                memcpy(str_obj, verification_url, length + 1);
                obj->verification_url = str_obj;
            } else {
                result = -1;
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * access token 
 */
const char*
lmd_get_access_token_ref(
    lmd* obj)
{
    const char* result;
    result = NULL;
    if (obj) {
        result = obj->access_token;
    } else {
        result = NULL;
        errno = EINVAL;
    }
    return result;
}

/**
 * access token
 */
int
lmd_set_access_token(
    lmd* obj,
    const char* token)
{
    int result;
    result = 0;
    if (obj) {
        size_t length;
        length = 0;
        if (token) {
            length = strlen(token);
        }
        lmd_set_access_token_0(obj, token, length);
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * access token
 */
int
lmd_set_access_token_0(
    lmd* obj,
    const char* token,
    size_t length)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = obj->access_token != token;
        if (obj->access_token) {
            free(obj->access_token);
            obj->access_token = NULL;
        }
        if (token) {
            char* str_obj;
            str_obj = NULL;
            if (length) {
                str_obj = (char*)malloc(length + 1); 
            }
            if (str_obj) {
                memcpy(str_obj, token, length + 1);
                obj->access_token = str_obj;
            } else {
                result = -1;
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * token type
 */
const char*
lmd_get_token_type_ref(
    lmd* obj)
{
    const char* result;
    result = NULL;
    if (obj) {
        result = obj->token_type;
    } else {
        result = NULL;
        errno = EINVAL;
    }
    return result;
}


/**
 * token type
 */
int
lmd_set_token_type(
    lmd* obj,
    const char* type)
{
    int result;
    result = 0;
    if (obj) {
        size_t length;
        length = 0;
        if (type) {
            length = strlen(type);
        }
        lmd_set_token_type_0(obj, type, length);
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}


/**
 * token type
 */
int
lmd_set_token_type_0(
    lmd* obj,
    const char* token_type,
    size_t length)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = obj->token_type != token_type;
        if (obj->token_type) {
            free(obj->token_type);
            obj->token_type = NULL;
        }
        if (token_type) {
            char* str_obj;
            str_obj = NULL;
            if (length) {
                str_obj = (char*)malloc(length + 1); 
            }
            if (str_obj) {
                memcpy(str_obj, token_type, length + 1);
                obj->token_type = str_obj;
            } else {
                result = -1;
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;

}


/**
 * refresh token
 */
const char*
lmd_get_refresh_token_ref(
    lmd* obj)
{
    const char* result;
    result = NULL;
    if (obj) {
        result = obj->refresh_token;
    } else {
        result = NULL;
        errno = EINVAL;
    }
    return result;
}


/**
 * refresh token
 */
int
lmd_set_refresh_token(
    lmd* obj,
    const char* refresh_token)
{
    int result;
    result = 0;
    if (obj) {
        size_t length;
        length = 0;
        if (refresh_token) {
            length = strlen(refresh_token);
        }
        lmd_set_refresh_token_0(obj, refresh_token, length);
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * refresh token
 */
int
lmd_set_refresh_token_0(
    lmd* obj,
    const char* refresh_token,
    size_t length)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = obj->refresh_token != refresh_token;
        if (obj->refresh_token) {
            free(obj->refresh_token);
            obj->refresh_token = NULL;
        }
        if (refresh_token) {
            char* str_obj;
            str_obj = NULL;
            if (length) {
                str_obj = (char*)malloc(length + 1); 
            }
            if (str_obj) {
                memcpy(str_obj, refresh_token, length + 1);
                obj->refresh_token = str_obj;
            } else {
                result = -1;
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;

}

/**
 * scope
 */
const char*
lmd_get_scope_ref(
    lmd* obj)
{
    const char* result;
    result = NULL;
    if (obj) {
        result = obj->scope;
    } else {
        result = NULL;
        errno = EINVAL;
    }
    return result;
}

/**
 * scope
 */
int
lmd_set_scope(
    lmd* obj,
    const char* scope)
{
    int result;
    result = 0;
    if (obj) {
        size_t length;
        length = 0;
        if (scope) {
            length = strlen(scope);
        }
        lmd_set_scope_0(obj, scope, length);
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * scope
 */
int
lmd_set_scope_0(
    lmd* obj,
    const char* scope,
    size_t length)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = obj->scope != scope;
        if (obj->scope) {
            free(obj->scope);
            obj->scope = NULL;
        }
        if (scope) {
            char* str_obj;
            str_obj = NULL;
            if (length) {
                str_obj = (char*)malloc(length + 1); 
            }
            if (str_obj) {
                memcpy(str_obj, scope, length + 1);
                obj->scope = str_obj;
            } else {
                result = -1;
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}



/**
 * expiration seconds for oauth token
 */
int
lmd_get_expires_in(
    lmd* obj)
{
    int result;
    if (obj) {
        result = obj->expires_in;
    } else {
        result = 0;
        errno = EINVAL;
    }
    return result;
}

/**
 * expiration seconds for oauth token
 */
int
lmd_set_expires_in(
    lmd* obj,
    int expiration)
{
    int result;
    if (obj) {
        result = 0;
        obj->expires_in = expiration;
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}


/**
 * get token endpoint url
 */
const char*
lmd_get_token_endpoint_url_ref(
    lmd* obj)
{
    const char* result;
    result = NULL;
    if (obj) {
        result = obj->token_endpoint_url;
    } else {
        errno = EINVAL;
    }
    return result;
}


/**
 * set token endpoint url
 */
int
lmd_set_token_endpoint_url(
    lmd* obj,
    const char* url)
{
    int result;
    result = 0;
    if (obj) {
        size_t length;
        if (url) {
            length = strlen(url);
        } else {
            length = 0;
        }
        result = lmd_set_token_endpoint_url_0(obj, url, length);
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}


/**
 * set token end point url
 */
int
lmd_set_token_endpoint_url_0(
    lmd* obj,
    const char* url,
    size_t length)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = obj->token_endpoint_url != url;
        if (do_set) {
            if (obj->token_endpoint_url) {
                free(obj->token_endpoint_url);
                obj->token_endpoint_url = NULL;
            }
            if (url) {
                char* str_obj;
                size_t len;
                str_obj = NULL;
                str_obj = (char*)malloc(length + 1); 
                if (str_obj) {
                    memcpy(str_obj, url, length);
                    str_obj[length] = '\0';
                    obj->token_endpoint_url = str_obj;
                } else {
                    result = -1;
                }
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * set token endpoint request header options
 */
int
lmd_set_token_eprqhd_options(
    lmd* obj,
    const char** options,
    size_t options_size)
{
    int result;
    if (obj) {
        int do_set;
        do_set = obj->token_eprqhd_options != (char**)options;
        if (do_set) {
            size_t new_option_idx;
            char** new_options;
            if (options && options_size) {
                new_options = (char**) malloc(sizeof(char*) * options_size);
                if (new_options) {
                    for (new_option_idx = 0;
                        new_option_idx < options_size;
                        new_option_idx++) {
                        char* option;
                        size_t option_length;
                        option_length = strlen(options[new_option_idx]);
                        option = (char*)malloc(option_length + 1);
                        if (option) {
                            memcpy(option,
                                options[new_option_idx], option_length + 1);
                            new_options[new_option_idx] = option;
                        } else {
                            result = -1;
                            break;
                        }
                    }
                }
            }
            if (result == 0) {
                if (obj->token_eprqhd_options) {
                    size_t idx; 
                    for (idx = 0;
                        idx < obj->token_eprqhd_options_size; idx++) {
                        free(obj->token_eprqhd_options[idx]);
                    }
                    free(obj->token_eprqhd_options);
                }
                obj->token_eprqhd_options = new_options;
                obj->token_eprqhd_options_size = options_size;
                new_options = NULL;
            }

            if (new_options) {
                size_t idx; 
                for (idx = 0; idx < new_option_idx; idx++) {
                    free(new_options[idx]);
                }
                free(new_options);
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * get the size of token endpoint request header options
 */
size_t
lmd_get_token_eprqhd_options_size(
    lmd* obj)
{
    size_t result;
    result = 0;
    if (obj) {
        result = obj->token_eprqhd_options_size;
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * get options for token endpoint request header
 */
const char**
lmd_get_token_eprqhd_options_ref(
    lmd* obj)
{
    const char** result;
    result = NULL;
    if (obj) {
        result = (const char**)obj->token_eprqhd_options;
    } else {
        errno = EINVAL;
    }    
    return result;
}


/**
 * get device authorization token endpoint url
 */
const char*
lmd_get_device_auth_token_endpoint_url_ref(
    lmd* obj)
{
    const char* result;
    result = NULL;
    if (obj) {
        result = obj->device_auth_token_endpoint_url;
    } else {
        errno = EINVAL;
    }
    return result;
}


/**
 * set device auuthorization endpoint url
 */
int
lmd_set_device_auth_token_endpoint_url(
    lmd* obj,
    const char* url)
{
    int result;
    result = 0;
    if (obj) {
        size_t length;
        if (url) {
            length = strlen(url);
        } else {
            length = 0;
        }
        result = lmd_set_device_auth_token_endpoint_url_0(obj, url, length);
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}


/**
 * set device authorization token end point url
 */
int
lmd_set_device_auth_token_endpoint_url_0(
    lmd* obj,
    const char* url,
    size_t length)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = obj->device_auth_token_endpoint_url != url;
        if (do_set) {
            if (obj->device_auth_token_endpoint_url) {
                free(obj->device_auth_token_endpoint_url);
                obj->device_auth_token_endpoint_url = NULL;
            }
            if (url) {
                char* str_obj;
                size_t len;
                str_obj = NULL;
                str_obj = (char*)malloc(length + 1); 
                if (str_obj) {
                    memcpy(str_obj, url, length);
                    str_obj[length] = '\0';
                    obj->device_auth_token_endpoint_url = str_obj;
                } else {
                    result = -1;
                }
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * set device oauth token endpoint request post header
 */
int
lmd_set_dot_eprqhd_options(
    lmd* obj,
    const char** options,
    size_t options_size)
{
    int result;
    if (obj) {
        int do_set;
        do_set = obj->dot_eprqhd_options != (char**)options;
        if (do_set) {
            size_t new_option_idx;
            char** new_options;
            if (options && options_size) {
                new_options = (char**) malloc(sizeof(char*) * options_size);
                if (new_options) {
                    for (new_option_idx = 0;
                        new_option_idx < options_size;
                        new_option_idx++) {
                        char* option;
                        size_t option_length;
                        option_length = strlen(options[new_option_idx]);
                        option = (char*)malloc(option_length + 1);
                        if (option) {
                            memcpy(option,
                                options[new_option_idx], option_length + 1);
                            new_options[new_option_idx] = option;
                        } else {
                            result = -1;
                            break;
                        }
                    }
                }
            }
            if (result == 0) {
                if (obj->dot_eprqhd_options) {
                    size_t idx; 
                    for (idx = 0;
                        idx < obj->dot_eprqhd_options_size; idx++) {
                        free(obj->dot_eprqhd_options[idx]);
                    }
                    free(obj->dot_eprqhd_options);
                    obj->dot_eprqhd_options = NULL;
                }
                obj->dot_eprqhd_options = new_options;
                obj->dot_eprqhd_options_size = options_size;
                new_options = NULL;
            }

            if (new_options) {
                size_t idx; 
                for (idx = 0; idx < new_option_idx; idx++) {
                    free(new_options[idx]);
                }
                free(new_options);
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}
/**
 * get the size of device oauth token endpoint request header options
 */
size_t
lmd_get_dot_eprqhd_options_size(
    lmd* obj)
{
    size_t result;
    result = 0;
    if (obj) {
        result = obj->dot_eprqhd_options_size; 
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * get options of device oauth token endpoint request post header
 */
const char**
lmd_get_dot_eprqhd_options_ref(
    lmd* obj)
{
    const char** result;
    result = NULL;
    if (obj) {
        result = (const char**)obj->dot_eprqhd_options;
    } else {
        errno = EINVAL;
    }
    return result;
}


/**
 * user codde
 */
const char*
lmd_get_user_code_ref(
    lmd* obj)
{
    const char* result;
    if (obj) {
        result = obj->user_code;
    } else {
        result = NULL;
        errno = EINVAL;
    }
    return result;
}

/**
 * user code
 */
int
lmd_set_user_code(
    lmd* obj,
    const char* user_code)
{
    int result;
    result = 0;
    if (obj) {
        size_t length;
        if (user_code) {
            length = strlen(user_code);
        } else {
            length = 0;
        }
        lmd_set_user_code_0(obj, user_code, length);
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result; 
}

/**
 * set user code
 */
int
lmd_set_user_code_0(
    lmd* obj,
    const char* user_code,
    size_t length)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = obj->user_code != user_code;
        if (do_set) {
            if (obj->user_code) {
                free(obj->user_code);
                obj->user_code = NULL;
            }
            if (user_code) {
                char* str_obj;
                str_obj = NULL;
                str_obj = (char*)malloc(length + 1); 
                if (str_obj) {
                    memcpy(str_obj, user_code, length);
                    str_obj[length] = '\0';
                    obj->user_code = str_obj;
                } else {
                    result = -1;
                }
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * device code
 */
const char*
lmd_get_device_code_ref(
    lmd* obj) {
    const char* result;
    if (obj) {
        result = obj->device_code;
    } else {
        errno = EINVAL;
        result = NULL;
    }
    return result;
}

/**
 * device code
 */
int
lmd_set_device_code(
    lmd* obj,
    const char* device_code)
{
    int result;
    result = 0;
    if (obj) {
        size_t length;
        if (device_code) {
            length = strlen(device_code);
        } else {
            length = 0;
        }
        lmd_set_device_code_0(obj, device_code, length);
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result; 
}

/**
 * set device code
 */
int
lmd_set_device_code_0(
    lmd* obj,
    const char* device_code,
    size_t length)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = obj->device_code != device_code;
        if (do_set) {
            if (obj->device_code) {
                free(obj->device_code);
                obj->device_code = NULL;
            }
            if (device_code) {
                char* str_obj;
                str_obj = NULL;
                str_obj = (char*)malloc(length + 1); 
                if (str_obj) {
                    memcpy(str_obj, device_code, length);
                    str_obj[length] = '\0';
                    obj->device_code = str_obj;
                } else {
                    result = -1;
                }
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * set tenant for microsoft oauth token
 */
int
lmd_set_ms_tenant(
    lmd* obj,
    const char* tenant)
{
    int result;
    result = 0;
    if (obj) {
        size_t length;
        length = 0;
        if (tenant) {
            length = strlen(tenant);
        }
        lmd_set_ms_tenant_0(obj, tenant, length);
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * set tenant for microsoft oauth token
 */
int
lmd_set_ms_tenant_0(
    lmd* obj,
    const char* tenant,
    size_t length)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = obj->ms_tenant != tenant;
        if (do_set) {
            if (obj->ms_tenant) {
                free(obj->ms_tenant);
                obj->ms_tenant = NULL;
            }
            if (tenant) {
                char* str_obj;
                str_obj = NULL;
                str_obj = (char*)malloc(length + 1); 
                if (str_obj) {
                    memcpy(str_obj, tenant, length);
                    str_obj[length] = '\0';
                    obj->ms_tenant = str_obj;
                } else {
                    result = -1;
                }
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}


/**
 * get tenant for microsoft oauth token
 */
const char*
lmd_get_ms_tenant_ref(
    lmd* obj)
{
    const char* result;
    result = NULL;
    if (obj) {
        result = obj->ms_tenant;
    } else {
        errno = EINVAL;
    }
    return result;
}


/**
 * expiration seconds to poll for oath token
 */
int
lmd_get_polling_expires_in(
    lmd* obj)
{
    int result;
    if (obj) {
        result = obj->polling_expires_in;
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * expiration seconds to poll for auth token
 */
int
lmd_set_polling_expires_in(
    lmd* obj,
    int expires_in)
{
    int result;
    if (obj) {
        obj->polling_expires_in = expires_in;
        result = 0;
    } else {
        errno = EINVAL;
        result = -1;
    }
    return result;
}

/**
 * interval seconds to poll for oath token
 */
int
lmd_get_interval(
    lmd* obj)
{
    int result;
    if (obj) {
        result = obj->interval;
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * interval seconds to poll for auth token
 */
int
lmd_set_interval(
    lmd* obj,
    int interval)
{
    int result;
    if (obj) {
        obj->interval = interval;
        result = 0;
    } else {
        errno = EINVAL;
        result = -1;
    }
    return result;
}

/**
 * set oauth loader 
 */
int
lmd_set_oauth_token_loader(
    lmd* obj,
    int (*loader)(lmd*, json_object*))
{
    int result;
    result = 0;
    if (obj) {
        obj->oauth_token_loader = loader; 
    } else {
        errno = EINVAL;
        result = -1;
    }
    return result;
}

/**
 * set oauth loader 
 */
int
lmd_get_oauth_token_loader(
    lmd* obj,
    int (**loader)(lmd*, json_object*))
{
    int result;
    result = 0;
    if (obj) {
        if (loader) {
            *loader = obj->oauth_token_loader;
        }
    } else {
        errno = EINVAL;
        result = -1;
    }
    return result;
}


/**
 * set credential operation
 */
int
lmd_set_credential_op(
    lmd* obj,
    credential_op op)
{
    int result;
    if (obj) {
        result = 0;
        obj->credential_operation = op;
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * get credential operation
 */
credential_op
lmd_get_credential_op(
    lmd* obj)
{
    credential_op result;
    result = CDT_OP_GET;
    if (obj) {
        result = obj->credential_operation;
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * set verbose level
 */
int
lmd_set_verbose_level(
    lmd* obj,
    int verbose_level)
{
    int result;
    if (obj) {
        result = 0;
        obj->verbose_level = verbose_level;
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * get verbose level
 */
int
lmd_get_verbose_level(
    lmd* obj)
{
    int result;
    result = 0;
    if (obj) {
        result = obj->verbose_level;
    } else {
        errno = EINVAL;
    }

    return result;
}


/**
 * set generator mode
 */
int
lmd_set_generator_mode(
    lmd* obj,
    int mode)
{
    int result;
    if (obj) {
        obj->generator_mode = mode;
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * get non zero if limited device info is running on generator mode.
 */
int
lmd_is_generator_mode(
    lmd* obj)
{
    int result;
    result = 0;
    if (obj) {
        result = obj->generator_mode;
    } else {
        errno = EINVAL;
    }
    return result;
}
 
/**
 * get string representation
 */
char*
lmd_get_str_representation(
    lmd* obj)
{
    struct {
        const char* name;
        const char* value;
        size_t name_size;
        size_t value_size;
    } name_str_values[] = {
        {
            .name = "token_endpoint_url",
            .value = lmd_get_token_endpoint_url_ref(obj),
        },
        {
            .name = "client_id",
            .value = lmd_get_client_id_ref(obj)
        },
        {
            .name = "client_secret",
            .value = lmd_get_client_secret_ref(obj)
        },
        {
            .name = "token_endpoint",
            .value = lmd_get_token_endpoint_url_ref(obj)
        },
        {
            .name = "device_auth_token_endpoint",
            .value = lmd_get_device_auth_token_endpoint_url_ref(obj)
        },
        {
            .name = "verification_url",
            .value = lmd_get_verification_url_ref(obj)
        },
        {
            .name = "user_code",
            .value = lmd_get_user_code_ref(obj)
        },
        {
            .name = "device_code",
            .value = lmd_get_device_code_ref(obj)
        },
        {
            .name = "access_token",
            .value = lmd_get_access_token_ref(obj)
        },
        {
            .name = "refresh_token",
            .value = lmd_get_refresh_token_ref(obj)
        },
        {
            .name = "scope",
            .value = lmd_get_scope_ref(obj)
        }
    };
    struct {
        const char* name;
        size_t name_size;
        int value;
    } name_int_values[] = {
        {
            .name = "polling_expires_in",
            .value = lmd_get_polling_expires_in(obj)
        },
        {
            .name = "interval",
            .value = lmd_get_interval(obj)
        },
        {
            .name = "expires_in",
            .value = lmd_get_expires_in(obj)
        },
        {
            .name = "credential_operation",
            .value = (int)lmd_get_credential_op(obj)
        },
        {
            .name = "generator_mode",
            .value = lmd_is_generator_mode(obj)
        }

    };

    int i;
    size_t buffer_size;
    char* result;
    const static size_t int_buffer_size = 20;
    result = NULL;
    buffer_size = 0;
    for (i = 0; i < sizeof(name_str_values) / sizeof(name_str_values[0]); i++) {
        name_str_values[i].name_size = strlen(name_str_values[i].name);
        if (name_str_values[i].value) {
            name_str_values[i].value_size = strlen(name_str_values[i].value);
        } else {
            name_str_values[i].value_size = 0;
        }
        buffer_size += name_str_values[i].name_size;
        buffer_size += name_str_values[i].value_size;
        /* add the space for new line */
        buffer_size += 2;
    }
    for (i = 0; i < sizeof(name_int_values) / sizeof(name_int_values[0]); i++) {
        name_int_values[i].name_size = strlen(name_int_values[i].name);
        buffer_size += name_int_values[i].name_size;
        buffer_size += int_buffer_size + 2; 
    }
    result = (char*)malloc(buffer_size + 1);
    if (result) {
        char* buffer_ptr;
        buffer_ptr = result;
        for (i = 0; i < sizeof(name_str_values) / sizeof(name_str_values[0]);
            i++) {
            memcpy(buffer_ptr, name_str_values[i].name,
                name_str_values[i].name_size); 
            buffer_ptr += name_str_values[i].name_size;
            buffer_ptr[0] = '\n';
            buffer_ptr++;
            memcpy(buffer_ptr, name_str_values[i].value,
                name_str_values[i].value_size);
            buffer_ptr += name_str_values[i].value_size;
            buffer_ptr[0] = '\n';
            buffer_ptr++;
        }
        for (i = 0; i < sizeof(name_int_values) / sizeof(name_int_values[0]);
            i++) {
            memcpy(buffer_ptr, name_int_values[i].name,
                name_int_values[i].name_size);
            buffer_ptr += name_int_values[i].name_size;
            buffer_ptr[0] = '\n';
            buffer_ptr++;
            buffer_ptr += snprintf(buffer_ptr, int_buffer_size,
                "%d\n", name_int_values[i].value);
        }
        buffer_ptr[0] = '\0';
    }
    return result;
}

/**
 * release heap object which is made in this interface
 */
void
lmd_free_object(
    lmd* obj,
    void* heap_obj)
{
    free(heap_obj);
}


/* vi: se ts=4 sw=4 et: */
