#ifndef __PROC_SCHED_H__
#define __PROC_SCHED_H__

#include <unistd.h>
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
#define _PROC_SCHED_ITFC_BEGIN extern "C" {
#define _PROC_SCHED_ITFC_END }
#else
#define _PROC_SCHED_ITFC_BEGIN 
#define _PROC_SCHED_ITFC_END 
#endif

_PROC_SCHED_ITFC_BEGIN 


/**
 * yield to another threads
 */
void
proc_sched_yield();

_PROC_SCHED_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif
