#include "config.h"
#include "user_resource.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
WASM_EXPORT
void
user_resource_free(
    void* obj)
{
    user_resource_i_free(obj);
}



/* vi: se ts=4 sw=4 et: */
