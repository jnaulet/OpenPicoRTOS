#include "picoRTOS_queue.h"
#include "picoRTOS_port.h"

#define IS_POW2(x) (((x) & ((x) - 1)) == 0)

void picoRTOS_queue_head_init(struct picoRTOS_queue_head *ctx, size_t count)
{
    /* count must be a power of 2 */
    picoRTOS_assert_fatal(IS_POW2(count), return );

    ctx->lock = PICORTOS_FUTEX_INITIALIZER;
    ctx->mask = count - 1;
    ctx->w = 0;
    ctx->r = ctx->mask;
    ctx->i = 0;

    picoRTOS_flush_dcache(ctx, sizeof(*ctx));
}

int picoRTOS_queue_head_pop(struct picoRTOS_queue_head *ctx)
{
    size_t next;

    picoRTOS_futex_lock(&ctx->lock);
    picoRTOS_invalidate_dcache(ctx, sizeof(*ctx));

    next = (ctx->r + 1) & ctx->mask;

    if (next == ctx->w) {
        picoRTOS_futex_unlock(&ctx->lock);
        return -EAGAIN;
    }

    ctx->r = next;
    ctx->i = next;

    picoRTOS_flush_dcache(ctx, sizeof(*ctx));
    picoRTOS_futex_unlock(&ctx->lock);

    return 0;
}

int picoRTOS_queue_head_push(struct picoRTOS_queue_head *ctx)
{
    size_t next;

    picoRTOS_futex_lock(&ctx->lock);
    picoRTOS_invalidate_dcache(ctx, sizeof(*ctx));

    next = (ctx->w + 1) & ctx->mask;

    if (next == ctx->r) {
        picoRTOS_futex_unlock(&ctx->lock);
        return -EAGAIN;
    }

    ctx->i = ctx->w;
    ctx->w = next;

    picoRTOS_flush_dcache(ctx, sizeof(*ctx));
    picoRTOS_futex_unlock(&ctx->lock);

    return 0;
}
