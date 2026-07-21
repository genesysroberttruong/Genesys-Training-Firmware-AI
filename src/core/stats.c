/*
 * stats.c - Running min / max / mean accumulator.
 */
#include "stats.h"

#include <limits.h>

void stats_reset(stats_t *s)
{
    s->min   = 0;
    s->max   = INT16_MIN;
    s->sum   = 0;
    s->count = 0;
}

void stats_update(stats_t *s, int16_t value)
{
    if (value < s->min) {
        s->min = value;
    }
    if (value > s->max) {
        s->max = value;
    }
    s->sum += value;
    s->count++;
}

int16_t stats_mean(const stats_t *s)
{
    if (s->count == 0u) {
        return 0;
    }
    return (int16_t)(s->sum / (int32_t)s->count);
}
