#ifndef __URL_PATH_ITERATE_H__
#define __URL_PATH_ITERATE_H__


#ifdef __cplusplus
#define _URL_PATH_ITERATE_ITFC_BEGIN extern "C" {
#define _URL_PATH_ITERATE_ITFC_END }
#else
#define _URL_PATH_ITERATE_ITFC_BEGIN 
#define _URL_PATH_ITERATE_ITFC_END 
#endif
_URL_PATH_ITERATE_ITFC_BEGIN 

/**
 * iterate url path 
 */
int
url_path_iterate(
    const char* protocol,
    const char* host,
    const char* path,
    int (*iterator)(void*, const char*, const char*, const char*),
    void* user_data);

_URL_PATH_ITERATE_ITFC_END 
/* vi: se ts=4 sw=4 et: */
#endif
