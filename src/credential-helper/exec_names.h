#ifndef __EXEC_NAMES_H__
#define __EXEC_NAMES_H__


#ifdef __cplusplus
#define _EXEC_NAMES_ITFC_BEGIN extern "C" {
#define _EXEC_NAMES_ITFC_END }
#else
#define _EXEC_NAMES_ITFC_BEGIN 
#define _EXEC_NAMES_ITFC_END 
#endif

_EXEC_NAMES_ITFC_BEGIN 

/**
 * ui token generator executable name
 */
const char*
exec_names_get_ui_token_gen();

/**
 * ui token generator executable relative from app directory
 */
const char*
exec_names_get_ui_token_gen_relpath();

_EXEC_NAMES_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif
