#include "l10n.h"
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <pathcch.h>

#include "exe_info.h"
#include "str_conv.h"


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
    int state;
    LPWSTR exe_dir_w;
    LPWSTR locpath_w;
    result = NULL;
    exe_dir_w = NULL;
    locpath_w = NULL;
    state = 0;
    exe_dir = exe_info_get_exe_dir();
    state = exe_dir ? 0 : -1;
    if (state == 0) {
        exe_dir_w = str_conv_utf8_to_utf16(
            exe_dir,
            strlen(exe_dir) + 1,
            l10n_alloc, l10n_free);
        state = exe_dir_w ? 0 : -1; 
    }


    if (state == 0) {
        HRESULT h_res;
        h_res = PathAllocCombine(
            exe_dir_w, 
            L"locale", 
            PATHCCH_ALLOW_LONG_PATHS, &locpath_w);
        state = SUCCEEDED(h_res) ? 0 : -1;
    }
    if (state == 0) {
        result = str_conv_utf16_to_utf8(
            locpath_w,
            (wcslen(locpath_w) + 1) * sizeof(WCHAR),
            l10n_alloc, l10n_free);
    }

    if (locpath_w) {
        LocalFree(locpath_w);
    }
    if (exe_dir_w) {
        l10n_free(exe_dir_w);
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
