#include <time.h>

#ifndef TIMER_H_
#define TIMER_H_

typedef struct _Timer {
    struct timespec start;
    struct timespec end;
} Timer;

void
Timer_start(Timer *timer);

void
Timer_end(Timer *timer);

double
Timer_dur_sec(Timer *timer);

double
Timer_dur_ms(Timer *timer);

#endif
