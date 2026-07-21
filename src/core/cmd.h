/*
 * cmd.h - Simple line-based console command handler.
 *
 * Polls the UART for received bytes, buffers them through a ring buffer, and
 * on each completed line dispatches a command. Supported: help, reset, stats.
 */
#ifndef CORE_CMD_H
#define CORE_CMD_H

void cmd_task(void);   /* registered with the scheduler */

#endif /* CORE_CMD_H */
