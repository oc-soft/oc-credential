#include "path.h"
#include <stdlib.h>

static void*
path_alloc(
    size_t size);

/**
 * join path
 */
char*
path_join(
    const char* path_1,
    const char* path_2,
    void* (*alloc_mem)(size_t),
    void (*free_mem)(void*))
{
    size_t path_1_len;
    size_t path_2_len;
    size_t path_1_copy_len;
    size_t path_2_copy_len;
    size_t path_2_start_idx;
    size_t idx;
    char* result;
    if (!alloc_mem) {
        alloc_mem = path_alloc;
    }
    if (!free_mem) {
        free_mem = path_free;
    }
    result = NULL;
    path_1_len = strlen(path_1); 
    path_1_copy_len = path_1_len;
    path_2_len = strlen(path_2);
    if (path_1_len > 1) {
        for (idx = 0; idx < path_1_len - 1; idx++) {
            if (path_1[path_1_len - idx - 1] == '/') {
                path_1_copy_len--;
            } else {
                break;
            }
        }
    }
    path_2_start_idx = 0;
    for (idx = 0; idx < path_2_len; idx++) {
        if (path_2[idx] != '/') {
            break;
        }
        path_2_start_idx++;
    } 
    path_2_copy_len= path_2_len - path_2_start_idx;

    result = (char*)alloc_mem(path_1_copy_len + 1 + path_2_copy_len + 1);
    if (result) {
        char* ptr;
        ptr = result;
        memcpy(ptr, path_1, path_1_copy_len);
        ptr += path_1_copy_len;
        *ptr = '/';
        ptr++;
        memcpy(ptr, &path_2[path_2_start_idx], path_2_copy_len);
        ptr += path_2_copy_len;
        *ptr = '\0';
    }
    return result;
}

static void*
path_alloc(
    size_t size)
{
    return malloc(size);
}


/**
 * free heap object
 */
void
path_free(
    void* obj)
{
    free(obj);
}



/* vi: se ts=4 sw=4 et: */
