#include "gpio-nxp_siul2.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"

#include <stdint.h>

struct GPIO_NXP_SIUL2 {
    volatile uint16_t PGPDO;
    uint16_t RESERVED0[31];
    volatile uint16_t PGPDI;
    /* uint16_t RESERVED1[31]; */
    /* volatile uint32_t MPGPDO; */
};

/* Function: gpio_nxp_siul2_init
 * Initializes a GPIO block
 *
 * Parameters:
 *  ctx - The GPIO block to init
 *  base - The GPIO block base address
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int gpio_nxp_siul2_init(struct gpio_nxp_siul2 *ctx, int base)
{
    ctx->base = (struct GPIO_NXP_SIUL2*)base; // NOLINT
    return 0;
}

/* Function: gpio_nxp_siul2_gpio_init
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
int gpio_nxp_siul2_gpio_init(struct gpio *ctx, struct gpio_nxp_siul2 *parent, size_t pin)
{
    picoRTOS_assert(pin < (size_t)GPIO_NXP_SIUL2_PIN_COUNT, return -EINVAL);

    ctx->parent = parent;
    ctx->mask = (uint16_t)(0x8000u >> pin);
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
    if (value ^ ctx->invert) ctx->parent->base->PGPDO |= ctx->mask;
    else ctx->parent->base->PGPDO &= ~ctx->mask;
}

bool gpio_read(struct gpio *ctx)
{
    return ((ctx->parent->base->PGPDI & ctx->mask) != 0) ^ ctx->invert;
}

void gpio_toggle(struct gpio *ctx)
{
    ctx->parent->base->PGPDO ^= ctx->mask;
}

struct gpio *gpio_claim(struct gpio *ctx)
{
    /* FIXME: the GPIOs are interweaved on SIUL2 */
    picoRTOS_mpu_add_region(ctx->parent, sizeof(*ctx->parent), MM_URW);
    picoRTOS_mpu_add_region(ctx->parent->base, sizeof(*ctx->parent->base),
                            MM_URW | MM_NON_CACHEABLE);
    return ctx;
}
