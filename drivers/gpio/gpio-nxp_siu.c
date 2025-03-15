#include "gpio-nxp_siu.h"
#include "picoRTOS.h"

struct GPIO_NXP_SIU {
    volatile uint32_t PGPDO;
    uint32_t RESERVED0[15];
    volatile uint32_t PGPDI;
    uint32_t RESERVED1[15];
};

/* Function: gpio_nxp_siu_init
 * Initializes a GPIO
 *
 * Parameters:
 *  ctx - The GPIO to init
 *  base - The GPIO block base address
 *  pin - The GPIO pin number on that block
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int gpio_nxp_siu_init(struct gpio *ctx, int base, size_t pin)
{
    picoRTOS_assert(pin < (size_t)GPIO_NXP_SIU_PIN_COUNT, return -EINVAL);

    ctx->base = (struct GPIO_NXP_SIU*)base;
    ctx->mask = (uint32_t)(0x80000000u >> pin);
    ctx->invert = false;

    return 0;
}

static int set_invert(struct gpio *ctx, gpio_invert_t invert)
{
    picoRTOS_assert(invert != GPIO_INVERT_IGNORE, return -EINVAL);
    picoRTOS_assert(invert < GPIO_INVERT_COUNT, return -EINVAL);

    ctx->invert = (invert == GPIO_INVERT_ENABLE);
    return 0;
}

int gpio_setup(struct gpio *ctx, const struct gpio_settings *settings)
{
    int res;

    if (settings->invert != GPIO_INVERT_IGNORE &&
        (res = set_invert(ctx, settings->invert)) < 0)
        return res;

    return 0;
}

void gpio_write(struct gpio *ctx, bool value)
{
    if (value ^ ctx->invert) ctx->base->PGPDO |= ctx->mask;
    else ctx->base->PGPDO &= ~ctx->mask;
}

bool gpio_read(struct gpio *ctx)
{
    return ((ctx->base->PGPDI & ctx->mask) != 0) ^ ctx->invert;
}

void gpio_toggle(struct gpio *ctx)
{
    ctx->base->PGPDO ^= ctx->mask;
}
