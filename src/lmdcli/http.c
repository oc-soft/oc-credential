#include "http.h"


const char*
http_find_body(
    const char* http_response,
    size_t length)
{
    const static char* end_of_hdr = "\r\n\r\n";
    const static size_t end_of_hdr_len = 4; 
    const char* result;

    result = memmem(http_response, length, end_of_hdr, end_of_hdr_len); 

    return result; 
}

/* vi: se ts=4 sw=4 et: */
