#include "std_io_ex.h"
#include <stdio.h>

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
    result = vsnprintf(buffer, buffer_size, format, arg);
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
    return vfprintf(fs, format, arg);
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

    return str;
}

void
std_io_ex_free_converted_str(
    char* str)
{
}

/* vi: se ts=4 sw=4 et: */
