#include "lmd.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


struct _lmd {
    /* reference count */
    unsigned int ref_count;

    /* client id */
    char* client_id;

    /* client secret */
    char* client_secret;

    /* verification url */
    char* verification_url;

    /* token endpoint url */
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

    
    /* verbose level */
    int verbose_level;
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
        result->client_id = NULL;
        result->client_secret = NULL;
        result->verification_url = NULL;
        result->token_endpoint_url = NULL;
        result->device_auth_token_endpoint_url = NULL;
        result->device_code = NULL;
        result->user_code = NULL;
        result->expires_in = 0;
        result->access_token = NULL;
        result->token_type = NULL;
        result->refresh_token = NULL;
        result->scope = NULL;
        result->polling_expires_in = 0;
        result->interval = 0;
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
            lmd_set_client_id(obj, NULL);
            lmd_set_client_secret(obj, NULL);
            lmd_set_device_auth_token_endpoint_url(obj, NULL);
            lmd_set_token_endpoint_url(obj, NULL);
            lmd_set_user_code(obj, NULL);
            lmd_set_device_code(obj, NULL);
             
            lmd_set_access_token(obj, NULL);
            lmd_set_token_type(obj, NULL);
            lmd_set_scope(obj, NULL);
            lmd_set_refresh_token(obj, NULL);
            free(obj);
        }
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
