#include <stdio.h>
#include <curl/curl.h>
#include <libintl.h>
#include <string.h>

#include "lmd_parser.h"
#include "lmd.h"
#include "lmd_connections.h"
#include "lmd_requests.h"
#include "client_secret.h"
#include "client_id.h"

/**
 * progress
 */
typedef struct _lmd_progress lmd_progress;

/**
 * progress
 */
struct _lmd_progress {
    /**
     * last second updating
     */
    int last_sec_updating;

};

/**
 * print progress for oauth token
 */
static int
print_progress_for_oauth_token(
    lmd_progress* progress, 
    int elapse,
    lmd* obj);

/**
 * entry point
 */
int
main(
    int argc,
    char** argv)
{
    int result;
    lmd* limited_acc;
    lmd_progress progress;
    curl_global_init(CURL_GLOBAL_WIN32);

    memset(&progress, 0, sizeof(progress));
    limited_acc = lmd_parser_parse_from_commands(argc, argv);
    result = limited_acc ? 0 : -1;
    if (result == 0) {
        result = lmd_set_client_secret(limited_acc, client_secret_get());
    }
    if (result == 0 && !lmd_get_client_id_ref(limited_acc)) {
        result = lmd_set_client_id(limited_acc, client_id_get());
    }

    if (result == 0) {
        result = lmd_connections_load_discovery_document(limited_acc);
    }
    if (result == 0) {
        result = lmd_requests_load_device_and_user_code(limited_acc);
    }

    if (result == 0) {
        result = lmd_requests_poll_oauth_token(limited_acc,
            (int (*)(void*, int, lmd*))print_progress_for_oauth_token,
            &progress);
    }  

    if (result ==  0) {
        if (lmd_get_verbose_level(limited_acc) >= 5) {
            char* str_lmd;
            str_lmd = lmd_get_str_representation(limited_acc);
            if (str_lmd) {
                puts(str_lmd);
            } 
            lmd_free_object(limited_acc, str_lmd); 
        }
    }

    result = 0;
    if (limited_acc) {
        lmd_release(limited_acc);
    }
    curl_global_cleanup();
    return result; 
}

/**
 * print progress for oauth token
 */
static int
print_progress_for_oauth_token(
    lmd_progress* progress,
    int elapse,
    lmd* lmd_obj)
{
    int result;
    result = 0;
    if (lmd_get_verbose_level(lmd_obj) < 5) {
        if (progress->last_sec_updating != elapse) {
            const char* verification_url;
            const char* user_code;
            int expires_in;
            verification_url = lmd_get_verification_url_ref(lmd_obj);
            user_code = lmd_get_user_code_ref(lmd_obj);
            expires_in = lmd_get_polling_expires_in(lmd_obj);

            if (!progress->last_sec_updating) {
                printf(
                    gettext(
                        "URL:\n"
                        "\x1b[1m%s\x1b[0m\n"
                        "Code:\n"
                        "\x1b[1m%s\x1b[0m\n"), 
                    verification_url, user_code); 
            } else {
            }
            printf(
                gettext(
                    "\r"
                    "Open browser, visit the url and input the code as above" 
                    " in %d seconds."),
                expires_in - elapse); 
            fflush(stdout);
            
            progress->last_sec_updating = elapse; 
        }
    }
    return result;
}



/* vi: se ts=4 sw=4 et: */
