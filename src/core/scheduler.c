/*
 * scheduler.c - Cooperative scheduler + SysTick interrupt handler.
 */
#include "scheduler.h"
#include "lm3s6965.h"

#define MAX_TASKS 8

typedef struct {
    task_fn  fn;
    uint32_t period;   /* ticks between runs        */
    uint32_t next;     /* tick at which to run next */
} task_t;

static task_t          s_tasks[MAX_TASKS];
static uint32_t        s_ntasks;
static volatile uint32_t s_ticks;   /* updated in interrupt context */

/*
 * SysTick exception handler. Runs every time the SysTick counter reaches 0.
 * Kept deliberately tiny: it only bumps the tick counter.
 */
void SysTick_Handler(void)
{
    s_ticks++;
}

uint32_t scheduler_ticks(void)
{
    return s_ticks;
}

void scheduler_init(void)
{
    s_ntasks = 0;

    /* Configure SysTick: reload value sets the tick period. The exact rate
     * under QEMU is not important here - tasks only care about relative
     * ticks, not wall-clock time. */
    SYST_RVR = 50000u - 1u;
    SYST_CVR = 0u;
    SYST_CSR = SYST_CSR_CLKSOURCE | SYST_CSR_TICKINT | SYST_CSR_ENABLE;
}

void scheduler_add(task_fn fn, uint32_t period)
{
    if (s_ntasks < MAX_TASKS) {
        s_tasks[s_ntasks].fn     = fn;
        s_tasks[s_ntasks].period = period;
        s_tasks[s_ntasks].next   = period;
        s_ntasks++;
    }
}

void scheduler_run(void)
{
    uint32_t now = s_ticks;

    for (uint32_t i = 0; i < s_ntasks; i++) {
        /* Signed comparison so the test survives counter wrap-around. */
        if ((int32_t)(now - s_tasks[i].next) >= 0) {
            s_tasks[i].next += s_tasks[i].period;
            s_tasks[i].fn();
        }
    }
}
