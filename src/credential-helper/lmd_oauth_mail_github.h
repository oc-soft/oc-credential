#ifndef __LMD_OAUTH_MAIL_GITHUB_H__
#define __LMD_OAUTH_MAIL_GITHUB_H__

#include "lmd.h"

#ifdef __cplusplus
#define _LMD_OAUTH_MAIL_GITHUB_ITFC_BEGIN extern "C" {
#define _LMD_OAUTH_MAIL_GITHUB_ITFC_END }
#else
#define _LMD_OAUTH_MAIL_GITHUB_ITFC_BEGIN 
#define _LMD_OAUTH_MAIL_GITHUB_ITFC_END 
#endif

_LMD_OAUTH_MAIL_GITHUB_ITFC_BEGIN 

/**
 * bind oauth token and email
 */
int
lmd_oauth_mail_github_bind(
    lmd* obj); 

_LMD_OAUTH_MAIL_GITHUB_ITFC_END
/* vi: se ts=4 sw=2 et: */
#endif

