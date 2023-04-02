#include <stdio.h>
#include <stddef.h>
#include "credential_desc.h"

int
main(
    int argc,
    char** argv)
{
    int result;
    credential_desc* desc;
    result = 0;
    
    desc = credential_desc_decode_from_file(stdin);

    result = desc ? 0 : -1;
    if (result == 0) {
        credential_desc_encode_into_file(desc, stdout);
    }

    if (desc) {
        credential_desc_free(desc);
    } 
    return result;
}


/* vi: se ts=4 sw=4 et: */
