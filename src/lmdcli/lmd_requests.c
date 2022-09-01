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
 * create parameter for device and user code request 
 */
char*
lmd_requests_create_device_and_user_code_param(
    lmd* obj);


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
        char* param;
        param = NULL;
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
            param = lmd_requests_create_param_for_oauth_token(obj);
        }
        
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
        struct {
            const char* key;
            int (*set_value)(lmd*, const char*, size_t);
        } key_set_str[] = {
            {
                "access_token",
                lmd_set_access_token_0
            },
            {
                "token_type",
                lmd_set_token_type_0
            },
            {
                "scope",
                lmd_set_scope_0
            },
            {
                "refresh_token",
                lmd_set_refresh_token_0
            }
        }; 

        struct {
            const char* key;
            int (*set_value)(lmd*, int);
        } key_set_int[] = {
            {
                "expires_in",
                lmd_set_expires_in
            }
        };
        size_t idx;
        for (idx = 0;
            idx < sizeof(key_set_str) / sizeof(key_set_str[0]);
            idx++) {
            json_object* str_json; 
            json_bool js_state;
            str_json = NULL;
            js_state = json_object_object_get_ex(
                json, key_set_str[idx].key, &str_json);
            result = js_state ? 0 : -1;
            if (result == 0) {
                result = key_set_str[idx].set_value(
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
                idx < sizeof(key_set_int) / sizeof(key_set_int[0]); 
                idx++) {
                json_object* int_json;
                json_bool js_state;
                int_json = NULL;
                js_state = json_object_object_get_ex(
                    json, key_set_int[idx].key, &int_json);
                result = js_state ? 0 : -1;
                if (result == 0) {
                    result = key_set_int[idx].set_value(
                        obj,
                        (int)json_object_get_int(int_json));
                }
                if (result) {
                    break;
                }
            }
        }
        *has_oauth = result == 0;
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
    json_object* err_obj;
    json_bool state;
    int result;
    err_obj = NULL;
    state = json_object_object_get_ex(json, "error", &err_obj); 
    
    result = err_obj != NULL;
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
    struct timespec sleep_req;
    double start_sec;
    double last_trying_sec;
    result = 0;
    memset(&start_time, 0, sizeof(start_time));
    clock_gettime(CLOCK_REALTIME, &start_time);

    start_sec = start_time.tv_sec;
    start_sec += (double)start_time.tv_nsec * pow(10.0, -9);
    last_trying_sec = start_sec - 1; 
    sleep_req.tv_sec = 1;
    /* sleep_req.tv_nsec = 5l * (long)pow(10, 9); */

    while (1) {
        int elapse;
        struct timespec cur_time;
        double cur_sec;
        int having_oauth;
        having_oauth = 0;
        
        clock_gettime(CLOCK_REALTIME, &cur_time);
        cur_sec = cur_time.tv_sec;
        cur_sec += (double)cur_time.tv_nsec * pow(10.0, -9);

        elapse = (int)round(cur_sec - last_trying_sec);
        
        if (elapse > lmd_get_interval(obj)) {
            result = lmd_requests_try_load_oauth(
                obj, curl, &having_oauth);
            last_trying_sec = cur_sec;
            if (result == 0 && lmd_get_verbose_level(obj) >= 5) {
                printf("user code: %s\n"
                    "end point url: %s\n",
                    lmd_get_user_code_ref(obj),
                    lmd_get_verification_url_ref(obj));
            }
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
#if 0
        if (nanosleep(&sleep_req, NULL) == -1) {
            errno = 0;
        }
#endif
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
            char* param;
            json_object* json;
            json = NULL;
            param = lmd_requests_create_device_and_user_code_param(obj);
            result = param ? 0 : -1;
            if (result == 0) {
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
            if (param) {
                lmd_i_free(param);
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
        const char* key;
        int (*set_data)(lmd*, const char*, size_t);
    } key_set_str_data[] = {
        {
            "device_code",
            lmd_set_device_code_0,
        },
        {
            "user_code",
            lmd_set_user_code_0
        },
        {
            "verification_url",
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
        str_json = NULL;
        js_state = json_object_object_get_ex(
            json, key_set_str_data[idx].key, &str_json);
        result = js_state ? 0 : -1;
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
 * create parameter for device and user code request 
 */
char*
lmd_requests_create_device_and_user_code_param(
    lmd* obj)
{
    const static char* fmt = "client_id=%s&scope=email";
    const char* client_id;
    size_t str_size;
    char* result;
    int state;
    result = NULL;
    state = 0;
    str_size = 0;
    client_id = lmd_get_client_id_ref(obj);
    state = client_id ? 0 : -1;
    
    if (state == 0) { 
        str_size = strlen(fmt) + strlen(client_id) + 1;  
        result = (char*)lmd_i_alloc(str_size);
        state = result ? 0 : -1;
    }
    if (state == 0) {
        snprintf(result, str_size, fmt, client_id);
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
    const static char* fmt = "client_id=%s&"
        "client_secret=%s&"
        "device_code=%s&"
        "grant_type=urn:ietf:params:oauth:grant-type:device_code";

    const char* client_secret;
    const char* device_code;
    const char* client_id;
    size_t str_size;
    char* result;
    int state;
    result = NULL;
    client_id = NULL;
    device_code = NULL;
    state = 0;
    str_size = 0;
    client_secret = lmd_get_client_secret_ref(obj);

    state = client_secret ? 0 : -1;
    if (state == 0) {
        client_id = lmd_get_client_id_ref(obj);
        state = client_id ? 0 : -1;
    }
    if (state == 0) {
        device_code = lmd_get_device_code_ref(obj);
        state = device_code ? 0 : -1;
    }
    if (state == 0) { 
        str_size = strlen(fmt)
            + strlen(client_id)
            + strlen(client_secret)
            + strlen(device_code) + 1;  
        result = (char*)lmd_i_alloc(str_size);
        state = result ? 0 : -1;
    }
    if (state == 0) {
        snprintf(result, str_size, fmt, client_id, client_secret, device_code);
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
