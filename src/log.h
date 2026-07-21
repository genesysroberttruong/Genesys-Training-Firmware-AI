/*
 * log.h - printf-style formatted logging over the UART console.
 */
#ifndef LOG_H
#define LOG_H

/* Formats into a small static buffer, then writes it to UART0.
 * Supports the usual integer/string conversions (%d %u %x %s %c %lu ...).
 * Avoid %f - floating point is intentionally not linked in this build. */
void log_printf(const char *fmt, ...);

#endif /* LOG_H */
