/*
 * main.c - Firmware entry point.
 *
 * Brings up the console, seeds the simulated sensor, then hands control to a
 * cooperative scheduler running two periodic tasks:
 *   - telemetry_task: sample the sensor and print a frame
 *   - cmd_task      : service console commands
 */
#include "hal/uart.h"
#include "core/scheduler.h"
#include "core/sensor.h"
#include "core/telemetry.h"
#include "core/cmd.h"
#include "log.h"

int main(void)
{
    uart_init();
    sensor_init();

    log_printf("\n");
    log_printf("=== Genesys Firmware Demo  (LM3S6965 / Cortex-M3 / QEMU) ===\n");
    log_printf("modules: scheduler + sensor + stats + telemetry + cmd\n");
    log_printf("type 'help' + Enter for console commands\n\n");

    scheduler_init();
    scheduler_add(telemetry_task, 25);   /* report every 25 ticks */
    scheduler_add(cmd_task,        5);    /* poll console every 5 ticks */

    for (;;) {
        scheduler_run();
    }

    return 0;
}
