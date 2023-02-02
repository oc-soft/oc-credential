#ifndef __CRED_HELPER_PARSER_H__
#define __CRED_HELPER_PARSER_H__

#include "cred_helper.h"

#ifdef __cplusplus
#define _CRED_HELPER_PARSER_ITFC_BEGIN extern "C" {
#define _CRED_HELPER_PARSER_ITFC_END }
#else
#define _CRED_HELPER_PARSER_ITFC_BEGIN 
#define _CRED_HELPER_PARSER_ITFC_END 
#endif

_CRED_HELPER_PARSER_ITFC_BEGIN 

/**
 * parse option string and load lmd
 */
cred_helper*
cred_helper_parser_parse_from_commands(
    int argc,
    char** argv);


_CRED_HELPER_PARSER_ITFC_END

/* vi: se ts=4 sw=4 et: */
#endif
