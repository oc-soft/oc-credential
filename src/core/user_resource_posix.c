#include "user_resource.h"
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>


#ifdef HAVE_EMSCRIPTEN_H
#include <emscripten.h>
#endif


#include "user_resource_i.h"
#include "user_resource_const.h"
#include "wasm_i.h"




/**
 * get credential data directory 
 */
WASM_EXPORT
char*
user_resource_get_credential_data_directory()
{
    char* result;
    char* home_dir;
    result = NULL;
    home_dir = NULL;
    home_dir = getenv("HOME"); 
    if (home_dir == NULL) {
        struct passwd* pw;
        pw = getpwuid(getuid());
        if (pw) {
            home_dir = pw->pw_dir; 
        }
    }
    if (home_dir) {
        size_t data_dir_name_len;
        size_t buffer_size;
        data_dir_name_len = strlen(OC_DATA_DIR_NAME);   
        buffer_size = strlen(home_dir); 
        buffer_size += 1;
        buffer_size += data_dir_name_len;
        buffer_size += 2;
        result = user_resource_i_alloc(buffer_size);
        if (result) {
            snprintf(result, buffer_size, "%s/%s/", home_dir, OC_DATA_DIR_NAME);
        }
        
    }
    return result;
}



/**
 * create credential data directory if not exits
 */
int
user_resource_create_credential_data_directory_if_not()
{
    int result;
    char* data_dir;
    data_dir = user_resource_get_credential_data_directory();
    result = data_dir ? 0 : -1;
    if (result == 0) {
        struct stat st;
        int state;
        memset(&st, 0, sizeof(st));
        state = stat(data_dir, &st); 
        if (state == 0) {
            result = (st.st_mode & S_IFMT) == S_IFDIR ? 0 : -1;
        } else {
            result = mkdir(data_dir, S_IRWXU);
        }
    }
    if (data_dir) {
        user_resource_free(data_dir);
    }
    return result;
}
/* vi: se ts=4 sw=4 et: */
