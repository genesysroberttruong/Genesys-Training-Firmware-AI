/*
 * scheduler.h - Tiny cooperative, tick-driven task scheduler.
 *
 * A SysTick interrupt increments a millisecond-ish tick counter. Tasks are
 * registered with a period (in ticks); scheduler_run(), called from the main
 * super-loop, invokes each task when its period has elapsed. Tasks must run
 * to completion and never block.
 */
#ifndef CORE_SCHEDULER_H
#define CORE_SCHEDULER_H

#include <stdint.h>

typedef void (*task_fn)(void);

void     scheduler_init(void);                       /* start the SysTick tick */
void     scheduler_add(task_fn fn, uint32_t period); /* register a periodic task */
void     scheduler_run(void);                        /* dispatch due tasks (call in loop) */
uint32_t scheduler_ticks(void);                      /* ticks since boot */

#endif /* CORE_SCHEDULER_H */
