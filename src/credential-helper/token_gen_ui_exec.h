#ifndef __TOKEN_GEN_UI_EXEC_H__
#define __TOKEN_GEN_UI_EXEC_H__


#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __cplusplus
#define _TOKEN_GEN_UI_EXEC_BEGIN extern "C" {
#define _TOKEN_GEN_UI_EXEC_END }
#else
#define _TOKEN_GEN_UI_EXEC_BEGIN 
#define _TOKEN_GEN_UI_EXEC_END 
#endif

_TOKEN_GEN_UI_EXEC_BEGIN 

/**
 * get executable path
 */
char*
token_gen_ui_exec_get_path();

/**
 * free object
 */
void
token_gen_ui_exec_free(
    void* obj);

_TOKEN_GEN_UI_EXEC_END
/* vi: se ts=4 sw=4 et: */
#endif
