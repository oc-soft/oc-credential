#include "uuid_i.h"
#include <uuid/uuid.h>

#ifndef UUID_STR_LEN
#define UUID_STR_LEN 37
#endif
/**
 * generate uuid
 */
int
uuid_i_generate(
    char** uuid,
    void* (*alloc)(size_t))
{
    uuid_t uuid_0;
    char* uuid_str;
    int result;
    result = 0;
    uuid_str = (char*)alloc(UUID_STR_LEN);
    result = uuid_str ? 0 : -1;
    if (result == 0) {
        uuid_generate(uuid_0);
        uuid_unparse(uuid_0, uuid_str);
        *uuid = uuid_str;
    }
    return result; 
}


/* vi: se ts=4 sw=4 et: */
