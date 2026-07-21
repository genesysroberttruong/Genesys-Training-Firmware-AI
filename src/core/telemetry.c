/*
 * telemetry.c - Samples the sensor once per period, folds the value into the
 * running statistics, and prints a one-line telemetry frame to the console.
 *
 * Frame format:
 *   [t=<ticks>] raw=<v> min=<v> max=<v> mean=<v> n=<count>
 * where each value is degrees Celsius with two decimal places.
 */
#include "telemetry.h"
#include "sensor.h"
#include "stats.h"
#include "scheduler.h"
#include "../log.h"

#include <stdlib.h>   /* abs() */

static stats_t s_stats;
static int     s_inited;

void telemetry_reset(void)
{
    stats_reset(&s_stats);
    s_inited = 1;
}

/* Split a centi-degree value into whole and fractional parts for printing. */
static int whole(int16_t v)  { return v / 100; }
static int frac(int16_t v)   { return abs(v % 100); }

void telemetry_task(void)
{
    if (!s_inited) {
        telemetry_reset();
    }

    int16_t v = sensor_read();
    stats_update(&s_stats, v);

    int16_t mn = s_stats.min;
    int16_t mx = s_stats.max;
    int16_t mean = stats_mean(&s_stats);

    log_printf("[t=%lu] raw=%d.%02d min=%d.%02d max=%d.%02d mean=%d.%02d n=%lu\n",
               (unsigned long)scheduler_ticks(),
               whole(v),  frac(v),
               whole(mn), frac(mn),
               whole(mx), frac(mx),
               whole(mean), frac(mean),
               (unsigned long)s_stats.count);
}
