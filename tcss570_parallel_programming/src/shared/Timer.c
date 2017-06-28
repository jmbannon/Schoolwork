#include "Timer.h"
#include <time.h>

void
Timer_start(Timer *timer)
{
    timer->start = clock();
}

void
Timer_end(Timer *timer)
{
    timer->end = clock();
}

double
Timer_dur_sec(Timer *timer)
{
    return (double)(timer->end - timer->start) / (double)CLOCKS_PER_SEC;
}
