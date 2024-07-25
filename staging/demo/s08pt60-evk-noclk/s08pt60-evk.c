#include "s08pt60-evk.h"
#include "picoRTOS_device.h"

#include "clock-mc9s08ptxx.h"
#include "mux-mc9s08ptxx.h"

#include "picoRTOS_port.h"
#include <generated/autoconf.h>

static void mux_init(void)
{
    struct mux PORTA;
    struct mux PORTH;

    (void)mux_mc9s08ptxx_init(&PORTA, (size_t)MUX_MC9S08PTXX_PORT_A);
    (void)mux_mc9s08ptxx_init(&PORTH, (size_t)MUX_MC9S08PTXX_PORT_H);

    (void)mux_mc9s08ptxx_output(&PORTA, (size_t)3); /* TICK */
    (void)mux_mc9s08ptxx_output(&PORTH, (size_t)1); /* RED */
    (void)mux_mc9s08ptxx_output(&PORTH, (size_t)0); /* GREEN */
}

static int gpio_init(/*@partial@*/ struct s08pt60_evk *ctx)
{
    (void)gpio_mc9s08ptxx_init(&ctx->TICK, (size_t)GPIO_MC9S08PTXX_PORT_A, (size_t)3);
    (void)gpio_mc9s08ptxx_init(&ctx->RED, (size_t)GPIO_MC9S08PTXX_PORT_H, (size_t)1);
    (void)gpio_mc9s08ptxx_init(&ctx->GREEN, (size_t)GPIO_MC9S08PTXX_PORT_H, (size_t)0);

    /* turn off */
    gpio_write(&ctx->RED, true);
    gpio_write(&ctx->GREEN, true);

    return 0;
}

static int wd_init(/*@partial@*/ struct s08pt60_evk *ctx)
{
    (void)wd_mc9s08ptxx_init(&ctx->WD, CLOCK_NXP_MC9S08PTXX_LPO);
    return 0;
}

int s08pt60_evk_init(struct s08pt60_evk *ctx)
{
    /* wd */
    (void)wd_init(ctx);
    (void)wd_stop(&ctx->WD);

    mux_init();
    (void)gpio_init(ctx);

    return 0;
}
