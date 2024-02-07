#include "n76e003.h"
#include "picoRTOS_device.h"

#include "clock-n76e003.h"
#include "mux-n76e003.h"

static void clock_init(void)
{
    struct clock_settings CLOCK_settings = {
        CLOCK_N76E003_FOSC_FHIRC,
        CLOCK_N76E003_FHIRC_16_0MHZ,
        0ul,    /* feclk */
        1u,     /* clkdiv */
    };

    (void)clock_n76e003_init(&CLOCK_settings);
}

static void mux_init(void)
{
    struct mux P0;
    struct mux P1;

    (void)mux_n76e003_init(&P0, (size_t)0);
    (void)mux_n76e003_init(&P1, (size_t)1);

    (void)mux_n76e003_mode(&P0, (size_t)6, MUX_N76E003_MODE_QUASIBDIR); /* TXD */
    (void)mux_n76e003_mode(&P0, (size_t)7, MUX_N76E003_MODE_QUASIBDIR); /* RXD */

    (void)mux_n76e003_mode(&P1, (size_t)2, MUX_N76E003_MODE_QUASIBDIR); /* D2 */
    (void)mux_n76e003_mode(&P1, (size_t)3, MUX_N76E003_MODE_QUASIBDIR); /* OUT0 */
    (void)mux_n76e003_mode(&P1, (size_t)4, MUX_N76E003_MODE_QUASIBDIR); /* OUT1 */
    (void)mux_n76e003_mode(&P1, (size_t)5, MUX_N76E003_MODE_PUSHPULL);  /* TICK */
}

static int gpio_init(/*@partial@*/ struct n76e003 *ctx)
{
    (void)gpio_n76e003_init(&ctx->TICK, (size_t)1, (size_t)5);
    (void)gpio_n76e003_init(&ctx->D2, (size_t)1, (size_t)2);
    (void)gpio_n76e003_init(&ctx->OUT0, (size_t)1, (size_t)3);
    (void)gpio_n76e003_init(&ctx->OUT1, (size_t)1, (size_t)4);

    /* Force OUT0 to GND (default) */
    (void)gpio_write(&ctx->OUT0, false);

    return 0;
}

static int uart_init(/*@partial@*/ struct n76e003 *ctx)
{
    const struct uart_settings UART_settings = {
        9600ul,
        (size_t)8,
        UART_PAR_NONE,
        UART_CSTOPB_1BIT,
    };

    (void)uart_n76e003_init(&ctx->UART0, UART_N76E003_UART0_T1);
    return uart_setup(&ctx->UART0, &UART_settings);
}

int n76e003_init(struct n76e003 *ctx)
{
    /* clocks */
    clock_init();
    mux_init();

    /* peripherals */
    (void)gpio_init(ctx);
    (void)uart_init(ctx);

    return 0;
}
