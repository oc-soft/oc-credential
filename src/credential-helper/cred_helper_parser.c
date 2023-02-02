#include "cred_helper_parser.h"
#include <stddef.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

/**
 * display usage
 */
void
cred_helper_parser_display_usage();

/**
 * parse option string and load cred_helper
 */
cred_helper*
cred_helper_parser_parse_from_commands(
    int argc,
    char** argv)
{
    cred_helper* result;
    result = cred_helper_create();
    if (result) {
        int print_help;
        print_help = 0;
        while (1) {
            static struct option long_opts[] = {
                {
                    .name = "cid",
                    .has_arg = required_argument,
                    .flag = NULL,
                    .val = 'i' 
                },
                {
                    .name = "verbose",
                    .has_arg = optional_argument,
                    .flag = NULL,
                    .val = 'v'
                },
                {
                    .name = "get-test",
                    .has_arg = no_argument,
                    .flag = NULL,
                    .val = 'u'
                },
                {
                    .name = "generator",
                    .has_arg = no_argument,
                    .flag = NULL,
                    .val = 'g'
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
            int do_parse;
            do_parse = 1;
            switch (getopt_long(argc, argv, "hugi:v::", long_opts, NULL)) {
            case 'i':
                cred_helper_set_client_id(result, optarg); 
                break;
            case 'u':
                cred_helper_set_test_mode_to_get(result, 1);
                break;
            case 'g':
                cred_helper_set_generator_mode(result, 1);
                break;
            case 'v':
                if (optarg) {
                    char* endptr;
                    long level;
                    endptr = NULL;
                    level = strtol(optarg, &endptr, 10);
                    if (optarg != endptr) {
                        cred_helper_set_verbose_level(result, (int)level);
                    }
                } else {
                    cred_helper_set_verbose_level(result, 5);
                }
                break;
            case 'h':
                do_parse = 0;
                print_help = 1;
                break; 
            default:
                do_parse = 0;
            }
            if (!do_parse) {
                break;
            }
        }

        while (optind < argc) {
            if (strcmp(argv[optind], "get") == 0) {
                cred_helper_set_credential_op(result, CDT_OP_GET);
            } else if (strcmp(argv[optind], "store") == 0) {
                cred_helper_set_credential_op(result, CDT_OP_STORE);
            } else if (strcmp(argv[optind], "erase") == 0) {
                cred_helper_set_credential_op(result, CDT_OP_ERASE);
            }
            optind++;
        }
        if (print_help) {
            cred_helper_parser_display_usage();
        }
    }
    return result;
}

/**
 * display usage
 */
void
cred_helper_parser_display_usage()
{
    puts(
        "cred_helper [OPTION]\n"
        "-i, --cid=[CID]           set client id\n"
        "-u, --auth-url=[URL]      set authentication url\n"
        "-v, --verbose=[LEVEL?]    set verbose level\n"
        "-h, --help                display this message\n"
        "[GIT_OPERATION]           get, store, erase");
}

/* vi: se ts=4 sw=4 et: */
