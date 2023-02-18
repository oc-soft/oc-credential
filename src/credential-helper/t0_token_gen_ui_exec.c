#include "token_gen_ui_exec.h"
#include "exec_names.h"
#include <stdio.h>

int
main(
    int argc,
    char** argv)
{
    char* exec_path;
    const char* exec_rel_path;
    int result;
    result = 0;
    exec_rel_path = exec_names_get_ui_token_gen_relpath();

    exec_path = token_gen_ui_exec_get_path();
    result = exec_path ? 0 : -1;
    if (result == 0) {
        printf("exec_path: %s\n", exec_path); 
    }
    if (result == 0) {
        printf("exec_rel_path: %s\n", exec_rel_path);
    }
    if (exec_path) {
        token_gen_ui_exec_free(exec_path);
    }

    return result;
}

/* vi: se ts=4 sw=4 et: */
