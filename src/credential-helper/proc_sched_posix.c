#include "proc_sched.h"
#include <sched.h>

/**
 * yield to another threads
 */
void
proc_sched_yield()
{
    sched_yield();
}

/* vi: se ts=4 sw=4 et: */
