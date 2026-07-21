/*
 * stats.h - Running min / max / mean over a stream of samples.
 */
#ifndef CORE_STATS_H
#define CORE_STATS_H

#include <stdint.h>

typedef struct {
    int16_t  min;
    int16_t  max;
    int32_t  sum;
    uint32_t count;
} stats_t;

void    stats_reset(stats_t *s);
void    stats_update(stats_t *s, int16_t value);
int16_t stats_mean(const stats_t *s);

#endif /* CORE_STATS_H */
