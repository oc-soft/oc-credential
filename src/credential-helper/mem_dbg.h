#ifndef __MEM_DBG_H__
#define __MEM_DBG_H__

#ifdef __cplusplus
#define _MEM_DBG_ITFC_BEGIN extern "C" {
#define _MEM_DBG_ITFC_END }
#else
#define _MEM_DBG_ITFC_BEGIN 
#define _MEM_DBG_ITFC_END 
#endif

_MEM_DBG_ITFC_BEGIN 


/**
 * start memory trace
 */
void
mem_dbg_start_trace();

/**
 * stop memory trace
 */
void
mem_dbg_stop_trace();

_MEM_DBG_ITFC_END 

/* vi: se ts=4 sw=4: */
#endif
