#include "exe_info.h"
#include <unistd.h>
#include <stdlib.h>
#include <libproc.h>
#include <string.h>

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
    path_buff = (char*)exe_info_alloc(PROC_PIDPATHINFO_SIZE);
    result = NULL;
    if (path_buff) {
        int size;
	    size = proc_pidpath((int)getpid(), path_buff, PROC_PIDPATHINFO_SIZE);
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
                result = (char*)exe_info_alloc(size - i);
                if (result) {
                    memcpy(result, name_ptr, size - i - 1);
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
    char* path_buff;
    result = (char*)exe_info_alloc(PROC_PIDPATHINFO_SIZE);
    if (result) {
        int size;
	    size = proc_pidpath((int)getpid(), result, PROC_PIDPATHINFO_SIZE);
        if (size) {
            size_t i;
            for (i = size - 1; i > 0; i--) {
                if (result[i] == '/') {
                    result[i] = '\0';
                    break;
                }
            }
        } else  {
            exe_info_free(result);
            result = NULL;
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
