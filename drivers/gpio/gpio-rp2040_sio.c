#include "gpio-rp2040_sio.h"
#include "picoRTOS.h"

struct GPIO_RP2040_SIO {
    volatile uint32_t GPIO_IN;
    volatile uint32_t GPIO_HI_IN;
    uint32_t RESERVED0;
    volatile uint32_t GPIO_OUT;
    volatile uint32_t GPIO_OUT_SET;
    volatile uint32_t GPIO_OUT_CLR;
    volatile uint32_t GPIO_OUT_XOR;
    volatile uint32_t GPIO_OE;
    volatile uint32_t GPIO_OE_SET;
    volatile uint32_t GPIO_OE_CLR;
    volatile uint32_t GPIO_OE_XOR;
    volatile uint32_t GPIO_HI_OUT;
    volatile uint32_t GPIO_HI_OUT_SET;
    volatile uint32_t GPIO_HI_OUT_CLR;
    volatile uint32_t GPIO_HI_OUT_XOR;
    volatile uint32_t GPIO_HI_OE;
    volatile uint32_t GPIO_HI_OE_SET;
    volatile uint32_t GPIO_HI_OE_CLR;
    volatile uint32_t GPIO_HI_OE_XOR;
};

/* Function: gpio_rp2040_sio_init
 * Initializes a GPIO
 *
 * Parameters:
 *  ctx - The GPIO to init
 *  base - The GPIO block base address
 *  pin - The GPIO pin number on that block
 *  dir - The GPIO direction
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int gpio_rp2040_sio_init(struct gpio *ctx, int base,
                         size_t pin, gpio_rp2040_sio_dir_t dir)
{
    picoRTOS_assert(pin < (size_t)GPIO_RP2040_SIO_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(dir < GPIO_RP2040_SIO_DIR_COUNT, return -EINVAL);

    ctx->base = (struct GPIO_RP2040_SIO*)base;
    ctx->mask = (uint32_t)(1 << pin);
    ctx->invert = false;

    if (dir == GPIO_RP2040_SIO_DIR_OUTPUT)
        ctx->base->GPIO_OE_SET = ctx->mask;
    else
        ctx->base->GPIO_OE_CLR = ctx->mask;

    return 0;
}

static int set_invert(struct gpio *ctx, gpio_invert_t invert)
{
    picoRTOS_assert(invert != GPIO_INVERT_IGNORE, return -EINVAL);
    picoRTOS_assert(invert < GPIO_INVERT_COUNT, return -EINVAL);

    ctx->invert = (invert == GPIO_INVERT_ENABLE);
    return 0;
}

int gpio_setup(struct gpio *ctx, struct gpio_settings *settings)
{
    int res;

    if (settings->invert != GPIO_INVERT_IGNORE &&
        (res = set_invert(ctx, settings->invert)) < 0)
        return res;

    return 0;
}

void gpio_write(struct gpio *ctx, bool value)
{
    if (value ^ ctx->invert) ctx->base->GPIO_OUT_SET = ctx->mask;
    else ctx->base->GPIO_OUT_CLR = ctx->mask;
}

bool gpio_read(struct gpio *ctx)
{
    return ((ctx->base->GPIO_IN & ctx->mask) != 0) ^ ctx->invert;
}

void gpio_toggle(struct gpio *ctx)
{
    ctx->base->GPIO_OUT_XOR = ctx->mask;
}
