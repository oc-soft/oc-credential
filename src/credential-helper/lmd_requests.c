#include "lmd_requests.h"
#include <string.h>
#include <curl/curl.h>
#include <time.h>
#include <json-c/json.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include <math.h>
#include "buffer/char_buffer.h"
#include "lmd_i.h"
#include "logging.h"

/**
 * write data into buffer
 */
size_t
lmd_requests_write_to_buffer(
    char* ptr,
    size_t size,
    size_t nmemb,
    buffer_char_buffer* buffer);

/**
 * create parameter for oauth_token
 */
char*
lmd_requests_create_param_for_oauth_token(
    lmd* obj);

/**
 * load device and user code with json object
 */
int
lmd_requests_load_device_and_user_code_with_json(
    lmd* obj,
    json_object* json);


/**
 * try load oauth token
 */
int
lmd_requests_try_load_oauth(
    lmd* obj,
    CURL* curl,
    int* has_oauth); 

/**
 * load oauth token
 */
int
lmd_requests_load_oauth(
    lmd* obj,
    CURL* curl,
    int (*progress)(void*, int, lmd*),
    void* user_data); 

/**
 * load oauth token
 */
int
lmd_requests_load_oauth_token(
    lmd* obj,
    json_object* json,
    int* has_oauth);

/**
 * check an error about getting oauth token.
 */
int
lmd_request_has_error_about_oauth_token(
    lmd* obj,
    json_object* json);


/**
 * poll oauth token
 */
int
lmd_requests_poll_oauth_token(
    lmd* obj,
    int (*progress)(void*, int, lmd*),
    void* user_data)
{
    int result;
    CURL* curl;
    result = 0;

    curl = curl_easy_init();
    result = curl ? 0 : -1;
    if (result == 0) {
        CURLcode curl_res;
        struct curl_slist* opt_headers;
        char* param;
        param = NULL;
        opt_headers = NULL;
        if (result == 0) {
            curl_res = curl_easy_setopt(curl,
                CURLOPT_URL,
                lmd_get_token_endpoint_url_ref(obj));
            result = curl_res == CURLE_OK ? 0 : -1;
        }

        if (result == 0) {
            curl_res = curl_easy_setopt(curl,
                CURLOPT_WRITEFUNCTION,
                    lmd_requests_write_to_buffer);
            result = curl_res == CURLE_OK ? 0 : -1;
        }
        if (result == 0) {
            const char** headers;
            size_t headers_size;
            size_t idx;
            headers = lmd_get_token_eprqhd_options_ref(
                    obj);
            headers_size = lmd_get_token_eprqhd_options_size(
                obj);
            
            for (idx = 0; idx < headers_size; idx++) {
                struct curl_slist* opt_headers_res;
                opt_headers_res = curl_slist_append(
                    opt_headers, headers[idx]);
                result = opt_headers_res ? 0 : -1;
                if (result == 0) {
                    opt_headers = opt_headers_res;
                }
                if (result) {
                    break;
                }
            }
        }
        if (result == 0) {
            if (opt_headers) {
                curl_res = curl_easy_setopt(
                    curl, CURLOPT_HTTPHEADER, opt_headers);
                result = curl_res == CURLE_OK ? 0 : -1;
            }
        }
        if (result == 0) {
            param = lmd_requests_create_param_for_oauth_token(obj);
        }
        logging_log(LOG_LEVEL_DEBUG,
            "request param for oauth token: %s",
            param ? param : "null");
 
        if (result == 0 && lmd_get_verbose_level(obj) >= 5) {
            puts(param);
        }
        if (result == 0) {
            curl_res = curl_easy_setopt(curl,
                CURLOPT_WRITEFUNCTION,
                    lmd_requests_write_to_buffer);
            result = curl_res == CURLE_OK ? 0 : -1;
        }

        if (result == 0) {
            curl_res = curl_easy_setopt(curl,
                CURLOPT_POSTFIELDS, param);
            result = curl_res == CURLE_OK ? 0 : -1;
        }
        
        if (result == 0) {
            result = lmd_requests_load_oauth(obj, curl, progress, user_data);
        }
        
        if (param) {
            lmd_i_free(param); 
        }
        if (opt_headers) {
            curl_slist_free_all(opt_headers);
        }
    }
    if (curl) {
        curl_easy_cleanup(curl);
    }

    return result;    
    
}

/**
 * try load oauth token
 */
int
lmd_requests_try_load_oauth(
    lmd* obj,
    CURL* curl,
    int* has_oauth)
{
    int result;
    json_object* json;
    CURLcode curl_res;
    buffer_char_buffer* buffer;
    json = NULL;
    result = 0;

    buffer = buffer_char_buffer_create_00(
        lmd_i_alloc,
        lmd_i_realloc,
        lmd_i_free,
        lmd_i_mem_copy,
        lmd_i_mem_move,
        100);
    result = buffer ? 0 : -1;
    if (result == 0) {
        curl_res = curl_easy_setopt(curl,
            CURLOPT_WRITEDATA, buffer);
        result = curl_res == CURLE_OK ? 0 : -1;
    }
    if (result == 0) {
        curl_res = curl_easy_setopt(curl,
            CURLOPT_SSL_OPTIONS, 
            CURLSSLOPT_NATIVE_CA);
        result = curl_res == CURLE_OK ? 0 : -1;
    }
    if (result == 0) {
        curl_res = curl_easy_perform(curl);
        result = curl_res == CURLE_OK ? 0 : -1;
    }
     
    if (result == 0) {
        char null_char = '\0';
        result = buffer_char_buffer_append(buffer, &null_char, 1);
    }
    if (result == 0) {
        logging_log(LOG_LEVEL_DEBUG,
            "ouath token response: %s",
            buffer_char_buffer_get_data(buffer));
    }
    if (result == 0 && lmd_get_verbose_level(obj) >= 5) {
        puts(buffer_char_buffer_get_data(buffer));
    }
    if (result == 0) {
        json = json_tokener_parse(buffer_char_buffer_get_data(buffer));
        result = json ? 0 : -1;
    } 
     
    if (result == 0) {
        result = lmd_requests_load_oauth_token(
            obj, json, has_oauth);
    }
    if (json) {
        json_object_put(json);
    }
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
    if (buffer) {
        buffer_char_buffer_release(buffer);
    }
    return result;
}

/**
 * load oauth token
 */
int
lmd_requests_load_oauth_token(
    lmd* obj,
    json_object* json,
    int* has_oauth)
{
    int result;
    result = 0;
    if (!lmd_request_has_error_about_oauth_token(obj, json)) {

        int (*oauth_token_loader)(lmd*, json_object*);  
        oauth_token_loader = NULL;

        result = lmd_get_oauth_token_loader(obj, &oauth_token_loader);
        if (result == 0) {
            if (oauth_token_loader) {
                result = oauth_token_loader(obj, json);
                *has_oauth = result == 0;
            } else {
                *has_oauth = 0;
            }
        } else {
            *has_oauth = 0;
        }
    } else {
        *has_oauth = 0;
    }
    return result;
}

/**
 * check an error about getting oauth token.
 */
int
lmd_request_has_error_about_oauth_token(
    lmd* obj,
    json_object* json)
{
    int result;
    result = 1;
    if (obj) {
        int state;
        int (*error_parser)(json_object*, int*);
        error_parser = NULL;
        lmd_get_oauth_response_error_parser(obj, &error_parser);
        if (error_parser) {
            int has_error;
            int state;
            has_error = 0;
            state = error_parser(json, &has_error);
            if (state == 0) {
                result = has_error;
            }
        } 
    } 
    return result;
}


/**
 * load oauth token
 */
int
lmd_requests_load_oauth(
    lmd* obj,
    CURL* curl,
    int (*progress)(void*, int, lmd*),
    void* user_data)
{
    int result;
    struct timespec start_time;
    double start_sec;
    double last_trying_sec;
    result = 0;
    memset(&start_time, 0, sizeof(start_time));
    clock_gettime(CLOCK_REALTIME, &start_time);

    start_sec = start_time.tv_sec;
    start_sec += (double)start_time.tv_nsec * pow(10.0, -9);
    last_trying_sec = start_sec; 

    while (1) {
        int elapse;
        struct timespec cur_time;
        double cur_sec;
        int having_oauth;
        having_oauth = 0;
        
        clock_gettime(CLOCK_REALTIME, &cur_time);
        cur_sec = cur_time.tv_sec;
        cur_sec += (double)cur_time.tv_nsec * pow(10.0, -9);

        elapse = (int)floor(cur_sec - last_trying_sec);
        
        if (elapse > lmd_get_interval(obj)) {
            result = lmd_requests_try_load_oauth(
                obj, curl, &having_oauth);
            last_trying_sec = cur_sec;
        }
        elapse = (int)round(cur_sec - start_sec); 

        if (result == 0) {
            result = progress(user_data, elapse, obj); 
        }
         
        if (elapse > lmd_get_polling_expires_in(obj)) {
            break;
        }
        if (result) {
            break;
        }
        if (having_oauth) {
            break;
        }
        sleep(1);
    }
    return result;
    
}



/**
 * request device and user code
 */
int
lmd_requests_load_device_and_user_code(
    lmd* obj)
{
    int result;
    CURL* curl;
    result = 0;
    logging_log(LOG_LEVEL_DEBUG, "load device and user code");
    logging_log(LOG_LEVEL_DEBUG, "token endpoint url: %s",
        lmd_get_device_auth_token_endpoint_url_ref(obj) ? 
        lmd_get_device_auth_token_endpoint_url_ref(obj) : "none");
    curl = curl_easy_init();
    result = curl ? 0 : -1;
    if (result == 0) {
        buffer_char_buffer* buffer;
        buffer = buffer_char_buffer_create_00(
            lmd_i_alloc,
            lmd_i_realloc,
            lmd_i_free,
            lmd_i_mem_copy,
            lmd_i_mem_move,
            100);
        result = buffer ? 0 : -1;
        if (result == 0) {
            CURLcode curl_res;
            struct curl_slist* opt_headers;
            const char* param;
            json_object* json;
            json = NULL;
            opt_headers = NULL;
            param = lmd_get_device_user_code_param_ref(obj);  
            result = param ? 0 : -1;
            if (result == 0) {
                logging_log(LOG_LEVEL_DEBUG,
                    "device user code param: %s", param);
                curl_res = curl_easy_setopt(curl,
                    CURLOPT_POSTFIELDS, param);
                result = curl_res == CURLE_OK ? 0 : -1;
            }
            if (result == 0) {
                curl_res = curl_easy_setopt(curl,
                    CURLOPT_URL,
                    lmd_get_device_auth_token_endpoint_url_ref(obj));
                result = curl_res == CURLE_OK ? 0 : -1;
            }

            if (result == 0) {
                const char** headers;
                size_t headers_size;
                size_t idx;
                headers = lmd_get_dot_eprqhd_options_ref(
                        obj);
                headers_size = lmd_get_dot_eprqhd_options_size(
                    obj);
                
                for (idx = 0; idx < headers_size; idx++) {
                    struct curl_slist* opt_headers_res;
                    opt_headers_res = curl_slist_append(
                        opt_headers, headers[idx]);
                    result = opt_headers_res ? 0 : -1;
                    if (result == 0) {
                        opt_headers = opt_headers_res;
                    }
                    if (result) {
                        break;
                    }
                }
            }
            if (result == 0) {
                if (opt_headers) {
                    curl_res = curl_easy_setopt(
                        curl, CURLOPT_HTTPHEADER, opt_headers);
                    result = curl_res == CURLE_OK ? 0 : -1;
                }
            }
            if (result == 0) {
                curl_res = curl_easy_setopt(curl,
                    CURLOPT_WRITEFUNCTION,
                        lmd_requests_write_to_buffer);
                result = curl_res == CURLE_OK ? 0 : -1;
            }

            if (result == 0) {
                curl_res = curl_easy_setopt(curl,
                    CURLOPT_WRITEDATA, buffer);
                result = curl_res == CURLE_OK ? 0 : -1;
            }
            if (result == 0) {
                curl_res = curl_easy_setopt(curl,
                    CURLOPT_SSL_OPTIONS,
                    CURLSSLOPT_NATIVE_CA);
                result = curl_res == CURLE_OK ? 0 : -1;
            }
            if (result == 0) {
                curl_res = curl_easy_perform(curl);
                result = curl_res == CURLE_OK ? 0 : -1;
            }
            if (result == 0) {
                char null_char = '\0';
                result = buffer_char_buffer_append(buffer, &null_char, 1);
            }
            
            if (result == 0) {
                logging_log(LOG_LEVEL_DEBUG, 
                    "response: %s",
                    buffer_char_buffer_get_data(buffer));
            }
            if (result == 0 && lmd_get_verbose_level(obj) >= 5) {
                puts(buffer_char_buffer_get_data(buffer));
            }
            if (result == 0) {
                json = json_tokener_parse(buffer_char_buffer_get_data(buffer));
                result = json ? 0 : -1;
            } 
            if (result == 0) {
                result = lmd_requests_load_device_and_user_code_with_json(
                    obj, json);
            }
            if (json) {
                json_object_put(json);
            }
            if (opt_headers) {
                curl_slist_free_all(opt_headers);
            }
        }
        if (buffer) {
            buffer_char_buffer_release(buffer);
        }
    }
    if (curl) {
        curl_easy_cleanup(curl);
    }

    return result;
}

/**
 * load device and user code with json object
 */
int
lmd_requests_load_device_and_user_code_with_json(
    lmd* obj,
    json_object* json)
{
    struct {
        const char* keys[3];
        int (*set_data)(lmd*, const char*, size_t);
    } key_set_str_data[] = {
        {
            {
                "device_code",
                NULL
            },
            lmd_set_device_code_0,
        },
        {
            {
                "user_code",
                NULL
            },
            lmd_set_user_code_0
        },
        {
            {
                "verification_url",
                "verification_uri",
                NULL
            },
            lmd_set_verification_url_0
        }
    };
    struct {
        const char* key;
        int (*set_data)(lmd*, int);
    } key_set_int_data[] = {
        {
            "expires_in",
            lmd_set_polling_expires_in,
        },
        {
            "interval",
            lmd_set_interval
        }
    };
    size_t idx;
    int result;
    result = 0;
    for (idx = 0;
        idx < sizeof(key_set_str_data) / sizeof(key_set_str_data[0]); 
        idx++) {
        json_object* str_json;
        json_bool js_state;
        size_t key_idx;
        str_json = NULL;
        
        {
            result = -1;
            key_idx = 0;
            while (key_set_str_data[idx].keys[key_idx]) {
                js_state = json_object_object_get_ex(
                    json, key_set_str_data[idx].keys[key_idx], &str_json);
                if (js_state) {
                    result = 0;
                    break;
                }
                key_idx++;
            }
        }
        if (result == 0) {
            result = key_set_str_data[idx].set_data(
                obj,
                json_object_get_string(str_json),
                json_object_get_string_len(str_json));
        }
        if (result) {
            break;
        }
    }

    if (result == 0) {
        for (idx = 0;
            idx < sizeof(key_set_int_data) / sizeof(key_set_int_data[0]); 
            idx++) {
            json_object* int_json;
            json_bool js_state;
            int_json = NULL;
            js_state = json_object_object_get_ex(
                json, key_set_int_data[idx].key, &int_json);
            result = js_state ? 0 : -1;
            if (result == 0) {
                result = key_set_int_data[idx].set_data(
                    obj,
                    (int)json_object_get_int(int_json));
            }
            if (result) {
                break;
            }
        }
    }

    return result;
}


/**
 * create parameter for oauth_token
 */
char*
lmd_requests_create_param_for_oauth_token(
    lmd* obj)
{
    const char* client_secret;
    const char* device_code;
    const char* client_id;
    lmd_oauth_token_param_creator* param_creator;
    size_t str_size;
    char* result;
    int state;
    result = NULL;
    client_id = NULL;
    device_code = NULL;
    param_creator = NULL;
    state = 0;
    str_size = 0;
    client_secret = lmd_get_client_secret_ref(obj);

    if (state == 0) {
        client_id = lmd_get_client_id_ref(obj);
        state = client_id ? 0 : -1;
    }
    if (state == 0) {
        device_code = lmd_get_device_code_ref(obj);
        state = device_code ? 0 : -1;
    }
    if (state == 0) {
        state = lmd_get_oauth_token_param_creator(obj, &param_creator);
    }
    logging_log(LOG_LEVEL_DEBUG, "param creator: %p", param_creator);
 
    if (state == 0) { 
        char* param;
        param = NULL;
        state = lmd_oauth_token_param_creator_create_param(
            param_creator,
            client_id, device_code, client_secret, &param, lmd_i_alloc);
        if (state == 0) {
            result = param;
        }
    }
    if (param_creator) {
        lmd_oauth_token_param_creator_release(param_creator);
    }
    return result; 
}


/**
 * write data into buffer
 */
size_t
lmd_requests_write_to_buffer(
    char* ptr,
    size_t size,
    size_t nmemb,
    buffer_char_buffer* buffer)
{
    size_t result;
    if (size > 0 && nmemb > 0) {
        int state;
        size_t data_size;
        data_size = size * nmemb;
        state = buffer_char_buffer_append(buffer, ptr, data_size);
        if (state == 0) {
            result = data_size;
        } else {
            result = 0;
        }
    } else {
        result = 0;
    }
    return result;
}


/* vi: se ts=4 sw=4 et: */
