#include "option.h"
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <getopt.h>

/**
 * option
 */
struct _option {
    
    /**
     * reference count
     */
    unsigned int ref_count;

    /**
     * verbose level
     */
    int verbose_level;
};

/**
 * create option
 */
option*
option_create()
{
    option* result;
    result = (option*)malloc(sizeof(option));
    if (result) {
        result->ref_count = 1;
        result->verbose_level = 0; 
    }
    return result;
}

/**
 * incremtent reference count
 */
unsigned int
option_retain(
    option* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = ++obj->ref_count;
    } else {
        errno = EINVAL;
    }
    return result;
}

/**
 * decremtent reference count
 */
unsigned int
option_release(
    option* obj)
{
    unsigned int result;
    result = 0;
    if (obj) {
        result = --obj->ref_count;
        if (result == 0) {
            free(obj);
        }
    } else {
        errno = EINVAL;
    }
    return result;
}


/**
 * parse option from commands
 */
option*
option_parse_from_commands(
    int argc,
    char** argv)
{
    option* result;
    result = option_create(); 
    if (result) {
        while (1) {
           static struct option long_opts[] = {
           }
            int do_parse;
            do_parse = 1;
            switch (getopt_long(argc, argv, "i:u:", long_opts, NULL)) {
            case 'v':
                if (optarg) {
                } else {
                     
                }
                break;
            }
        }
    }
    return result;
}

/* vi: se ts=4 sw=4 et: */
