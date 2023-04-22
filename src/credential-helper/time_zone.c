#include "time_zone.h"
#include <time.h>

void
time_zone_set()
{
    tzset();
}

/* vi: se ts=4 sw=4 et: */
