/*
 * telemetry.h - Periodic task that samples the sensor and reports a frame.
 */
#ifndef CORE_TELEMETRY_H
#define CORE_TELEMETRY_H

void telemetry_task(void);    /* registered with the scheduler */
void telemetry_reset(void);   /* clear the running statistics  */

#endif /* CORE_TELEMETRY_H */
