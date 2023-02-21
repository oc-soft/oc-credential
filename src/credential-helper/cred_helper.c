#include "cred_helper.h"
#include <stdlib.h>
#include <errno.h>
#include "cred_helper_i.h"
#include "ui_token_gen.h"
#include "lmd.h"

/**
 * crendential helper
 */
struct _cred_helper {
    /**
     * reference count
     */
    unsigned int ref_count;

    /**
     * credential helper for limited device 
     */
    lmd* limited_device;

    /**
     * token generator user interface
     */
    ui_token_gen* ui_token_generator;

    /**
     * client id
     */
    char* client_id;

    /**
     * verbose level
     */
    int verbose_level;

    /**
     * access token
     */
    char* access_token;

    /**
     * credential operation from git 
     */
    credential_op credential_operation;
    
 
    /**
     * test mode
     */
    int test_mode_get:1;

    /**
     * generator mode
     */
    int generator_mode:1;

    /**
     * run gui generator
     */
    int run_gui_gen:1;

    /**
     * run limited device generator
     */
    int run_lmd_gen:1;

    /**
     * if no zero then user request usage program
     */
    int usage_request:1;
};

/**
 * set new_value into string field
 */
static int
cred_helper_set_str_field(
    char** field,
    const char* value);

/**
 * create credential helper
 */
cred_helper*
cred_helper_create()
{
    cred_helper* result;
    lmd* limited_device;
    ui_token_gen* ui_token_generator;
    result = (cred_helper*)cred_helper_i_alloc(sizeof(cred_helper));
    limited_device = lmd_create();
    ui_token_generator = ui_token_gen_create();
    if (result && limited_device && ui_token_generator) {
        result->ref_count = 1;
        result->limited_device = limited_device;
        result->ui_token_generator = ui_token_generator;
        result->client_id = NULL;
        result->access_token = NULL;
        result->credential_operation = CDT_OP_UNKNOWN;
        result->verbose_level = 0;
        result->generator_mode = 0;
        result->test_mode_get = 0;
        result->run_gui_gen = 1;
        result->run_lmd_gen = 1;
        result->usage_request = 0;
    } else {
        if (ui_token_generator) {
            ui_token_gen_release(ui_token_generator);
        }
        if (limited_device) {
            lmd_release(limited_device);
        }
        if (result) {
            cred_helper_i_free(result);
            result = NULL;
        }
    }

    return result;
}

/**
 * increment reference count
 */
unsigned int
cred_helper_retain(
    cred_helper* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = ++obj->ref_count;
    } else {
        errno = EINVAL;
    }
    return result; 
}

/**
 * decrement reference count
 */
unsigned int
cred_helper_release(
    cred_helper* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = --obj->ref_count;
        if (result == 0) {
            cred_helper_set_client_id(obj, NULL);
            cred_helper_set_client_secret(obj, NULL);
            ui_token_gen_release(obj->ui_token_generator);
            lmd_release(obj->limited_device);
            cred_helper_i_free(obj);
        }
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * set credential operation
 */
int
cred_helper_set_credential_op(
    cred_helper* obj,
    credential_op op)
{
    int result;
    result = 0;
    if (obj) {
        obj->credential_operation = op;
    } else {
        errno = EINVAL;
        result = -1;
    }
    return result;
}

/**
 * get credential operation
 */
credential_op
cred_helper_get_credential_op(
    cred_helper* obj)
{
    credential_op result;
    result = CDT_OP_UNKNOWN;
    if (obj) {
        result = obj->credential_operation;
    } else {
        errno = EINVAL;
        result = -1;
    }
    return result;
}


/**
 * set client id
 */
int
cred_helper_set_client_id(
    cred_helper* obj,
    const char* client_id)
{
    int result;
    if (obj) {
        result = lmd_set_client_id(obj->limited_device, client_id);
        if (result == 0) {
            result = cred_helper_set_str_field(&obj->client_id, client_id);
            if (result) {
                lmd_set_client_id(obj->limited_device, obj->client_id);
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
cred_helper_get_client_id_ref(
    const cred_helper* obj)
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
 * get client secret
 */
const char*
cred_helper_get_client_secret_ref(
    const cred_helper* obj)
{
    const char* result;
    result = NULL;
    if (obj) {
        result = lmd_get_client_secret_ref(obj->limited_device);
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * set client secret
 */
int
cred_helper_set_client_secret(
    cred_helper* obj,
    const char* client_secret)
{
    int result;
    if (obj) {
        result = lmd_set_client_secret(obj->limited_device, client_secret);
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * get access token
 */
const char*
cred_helper_get_access_token_ref(
    const cred_helper* obj)
{
    const char* result;
    result = NULL;
    if (obj) {
        result = obj->access_token;
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * set access token
 */
int
cred_helper_set_access_token(
    cred_helper* obj,
    const char* access_token)
{
    int result;
    if (obj) {
        result = lmd_set_access_token(obj->limited_device, access_token);
        if (result == 0) {
            result = cred_helper_set_str_field(
                &obj->access_token, access_token);
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * set test mode to get
 */
int
cred_helper_set_test_mode_to_get(
    cred_helper* obj,
    int testing_mode)
{
    int result;
    if (obj) {
        result = 0;
        obj->test_mode_get = testing_mode;
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * get test mode to get
 */
int
cred_helper_is_test_mode_to_get(
    cred_helper* obj)
{
    int result;
    result = 0;
    if (obj) {
        result = obj->test_mode_get;
    } else {
        errno = EINVAL;
    }

    return result;
}
 

/**
 * set generator mode
 */
int
cred_helper_set_generator_mode(
    cred_helper* obj,
    int mode)
{
    int result;
    result = 0;
    if (obj) {
        result = lmd_set_generator_mode(obj->limited_device, mode);
        if (result == 0) {
            obj->generator_mode = mode;
        } 
    } else {
        errno = EINVAL;
        result = -1;
    }
    return result;
}

/**
 * get non zero if cred_helper info is running on generator mode.
 */
int
cred_helper_is_generator_mode(
    cred_helper* obj)
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
 * set the flag for running for gui generator.
 */
int
cred_helper_run_gui_generator(
    cred_helper* obj, 
    int running)
{
    int result;
    result = 0;
    if (obj) {
        obj->run_gui_gen = running;
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}


/**
 * get the flag for running for gui generator.
 */
int
cred_helper_dose_run_gui_generator(
    cred_helper* obj)
{
    int result;
    result = 0;
    if (obj) {
        result = obj->run_gui_gen;
    } else {
        errno = EINVAL;
    }

    return result;
}


/**
 * set the flag for running limited deivce generator.
 */
int
cred_helper_run_limited_device(
    cred_helper* obj, 
    int running)
{
    int result;
    result = 0;
    if (obj) {
        obj->run_lmd_gen = running;
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * get the flag for running limited deivce generator.
 */
int
cred_helper_dose_run_limited_device(
    cred_helper* obj)
{
    int result;
    result = 0;
    if (obj) {
        result = obj->run_lmd_gen;
    } else {
        errno = EINVAL;
    }

    return result;
}

/**
 * you get no zero if user request to get usage
 */
int
cred_helper_is_requested_usage(
    cred_helper* obj)
{
    int result;
    result = 0;
    if (obj) {
        result = obj->usage_request;
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * set no zero if user request to get usage
 */
int
cred_helper_set_requested_usage(
    cred_helper* obj,
    int usage_request)
{
    int result;
    result = 0;
    if (obj) {
        obj->usage_request = usage_request;
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * set verbose level
 */
int
cred_helper_set_verbose_level(
    cred_helper* obj,
    int verbose_level)
{
    int result;
    result = 0;
    if (obj) {
        result = lmd_set_verbose_level(obj->limited_device, verbose_level);
        if (result == 0) {
            obj->verbose_level = verbose_level;
        }
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * get verbose level
 */
int
cred_helper_get_verbose_level(
    cred_helper* obj)
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
 * get limited device access object
 */
lmd*
cred_helper_get_lmd(
    cred_helper* obj)
{
    lmd* result;
    result = NULL;
    if (obj) {
        lmd_retain(obj->limited_device);
        result = obj->limited_device;
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * get token generator user interface
 */
ui_token_gen*
cred_helper_get_ui_token_gen(
    cred_helper* obj)
{
    ui_token_gen* result;
    result = NULL;
    if (obj) {
        ui_token_gen_retain(obj->ui_token_generator); 
        result = obj->ui_token_generator;
    } else {
        errno = EINVAL;
    }
    return result;
}


/**
 * update access token with embeded lmd object
 */
int
cred_helper_update_access_token_with_lmd(
    cred_helper* obj)
{
    int result;
    if (obj) {
        result = cred_helper_set_str_field(
            &obj->access_token,
            lmd_get_access_token_ref(obj->limited_device));
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * set new_value into string field
 */
static int
cred_helper_set_str_field(
    char** field,
    const char* value)
{
    int result;
    int do_copy;
    do_copy = 0;
    result = 0;
    if (*field) {
        do_copy = *field != value;
    }
    if (do_copy) {
        char* new_field;
        if (value) { 
            new_field = cred_helper_i_strdup(value);
            result = new_field ? 0 : -1;
        } else {
            new_field = NULL;
        }
        if (result == 0) {
            if (*field) {
                cred_helper_i_free(*field);
            }
            *field = new_field;
        }
    }
    return result;
}

/* vi: se ts=4 sw=4 et: */

