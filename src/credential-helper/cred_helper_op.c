#include "cred_helper_op.h"
#include <errno.h>
#include <stdio.h>
#include <libintl.h>
#include <string.h>
#include "cred_helper.h"
#include "cred_helper_i.h"
#include "ui_token_gen.h"
#include "lmd.h"
#include "lmd_op.h"
#include "client_secret.h"
#include "client_id.h"
#include "credential_desc.h"
#include "credential_storage.h"
#include "buffer/char_buffer.h"
#include "logging.h"

/**
 * find client id and clietn secret
 */
static int
find_client_id_secret(
    const char* protocol,
    const char* host,
    const char* path,
    const char** client_id,
    const char** client_secret);

/**
 * get oauth token
 */
static int
get_oauth_token(
    cred_helper* obj,
    credential_desc* desc);

/**
 * get oauth token with user interface
 */
static int
get_oauth_token_with_ui(
    cred_helper* obj,
    credential_desc* desc);


/**
 * get oauth token with limited device
 */
static int
get_oauth_token_with_lmd(
    cred_helper* obj,
    credential_desc* desc);

/**
 * get oauth token
 */
static int
get_oauth_token_with_client(
    cred_helper* obj,
    const char* client_id,
    const char* client_secret);

/**
 * read all data from stdin
 */
static int
cred_helper_op_read_all(
    char** contents,
    size_t* contents_length);

/**
 * read crendential descriptor from stdin
 */
static credential_desc*
cred_helper_op_read_from_stdin();

/**
 * write crendential descriptor into stdout
 */
static int
cred_helper_op_write_into_stdout(
    const credential_desc* desc);


/**
 * do credential get operation 
 */
static int
cred_helper_op_get(
    cred_helper* obj,
    credential_desc* desc);

/**
 * do credential get operation 
 */
static int
cred_helper_op_get_0(
    cred_helper* obj,
    credential_desc* desc);

/**
 * do credential get operation 
 */
static int
cred_helper_op_get_for_test(
    cred_helper* obj,
    credential_desc* desc);


/**
 * do credential store operation
 */
static int
cred_helper_op_store(
    cred_helper* obj,
    credential_desc* desc);

/**
 * do credential erase operation
 */
static int
cred_helper_op_erase(
    cred_helper* obj,
    credential_desc* desc);


/**
 * run credential operation
 */
int
cred_helper_op_run(
    cred_helper* obj)
{
    int result;
    credential_desc* desc;
    desc = NULL;
    if (obj) {
        result = 0;
    } else {
        result = -1;
        errno = EINVAL;
    }
    if (!result) {
        if (!cred_helper_is_requested_usage(obj)) {
            if (!result) { 
                desc = cred_helper_op_read_from_stdin();
                result = desc ? 0 : -1; 
            }
            if (!result) {

                switch (cred_helper_get_credential_op(obj)) {
                case CDT_OP_GET:
                    result = cred_helper_op_get(obj, desc);
                    if (result == 0) {
                        result = cred_helper_op_write_into_stdout(desc);
                    }
                    break;
                case CDT_OP_STORE:
                    result = cred_helper_op_store(obj, desc);
                    break;
                case CDT_OP_ERASE:
                    result = cred_helper_op_erase(obj, desc);
                    break;
                default:
                    result = 0;
                    break;
                }
            }
        }
    }
    if (desc) {
        credential_desc_free(desc);
    }
    return result;
}

/**
 * find client id and clietn secret
 */
static int
find_client_id_secret(
    const char* protocol,
    const char* host,
    const char* path,
    const char** client_id,
    const char** client_secret)
{
    char* path_buffer;
    size_t path_buffer_len; 
    int result;
    const char* client_id_0;
    const char* client_secret_0;
    result = 0;
    path_buffer = NULL;
    client_id_0 = NULL;
    client_secret_0 = NULL;
    path_buffer_len = 0;
    if (path) {
        path_buffer_len = strlen(path);
        path_buffer = (char*)cred_helper_i_alloc(path_buffer_len + 1); 
        result = path_buffer ? 0 : -1;
        if (result == 0) {
            memcpy(path_buffer, path, path_buffer_len + 1);
        }
    }
    if (result == 0) {
        while (1) {
            if (!client_id_0) {
                client_id_0 = client_id_get(protocol, host,
                    path_buffer);
            }
            if (!client_secret_0) {
                client_secret_0 = client_secret_get(protocol, host,
                    path_buffer);
            }
 
            if (client_id_0 && client_secret_0) {
                break;
            } else {
                if (path_buffer) {
                    char* tmp_char;
                    tmp_char = strrchr(path_buffer, '/');
                    if (tmp_char) {
                        *tmp_char = '\0';
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            }
        }
    } 
    if (client_id) {
        *client_id = client_id_0;
    }
    if (client_secret) {
        *client_secret = client_secret_0;
    }
    result = client_id_0 && client_secret_0 ? 0 : -1;
    if (path_buffer) {
        cred_helper_i_free(path_buffer);
    }
    return result;
}


/**
 * get oauth token
 */
static int
get_oauth_token(
    cred_helper* obj,
    credential_desc* desc)
{
    int result;
    result = 0;
    logging_log(LOG_LEVEL_DEBUG, "get oauth token");
    if (cred_helper_dose_run_gui_generator(obj) 
        && cred_helper_dose_run_limited_device(obj)) {
        int state;
        state = get_oauth_token_with_ui(obj, desc);
        if (state) {
            state = get_oauth_token_with_lmd(obj, desc);
        }
        result = state;
    } else if (cred_helper_dose_run_gui_generator(obj)) {
        result = get_oauth_token_with_ui(obj, desc);
    } else if (cred_helper_dose_run_limited_device(obj)) {
        result = get_oauth_token_with_lmd(obj, desc);
    } else {
        result = 0;
    }
    return result;
}

/**
 * get oauth token with user interface
 */
static int
get_oauth_token_with_ui(
    cred_helper* obj,
    credential_desc* desc)
{
    int result;
    ui_token_gen* token_gen;
    char* descriptor;
    size_t descriptor_length;
    token_gen = NULL;
    descriptor = NULL;
    descriptor_length = 0;
    result = 0;

    logging_log(LOG_LEVEL_DEBUG, "get oauth token with ui");

    token_gen = cred_helper_get_ui_token_gen(obj);
    result = token_gen ? 0 : -1;

    if (result == 0) {
        result = credential_desc_encode(desc, &descriptor, &descriptor_length);
    }
    if (result == 0) {
        char* desc_encoded_res;
        size_t desc_encoded_res_size;
        desc_encoded_res = NULL;
        desc_encoded_res_size = 0;
        result = ui_token_gen_create_token(
            token_gen, descriptor, descriptor_length,
            &desc_encoded_res, &desc_encoded_res_size);
        if (result == 0) {
            credential_desc* desc_res;
            desc_res = credential_desc_decode(
                desc_encoded_res, desc_encoded_res_size); 

            result = desc_res ? 0 : -1;
            logging_log(LOG_LEVEL_DEBUG,
                "desc_res: %p", desc_res);
            if (result == 0) {
                logging_log(LOG_LEVEL_DEBUG,
                    "desc_res password: %p", desc_res->password);
                result = cred_helper_set_access_token(obj, desc_res->password);
            }
            if (desc_res) {
                credential_desc_free(desc_res);
            } 
        }
        if (desc_encoded_res) {
            ui_token_gen_free(desc_encoded_res);
        }
    }
    if (descriptor) {
        credential_desc_free_object(descriptor);
    }

    if (token_gen) {
        ui_token_gen_release(token_gen);
    }
    return result;
}


/**
 * get oauth token with limited device
 */
static int
get_oauth_token_with_lmd(
    cred_helper* obj,
    credential_desc* desc)
{
    const char* client_id;
    const char* client_secret;
    int result; 
    client_id = NULL;
    client_secret = NULL;
    result = find_client_id_secret(
        desc->protocol, desc->host, desc->path, &client_id, &client_secret);
    if (result == 0) {
        lmd* limited_acc;
        limited_acc = cred_helper_get_lmd(obj);
        if (limited_acc) {
            result = lmd_get_oauth_token_with_client(
                limited_acc, client_id, client_secret);
            if (result == 0) {
                result = cred_helper_update_access_token_with_lmd(obj);
            }
        } else {
            result = -1;
        }
        if (limited_acc) {
            lmd_release(limited_acc);
        }
    }
    return result;
}
    

/**
 * read contents from stdin
 */
int
cred_helper_op_read_all(
    char** contents,
    size_t* contents_length)
{
    int result;
    buffer_char_buffer* buffer; 
    char* tmp_buffer; 
    const size_t tmp_buffer_size = 256;
    tmp_buffer = NULL;
    buffer = buffer_char_buffer_create_00(
        cred_helper_i_alloc,
        cred_helper_i_realloc,
        cred_helper_i_free,
        cred_helper_i_mem_copy,
        cred_helper_i_mem_move, 100);

    result = buffer ? 0 : -1;
    if (result == 0) {
        tmp_buffer = (char*)cred_helper_i_alloc(tmp_buffer_size);
        result = tmp_buffer ? 0 : -1;
    } 

    while (!result) {
        size_t read_size;
        read_size = fread(tmp_buffer, 1, tmp_buffer_size, stdin);
        result = buffer_char_buffer_append(buffer, tmp_buffer, read_size); 
        if (!read_size) {
            break;
        }
    } 
    if (!result) {
        size_t buffer_size;
        char* tmp_contents;
        buffer_size = buffer_char_buffer_get_size(buffer);
        tmp_contents = (char*)cred_helper_i_alloc(buffer_size); 
        result = tmp_contents ? 0 : -1;
        if (!result) {
            buffer_char_buffer_copy_to(buffer, tmp_contents);
            *contents = tmp_contents;
            *contents_length = buffer_size;
        }
    }
    if (tmp_buffer) {
        cred_helper_i_free(tmp_buffer); 
    }
    if (buffer) {
        buffer_char_buffer_release(buffer);
    }
    return result;
}

/**
 * read credential_desc from stdin
 */
static credential_desc*
cred_helper_op_read_from_stdin()
{
    char* contents;
    size_t contents_length;
    credential_desc* result;
    int state;
    result = NULL;
    contents = NULL;
    contents_length = 0;
    state = cred_helper_op_read_all(&contents, &contents_length);
    if (state == 0) {
        result = credential_desc_decode(contents, contents_length);
    }

    if (contents) {
        cred_helper_i_free(contents);
    }
    return result;
}

/**
 * write credential_desc into stdout
 */
static int
cred_helper_op_write_into_stdout(
    const credential_desc* desc)
{
    char* contents;
    size_t contents_length;
    int result;
    contents = NULL;
    contents_length = 0;

    result = credential_desc_encode(desc, &contents, &contents_length);

    if (result == 0) {
        size_t size_written;
        size_written = fwrite(contents, contents_length, 1, stdout);
        result = size_written == contents_length ? 0 : -1;
    }

    if (contents) {
        credential_desc_free_object(contents);
    }
    return result;
}

/**
 * do credential get operation 
 */
static int
cred_helper_op_get(
    cred_helper* obj,
    credential_desc* desc)
{
    int result;
    result = 0;
    if (cred_helper_is_test_mode_to_get(obj) == 0) {
        result = cred_helper_op_get_0(obj, desc);
    } else {
        result = cred_helper_op_get_for_test(obj, desc); 
    }
    return result;
}

/**
 * do credential get operation 
 */
static int
cred_helper_op_get_for_test(
    cred_helper* obj,
    credential_desc* desc)
{
    const char* client_id;
    const char* client_secret;
    int result;

    client_id = NULL;
    client_secret = NULL;
    result = find_client_id_secret(desc->protocol,
        desc->host, desc->path, &client_id, &client_secret);
    if (result == 0) {
        result = credential_desc_set_username(desc, client_id);
    }  
    if (result == 0) {
        result = credential_desc_set_password(desc, client_secret);
    }
    return result;
}



/**
 * do credential get operation 
 */
static int
cred_helper_op_get_0(
    cred_helper* obj,
    credential_desc* desc)
{
    int result;
    char* password;

    result = 0;
    password = NULL;
    logging_log(LOG_LEVEL_DEBUG, "operation get credential");
    if (!lmd_is_generator_mode(0)) {
        result = credential_storage_find_password(desc->protocol, desc->host,
            desc->path, desc->username, &password);
    }
    if (!password) {
        result = get_oauth_token(obj, desc);
        if (result == 0) {
            const char* acc_token;
            acc_token = cred_helper_get_access_token_ref(obj);
            if (acc_token) {
                result = credential_desc_set_username(desc, "bearer");
                if (result == 0) {
                    result = credential_desc_set_password(desc, acc_token);
                }
            }
        }
    } else {
        result = credential_desc_set_password(desc, password);
    }
    if (password) {
        credential_storage_free_object(password);
    }
    return result;
}

/**
 * do credential store operation
 */
int
cred_helper_op_store(
    cred_helper* obj,
    credential_desc* desc)
{
    int result;
    if (!cred_helper_is_generator_mode(obj)) {
        result = credential_storage_store_password(
            desc->protocol, desc->host, desc->path,
            desc->username, desc->password);
    } else {
        result = 1;
    }
    return result;
}

/**
 * do credential erase operation
 */
int
cred_helper_op_erase(
    cred_helper* obj,
    credential_desc* desc)
{
    int result; 
    if (!cred_helper_is_generator_mode(obj)) {
        result = credential_storage_remove_password(
            desc->protocol, desc->host, desc->path, desc->username);
    } else {
        result = 1;
    }
    return result;
}


/* vi: se ts=4 sw=4 et: */
