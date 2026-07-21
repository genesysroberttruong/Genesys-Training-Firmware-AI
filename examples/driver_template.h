/*
 * driver_template.h - COPY ME.
 *
 * The canonical shape of a memory-mapped peripheral driver for this project.
 * To use: copy this pair, rename the `dt_`/`DT_` prefixes to your peripheral,
 * fill the register offsets and bit masks from docs/datasheets/, and move the
 * files into src/hal/. This file is NOT part of the firmware build.
 *
 * Point the AI at it:  "Write a driver for <part> following
 *                       examples/driver_template.c."
 */
#ifndef DRIVER_TEMPLATE_H
#define DRIVER_TEMPLATE_H

#include <stdint.h>

/* Explicit, non-throwing status codes - drivers report, they don't abort. */
typedef enum {
    DRV_OK          =  0,
    DRV_ERR_TIMEOUT = -1,
    DRV_ERR_PARAM   = -2,
} drv_status_t;

void         dt_init(void);
drv_status_t dt_write_reg(uint8_t addr, uint32_t value);
drv_status_t dt_read_reg(uint8_t addr, uint32_t *out);

#endif /* DRIVER_TEMPLATE_H */
