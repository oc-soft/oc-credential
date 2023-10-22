#ifndef __SERVICE_OAUTH_TOKEN_PARSER_H__
#define __SERVICE_OAUTH_TOKEN_PARSER_H__

#include <json-c/json.h>
#include "lmd.h"

#ifdef __cplusplus
#define _SERVICE_OAUTH_TOKEN_PARSER_ITFC_BEGIN extern "C" {
#define _SERVICE_OAUTH_TOKEN_PARSER_ITFC_END }
#else
#define _SERVICE_OAUTH_TOKEN_PARSER_ITFC_BEGIN 
#define _SERVICE_OAUTH_TOKEN_PARSER_ITFC_END
#endif

_SERVICE_OAUTH_TOKEN_PARSER_ITFC_BEGIN 

/**
 * get oauth token loader
 */
int
service_oauth_token_parser_get(
    const char* service,
    int (**loader)(lmd*, json_object* json));


_SERVICE_OAUTH_TOKEN_PARSER_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif


