#include "Timer.h"
#include <time.h>

void
Timer_start(Timer *timer)
{
    clock_gettime(CLOCK_MONOTONIC, &timer->start);
}

void
Timer_end(Timer *timer)
{
    clock_gettime(CLOCK_MONOTONIC, &timer->end);
}

double
Timer_dur_ms(Timer *timer)
{
    return ((timer->end.tv_nsec - timer->start.tv_nsec) / 1e6) + ((timer->end.tv_sec - timer->start.tv_sec) * 1e3);
}

double
Timer_dur_sec(Timer *timer)
{
    return Timer_dur_ms(timer) / 1e3;
}
