#ifndef __HTTP_H__
#define __HTTP_H__

#include <stddef.h>

#ifdef __cplusplus
#define _HTTP_ITFC_BEGIN extern "C" {
#define _HTTP_ITFC_END }
#else
#define _HTTP_ITFC_BEGIN 
#define _HTTP_ITFC_END 
#endif

_HTTP_ITFC_BEGIN 



/**
 * find http body
 */
const char*
http_find_body(
    const char* http_response,
    size_t length);


_HTTP_ITFC_END 
/* vi: se ts=4 sw=4 et: */
#endif
