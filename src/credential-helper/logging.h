#ifndef __LOGGING_H__
#define __LOGGING_H__

#ifdef __cplusplus
#define _LOGGING_ITFC_BEGIN extern "C" {
#define _LOGGING_ITFC_END }
#else
#define _LOGGING_ITFC_BEGIN 
#define _LOGGING_ITFC_END 
#endif

_LOGGING_ITFC_BEGIN 

#define	LOG_LEVEL_EMERG     0
#define	LOG_LEVEL_ALERT     1
#define	LOG_LEVEL_CRIT      2
#define	LOG_LEVEL_ERR       3
#define	LOG_LEVEL_WARNING   4
#define	LOG_LEVEL_NOTICE    5
#define	LOG_LEVEL_INFO      6
#define	LOG_LEVEL_DEBUG     7


/**
 * start logging
 */
void
logging_start();

/**
 * log
 */
void
logging_log(
    int priority,
    const char* format,
    ...);

/**
 * stop logging
 */
void
logging_stop();

_LOGGING_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif
