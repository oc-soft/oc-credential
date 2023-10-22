#include "lmd_github_connections.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
#include "logging.h"


/**
 * get the url to poll oauth token
 */
const char*
lmd_github_connections_get_token_endpoint_url();

/**
 * get the url to verify device code:
 */
const char*
lmd_github_connections_get_device_auth_endpoint_url();

/**
 * load github oauth related endpoint urls
 */
int
lmd_github_connections_load_oauth_endpoint_urls(
    lmd* obj)
{
    int result;
    result = lmd_set_token_endpoint_url(obj,
        lmd_github_connections_get_token_endpoint_url());


    if (result == 0) {
        const char* options[] = {
            "Accept: application/json"
        };
        result = lmd_set_token_eprqhd_options(obj,
            options, sizeof(options) / sizeof(options[0])); 
    }

    if (result == 0) {
        result = lmd_set_device_auth_token_endpoint_url(obj,
            lmd_github_connections_get_device_auth_endpoint_url());

    }
    if (result == 0) {
        const char* options[] = {
            "Accept: application/json"
        };
        result = lmd_set_dot_eprqhd_options(obj,
            options, sizeof(options) / sizeof(options[0])); 
    }
    return result;
}

/**
 * get the url to poll oauth token
 */
const char*
lmd_github_connections_get_token_endpoint_url()
{
    return "https://github.com/login/oauth/access_token";
}

/**
 * get the url to verify device code:
 */
const char*
lmd_github_connections_get_device_auth_endpoint_url()
{
    return "https://github.com/login/device/code";
}

/* vi: se ts=4 sw=4 et: */
