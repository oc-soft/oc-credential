#include "config.h"
#include <syslog.h>
#include <stdarg.h>
#include <stddef.h>
#include "exe_info.h"

/**
 * the messaige is written into log file if input priority is greater than or
 * equals level.
 */
static int
logging_level = DEFAULT_LOGGING_LEVEL;

static char*
logging_id;
/**
 * start logging
 */
void
logging_start()
{
    if (!logging_id) {
        logging_id = exe_info_get_exe_name();
        openlog(logging_id, 0, LOG_USER);
    }
}

/**
 * log
 */
void
logging_log(
    int priority,
    const char* format,
    ...)
{
    if (priority >= logging_level) {
        va_list v_arg;
        const static int levels[] = {
            LOG_EMERG,
            LOG_ALERT,
            LOG_CRIT,
            LOG_ERR,
            LOG_WARNING,
            LOG_NOTICE,
            LOG_INFO,
            LOG_DEBUG
        };
        va_start(v_arg, format);
        if (priority >= sizeof(levels) / sizeof(levels[0])) {
            priority = sizeof(levels) / sizeof(levels[0]) - 1; 
        } else if (priority < 0) {
            priority = 0;
        }
        
        vsyslog(levels[priority], format, v_arg);

        va_end(v_arg);
    }
}

/**
 * set logging level
 */
void
logging_set_level(
    int priority)
{
    logging_level = priority;
}

/**
 * stop logging
 */
void
logging_stop()
{
    closelog();
    if (logging_id) {
        exe_info_free(logging_id);
        logging_id = NULL;
    }
}


/* vi: se ts=4 sw=4 et: */
