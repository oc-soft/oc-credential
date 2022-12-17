#include "user_resource.h"

#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "user_resource_i.h"
#include "user_resource_const.h"
/**
 * get credential data directory 
 */
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
 * get credential data directory 
 */
char*
user_resource_get_credential_data_path()
{
    char* result;
    char* data_dir;
    result = NULL;
    data_dir = user_resource_get_credential_data_directory();
    if (data_dir) {
        size_t buffer_size;
        buffer_size = strlen(data_dir); 
        buffer_size += strlen(OC_DB_FILE_NAME);
        buffer_size += strlen(OC_DB_FILE_EXT);
        buffer_size++;
        result = user_resource_i_alloc(buffer_size);
        if (result) {
            snprintf(result, buffer_size, "%s%s%s", 
                data_dir, OC_DB_FILE_NAME, OC_DB_FILE_EXT);
        }
        
    }
    if (data_dir) {
        user_resource_free(data_dir);
    }
    return result;
}


/**
 * free object
 */
void
user_resource_free(
    void* obj)
{
    user_resource_i_free(obj);
}



/* vi: se ts=4 sw=4 et: */
