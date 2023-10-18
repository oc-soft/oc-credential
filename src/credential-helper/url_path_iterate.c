#include "url_path_iterate.h"
#include <stdlib.h>
#include <string.h>

/**
 * iterate url path 
 */
int
url_path_iterate(
    const char* protocol,
    const char* host,
    const char* path,
    int (*iterator)(void*, const char*, const char*, const char*),
    void* user_data)
{
    char* path_buffer;
    size_t path_buffer_len; 
    int result;
    result = 0;
    path_buffer = NULL;
    path_buffer_len = 0;
    if (path) {
        path_buffer_len = strlen(path);
        path_buffer = (char*)malloc(path_buffer_len + 1); 
        result = path_buffer ? 0 : -1;
        if (result == 0) {
            memcpy(path_buffer, path, path_buffer_len + 1);
        }
    }
    if (result == 0) {
        while (1) {
            result = iterator(user_data, protocol, host, path_buffer);
            if (result) {
                break;
            } else {
                if (path_buffer) {
                    char* tmp_char;
                    tmp_char = strrchr(path_buffer, '/');
                    if (tmp_char) {
                        *tmp_char = '\0';
                    } else {
                        if (path_buffer[0]) {
                            path_buffer[0] = '\0';
                        } else {
                            break;
                        }
                    }
                } else {
                    break;
                }
            }
        }
    } 
    if (path_buffer) {
        free(path_buffer);
    }
    return result;
}

/* vi: se ts=4 sw=4 et: */
