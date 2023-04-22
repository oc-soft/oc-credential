#include "path.h"
#include <stdlib.h>
#include <errno.h>
#include <windows.h>
#include <Pathcch.h>
#include <strsafe.h>
#include "str_conv.h"


/**
 * alloc memory on heap
 */
static void*
path_join_i_alloc(
    size_t size);

/**
 * free memory from heap
 */
static void
path_join_i_free(
    void* obj);


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
    char* result;
    
    result = NULL;
    if (path_1 && path_2) {
        LPWSTR path_1w;
        LPWSTR path_2w;

        path_1w = str_conv_utf8_to_utf16(
            path_1, strlen(path_1) + 1, NULL, NULL);
        path_2w = str_conv_utf8_to_utf16(
            path_2, strlen(path_2) + 1, NULL, NULL);
        if (path_1w && path_2w) {
            LPWSTR path_12w;
            HRESULT hr;
            size_t path_12w_size;
            path_12w_size = 0;
            path_12w = NULL;
            hr = PathAllocCombine(path_1w, path_2w, PATHCCH_ALLOW_LONG_PATHS,
                &path_12w);

            if (SUCCEEDED(hr)) {
                hr = StringCchLengthW(
                    path_12w, STRSAFE_MAX_CCH, &path_12w_size);
            }
            if (SUCCEEDED(hr)) {
                result = str_conv_utf16_to_utf8(path_12w, path_12w_size + 1,
                    alloc_mem ? alloc_mem : path_join_i_alloc, 
                    free_mem ? free_mem : path_join_i_free);
            }
            if (path_12w) {
                LocalFree(path_12w);
            }
        }
        if (path_1w) {
            str_conv_free(path_1w);    
        }
        if (path_2w) {
            str_conv_free(path_2w); 
        }
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * alloc memory on heap
 */
static void*
path_join_i_alloc(
    size_t size)
{
    return malloc(size);
}

/**
 * free memory from heap
 */
static void
path_join_i_free(
    void* obj)
{
    free(obj);
}



/* vi: se ts=4 sw=4 et: */
