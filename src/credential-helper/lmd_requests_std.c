#include "lmd_requests_std.h"
#include <stdio.h>
#include <string.h>
#include "lmd_i.h"

/**
 * create parameter for oauth_token
 */
char*
lmd_requests_std_create_param_for_oauth_token(
    lmd* obj)
{
    const static char* fmt = "client_id=%s&"
        "device_code=%s&"
        "grant_type=urn:ietf:params:oauth:grant-type:device_code";

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

    client_id = lmd_get_client_id_ref(obj);
    state = client_id ? 0 : -1;
    if (state == 0) {
        device_code = lmd_get_device_code_ref(obj);
        state = device_code ? 0 : -1;
    }
    if (state == 0) { 
        str_size = strlen(fmt)
            + strlen(client_id)
            + strlen(device_code) + 1;  
        result = (char*)lmd_i_alloc(str_size);
        state = result ? 0 : -1;
    }
    if (state == 0) {
        snprintf(result, str_size, fmt, client_id, device_code);
    }
    return result; 
}


/* vi: se ts=4 sw=4 et: */
