#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include <json-c/json_object.h>
#ifdef __cplusplus
#define _APP_CONFIG_ITFC_BEGIN extern "C" {
#define _APP_CONFIG_ITFC_END }
#else
#define _APP_CONFIG_ITFC_BEGIN 
#define _APP_CONFIG_ITFC_END
#endif

_APP_CONFIG_ITFC_BEGIN 

/**
 * get app config config
 */
json_object*
app_config_get();

/**
 * initialize runtime configuration.
 */
int
app_config_start();


/**
 * stop runtime configuration.
 */
int
app_config_stop();



_APP_CONFIG_ITFC_END
/* vi: se ts=4 sw=4 et: */

#endif
