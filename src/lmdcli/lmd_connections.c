#include "lmd_connections.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>

#include "buffer/char_buffer.h"
#include "lmd_i.h"

/**
 * get hostname for discoverry document.
 */
static const char*
lmd_connections_get_discovery_doc_url();

/**
 * load discovery document json format
 */
int
lmd_connections_load_discovery_document_with_json(
    lmd* obj,
    json_object* doc_obj);

/**
 * write data into buffer
 */
static size_t
lmd_connections_write_to_buffer(
    char* ptr,
    size_t size,
    size_t nmemb,
    buffer_char_buffer* buffer);

/**
 * load discovery document
 */
int
lmd_connections_load_discovery_document(
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
            if (result == 0) {
                curl_res = curl_easy_setopt(curl,
                    CURLOPT_URL,
                    lmd_connections_get_discovery_doc_url());
                result = curl_res == CURLE_OK ? 0 : -1;
            }

            if (result == 0) {
                curl_res = curl_easy_setopt(curl,
                    CURLOPT_WRITEFUNCTION,
                        lmd_connections_write_to_buffer);
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
                result = lmd_connections_load_discovery_document_with_json(
                    obj, json);
            }
            if (json) {
                json_object_put(json);
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
 * write data into buffer
 */
static size_t
lmd_connections_write_to_buffer(
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

/**
 * load discovery document json format
 */
int
lmd_connections_load_discovery_document_with_json(
    lmd* obj,
    json_object* doc_obj)
{
    int result;
    json_object* te_obj;
    json_bool js_state;
    result = 0;
    te_obj = NULL;

    js_state = json_object_object_get_ex(doc_obj, "token_endpoint", &te_obj);
    result = js_state ? 0 : -1;
    if (result == 0) {
        result = json_object_is_type(te_obj, json_type_string) != 0 ? 0 : -1;
    }
    if (result == 0) {
        size_t len;
        const char* te_str;
        len = 0;
        te_str = NULL;
        te_str = json_object_get_string(te_obj);
        len = json_object_get_string_len(te_obj);
        result = lmd_set_token_endpoint_url_0(obj, te_str, len);
    }
    if (result == 0) {
        js_state = json_object_object_get_ex(doc_obj,
            "device_authorization_endpoint", &te_obj);
        result = js_state ? 0 : -1;
    }
    if (result == 0) {
        size_t len;
        const char* te_str;
        len = 0;
        te_str = NULL;
        te_str = json_object_get_string(te_obj);
        len = json_object_get_string_len(te_obj);
        result = lmd_set_device_auth_token_endpoint_url_0(obj, te_str, len);
    }

    return result;
}


/**
 * get url for discoverry document.
 */
static const char*
lmd_connections_get_discovery_doc_url()
{
    return "https://accounts.google.com/.well-known/openid-configuration";
}

/* vi: se ts=4 sw=4 et: */
