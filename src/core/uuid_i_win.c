#include "uuid_i.h"
#include <Rpc.h>
/**
 * generate uuid
 */
int
uuid_i_generate(
    char** uuid,
    void* (*alloc)(size_t))
{
    UUID* uuid_0;
    int result;
    result = 0;
    uuid_0 = (UUID*)alloc(sizeof(GUID));
    if (uuid_0) {
        UuidCreate(uuid_0);
        result = 0;
        *uuid = (char*)uuid_0;
    } else {
        result = -1;
    }

    return result; 
}


/* vi: se ts=4 sw=4 et: */
