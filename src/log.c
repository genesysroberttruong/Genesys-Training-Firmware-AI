/*
 * log.c - Formats messages with vsnprintf and pushes them to the UART.
 *
 * Using vsnprintf keeps us off the stdio FILE machinery: we format into a
 * fixed buffer and hand the bytes to the UART driver ourselves.
 */
#include "log.h"
#include "hal/uart.h"

#include <stdarg.h>
#include <stdio.h>

static char s_buf[160];

void log_printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(s_buf, sizeof(s_buf), fmt, ap);
    va_end(ap);

    if (n < 0) {
        return;
    }
    if ((unsigned)n > sizeof(s_buf) - 1u) {
        n = (int)(sizeof(s_buf) - 1u);  /* output was truncated */
    }
    uart_write(s_buf, (uint32_t)n);
}
