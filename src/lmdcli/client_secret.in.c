#include "client_secret.h"


/**
 * get client secret
 */
const char*
client_secret_get(
    const char* protocol,
    const char* host,
    const char* path)
{
    return "@CLIENT_SECRET@";
}

/* vi: se ts=4 sw=4 et: */
