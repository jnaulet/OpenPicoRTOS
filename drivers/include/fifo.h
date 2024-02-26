#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>
#include <stddef.h>

/*
 * useful on small archs
 */
struct fifo_head8 {
    uint8_t m;
    volatile uint8_t r;
    volatile uint8_t w;
};

struct fifo_head {
    size_t m;
    volatile size_t r;
    volatile size_t w;
};

#define fifo_head_init(ctx, mask)                         \
    { picoRTOS_assert_void(((mask) & ((mask) + 1)) == 0);   \
      (ctx)->m = (mask);                                    \
      (ctx)->r = (mask);                                    \
      (ctx)->w = 0; }

#define fifo_head_is_writable(ctx)              \
    ((((ctx)->w + 1) & (ctx)->m) != (ctx)->r)
#define fifo_head_push(ctx)                     \
    { (ctx)->w = ((ctx)->w + 1) & (ctx)->m; }
#define fifo_head_is_readable(ctx)              \
    ((((ctx)->r + 1) & (ctx)->m) != (ctx)->w)
#define fifo_head_pop(ctx)                      \
    { (ctx)->r = ((ctx)->r + 1) & (ctx)->m; }

#endif
