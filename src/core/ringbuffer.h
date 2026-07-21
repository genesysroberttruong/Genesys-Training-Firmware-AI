/*
 * ringbuffer.h - Fixed-size single-producer/single-consumer byte FIFO.
 *
 * Capacity is RB_SIZE and MUST be a power of two so the index wrap can use a
 * cheap bit-mask instead of a modulo. Used here to buffer received console
 * bytes between polls of the command task.
 */
#ifndef CORE_RINGBUFFER_H
#define CORE_RINGBUFFER_H

#include <stdint.h>

#define RB_SIZE 64u   /* must be a power of two */

typedef struct {
    uint8_t  buf[RB_SIZE];
    volatile uint16_t head;   /* write index */
    volatile uint16_t tail;   /* read  index */
} ringbuffer_t;

void     rb_init(ringbuffer_t *rb);
int      rb_put(ringbuffer_t *rb, uint8_t byte);   /* 0 = ok, -1 = full  */
int      rb_get(ringbuffer_t *rb, uint8_t *out);   /* 0 = ok, -1 = empty */
uint16_t rb_count(const ringbuffer_t *rb);

#endif /* CORE_RINGBUFFER_H */
