#include "user_resource.h"
#include <sys/stat.h>
#include <string.h>
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
