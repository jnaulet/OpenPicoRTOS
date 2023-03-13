#include "gpio-ti_f28x.h"
#include "picoRTOS.h"

struct GPIO_DATA_REGS {
    volatile uint32_t GPnDAT;
    volatile uint32_t GPnSET;
    volatile uint32_t GPnCLEAR;
    volatile uint32_t GPnTOGGLE;
};

/* Function: gpio_ti_f28x_init
 * Initialises a GPIO
 *
 * Parameters:
 *  ctx - The gpio to init
 *  base - The gpio block base address
 *  pin - The gpio pin number (on this block)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int gpio_ti_f28x_init(struct gpio *ctx, struct GPIO_DATA_REGS *base, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)GPIO_TI_F28X_PIN_COUNT)) return -EINVAL;

    ctx->base = base;
    /* compute pin mask */
    ctx->mask = (uint32_t)(1ul << pin);

    return 0;
}

/* hooks */

void gpio_write(struct gpio *ctx, bool value)
{
    ASM(" eallow");
    if (value) ctx->base->GPnSET = ctx->mask;
    else ctx->base->GPnCLEAR = ctx->mask;
    ASM(" edis");
}

bool gpio_read(struct gpio *ctx)
{
    return (ctx->mask & ctx->base->GPnDAT) != 0;
}

void gpio_toggle(struct gpio *ctx)
{
    ASM(" eallow");
    ctx->base->GPnTOGGLE = ctx->mask;
    ASM(" edis");
}
