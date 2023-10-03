#ifndef __STD_FILE_DESC_H__
#define __STD_FILE_DESC_H__
#include "file_desc.h"

#ifdef __cplusplus
#define _STD_FILE_DESC_ITFC_BEGIN extern "C" {
#define _STD_FILE_DESC_ITFC_END }
#else
#define _STD_FILE_DESC_ITFC_BEGIN 
#define _STD_FILE_DESC_ITFC_END 
#endif

_STD_FILE_DESC_ITFC_BEGIN 

/**
 * create file descritor object
 */
file_desc*
std_file_desc_create(
    int fd);


_STD_FILE_DESC_ITFC_END

/* vi: se ts=4 sw=4 et: */
#endif
