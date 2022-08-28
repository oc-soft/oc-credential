#ifndef __LMD_PARSER_H__
#define __LMD_PARSER_H__

#include "lmd.h"

#ifdef __cplusplus
#define _LMD_PARSER_ITFC_BEGIN extern "C" {
#define _LMD_PARSER_ITFC_END }
#else
#define _LMD_PARSER_ITFC_BEGIN 
#define _LMD_PARSER_ITFC_END 
#endif

_LMD_PARSER_ITFC_BEGIN 

/**
 * parse option string and load lmd
 */
lmd*
lmd_parser_parse_from_commands(
    int argc,
    char** argv);


_LMD_PARSER_ITFC_END

/* vi: se ts=4 sw=4 et: */
#endif
