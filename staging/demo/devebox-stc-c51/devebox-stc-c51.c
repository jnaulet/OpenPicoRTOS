#include "devebox-stc-c51.h"
#include "picoRTOS_device.h"

#include "mux-8051.h"

static void mux_init(void)
{
    struct mux P3;

    (void)mux_8051_init(&P3, (size_t)3);

    (void)mux_8051_mode(&P3, (size_t)0, MUX_8051_MODE_QUASIBDIR);   /* R */
    (void)mux_8051_mode(&P3, (size_t)1, MUX_8051_MODE_QUASIBDIR);   /* T */
    (void)mux_8051_mode(&P3, (size_t)3, MUX_8051_MODE_PUSHPULL);    /* TICK */
    (void)mux_8051_mode(&P3, (size_t)4, MUX_8051_MODE_QUASIBDIR);   /* D2 */
}

static int gpio_init(/*@partial@*/ struct devebox_stc_c51 *ctx)
{
    (void)gpio_8051_init(&ctx->TICK, (size_t)3, (size_t)3);
    (void)gpio_8051_init(&ctx->D2, (size_t)3, (size_t)4);
    (void)gpio_8051_init(&ctx->R, (size_t)3, (size_t)0);
    (void)gpio_8051_init(&ctx->T, (size_t)3, (size_t)1);

    return 0;
}

int devebox_stc_c51_init(struct devebox_stc_c51 *ctx)
{
    mux_init();

    /* peripherals */
    (void)gpio_init(ctx);

    return 0;
}
