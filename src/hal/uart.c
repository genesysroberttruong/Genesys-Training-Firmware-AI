/*
 * uart.c - Polled UART0 driver.
 *
 * Under QEMU's lm3s6965evb machine the UART needs no clock/baud setup: with
 * "-nographic" QEMU wires UART0 straight to the terminal's stdin/stdout.
 * On real silicon you would enable the peripheral clock and program the
 * baud-rate divisors here first.
 */
#include "uart.h"
#include "lm3s6965.h"

void uart_init(void)
{
    /* Nothing to configure on the QEMU model. */
}

void uart_putc(char c)
{
    while (UART0_FR & UART_FR_TXFF) {
        /* wait until the transmit FIFO has room */
    }
    UART0_DR = (uint32_t)(unsigned char)c;
}

void uart_write(const char *s, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        if (s[i] == '\n') {
            uart_putc('\r');    /* translate LF -> CRLF for terminals */
        }
        uart_putc(s[i]);
    }
}

void uart_puts(const char *s)
{
    while (*s) {
        if (*s == '\n') {
            uart_putc('\r');
        }
        uart_putc(*s++);
    }
}

int uart_getc(void)
{
    if (UART0_FR & UART_FR_RXFE) {
        return -1;              /* receive FIFO empty */
    }
    return (int)(UART0_DR & 0xFFu);
}
