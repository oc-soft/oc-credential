#include "l10n.h"
#include "l10n_i.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <path.h>
#include "exe_info.h"

/**
 * allocate memory
 */
static void*
l10n_alloc(
    size_t size);

/**
 * get locale path file name
 */
static const char*
l10n_get_locpath_name();

/**
 * get file path for locale path
 */
static char* 
l10n_get_locpath_file_path();

/**
 * read locale path from file
 */
static char*
l10n_read_locpath_from_file(
    const char* file);

/**
 * get locale directory
 */
char*
l10n_get_locale_dir()
{
    char* result;
    char* exe_dir;
    char* locpath_file;
    exe_dir = NULL;
    result = NULL;
    locpath_file = NULL;

    locpath_file = l10n_i_get_locpath_file_path();

    if (locpath_file) {
        result = l10n_i_read_locpath_from_file(locpath_file);
    }

    if (!result) {
        exe_dir = exe_info_get_exe_dir();
        if (exe_dir) {
            result = path_join(exe_dir, "locale", l10n_alloc, l10n_free);
        }
    }
    if (locpath_file) {
        l10n_i_free(locpath_file);
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
    return l10n_i_alloc(size);
}

/**
 * free heap memory allocated by this api.
 */
void
l10n_free(
    void* obj)
{
    l10n_i_free(obj);
}
/* vi: se ts=4 sw=4 et: */
