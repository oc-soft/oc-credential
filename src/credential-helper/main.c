#include "config.h"
#include <stdio.h>
#include <curl/curl.h>
#include <libintl.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

#include "app_config.h"
#include "cred_helper_parser.h"
#include "cred_helper.h"
#include "cred_helper_op.h"
#include "credential_storage.h"
#include "l10n.h"
#include "mem_dbg.h"
/**
 * initialize internationalization
 */
static int
setup_l10n();


/**
 * entry point
 */
int
main(
    int argc,
    char** argv)
{
    int result;
    cred_helper* cred_helper;

    cred_helper = NULL;
    mem_dbg_start_trace();
    setlocale(LC_ALL, "");
    result = setup_l10n();    
    curl_global_init(CURL_GLOBAL_WIN32);
    if (result == 0) {
        result = app_config_start();
    }
    if (result == 0) {
        result = credential_storage_start();
    }
    if (result == 0) {
        cred_helper = cred_helper_parser_parse_from_commands(argc, argv);
        result = cred_helper ? 0 : -1;
    }
    if (result == 0) {
        result = cred_helper_op_run(cred_helper);
    }
    if (cred_helper) {
        cred_helper_release(cred_helper);
    }
    credential_storage_stop();
    app_config_stop();
    curl_global_cleanup();
    return result; 
}

/**
 * setup internationalization
 */
static int
setup_l10n()
{
    int result;
    char* locale_dir;
    result = 0;
    locale_dir = l10n_get_locale_dir();
    result = locale_dir ? 0 : -1;
    if (result == 0) {
        textdomain(GETTEXT_DOMAIN);
        bindtextdomain(GETTEXT_DOMAIN, locale_dir);
    }
    if (locale_dir) {
        l10n_free(locale_dir);
    }
    return result;
}



/* vi: se ts=4 sw=4 et: */
