/*
 * sensor.h - Simulated temperature sensor.
 *
 * There is no real sensor on the QEMU machine, so we synthesise a repeatable
 * signal: a slow sine sweep plus a little deterministic noise. Values are in
 * hundredths of a degree Celsius (e.g. 2537 == 25.37 C).
 */
#ifndef CORE_SENSOR_H
#define CORE_SENSOR_H

#include <stdint.h>

void    sensor_init(void);
int16_t sensor_read(void);   /* temperature in 0.01 C units */

#endif /* CORE_SENSOR_H */
