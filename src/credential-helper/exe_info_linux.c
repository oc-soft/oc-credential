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
