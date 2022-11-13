#include "l10n.h"
#include <string.h>
#include <limits.h>
#include <stdio.h>

#include "exe_info.h"

/**
 * allocate memory
 */
static void*
l10n_alloc(
    size_t size);

/**
 * get locale directory
 */
char*
l10n_get_locale_dir()
{
    char* result;
    char* exe_dir;
    result = (char*)l10n_alloc(PATH_MAX);
    exe_dir = exe_info_get_exe_dir();
    if (result) {
        snprintf(result, PATH_MAX, "%s/locale", exe_dir); 
    }
    if (exe_dir) {
        exe_info_free(exe_dir);
    }
    return result;
}

/**
 * allocate memory
 */
static void*
l10n_alloc(
    size_t size)
{
    return malloc(size);
}

/**
 * free heap memory allocated by this api.
 */
void
l10n_free(
    void* obj)
{
    free(obj);
}
/* vi: se ts=4 sw=4 et: */
