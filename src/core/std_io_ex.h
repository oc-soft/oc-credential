#ifndef __STD_IO_EX_H__
#define __STD_IO_EX_H__

#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
#define _STD_IO_EX_ITFC_BEGIN extern "C" {
#define _STD_IO_EX_ITFC_END }
#else
#define _STD_IO_EX_ITFC_BEGIN 
#define _STD_IO_EX_ITFC_END
#endif

_STD_IO_EX_ITFC_BEGIN 

/**
 * vsnpintf
 */
int
std_io_ex_vsnprintf(
    char* buffer,
    size_t buffer_size,
    const char* format,
    va_list arg); 



/**
 * snpintf
 */
int
std_io_ex_snprintf(
    char* buffer,
    size_t buffer_size,
    const char* format,
    ...);

/**
 * print out formated string with arg to fs.
 * format is expected as utf8 string.
 */
int
std_io_ex_vfprintf(
    FILE* fs,
    const char* format,
    va_list arg);

/**
 * print out formated string to fs.
 * format is expected as utf8 string.
 */
int
std_io_ex_fprintf(
    FILE* fs,
    const char* format,
    ...);



/**
 * convert string for output stream
 */
char*
std_io_ex_convert_for_out_str(
    FILE* fs,
    const char* str);



/**
 * free converted string 
 */
void
std_io_ex_free_converted_str(
    char* str);

_STD_IO_EX_ITFC_END

/* vi: se ts=4 sw=4 et: */
#endif
