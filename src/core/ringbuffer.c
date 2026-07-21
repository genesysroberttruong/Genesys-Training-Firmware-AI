/*
 * ringbuffer.c - Power-of-two byte FIFO implementation.
 */
#include "ringbuffer.h"

#define RB_MASK (RB_SIZE - 1u)

void rb_init(ringbuffer_t *rb)
{
    rb->head = 0;
    rb->tail = 0;
}

uint16_t rb_count(const ringbuffer_t *rb)
{
    return (uint16_t)((rb->head - rb->tail) & RB_MASK);
}

int rb_put(ringbuffer_t *rb, uint8_t byte)
{
    uint16_t next = (uint16_t)((rb->head + 1u) & RB_MASK);
    if (next == rb->tail) {
        return -1;              /* buffer full - drop the byte */
    }
    rb->buf[rb->head] = byte;
    rb->head = next;
    return 0;
}

int rb_get(ringbuffer_t *rb, uint8_t *out)
{
    if (rb->head == rb->tail) {
        return -1;              /* buffer empty */
    }
    *out = rb->buf[rb->tail];
    rb->tail = (uint16_t)((rb->tail + 1u) & RB_MASK);
    return 0;
}
