#include "devebox-stc-c51.h"
#include "picoRTOS_device.h"

#include "clock-stc12c5axx.h"
#include "mux-8051.h"

#include "generated/autoconf.h"

static void clock_init(void)
{
    struct clock_settings CLOCK_settings = {
        CLOCK_STC12C5AXX_X12,   /* T1 */
        CLOCK_STC12C5AXX_X1     /* BRT */
    };

    (void)clock_stc12c5axx_init(&CLOCK_settings);
}

static void mux_init(void)
{
    struct mux P3;

    (void)mux_8051_init(&P3, (size_t)3);

    (void)mux_8051_mode(&P3, (size_t)0, MUX_8051_MODE_QUASIBDIR);   /* RXD */
    (void)mux_8051_mode(&P3, (size_t)1, MUX_8051_MODE_QUASIBDIR);   /* TXD */

    (void)mux_8051_mode(&P3, (size_t)3, MUX_8051_MODE_PUSHPULL);    /* TICK */
    (void)mux_8051_mode(&P3, (size_t)4, MUX_8051_MODE_QUASIBDIR);   /* D2 */
}

static int gpio_init(/*@partial@*/ struct devebox_stc_c51 *ctx)
{
    (void)gpio_8051_init(&ctx->TICK, (size_t)3, (size_t)3);
    (void)gpio_8051_init(&ctx->D2, (size_t)3, (size_t)4);

    return 0;
}

static int uart_init(/*@partial@*/ struct devebox_stc_c51 *ctx)
{
    struct uart_settings UART_settings = {
        115200ul,
        (size_t)8,
        UART_PAR_NONE,
        UART_CSTOPB_1BIT,
    };

    (void)uart_8051_init(&ctx->UART, CLOCK_STC12C5AXX_T1);
    (void)uart_setup(&ctx->UART, &UART_settings);

#if CONFIG_UART_8051_IRQDRIVEN == 1
    /* IRQ-driven buffers */
    static char rx_buf[16];
    static char tx_buf[16];

    struct uart_8051_irqdriven_settings IRQ_settings = {
        (picoRTOS_irq_t)IRQ_RI_TI,
        (picoRTOS_priority_t)1,
        rx_buf,
        sizeof(rx_buf) - 1,
        tx_buf,
        sizeof(tx_buf) - 1,
    };

    (void)uart_8051_setup_irqdriven(&ctx->UART, &IRQ_settings);
#endif

    return 0;
}

int devebox_stc_c51_init(struct devebox_stc_c51 *ctx)
{
    clock_init();
    mux_init();

    /* peripherals */
    (void)gpio_init(ctx);
    (void)uart_init(ctx);

    return 0;
}
