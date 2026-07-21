/*
 * lm3s6965.h - Minimal register definitions for the TI Stellaris LM3S6965
 *              as emulated by QEMU's "lm3s6965evb" machine (Cortex-M3).
 *
 * We only define the handful of peripherals this demo actually touches:
 *   - UART0  (serial console, routed to stdio by "qemu ... -nographic")
 *   - SysTick (the ARMv7-M system timer, our scheduler tick source)
 */
#ifndef LM3S6965_H
#define LM3S6965_H

#include <stdint.h>

#define __IO volatile

/* ------------------------------------------------------------------ */
/* UART0 (PL011-style Stellaris UART)                                  */
/* ------------------------------------------------------------------ */
#define UART0_BASE      0x4000C000u
#define UART0_DR        (*(__IO uint32_t *)(UART0_BASE + 0x000)) /* data      */
#define UART0_FR        (*(__IO uint32_t *)(UART0_BASE + 0x018)) /* flags     */

#define UART_FR_RXFE    (1u << 4)   /* receive  FIFO empty */
#define UART_FR_TXFF    (1u << 5)   /* transmit FIFO full  */

/* ------------------------------------------------------------------ */
/* SysTick (Cortex-M3 / ARMv7-M system timer, at 0xE000E010)           */
/* ------------------------------------------------------------------ */
#define SYST_CSR        (*(__IO uint32_t *)0xE000E010u) /* control/status */
#define SYST_RVR        (*(__IO uint32_t *)0xE000E014u) /* reload value   */
#define SYST_CVR        (*(__IO uint32_t *)0xE000E018u) /* current value  */

#define SYST_CSR_ENABLE     (1u << 0)   /* counter enable          */
#define SYST_CSR_TICKINT    (1u << 1)   /* interrupt on count to 0 */
#define SYST_CSR_CLKSOURCE  (1u << 2)   /* use processor clock     */

#endif /* LM3S6965_H */
