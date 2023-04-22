#ifndef __TIME_ZONE_H__
#define __TIME_ZONE_H__

#ifdef __cplusplus
#define _TIME_ZONE_ITFC_BEGIN extern "C" {
#define _TIME_ZONE_ITFC_END }
#else
#define _TIME_ZONE_ITFC_BEGIN 
#define _TIME_ZONE_ITFC_END 
#endif

_TIME_ZONE_ITFC_BEGIN 


/**
 * set time zone 
 */
void
time_zone_set();

_TIME_ZONE_ITFC_END
/* vi: se ts=4 sw=4 et: */
#endif
