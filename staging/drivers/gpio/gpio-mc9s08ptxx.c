#include "gpio-mc9s08ptxx.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

int gpio_mc9s08ptxx_init(struct gpio *ctx, size_t port, size_t pin)
{
    picoRTOS_assert(port < (size_t)GPIO_MC9S08PTXX_PORT_COUNT, return -EINVAL);
    picoRTOS_assert(pin < (size_t)GPIO_MC9S08PTXX_PIN_COUNT, return -EINVAL);

    ctx->port = port;
    ctx->mask = (uint8_t)(1 << pin);
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

int gpio_setup(struct gpio *ctx, struct gpio_settings *settings)
{
    int res;

    if (settings->invert != GPIO_INVERT_IGNORE &&
        (res = set_invert(ctx, settings->invert)) < 0)
        return res;

    return 0;
}

/*@-nullderef@*/
void gpio_write(struct gpio *ctx, bool value)
{
    volatile uint8_t *PORT = (uint8_t*)ADDR_PORT;

    if (value ^ ctx->invert) PORT[ctx->port] |= ctx->mask;
    else PORT[ctx->port] &= ~ctx->mask;
}

bool gpio_read(struct gpio *ctx)
{
    volatile const uint8_t *PORT = (const uint8_t*)ADDR_PORT;

    return ((PORT[ctx->port] & ctx->mask) != 0) ^ ctx->invert;
}

void gpio_toggle(struct gpio *ctx)
{
    volatile uint8_t *PORT = (uint8_t*)ADDR_PORT;

    /* cppcheck-suppress nullPointer */
    PORT[ctx->port] ^= ctx->mask;
}
/*@=nullderef@*/
