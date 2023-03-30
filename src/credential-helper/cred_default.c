#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <getopt.h>

#include "user_resource.h"

/**
 * credential helper default setting option
 */
typedef struct _cred_default cred_default;

/**
 * credential helper default setting option
 */
struct _cred_default {
    /* append new line at end output*/
    int append_nl;
};

/**
 * output help message
 */
static int
print_help(
    const cred_default* option);

/**
 * output data path
 */
static int
print_data_path(
    const cred_default* option);

/**
 * output resource directory
 */
static int
print_resource_dir(
    const cred_default* option);

/**
 * entry point
 */
int
main(
    int argc,
    char** argv)
{
    int result;
    cred_default def_outoption = {
        .append_nl = 1
    };
    static struct option options[] = {
        {
            .name = "data-path",
            .has_arg = no_argument,
            .flag = NULL,
            .val = 'd'
        },
        {
            .name = "res-dir",
            .has_arg = no_argument,
            .flag = NULL,
            .val = 'r'
        },
        {
            .name = "no-newline",
            .has_arg = no_argument,
            .flag = NULL,
            .val = 'n'
        },
        {
            .name = "help",
            .has_arg = no_argument,
            .flag = NULL,
            .val = 'h'
        },
        {
            .name = NULL,
            .has_arg = 0,
            .flag = NULL,
            .val = 0
        }
    };
    int (*processor)(const cred_default*);
    result = 0;
    processor = print_help;

    do {
        int opt;
        int done_process_opt;
        done_process_opt = 0;
        opt = getopt_long(argc, argv, "hdrn", options, NULL);
        switch (opt) {
        case 'd':
            processor = print_data_path;
            break;
        case 'r':
            processor = print_resource_dir;
            break;
        case 'n':
            def_outoption.append_nl = 0;
            break;
        case 'h':
        case '?':
            break;
        default:
            done_process_opt = 1;
        }
        if (done_process_opt) {
            break;
        } 
    } while (1);

    result = processor(&def_outoption);
    
    return result;
}


/**
 * output help message
 */
static int
print_help(
    const cred_default* option)
{
    int result;
    result = 0;
    puts("cred_default [OPTION]\n"
        "-d, --data-path       prints credential helper data path.\n"
        "-r, --res-dir         prints credential helper resource directory.\n"
        "-n, --no-newline      does not append new line at end of output.\n"
        "-h, --help            prints this message.\n");
    return result;
}

/**
 * output data path
 */
static int
print_data_path(
    const cred_default* option)
{
    int result;
    char* data_path;
    result = 0;
    data_path = user_resource_get_credential_data_path();
    if (data_path) {
        puts(data_path);
        if (option->append_nl) {
            puts("\n");
        }
    }

    if (data_path) {
        user_resource_free(data_path);
    }

    return result;
}

/**
 * output resource directory
 */
static int
print_resource_dir(
    const cred_default* option)
{
    int result;
    char* data_dir;
    result = 0;
    data_dir = user_resource_get_credential_data_directory();
    if (data_dir) {
        puts(data_dir);
        if (option->append_nl) {
            puts("\n");
        }
    }

    if (data_dir) {
        user_resource_free(data_dir);
    }

    return result;
}


/* vi: se ts=4 sw=4 et: */
