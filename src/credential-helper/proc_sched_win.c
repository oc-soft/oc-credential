#include "proc_sched.h"
#include <windows.h>

/**
 * yield to another threads
 */
void
proc_sched_yield()
{
    SwitchToThread();
}

/* vi: se ts=4 sw=4 et: */
