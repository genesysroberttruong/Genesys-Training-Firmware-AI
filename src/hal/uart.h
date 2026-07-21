/*
 * uart.h - Polled UART0 driver for the console.
 */
#ifndef HAL_UART_H
#define HAL_UART_H

#include <stdint.h>

void uart_init(void);                          /* prepare the console UART   */
void uart_putc(char c);                         /* blocking single-byte write */
void uart_write(const char *s, uint32_t len);   /* write len bytes            */
void uart_puts(const char *s);                  /* write a NUL-terminated str */
int  uart_getc(void);                           /* read a byte, or -1 if none */

#endif /* HAL_UART_H */
