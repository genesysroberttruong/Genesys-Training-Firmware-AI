/*
 * cmd.c - Console command handler.
 *
 * Data path:  UART RX  ->  ring buffer  ->  line assembly  ->  dispatch
 *
 * The ring buffer decouples byte arrival from line processing, mirroring how
 * a real interrupt-driven UART driver would hand bytes to the application.
 */
#include "cmd.h"
#include "ringbuffer.h"
#include "telemetry.h"
#include "../hal/uart.h"
#include "../log.h"

#include <string.h>

static ringbuffer_t s_rx;
static char         s_line[64];
static uint8_t      s_len;
static int          s_inited;

static void handle_line(const char *line)
{
    if (strcmp(line, "help") == 0) {
        log_printf("commands: help, reset, stats\n");
    } else if (strcmp(line, "reset") == 0) {
        telemetry_reset();
        log_printf("statistics reset\n");
    } else if (strcmp(line, "stats") == 0) {
        log_printf("(statistics are printed on every telemetry frame)\n");
    } else {
        log_printf("unknown command: '%s' (try 'help')\n", line);
    }
}

void cmd_task(void)
{
    if (!s_inited) {
        rb_init(&s_rx);
        s_len    = 0;
        s_inited = 1;
    }

    /* Drain whatever the UART has received into the ring buffer. */
    int c;
    while ((c = uart_getc()) >= 0) {
        (void)rb_put(&s_rx, (uint8_t)c);
    }

    /* Assemble complete lines and dispatch them. */
    uint8_t b;
    while (rb_get(&s_rx, &b) == 0) {
        if (b == '\r' || b == '\n') {
            s_line[s_len] = '\0';
            if (s_len > 0) {
                handle_line(s_line);
            }
            s_len = 0;
        } else if (s_len < sizeof(s_line) - 1u) {
            s_line[s_len++] = (char)b;
        }
    }
}
