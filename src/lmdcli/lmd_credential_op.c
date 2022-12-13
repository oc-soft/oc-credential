#include "lmd_credential_op.h"
#include <errno.h>
#include <stdio.h>
#include <libintl.h>
#include <string.h>
#include "lmd.h"
#include "lmd_i.h"
#include "lmd_connections.h"
#include "lmd_requests.h"
#include "client_secret.h"
#include "client_id.h"
#include "credential_desc.h"
#include "credential_storage.h"
#include "buffer/char_buffer.h"

/**
 * progress
 */
typedef struct _lmd_progress lmd_progress;

/**
 * progress
 */
struct _lmd_progress {
    /**
     * last second updating
     */
    int last_sec_updating;

};

/**
 * print progress for oauth token
 */
static int
print_progress_for_oauth_token(
    lmd_progress* progress, 
    int elapse,
    lmd* obj);

/**
 * get oauth token
 */
static int
get_oauth_token(
    lmd* limited_acc);

/**
 * read all data from stdin
 */
static int
lmd_credential_op_read_all(
    char** contents,
    size_t* contents_length);

/**
 * read crendential descriptor from stdin
 */
static credential_desc*
lmd_credential_op_read_from_stdin();

/**
 * write crendential descriptor into stdout
 */
static int
lmd_credential_op_write_into_stdout(
    const credential_desc* desc);


/**
 * do credential get operation 
 */
static int
lmd_credential_op_get(
    lmd* obj,
    credential_desc* desc);

/**
 * do credential store operation
 */
static int
lmd_credential_op_store(
    lmd* obj,
    credential_desc* desc);

/**
 * do credential remove operation
 */
static int
lmd_credential_op_remove(
    lmd* obj,
    credential_desc* desc);


/**
 * run credential operation
 */
int
lmd_credential_op_run(
    lmd* obj)
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
        desc = lmd_credential_op_read_from_stdin();
    }
    if (!result) {

        switch (lmd_get_credential_op(obj)) {
        case CDT_OP_GET:
            result = lmd_credential_op_get(obj, desc);
            if (result == 0) {
                result = lmd_credential_op_write_into_stdout(desc);
            }
            break;
        case CDT_OP_STORE:
            result = lmd_credential_op_store(obj, desc);
            break;
        case CDT_OP_ERASE:
            result = lmd_credential_op_remove(obj, desc);
            break;
        default:
            result = 0;
            break;
        }
    }
    if (desc) {
        credential_desc_free(desc);
    }
    return result;
}

/**
 * print progress for oauth token
 */
static int
print_progress_for_oauth_token(
    lmd_progress* progress,
    int elapse,
    lmd* lmd_obj)
{
    int result;
    result = 0;
    if (lmd_get_verbose_level(lmd_obj) < 5) {
        if (progress->last_sec_updating != elapse) {
            const char* verification_url;
            const char* user_code;
            int expires_in;
            verification_url = lmd_get_verification_url_ref(lmd_obj);
            user_code = lmd_get_user_code_ref(lmd_obj);
            expires_in = lmd_get_polling_expires_in(lmd_obj);

            if (!progress->last_sec_updating) {
                printf(
                    gettext(
                        "URL:\n"
                        "\x1b[1m%s\x1b[0m\n"
                        "Code:\n"
                        "\x1b[1m%s\x1b[0m\n"), 
                    verification_url, user_code); 

            } else {
            }
            printf("\r");
            printf(
                gettext(
                    "Open browser, visit the url and input the code as above" 
                    " in %d seconds."),
                expires_in - elapse); 
            fflush(stdout);
            
            progress->last_sec_updating = elapse; 
        }
    }
    return result;
}

/**
 * get oauth token
 */
static int
get_oauth_token(
    lmd* limited_acc)
{
    int result;
    lmd_progress progress;
    result = 0;
    if (result == 0) {
        memset(&progress, 0, sizeof(progress));
    }
    if (result == 0) {
        result = lmd_set_client_secret(limited_acc, client_secret_get());
    }
    if (result == 0 && !lmd_get_client_id_ref(limited_acc)) {
        result = lmd_set_client_id(limited_acc, client_id_get());
    }

    if (result == 0) {
        result = lmd_connections_load_discovery_document(limited_acc);
    }
    if (result == 0) {
        result = lmd_requests_load_device_and_user_code(limited_acc);
    }

    if (result == 0) {
        result = lmd_requests_poll_oauth_token(limited_acc,
            (int (*)(void*, int, lmd*))print_progress_for_oauth_token,
            &progress);
    }  
    if (result ==  0) {
        if (lmd_get_verbose_level(limited_acc) >= 5) {
            char* str_lmd;
            str_lmd = lmd_get_str_representation(limited_acc);
            if (str_lmd) {
                puts(str_lmd);
            } 
            lmd_free_object(limited_acc, str_lmd); 
        }
    }

    return result;
}

/**
 * read contents from stdin
 */
int
lmd_credential_op_read_all(
    char** contents,
    size_t* contents_length)
{
    int result;
    buffer_char_buffer* buffer; 
    char* tmp_buffer; 
    const size_t tmp_buffer_size = 256;
    tmp_buffer = NULL;
    buffer = buffer_char_buffer_create_00(
        lmd_i_alloc,
        lmd_i_realloc,
        lmd_i_free,
        lmd_i_mem_copy,
        lmd_i_mem_move, 100);

    result = buffer ? 0 : -1;
    if (result == 0) {
        tmp_buffer = (char*)lmd_i_alloc(tmp_buffer_size);
        result = tmp_buffer ? 0 : -1;
    } 

    while (!result) {
        size_t read_size;
        read_size = fread(tmp_buffer, tmp_buffer_size, 1, stdin);
        result = buffer_char_buffer_append(buffer, tmp_buffer, read_size); 
        if (!read_size) {
            break;
        }
    } 
    if (!result) {
        size_t buffer_size;
        char* tmp_contents;
        buffer_size = buffer_char_buffer_get_size(buffer);
        tmp_contents = (char*)lmd_i_alloc(buffer_size); 
        result = tmp_contents ? 0 : -1;
        if (!result) {
            buffer_char_buffer_copy_to(buffer, tmp_contents);
            *contents = tmp_contents;
            *contents_length = buffer_size;
        }
    }
    if (tmp_buffer) {
        lmd_i_free(tmp_buffer); 
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
lmd_credential_op_read_from_stdin()
{
    char* contents;
    size_t contents_length;
    credential_desc* result;
    int state;
    result = NULL;
    contents = NULL;
    contents_length = 0;

    state = lmd_credential_op_read_all(&contents, &contents_length);
    if (state == 0) {
        result = credential_desc_decode(contents, contents_length);
    }

    if (contents) {
        lmd_i_free(contents);
    }
    return result;
}

/**
 * write credential_desc into stdout
 */
static int
lmd_credential_op_write_into_stdout(
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
lmd_credential_op_get(
    lmd* obj,
    credential_desc* desc)
{
    int result;
    char* token;

    token = NULL;
    result = credential_storage_find_token(desc->protocol, desc->host,
        desc->path, desc->username, &token);

    if (!token) {
        result = get_oauth_token(obj);
        if (result == 0) {
            const char* acc_token;
            acc_token = lmd_get_access_token_ref(obj);
            if (acc_token) {
                result = credential_desc_set_username(desc, "bearer");
                if (result == 0) {
                    result = credential_desc_set_password(desc, acc_token);
                }
            }
        }
    } else {
        result = credential_desc_set_password(desc, NULL);
    }
    return result;
}

/**
 * do credential store operation
 */
int
lmd_credential_op_store(
    lmd* obj,
    credential_desc* desc)
{
    int result;
    result = credential_storage_store_token(
        desc->protocol, desc->host, desc->path, desc->username, desc->password);
    return result;
}

/**
 * do credential remove operation
 */
int
lmd_credential_op_remove(
    lmd* obj,
    credential_desc* desc)
{
    int result; 
    result = credential_storage_remove_token(
        desc->protocol, desc->host, desc->path, desc->username);
    return result;
}


/* vi: se ts=4 sw=4 et: */
