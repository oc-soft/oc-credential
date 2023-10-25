#include "str_conv.h"
#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>


int
wmain(
    int argc,
    wchar_t** argv)
{
    int result;
    int i;
    result = 0;

    for (i = 1; i < argc; i++) {
        char* utf8_str;
        char* cp_str;
        utf8_str = NULL;
        cp_str = NULL;
        utf8_str = (char*)str_conv_utf16_to_utf8(
            argv[i], wcslen(argv[i]) + 1,
            NULL, NULL);

        result = utf8_str ? 0 : -1;
        if (result == 0) {
            cp_str = str_conv_utf8_to_cp_str(
                utf8_str, strlen(utf8_str),
                GetConsoleOutputCP(), NULL, NULL);
            result = cp_str ? 0 : -1;
        }

        if (result == 0) {
            fputws(argv[i], stdout);
            fputwc(L',', stdout);
            fputs(cp_str, stdout);
            fputwc(L'\n', stdout);
        }

        str_conv_free(utf8_str);
        str_conv_free(cp_str);

        if (result) {
            break;
        }

    }

    return result;
}



/* vi: se ts=4 sw=4 et: */
