#include "dbg.h"
#include <unistd.h>
#include <stdio.h>
#include "logging.h"

static int sleep_secs = -1;

/**
 *  set sleep seconds for debugger 
 */
void
dbg_set_sleep(
    int secs)
{
    sleep_secs = secs;
}



/**
 * sleep specified secs  
 */
void
dbg_sleep()
{
    if (sleep_secs > 0) {
        fprintf(stderr, "sleep start : %d\n", sleep_secs);
        sleep(sleep_secs);
        fputs("wake up\n", stderr);
    }
}

/**
 * simple trace 
 */
void
dbg_trace(
    const char* file,
    int line)
{
    fprintf(stderr, "%s: %d\n", file, line);
}

void
dbg_trace_log_fs_hdl()
{
    logging_log(LOG_LEVEL_DEBUG, "fprintf: %p", fprintf);
    logging_log(LOG_LEVEL_DEBUG, "stderr: %p", stderr);
}



/* vi: se ts=4 sw=4 et: */
