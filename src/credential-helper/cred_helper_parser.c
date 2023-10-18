#include "cred_helper_parser.h"
#include <stddef.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <libintl.h>
#include "logging.h"
#include "exe_info.h"

/**
 * display usage
 */
void
cred_helper_parser_display_usage();

/**
 * parse on or off str
 */
int
cred_helper_parser_parse_on_off(
    const char* on_off,
    int default_on_off);

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
                    .name = "service",
                    .has_arg = required_argument,
                    .val = 's'
                },
                {
                    .name = "generator",
                    .has_arg = no_argument,
                    .flag = NULL,
                    .val = 'g'
                },
                {
                    .name = "gui",
                    .has_arg = required_argument,
                    .flag = NULL,
                    .val = 'j',
                },
                {
                    .name = "limited",
                    .has_arg = required_argument,
                    .flag = NULL,
                    .val = 'l',
                },
                {
                    .name = "logging-level",
                    .has_arg = required_argument,
                    .val = 'e',
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
            switch (getopt_long(argc, argv,
                "hugs:e:i:j:l:v::", long_opts, NULL)) {
            case 'i':
                cred_helper_set_client_id(result, optarg); 
                break;
            case 'u':
                cred_helper_set_test_mode_to_get(result, 1);
                break;
            case 'g':
                cred_helper_set_generator_mode(result, 1);
                break;
            case 's':
                cred_helper_set_service(result, optarg);
                break;
            case 'j':
                cred_helper_run_gui_generator(result,
                    cred_helper_parser_parse_on_off(optarg, 1));
                break;
            case 'l':
                cred_helper_run_limited_device(result, 
                    cred_helper_parser_parse_on_off(optarg, 1));
                break;
            case 'e':
                {
                    char* endptr;
                    long level;
                    endptr = NULL;
                    level = strtol(optarg, &endptr, 10);
                    if (optarg != endptr) {
                        logging_set_level((int)level);
                    }
                }
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
            cred_helper_set_requested_usage(result, 1);
            cred_helper_parser_display_usage();
        }
    }
    return result;
}

/**
 * parse on or off str
 */
int
cred_helper_parser_parse_on_off(
    const char* on_off,
    int default_on_off)
{
    char tmp_on_off[6];
    size_t len;
    size_t on_off_len; 
    size_t idx;
    int result;
    const static char on_str[] = "on";
    const static char yes_str[] = "yes";
    const static char true_str[] = "true";
    const static char off_str[] = "off";
    const static char no_str[] = "no";
    const static char false_str[] = "false";
    const static struct {
        const char* word;
        size_t word_length;
        int on_off;
    } word_on_off[] = {
        { on_str, sizeof(on_str) - 1, 1 },
        { yes_str, sizeof(yes_str) - 1, 1 },
        { true_str, sizeof(true_str) - 1, 1 },
        { off_str, sizeof(off_str) - 1, 0 },
        { no_str, sizeof(no_str) - 1, 0 },
        { false_str, sizeof(false_str) - 1, 0 },
    };
    
    len = strlen(on_off);
    on_off_len = sizeof(tmp_on_off) - 1; 
    if (on_off_len > len) {
        on_off_len = len;
    }
    memcpy(tmp_on_off, on_off, on_off_len + 1); 
    for (idx = 0; idx < on_off_len; idx++) {
        tmp_on_off[idx] = (char)tolower(tmp_on_off[idx]);
    }
     
    result = default_on_off;
    for (idx = 0; sizeof(word_on_off) / sizeof(word_on_off[0]); idx++) {
        if (on_off_len == word_on_off[idx].word_length) {
            if (memcmp(tmp_on_off, word_on_off[idx].word, on_off_len) == 0) {
                result = word_on_off[idx].on_off;
                break;
            }
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
    char* exe_name;
    exe_name = exe_info_get_exe_name();
    if (exe_name) {
        printf(gettext(
"%s [OPTION]\n"
"-i, --cid=[CID]                set client id\n"
"-u, --auth-url=[URL]           set authentication url\n"
"-s, --service=[SERVICE]        set oauth token service provider\n"
"-v, --verbose=[LEVEL?]         set verbose level\n"
"-e, --logging-level=[LEVEL]    set logging level.\n"
"-g, --generator                run generator only\n"
"-j, --gui=[on|off]             run gui generator\n"
"-l, --limited=[on|off]         run generator for limited device\n" 
"-h, --help                     display this message\n"
"[GIT_OPERATION]                get, store, erase"),
            exe_name);
    }
    if (exe_name) {
        exe_info_free(exe_name);
    }
}

/* vi: se ts=4 sw=4 et: */
