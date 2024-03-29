#include "ui_token_gen.h"
#include "ui_token_gen_i.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "token_gen_ui.h"
#include "credential_desc.h"
#include "logging.h"

/**
 * create token generator user interface
 */
ui_token_gen*
ui_token_gen_create()
{
    ui_token_gen* result;
    result = (ui_token_gen*)ui_token_gen_i_alloc(sizeof(ui_token_gen));
    if (result) {
        result->ref_count = 1;
        result->service = NULL;
    }
    return result;
}


/**
 * increment reference count
 */
unsigned int
ui_token_gen_retain(
    ui_token_gen* obj)
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
ui_token_gen_release(
    ui_token_gen* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = --obj->ref_count; 
        if (result == 0) {
            ui_token_gen_set_service(obj, NULL);
            ui_token_gen_i_free(obj);
        }
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * set initial service for user interface
 */
int
ui_token_gen_set_service(
    ui_token_gen* obj,
    const char* service)
{
    int result;
    result = 0; 
    if (obj) {
        int do_set;
        do_set = obj->service != service;
        if (do_set) {
            char* new_service;
            new_service = NULL;
            if (service) {
                size_t length;
                length = strlen(service);
                new_service = (char*)ui_token_gen_i_alloc(length + 1);
                result = new_service ? 0 : -1;
                if (result == 0) {
                    memcpy(new_service, service, length + 1);
                }
            }
            if (result == 0) {
                if (obj->service) {
                    ui_token_gen_i_free(obj->service);
                }
                obj->service = new_service;
            }
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}


/**
 * get initial service for user inteface
 */
const char*
ui_token_gen_get_service_ref(
    ui_token_gen* obj)
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
 * create token
 */
int
ui_token_gen_create_token(
    ui_token_gen* obj,
    const char* descriptor,
    size_t descriptor_length,
    char** response,
    size_t* response_length)
{
    int result;
    if (obj) {
        credential_desc* desc; 
        char* token;
        token = NULL;
        logging_log(LOG_LEVEL_DEBUG, "ui token generator create token");
        desc = credential_desc_decode(descriptor, descriptor_length);
        result = desc ? 0 : -1;
        if (result == 0) {
            result = token_gen_ui_run(
                descriptor, descriptor_length, 
                ui_token_gen_get_service_ref(obj),
                &token);
        }
          
        if (token) {
            logging_log(LOG_LEVEL_DEBUG, "got token %s", token);
            if (result == 0) {
                result = credential_desc_set_password(desc, token);
            }
            if (result == 0) {
                result = credential_desc_set_username(desc, "bearer");
            }
            if (result == 0) {
                result = credential_desc_set_quit(desc, 1);
            } 
            if (result == 0) {
                char* encoded;
                size_t encoded_size;
                encoded = NULL;
                result = credential_desc_encode(desc, &encoded, &encoded_size);
                if (result == 0) {
                    char* desc_res;
                    desc_res = (char*)ui_token_gen_i_alloc(encoded_size);
                    result = desc_res ? 0 : -1;
                    if (result == 0) {
                        memcpy(desc_res, encoded, encoded_size);
                        *response = desc_res;
                        *response_length = encoded_size;
                    }
                }
                if (encoded) {
                    credential_desc_free_object(encoded);
                }
            }
        }
        if (token) {
            token_gen_ui_free(token);
        }
        if (desc) {
            credential_desc_free(desc);
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * free object created in this interface.
 */
void
ui_token_gen_free(
    void* obj)
{
    ui_token_gen_i_free(obj);
}

/* vi: se ts=4 sw=4 et: */
