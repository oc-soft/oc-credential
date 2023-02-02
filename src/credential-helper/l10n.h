#ifndef __L10N_H__
#define __L10N_H__

#ifdef __cplusplus
#define _L10N_BEGIN extern "C" {
#define _L10N_END }
#else
#define _L10N_BEGIN 
#define _L10N_END 
#endif

_L10N_BEGIN 

/**
 * get locale directory
 */
char*
l10n_get_locale_dir();


/**
 * free heap memory allocated by this api.
 */
void
l10n_free(
    void* obj);


_L10N_END 
/* vi: se ts=4 sw=4 et: */
#endif
