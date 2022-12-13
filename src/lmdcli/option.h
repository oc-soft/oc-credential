#ifndef __OPTION_H__
#define __OPTION_H__

#ifdef __cplusplus
#define _OPTION_ITFC_BEGIN extern "C" {
#define _OPTION_ITFC_END }
#else
#define _OPTION_ITFC_BEGIN 
#define _OPTION_ITFC_END 
#endif

_OPTION_ITFC_BEGIN 

/**
 * general option
 */
typedef struct _option option;


/**
 * incremtent reference count
 */
unsigned int
option_retain(
    option* obj);

/**
 * decremtent reference count
 */
unsigned int
option_release(
    option* obj);

/**
 * parse option from commands
 */
option*
option_parse_from_commands(
    int argc,
    char** argv);

_OPTION_ITFC_END 
/* vi: se ts=4 sw=4 et: */
#endif
