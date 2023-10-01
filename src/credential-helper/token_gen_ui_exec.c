#include "token_gen_ui_exec.h"
#include <stdio.h>
#include <stdlib.h>
#include "exe_info.h"
#include "path.h"
#include "exec_names.h"
#include "logging.h"

/**
 * get ui exec relative path
 */
const char*
token_gen_ui_exec_get_relpath();

/**
 * allocate object
 */
void*
token_gen_ui_exec_alloc(
    size_t size);

/**
 * get executable path
 */
char*
token_gen_ui_exec_get_path()
{
    char* exe_dir;
    char* result;
    result = NULL;
    logging_log(LOG_LEVEL_DEBUG, "token generator executable get path");
    exe_dir = exe_info_get_exe_dir();
    if (exe_dir) {
        result = path_join(exe_dir, token_gen_ui_exec_get_relpath(),
            token_gen_ui_exec_alloc,
            token_gen_ui_exec_free);
    }


    if (exe_dir) {
        exe_info_free(exe_dir);
    }
    return result;
}

/**
 * allocate object
 */
void*
token_gen_ui_exec_alloc(
    size_t size)
{
    return malloc(size);
}


/**
 * free object
 */
void
token_gen_ui_exec_free(
    void* obj)
{
    free(obj);
}


/**
 * get ui exec relative path
 */
const char*
token_gen_ui_exec_get_relpath()
{
    return exec_names_get_ui_token_gen_relpath();
}
/* vi: se ts=4 sw=4 et: */
