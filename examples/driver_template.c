/*
 * driver_template.c - Reference implementation of a memory-mapped peripheral
 * driver. See driver_template.h. Illustrative only; not built into the
 * firmware (the placeholder register map below is not a real peripheral).
 */
#include "driver_template.h"

#include <stddef.h>   /* NULL */

/* --- Register map --------------------------------------------------------
 * Fill these from the datasheet (docs/datasheets/). Keep the register NAMES
 * identical to the manual so your code and the datasheet line up during
 * review. The values below are placeholders. */
#define DT_BASE          0x40010000u          /* TODO: real base address */
#define DT_REG(off)      (*(volatile uint32_t *)(DT_BASE + (off)))

#define DT_CTRL          DT_REG(0x000)         /* control register        */
#define DT_STATUS        DT_REG(0x004)         /* status register         */
#define DT_DATA          DT_REG(0x008)         /* data register           */

#define DT_CTRL_ENABLE   (1u << 0)
#define DT_STATUS_READY  (1u << 0)

/* Always bound hardware waits so a stuck peripheral cannot hang the firmware. */
static drv_status_t wait_ready(void)
{
    for (uint32_t spins = 0; spins < 100000u; spins++) {
        if (DT_STATUS & DT_STATUS_READY) {
            return DRV_OK;
        }
    }
    return DRV_ERR_TIMEOUT;
}

void dt_init(void)
{
    /* On real silicon: enable the peripheral clock first (see the RCGC
     * registers in the datasheet), then configure and enable the device. */
    DT_CTRL = DT_CTRL_ENABLE;
}

drv_status_t dt_write_reg(uint8_t addr, uint32_t value)
{
    drv_status_t st = wait_ready();
    if (st != DRV_OK) {
        return st;
    }
    DT_DATA = value;
    DT_CTRL = DT_CTRL_ENABLE | ((uint32_t)addr << 8);
    return DRV_OK;
}

drv_status_t dt_read_reg(uint8_t addr, uint32_t *out)
{
    if (out == NULL) {
        return DRV_ERR_PARAM;
    }
    drv_status_t st = wait_ready();
    if (st != DRV_OK) {
        return st;
    }
    DT_CTRL = ((uint32_t)addr << 8);
    *out = DT_DATA;
    return DRV_OK;
}
