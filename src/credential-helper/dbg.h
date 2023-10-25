#ifndef __DBG_H__
#define __DBG_H__

#ifdef __cplusplus
#define _DBG_ITFC_BEGIN extern "C" {
#define _DBG_ITFC_END }
#else
#define _DBG_ITFC_BEGIN 
#define _DBG_ITFC_END 
#endif

_DBG_ITFC_BEGIN 

#define DBG_TRACE() dbg_trace(__FILE__, __LINE__)


/**
 *  set sleep seconds for debugger 
 */
void
dbg_set_sleep(
    int sleep_secs);


/**
 * sleep specified secs  
 */
void
dbg_sleep();


/**
 * simple trace 
 */
void
dbg_trace(
    const char* file,
    int line);


void
dbg_trace_log_fs_hdl();


_DBG_ITFC_END 

/* vi: se ts=4 sw=4 et: */
#endif

