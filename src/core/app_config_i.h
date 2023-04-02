#ifndef __APP_CONFIG_I_H__
#define __APP_CONFIG_I_H__

#include <stddef.h>
#ifdef __cplusplus
#define _APP_CONFIG_I_ITFC_BEGIN extern "C" {
#define _APP_CONFIG_I_ITFC_END }
#else
#define _APP_CONFIG_I_ITFC_BEGIN 
#define _APP_CONFIG_I_ITFC_END
#endif

_APP_CONFIG_I_ITFC_BEGIN 

/**
 * get app configuration raw data
 */
void
app_config_i_get_raw_data(
    const char** raw_data,
    size_t* length);

_APP_CONFIG_I_ITFC_END
/* vi: se ts=4 sw=4 et: */

#endif

