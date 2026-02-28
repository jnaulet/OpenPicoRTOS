#include "ch552.h"

#include "clock-ch552.h"
#include "mux-ch552.h"

static int ch552_clock_init(void)
{
    struct clock_settings CLOCK_settings = {
        CLOCK_CH552_OSC_XT,
        CLOCK_CH552_SYS_CK_SEL_110 /* Full powaaaa */
    };

    (void)clock_init(&CLOCK_settings);
    return 0;
}

static int ch552_mux_init(void)
{
    struct mux P3;

    (void)mux_ch552_init(&P3, MUX_CH552_PORT_P3);
    (void)mux_ch552_mode(&P3, (size_t)0, MUX_CH552_MODE_QUASIBDIR);

    return 0;
}

static int ch552_gpio_init(/*@partial@*/ struct ch552 *ctx)
{
    (void)gpio_8051_init(&ctx->D1, (size_t)3, (size_t)0);
    return 0;
}

int ch552_init(struct ch552 *ctx)
{
    (void)ch552_clock_init();
    (void)ch552_mux_init();
    (void)ch552_gpio_init(ctx);
    return 0;
}
