#include "lmd_op.h"

#include <stdlib.h>
#include <libintl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "buffer/char_buffer.h"

#include "lmd.h"
#include "lmd_i.h"
#include "lmd_requests.h"
#include "lmd_connections.h"
#include "lmd_ui.h"
#include "lmd_oauth_token_param_creator.h"
#include "logging.h"
#include "proc_sched.h"
#include "url_path_iterate.h"
#include "url_id.h"
#include "url_services.h"
#include "service_url_secret.h"
#include "service_url_client.h"
#include "service_secret.h"
#include "service_id.h"
#include "service_url_device_user_code_param.h"
#include "service_url_lmd_oauth_token_param.h"
#include "service_oauth_token_parser.h"
#include "service_oauth_response_error_parser.h"
#include "fd_io.h"

/**
 * progress
 */
typedef struct _lmd_progress lmd_progress;


/**
 * url to services iteration 
 */
typedef struct _url_to_services_iter_info url_to_services_iter_info;

/**
 * information block for url to id iteration 
 */
typedef struct _url_to_id_iter_info url_to_id_iter_info;


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
 * iteration info to find groups from url
 */
struct _url_to_services_iter_info {

    /**
     * services 
     */
    const char** services;

    /**
     * count of services
     */
    size_t services_size;
};

/**
 * information block for url to id iteration 
 */
struct _url_to_id_iter_info {
    /**
     * identifier
     */
    char* id;
};


/**
 * print progress for oauth token
 */
static int
print_progress_to_notty_for_oauth_token(
    lmd_progress* progress, 
    int elapse,
    lmd* obj);


/**
 * print progress for oauth token
 */
static int
print_progress_to_tty_for_oauth_token(
    lmd_progress* progress, 
    int elapse,
    lmd* obj);

/**
 * find services
 */
static int
lmd_op_find_services(
    url_to_services_iter_info* info,
    const char* protocol,
    const char* host,
    const char* path);

/**
 * find url id
 */
static int
lmd_op_find_url_id_iter(
    url_to_id_iter_info* info,
    const char* protocol,
    const char* host,
    const char* path);


/**
 * create item selelection lines
 */
static char*
lmd_op_create_item_selection_lines(
    const char** items,
    size_t items_size);

/**
 * select service
 */
static int
lmd_op_select_service_if_not(
    lmd* obj,
    const char* protocol,
    const char* host,
    const char* path,
    char** service);


/**
 * select service
 */
static int
lmd_op_select_service_0(
    lmd* obj,
    const char* protocol,
    const char* host,
    const char* path,
    char** service);

/**
 * get client secret from url
 */
int
lmd_op_get_secret_from_url(
    const char* protocol,
    const char* host,
    const char* path,
    const char* service,
    char** client_secret);

/**
 * get client id from url
 */
int
lmd_op_get_client_from_url(
    const char* protocol,
    const char* host,
    const char* path,
    const char* service,
    char** client_secret);

/**
 * create parameter for device and user code from url
 */
int
lmd_op_create_device_user_code_param_from_url(
    const char* protocol,
    const char* host,
    const char* path,
    const char* service,
    const char* client_id,
    char** param);

/**
 * get oauth token param creator from url
 */
int
lmd_op_get_oauth_token_param_creator_from_url(
    const char* protocol,
    const char* host,
    const char* path,
    const char* service,
    lmd_oauth_token_param_creator** param_creator);

/**
 * read number from stdin
 */
static int
read_number_from_ui(
    lmd_ui* ui,
    int* number); 

/**
 * find groups
 */
static int
lmd_op_find_services(
    url_to_services_iter_info* info,
    const char* protocol,
    const char* host,
    const char* path)
{
    int state; 
    int result;
    const char** services;
    size_t services_size;
    result = 0;
    services = NULL;
    services_size = 0; 
    state = url_services_get(protocol, host, path, &services, &services_size);
    if (state == 0) {
        info->services = services;
        info->services_size = services_size;
        result = 1;
    }
    return result;
}

/**
 * find url id
 */
static int
lmd_op_find_url_id_iter(
    url_to_id_iter_info* info,
    const char* protocol,
    const char* host,
    const char* path)
{
    int state;
    int result;
    char* id;

    id = NULL;
    state = 0;
    result = 0;
    state = url_id_get_id(protocol, host, path, &id, lmd_i_alloc);

    if (state == 0) {
        result = 1;
        info->id = id;
    }
    return result;
}


/**
 * select service
 */
int
lmd_op_select_service(
    lmd* obj,
    const char* protocol,
    const char* host,
    const char* path) {
    int result;
    if (obj) {
        char* service;
        char* client_id;
        char* secret;
        char* device_user_code_param;
        lmd_oauth_token_param_creator* oauth_token_param_creator;
        int (*oauth_token_loader)(lmd*, json_object*);
        int (*oauth_res_error_parser)(json_object*, int*);
        client_id = NULL;
        secret = NULL;
        service = NULL;
        device_user_code_param = NULL;
        oauth_token_param_creator = NULL;
        oauth_token_loader = NULL;
        oauth_res_error_parser = NULL;
        result = lmd_op_select_service_if_not(
            obj, protocol, host, path, &service);
        if (result == 0) {
            int need_client_secret;
            logging_log(LOG_LEVEL_DEBUG, "service : %s", service);
            need_client_secret = service_secret_need_secret(
                service);
            
            logging_log(LOG_LEVEL_DEBUG, "need secret: %s",
                need_client_secret ? "yes" : "no");
            if (need_client_secret) {
                result = lmd_op_get_secret_from_url(
                    protocol, host, path, service, &secret); 
            } 
        }
        if (result == 0) {
            result = lmd_op_get_client_from_url(
                protocol, host, path, service, &client_id);
        }
        logging_log(LOG_LEVEL_DEBUG, "client id: %s",
            client_id ? client_id : "null");

        if (result == 0) {
            result = lmd_op_create_device_user_code_param_from_url(
                protocol, host, path, service, client_id,
                &device_user_code_param);
        }
        logging_log(LOG_LEVEL_DEBUG, "device user code param: %s",
            device_user_code_param ?
            device_user_code_param : "null");

        if (result == 0) {
            result = lmd_op_get_oauth_token_param_creator_from_url(
                protocol, host, path, service, &oauth_token_param_creator);
        }
        if (result == 0) {
            result = service_oauth_token_parser_get(service,
                &oauth_token_loader);
        }
        if (result == 0) {
            result = service_oauth_response_error_parser_get(service,
                &oauth_res_error_parser);
        }

        if (result == 0 && secret) {
            result = lmd_set_client_secret(obj, secret);
        }
        if (result == 0) {
            result = lmd_set_oauth_response_error_parser(
                obj, oauth_res_error_parser);
        }
        if (result == 0) {
            result = lmd_set_oauth_token_loader(obj, oauth_token_loader);
        }
        if (result == 0) {
            result = lmd_set_oauth_token_param_creator(obj, 
                oauth_token_param_creator);
        }
        if (result == 0) {
            result = lmd_set_client_id(obj, client_id);
        }
        if (result == 0) {
            result = lmd_set_service(obj, service);
        }
        if (result == 0) {
            result = lmd_set_device_user_code_param(
                obj, device_user_code_param);
        }
        if (client_id) {
            lmd_i_free(client_id);
        }
        if (secret) {
            lmd_i_free(secret);
        }
        if (service) {
            lmd_i_free(service);
        }
        if (oauth_token_param_creator) {
            lmd_oauth_token_param_creator_release(oauth_token_param_creator);
        }
        if (device_user_code_param) {
            lmd_i_free(device_user_code_param);
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}
 
/**
 * select service
 */
static int
lmd_op_select_service_if_not(
    lmd* obj,
    const char* protocol,
    const char* host,
    const char* path,
    char** service)
{
    int result;
    int state;
    const char* service_org;
    int valid_service;
    valid_service = 0;
    service_org = lmd_get_service_ref(obj);

    if (service_org) {

        url_to_services_iter_info info;

        memset(&info, 0, sizeof(info));

        state = url_path_iterate(protocol, host, path, 
            (int (*)(void*, const char*, const char*, const char*))
                lmd_op_find_services, &info);

        result = state == 1 ? 0 : -1;
        if (result == 0) {
            size_t idx;

            for (idx = 0; idx < info.services_size; idx++) {
                valid_service = strcmp(info.services[idx], service_org) == 0;
                if (valid_service) {
                    break;
                }
            }
            if (valid_service && service) {
                size_t length;
                char* service_0;
                length = strlen(service_org);
                service_0 = (char*)lmd_i_alloc(length + 1);
                result = service_0 ? 0 : -1;
                if (result == 0) {
                    memcpy(service_0, service_org, length + 1);
                    *service = service_0;
                }
            }
        } else {
            valid_service = 0;
        }
    } else {
        valid_service = 0;
    }
    if (!valid_service) {
        result = lmd_op_select_service_0(obj, protocol, host, path, service);
    }
    return result;
}


/**
 * select service
 */
static int
lmd_op_select_service_0(
    lmd* obj,
    const char* protocol,
    const char* host,
    const char* path,
    char** service)
{
    int result;
    int state;
    char* item_lines;    
    int service_selection;
    item_lines = NULL;
    service_selection = -1;
    url_to_services_iter_info info;

    memset(&info, 0, sizeof(info));

    state = url_path_iterate(protocol, host, path, 
        (int (*)(void*, const char*, const char*, const char*))
            lmd_op_find_services, &info);
    result = state ? 0 : -1;
    if (result == 0) {
        item_lines = lmd_op_create_item_selection_lines(
            info.services, info.services_size);
        result = item_lines ? 0 : -1;
    }
    if (result == 0) {
        const char* select_message;
        if (fd_io_isatty(fd_io_fileno(stderr))) {
            select_message = gettext(
                "Select \x1b[1ma number\x1b[0m for Oauth token service "
                "or enter \x1b[1mEsc\x1b[0m if you cancel.\n");
        } else {
            select_message = gettext(
                "Select a number for Oauth token service "
                "or enter Esc if you cancel.\n");
        }
        fprintf(stderr, item_lines);
        fprintf(stderr, select_message);
        
        {
            lmd_ui* ui;
            ui = lmd_ui_create();
            if (ui) {
                while (1) {
                    int state;
                    int number;
                    number = 0;
                    state = read_number_from_ui(ui, &number); 
                    if (state == -1) {
                        result = -1;
                        break;
                    }
                    if (state == 0) {
                        if (0 < number && number <= info.services_size) {
                            service_selection = number;
                            break;
                        }
                    }
                }
            }
            if (ui) {
                lmd_ui_release(ui);
            }
        }
    } 
    
    logging_log(LOG_LEVEL_DEBUG, "service selection : %d", service_selection);
    if (result == 0 && service) {
        size_t length;
        char* service_0;
        length = strlen(info.services[service_selection - 1]);
        service_0 = (char*)lmd_i_alloc(length + 1);
        if (service_0) {
            memcpy(service_0, info.services[service_selection - 1],
                length + 1);
            *service = service_0;
        } else {
            result = -1;
        }
    }
    if (item_lines) {
        lmd_i_free(item_lines);
    }
    return result;
}



/**
 * create item selelection lines
 */
static char*
lmd_op_create_item_selection_lines(
    const char** items,
    size_t items_size)
{
    char* result;
    int idx;
    static size_t item_lines_size;
    const static char* item_line_fmt = "%1d. %s\n";
    size_t item_line_fmt_size;
    result = NULL;
    item_line_fmt_size = strlen(item_line_fmt) + 1;
    item_lines_size = 0;
    for (idx = 0; idx < items_size; idx++) {
        const char* item_str_rep = gettext(items[idx]);    
        size_t item_str_len;
        item_str_len = strlen(item_str_rep); 
        item_lines_size += item_line_fmt_size + item_str_len; 
    }
    result = (char*)lmd_i_alloc(item_lines_size);
    if (result) {
        char* output_ptr = result;
        for (idx = 0; idx < items_size; idx++) {
            const char* item_str_rep = gettext(items[idx]);    
            output_ptr += snprintf(output_ptr,  
                item_lines_size - (result - output_ptr),
                item_line_fmt, idx + 1, item_str_rep);
        }
    }
    return result;
}

/**
 * get oauth token
 */
int
lmd_op_get_oauth_token_with_client(
    lmd* limited_acc)
{
    int result;
    lmd_progress progress;
    result = 0;
    logging_log(LOG_LEVEL_DEBUG, "start oauth token");

    if (result == 0) {
        memset(&progress, 0, sizeof(progress));
        result = lmd_get_service_ref(limited_acc) ? 0 : -1;
    }
    if (result == 0) {
        result = lmd_get_client_id_ref(limited_acc) ? 0 : -1;
    }
    if (result == 0) {
        result = lmd_get_service_ref(limited_acc) ? 0 : -1;
    }
    if (result == 0) {
        logging_log(LOG_LEVEL_DEBUG, "load discovery document for %s",
            lmd_get_service_ref(limited_acc));

        result = lmd_connections_load_discovery_document(
            limited_acc, 
            lmd_get_service_ref(limited_acc));
    }
    if (result == 0) {
        result = lmd_requests_load_device_and_user_code(limited_acc);
    }
    if (result == 0) {
        int (*progress_call_back)(void*, int, lmd*);
        if (fd_io_isatty(fd_io_fileno(stderr))) {
            progress_call_back =
                (int (*)(void*, int, lmd*))
                    print_progress_to_tty_for_oauth_token;
        } else {
            progress_call_back =
                (int (*)(void*, int, lmd*))
                    print_progress_to_notty_for_oauth_token;
        }
        result = lmd_requests_poll_oauth_token(limited_acc,
            (int (*)(void*, int, lmd*))progress_call_back,
            &progress);
    }  
    return result;
}

/**
 * print progress for oauth token
 */
static int
print_progress_to_notty_for_oauth_token(
    lmd_progress* progress,
    int elapse,
    lmd* lmd_obj)
{
    int result;
    result = 0;
    if (!progress->last_sec_updating) {
        const char* verification_url;
        const char* user_code;
        int expires_in;
        verification_url = lmd_get_verification_url_ref(lmd_obj);
        user_code = lmd_get_user_code_ref(lmd_obj);
        expires_in = lmd_get_polling_expires_in(lmd_obj);
        fprintf(stderr,
            gettext(
                "Open browser, visit the url and input the code "
                "in %d seconds\n"
                "URL:\n"
                "%s\n"
                "Code:\n"
                "%s\n"),
            expires_in, verification_url, user_code); 
        progress->last_sec_updating = elapse + 1; 
    }
    return result;
}
/**
 * print progress for oauth token
 */
static int
print_progress_to_tty_for_oauth_token(
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
                fprintf(stderr,
                    gettext(
                        "URL:\n"
                        "\x1b[1m%s\x1b[0m\n"
                        "Code:\n"
                        "\x1b[1m%s\x1b[0m\n"), 
                    verification_url, user_code); 
            }
            fputs("\r", stderr);
            fprintf(stderr,
                gettext(
                    "Open browser, visit the url and input the code as above" 
                    " in %d seconds."),
                expires_in - elapse); 
            fflush(stderr);
            
            progress->last_sec_updating = elapse; 
        }

    }
    return result;
}

/**
 * read number from stdin
 */
static int
read_number_from_ui(
    lmd_ui* ui,
    int* number)
{
    int result;
    buffer_char_buffer* buffer;
    buffer = buffer_char_buffer_create(10); 
     
    result = buffer ? 0 : -1;
    if (result == 0) {
        while (1) {
            int a_char;
            a_char = lmd_ui_get_char(ui);
            if (a_char == EOF) {
                result = -1;
            } else if (a_char == '\b') {
                if (buffer_char_buffer_get_size(buffer)) {
                    result = buffer_char_buffer_remove(buffer,
                        buffer_char_buffer_get_size(buffer) - 1,
                        buffer_char_buffer_get_size(buffer));
                }
            } else if (a_char == '\n') {
                break;
            } else if (a_char == 0x1b) {
                /* esc */
                result = -1;
            } else {
                char chars[] = {
                    (char)a_char
                }; 
                result = buffer_char_buffer_append(buffer, chars, 1);
            }
            if (result) {
                break;
            }
        } 
    }
    if (result == 0) {
        if (buffer_char_buffer_get_size(buffer)) {
            char null_char = '\0';
            result = buffer_char_buffer_append(buffer, &null_char, 1); 
            if (result == 0) {
                char* data_ptr;
                long number_l;
                data_ptr = (char*)buffer_char_buffer_get_data(buffer);
                number_l = strtol(buffer_char_buffer_get_data(buffer),
                    &data_ptr, 0);
                if (data_ptr != buffer_char_buffer_get_data(buffer)) {
                    if (number) {
                        *number = number_l;
                    } 
                    result = 0;
                } else {
                    result = -2;
                }
            } 
        } else {
            result = -2;
        } 
    } 
    if (buffer) {
        buffer_char_buffer_release(buffer);
    }
    return result;
}

/**
 * get client secret from url
 */
int
lmd_op_get_secret_from_url(
    const char* protocol,
    const char* host,
    const char* path,
    const char* service,
    char** secret)
{
    char* url_id;
    char* service_id;
    int state;
    int result;
    url_to_id_iter_info url_iter_info;

    memset(&url_iter_info, 0, sizeof(url_iter_info));
 
    url_id = NULL;
    service_id = NULL;
    result = 0;
    
    state = url_path_iterate(protocol, host, path, 
        (int (*)(void*, const char*, const char*, const char*))
            lmd_op_find_url_id_iter,
        &url_iter_info);
    result = state ? 0 : -1;
    if (result == 0) {
        url_id = url_iter_info.id;    
    }
    logging_log(LOG_LEVEL_DEBUG, "url id: %s", url_id ? url_id : "null");
    if (result == 0) {
        result = service_id_get_id(service, &service_id, lmd_i_alloc);
    } else {
        result = -1;
    }
    logging_log(LOG_LEVEL_DEBUG, "service id: %s",
        service_id ? service_id : "null");
    if (result == 0) {
        char* secret_0;
        secret_0 = NULL;
        state = service_url_secret_get_secret(
            service_id, url_id, &secret_0, lmd_i_alloc);
        logging_log(LOG_LEVEL_DEBUG, "secret: %s",
        secret_0 ? secret_0 : "null");
     
        if  (state == 0) {
            if (secret) {
                *secret = secret_0;
                secret_0 = NULL;
            } else {
                result = -1;
            }
        } else {
            result = -1;
        }
        if (secret_0) {
            lmd_i_free(secret_0);
        }
    } else {
        result = -1;
    }
    if (service_id) {
        lmd_i_free(service_id);
    }
    if (url_id) {
        lmd_i_free(url_id);
    }
    return result;
}

/**
 * get client id from url
 */
int
lmd_op_get_client_from_url(
    const char* protocol,
    const char* host,
    const char* path,
    const char* service,
    char** client)
{
    char* url_id;
    char* service_id;
    int state;
    int result;
    url_to_id_iter_info url_iter_info;

    memset(&url_iter_info, 0, sizeof(url_iter_info));
 
    url_id = NULL;
    service_id = NULL;
    result = 0;
    
    state = url_path_iterate(protocol, host, path, 
        (int (*)(void*, const char*, const char*, const char*))
            lmd_op_find_url_id_iter,
        &url_iter_info);
    result = state ? 0 : -1;
    if (result == 0) {
        url_id = url_iter_info.id;    
    }
    if (result == 0) {
        result = service_id_get_id(service, &service_id, lmd_i_alloc);
    } else {
        result = -1;
    }
    if (result == 0) {
        char* client_0;
        client_0 = NULL;
        state = service_url_client_get_client(
            service_id, url_id, &client_0, lmd_i_alloc);
        if  (state == 0) {
            if (client) {
                *client = client_0;
                client_0 = NULL;
            } else {
                result = -1;
            }
        } else {
            result = -1;
        }
        if (client_0) {
            lmd_i_free(client_0);
        }
    } else {
        result = -1;
    }
    if (service_id) {
        lmd_i_free(service_id);
    }
    if (url_id) {
        lmd_i_free(url_id);
    }
    return result;
}

/**
 * create parameter for device and user code from url
 */
int
lmd_op_create_device_user_code_param_from_url(
    const char* protocol,
    const char* host,
    const char* path,
    const char* service,
    const char* client_id,
    char** param)
{
    char* url_id;
    char* service_id;
    int state;
    int result;
    url_to_id_iter_info url_iter_info;

    memset(&url_iter_info, 0, sizeof(url_iter_info));
 
    url_id = NULL;
    service_id = NULL;
    result = 0;
    
    state = url_path_iterate(protocol, host, path, 
        (int (*)(void*, const char*, const char*, const char*))
            lmd_op_find_url_id_iter,
        &url_iter_info);
    result = state ? 0 : -1;
    if (result == 0) {
        url_id = url_iter_info.id;    
    }
    if (result == 0) {
        result = service_id_get_id(service, &service_id, lmd_i_alloc);
    } else {
        result = -1;
    }
    if (result == 0) {
        char* param_0;
        param_0 = NULL;
        state = service_url_device_user_code_param_create(
            service_id, url_id, client_id, &param_0, lmd_i_alloc);
        if  (state == 0) {
            if (param) {
                *param = param_0;
                param_0 = NULL;
            } else {
                result = -1;
            }
        } else {
            result = -1;
        }
        if (param_0) {
            lmd_i_free(param_0);
        }
    } else {
        result = -1;
    }

    if (service_id) {
        lmd_i_free(service_id);
    }
    if (url_id) {
        lmd_i_free(url_id);
    }
    return result;
}

/**
 * get oauth token param creator from url
 */
int
lmd_op_get_oauth_token_param_creator_from_url(
    const char* protocol,
    const char* host,
    const char* path,
    const char* service,
    lmd_oauth_token_param_creator** param_creator)
{
    char* url_id;
    char* service_id;
    int state;
    int result;
    url_to_id_iter_info url_iter_info;

    memset(&url_iter_info, 0, sizeof(url_iter_info));
 
    url_id = NULL;
    service_id = NULL;
    result = 0;
    
    state = url_path_iterate(protocol, host, path, 
        (int (*)(void*, const char*, const char*, const char*))
            lmd_op_find_url_id_iter,
        &url_iter_info);
    result = state ? 0 : -1;
    if (result == 0) {
        url_id = url_iter_info.id;    
    }
    logging_log(LOG_LEVEL_DEBUG, "url id: %s", url_id ? url_id : "null");
    if (result == 0) {
        result = service_id_get_id(service, &service_id, lmd_i_alloc);
    } else {
        result = -1;
    }
    logging_log(LOG_LEVEL_DEBUG, "service id: %s",
        service_id ? service_id : "null");
    if (result == 0) {
        lmd_oauth_token_param_creator* param_creator_0;
        param_creator_0 = NULL;
        state = service_url_lmd_oauth_token_param_get_creator(
            service_id, url_id, &param_creator_0);
        logging_log(LOG_LEVEL_DEBUG, "param_creator: %p", param_creator_0);
     
        if  (state == 0) {
            if (param_creator) {
                *param_creator = param_creator_0;
                param_creator_0 = NULL;
            } else {
                result = -1;
            }
        } else {
            result = -1;
        }
        if (param_creator_0) {
            lmd_oauth_token_param_creator_release(param_creator_0);
        }
    } else {
        result = -1;
    }
    if (service_id) {
        lmd_i_free(service_id);
    }
    if (url_id) {
        lmd_i_free(url_id);
    }
    return result;
}


/* vi: se ts=4 sw=4 et: */
