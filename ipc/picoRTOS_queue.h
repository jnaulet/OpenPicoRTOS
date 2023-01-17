#ifndef PICORTOS_QUEUE_H
#define PICORTOS_QUEUE_H

#include <errno.h>
#include "picoRTOS.h"
#include "picoRTOS_futex.h"

/*
 * Don' use directly, use PICORTOS_QUEUE(type, count)
 */
struct picoRTOS_queue_head {
    picoRTOS_futex_t lock;
    size_t mask;
    volatile size_t r;
    volatile size_t w;
    volatile size_t i;
};

void picoRTOS_queue_head_init(/*@out@*/ struct picoRTOS_queue_head *ctx, size_t count);
int picoRTOS_queue_head_pop(struct picoRTOS_queue_head *ctx);
int picoRTOS_queue_head_push(struct picoRTOS_queue_head *ctx);

/* Macro: PICORTOS_QUEUE(type, count)
 * Declares a queue (FIFO)
 *
 * Queues are thread-safe
 *
 * Parameters:
 *  type - The type of data used in this queue (char, int, uint32_t, etc)
 *  count - The size of the queue in number of elements
 *
 * Example:
 * (start code)
 * static PICORTOS_QUEUE(int, 32) queue;
 * (end)
 *
 * Remarks:
 * count MUST be a power of 2 or picoRTOS will throw a debug exception
 */
#define PICORTOS_QUEUE(type, count)                     \
    struct {                                              \
        struct picoRTOS_queue_head head;                    \
        type buf[count];                                    \
    }

/* Macro: PICORTOS_QUEUE_INIT(queue)
 * Initializes a queue
 *
 * Parameters:
 *  queue - A pointer to a previously declared queue
 */
#define PICORTOS_QUEUE_INIT(queue)                                      \
    { picoRTOS_queue_head_init(&(queue)->head, sizeof((queue)->buf) / sizeof(*(queue)->buf)); \
      (queue)->buf[0] = 0; }

/* Macro: PICORTOS_QUEUE_READ(queue, rvalue)
 * Reads the first element of a queue
 *
 * Returns:
 * 0 if success, -EAGAIN if the queue is empty
 *
 * Parameters:
 *  queue - A pointer to a queue
 *  rvalue - A pointer to the (hopefully) returned value
 */
#define PICORTOS_QUEUE_READ(queue, rvalue)                      \
    ((picoRTOS_queue_head_pop(&(queue)->head) != -EAGAIN) ?       \
     (*(rvalue) = (queue)->buf[(queue)->head.i], 0) : -EAGAIN)

/* Macro: PICORTOS_QUEUE_WRITE(queue, value)
 * Writes a value at the end of a queue
 *
 * Returns:
 * 0 if write succeeded,  -EAGAIN if queue is full
 *
 * Parameters:
 *  queue - A pointer to a queue
 *  value - The value to add to the queue
 *
 * Remarks:
 * Queues elements are read using a pointer but written by direct copy
 */
#define PICORTOS_QUEUE_WRITE(queue, value)                           \
    ((picoRTOS_queue_head_push(&(queue)->head) != -EAGAIN) ?           \
     ((queue)->buf[(queue)->head.i] = value, 0) : -EAGAIN)

#endif
