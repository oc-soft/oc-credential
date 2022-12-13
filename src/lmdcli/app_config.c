#include "app_config.h"
#include <json-c/json_tokener.h>

/**
 * load runtime configuration object.
 */
static json_object*
app_config_load();


/**
 * application configuration
 */
static json_object*
app_config;

/**
 * get rc config
 */
json_object*
app_config_get()
{
    json_object* result;
    result = NULL;
    if (app_config) {
        json_object_get(app_config);
        result = app_config;
    }
    return result;
}


/**
 * load runtime configuration object.
 */
json_object*
app_config_load()
{
    const char* app_config;
    size_t length;
    struct json_tokener* tokener;
    json_object* result;
    result = NULL;
    app_config = NULL;
    length = 0;
    app_config_i_get_raw_data(&app_config, &length);
    tokener = json_tokener_new(); 
    if (tokener) {
        result = json_tokener_parse_ex(tokener, app_config, length);
    }
    if (tokener) {
        json_tokener_free(tokener);
    }
    return result;
}

/**
 * initialize runtime configuration.
 */
int
app_config_start()
{
    int result;
    result = 0;

    app_config = app_config_load();
    if (!app_config) {
        result = -1;
    }

    return result;
}


/**
 * stop runtime configuration.
 */
int
app_config_stop()
{
    int result;
    result = 0;
    if (app_config) {
        json_object_put(app_config);
        app_config = NULL; 
    }
    return result;
}

/* vi: se ts=4 sw=4 et: */
