#ifndef PICORTOS_QUEUE_H
#define PICORTOS_QUEUE_H

#include <errno.h>
#include "picoRTOS.h"
#include "picoRTOS_futex.h"

/*
 * Don't use directly, use PICORTOS_QUEUE(type, count)
 */
struct picoRTOS_queue_head {
    picoRTOS_futex_t lock;
    size_t mask;
    volatile size_t r;
    volatile size_t w;
    volatile size_t i;
} __attribute__((aligned(ARCH_L1_DCACHE_LINESIZE)));

void picoRTOS_queue_head_init(/*@out@*/ struct picoRTOS_queue_head *ctx, size_t count);
int picoRTOS_queue_head_pop(struct picoRTOS_queue_head *ctx);
int picoRTOS_queue_head_push(struct picoRTOS_queue_head *ctx);

/**
 * **PICORTOS_QUEUE**(<ins>type</ins>, <ins>count</ins>);
 * > Declares a queue
 * ### NOTES
 * > Queues are thread safe FIFOs to exchange data between 2 or more
 * > tasks
 * >
 * > <ins>Beware:</ins> <ins>count</ins> MUST be a power of two or the calling
 * > task will be killed with a `EINVAL` error code. Example:
 * ```c
 *     static PICORTOS_QUEUE(int, 32) queue;
 * ```
 * > On memory-protected systems you almost might want to put your queues
 * > in the `UNPRIVILEGED_DATA` area, but most of the time you want to
 * > use **picoRTOS_mpu_add_region()** to share it between only 2 tasks
 */
#define PICORTOS_QUEUE(type, count)                     \
    struct {                                              \
        struct picoRTOS_queue_head head;                    \
        type buf[count];                                    \
    }

/**
 * **PICORTOS_QUEUE_INIT**(<ins>queue</ins>);
 * > Dynamically initializes a <ins>queue</ins>.
 * ### NOTES
 * > The queue has to be declared first.
 */
#define PICORTOS_QUEUE_INIT(queue)                                      \
    { picoRTOS_queue_head_init(&(queue)->head, sizeof((queue)->buf) / sizeof(*(queue)->buf)); \
      (queue)->buf[0] = 0; }

/**
 * **PICORTOS_QUEUE_READ**(<ins>queue</ins>, <ins>rvalue</ins>);
 * > Reads the first element of a queue and put it into *<ins>rvalue</ins>
 * ### RETURN
 * > Returns 0 on successful read, -`EAGAIN` if the queue is empty
 */
#define PICORTOS_QUEUE_READ(queue, rvalue)                      \
    ((picoRTOS_queue_head_pop(&(queue)->head) != -EAGAIN) ?       \
     (*(rvalue) = (queue)->buf[(queue)->head.i], 0) : -EAGAIN)

/**
 * **PICORTOS_QUEUE_WRITE**(<ins>queue</ins>, <ins>value</ins>);
 * > Writes a <ins>value</ins> at the end of a queue
 * ### RETURN
 * > Returns 0 in successful write, -`EAGAIN` if the queue is full
 * ### NOTES
 * > Queues elements are read using a pointer but written by direct
 * > copy
 */
#define PICORTOS_QUEUE_WRITE(queue, value)                           \
    ((picoRTOS_queue_head_push(&(queue)->head) != -EAGAIN) ?           \
     ((queue)->buf[(queue)->head.i] = value, 0) : -EAGAIN)

#endif
