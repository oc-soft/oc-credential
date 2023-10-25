#include "config.h"
#include "std_io_ex.h"
#include <windows.h>
#include <errno.h>
#include <wchar.h>
#include <io.h>

#include "str_conv.h"

/**
 * duplicate string
 */
static char*
std_io_ex_str_dup(
    const char* str,
    size_t length);

/**
 * allocate memory
 */
static void*
std_io_ex_alloc(
    size_t size);

/**
 * free memory
 */
static void
std_io_ex_free(
    void* obj);

/**
 * vsnpintf
 */
int
std_io_ex_vsnprintf(
    char* buffer,
    size_t buffer_size,
    const char* format,
    va_list arg) 
{
    int result;
    result = _vsprintf_p(buffer, buffer_size, format, arg);
    return result;
}



/**
 * snpintf
 */
int
std_io_ex_snprintf(
    char* buffer,
    size_t buffer_size,
    const char* format,
    ...)
{
    int result;
    va_list arg;
    va_start(arg, format);
    result = std_io_ex_vsnprintf(buffer, buffer_size, format, arg);
    va_end(arg);
    return result;
}


/**
 * print out formated string with arg to fs.
 * format is expected as utf8 string.
 */
int
std_io_ex_vfprintf(
    FILE* fs,
    const char* format,
    va_list arg)
{
    int result;
    result = 0;
    if (format && fs) {

        int out_to_tty;
        out_to_tty = _isatty(_fileno(fs));
        if (out_to_tty) {
            char* format_cp;
            format_cp = str_conv_utf8_to_cp_str(format, strlen(format) + 1,
                GetConsoleOutputCP(), NULL, NULL);

            result = format_cp ? 0 : -1;
            if (result == 0) {
                result = _vfprintf_p(fs, format_cp, arg);
            }

            if (format_cp) {
                str_conv_free(format_cp);
            }
        } else {
            result = vfprintf(fs, format, arg);
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * print out formated string to fs.
 * format is expected as utf8 string.
 */
int
std_io_ex_fprintf(
    FILE* fs,
    const char* format,
    ...)
{
    va_list arg;
    int result;
    va_start(arg, format);
    result = std_io_ex_vfprintf(fs, format, arg);
    va_end(arg);
    return result;
}

/**
 * convert string for output stream
 */
char*
std_io_ex_convert_for_out_str(
    FILE* fs,
    const char* str)
{
    char* result;
    result = NULL;
    if (str) {
        if (fs) {
            int out_to_tty;
            out_to_tty = _isatty(_fileno(fs));
            if (out_to_tty) {
                result = str_conv_utf8_to_cp_str(
                    str, strlen(str) + 1, 
                    GetConsoleOutputCP(),
                    std_io_ex_alloc, std_io_ex_free);

            } else {
                result = std_io_ex_str_dup(str, strlen(str) + 1);
            }
        } else {
            errno = EINVAL;
        }
    }
    return result;
}

/**
 * allocate memory
 */
static void*
std_io_ex_alloc(
    size_t size)
{
    return malloc(size);
}

/**
 * free memory
 */
static void
std_io_ex_free(
    void* obj)
{
    free(obj);
}



/**
 * free converted string 
 */
void
std_io_ex_free_converted_str(
    char* str)
{
    std_io_ex_free(str);
}

/**
 * duplicate string
 */
static char*
std_io_ex_str_dup(
    const char* str,
    size_t length)
{
    char* result;
    result = NULL;
    if (str) {
        result = (char*)std_io_ex_alloc(length);
        if (result) {
            memcpy(result, str, length);
        }
    }
    return result;
}


/* vi: se ts=4 sw=4 et: */
