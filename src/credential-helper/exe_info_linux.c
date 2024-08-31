#include "exe_info.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

/**
 * allocate memory
 */
static void*
exe_info_alloc(
    size_t size);

/**
 * get executable name
 */
char*
exe_info_get_exe_name()
{
    char* result;
    char* path_buff;
    path_buff = (char*)exe_info_alloc(PATH_MAX);
    result = NULL;
    if (path_buff) {
        size_t size;
        size = readlink("/proc/self/exe", path_buff, PATH_MAX);
        if (size) {
            char* name_ptr;
            size_t i;
            name_ptr = NULL;
            for (i = size - 1; i > 0; i--) {
                if (path_buff[i] == '/') {
                    name_ptr = &path_buff[i + 1];
                    break;
                }
            }
            if (name_ptr) {
                result = (char*)exe_info_alloc(size - i + 1);
                if (result) {
                    memcpy(result, name_ptr, size - i);
                    result[size - i] = '\0';
                }
            }
        }
    }
    if (path_buff) {
        exe_info_free(path_buff);
    }
    return result;
}

/**
 * get executable directory
 */
char*
exe_info_get_exe_dir()
{
    char* result;
    result = (char*)exe_info_alloc(PATH_MAX);
    if (result) {
        size_t size;
        size = readlink("/proc/self/exe", result, PATH_MAX);
        if (size) {

            size_t i;
            for (i = size - 1; i > 0; i--) {
                if (result[i] == '/') {
                    result[i] = '\0';
                    break;
                }
            }
        }
    }
    return result;
}


/**
 * free resource
 */
void
exe_info_free(
    void* obj)
{
    free(obj);
}

/**
 * allocate memory
 */
static void*
exe_info_alloc(
    size_t size)
{
    return malloc(size);
}


/* vi: se ts=4 sw=4 et: */
