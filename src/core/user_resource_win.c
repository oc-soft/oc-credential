#include "user_resource.h"
#include "user_resource_i.h"
#include "user_resource_const.h"
#include <stddef.h>
#include <stdlib.h>
#include <windows.h>
#include <pathcch.h>
#include <Shlobj.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <Knownfolders.h>
#include <Objbase.h>
#include "str_conv.h"

/**
 * get credential data directory 
 */
char*
user_resource_get_credential_data_directory()
{
    char* result;
    wchar_t* home_dir;
    wchar_t* data_dir;
    int free_home_dir;
    result = NULL;
    home_dir = NULL;
    data_dir = NULL;
    free_home_dir = 0;
    home_dir = _wgetenv(L"HOME"); 
    if (home_dir == NULL) {
        HRESULT h_res;
        h_res = SHGetKnownFolderPath(
            &FOLDERID_LocalAppData,
            KF_FLAG_DEFAULT, NULL, &home_dir);
        free_home_dir = home_dir != NULL;

    }
    if (home_dir) {
        PathAllocCombine(home_dir, 
            OC_DATA_DIR_NAME_W,
            PATHCCH_ALLOW_LONG_PATHS,
            &data_dir);
    }

    if (data_dir) {
        size_t length;
        HRESULT h_res;
        length = 0;
        h_res = StringCchLengthW(data_dir, STRSAFE_MAX_CCH, &length);

        if (SUCCEEDED(h_res)) {
            result = str_conv_utf16_to_utf8(
                data_dir, length + 1,
                user_resource_i_alloc,
                user_resource_i_free);
        }
    }
    if (data_dir) {
        LocalFree(data_dir);
    }
    if (free_home_dir && home_dir) {
        CoTaskMemFree(home_dir); 
    }
    return result;
}

/**
 * create credential data directory if not exits
 */
int
user_resource_create_credential_data_directory_if_not()
{
    int result;
    char* data_dir;
    LPWSTR data_dir_w;
    data_dir_w = NULL;
    data_dir = user_resource_get_credential_data_directory();
    result = data_dir ? 0 : -1;
    if (result == 0) {
        size_t length;
        HRESULT h_res;
        length = 0;
        h_res = StringCchLengthA(data_dir, STRSAFE_MAX_CCH, &length);

        result = SUCCEEDED(h_res) ? 0 : -1;
        if (result == 0) {
            data_dir_w = str_conv_utf8_to_utf16(
                data_dir, length + 1,
                user_resource_i_alloc,
                user_resource_i_free);
            result = data_dir_w ? 0 : -1;
        }
 
    }
    if (result == 0) {
        BOOL dir_exists;
        dir_exists = PathIsDirectoryW(data_dir_w);
        if (dir_exists) {
            result = 0;
        } else {
            result = CreateDirectoryW(data_dir_w, NULL);
        }
    }
    if (data_dir_w) {
        user_resource_i_free(data_dir_w);
    }
    if (data_dir) {
        user_resource_free(data_dir);
    }
    return result;
}

/* vi: se ts=4 sw=4 et: */
