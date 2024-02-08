#include "gpio-n76e003.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#ifndef S_SPLINT_S
__sfr __at(ADDR_P0) P0;
__sfr __at(ADDR_P1) P1;
__sfr __at(ADDR_P2) P2;
__sfr __at(ADDR_P3) P3;
#else
static uint8_t P0;
static uint8_t P1;
static uint8_t P2;
static uint8_t P3;
#endif

int gpio_n76e003_init(/*@out@*/ struct gpio *ctx, size_t port, size_t pin)
{
    picoRTOS_assert(port < (size_t)GPIO_N76E003_PORT_COUNT, return -EINVAL);
    picoRTOS_assert(pin < (size_t)GPIO_N76E003_PIN_COUNT, return -EINVAL);

    ctx->port = port;
    ctx->mask = (uint8_t)1 << pin;
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
    switch (ctx->port) {
    case 0:
        if (value ^ ctx->invert) P0 |= ctx->mask;
        else P0 &= ~ctx->mask;
        break;

    case 1:
        if (value ^ ctx->invert) P1 |= ctx->mask;
        else P1 &= ~ctx->mask;
        break;

    case 2:
        if (value ^ ctx->invert) P2 |= ctx->mask;
        else P2 &= ~ctx->mask;
        break;

    case 3:
        if (value ^ ctx->invert) P3 |= ctx->mask;
        else P3 &= ~ctx->mask;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ break;
    }
}

bool gpio_read(struct gpio *ctx)
{
    switch (ctx->port) {
    case 0: return ((P0 & ctx->mask) != 0) ^ ctx->invert;
    case 1: return ((P1 & ctx->mask) != 0) ^ ctx->invert;
    case 2: return ((P2 & ctx->mask) != 0) ^ ctx->invert;
    case 3: return ((P3 & ctx->mask) != 0) ^ ctx->invert;
    default:
        picoRTOS_break();
        /*@notreached@*/ break;
    }

    return false;
}

void gpio_toggle(struct gpio *ctx)
{
    switch (ctx->port) {
    case 0: P0 ^= ctx->mask; break;
    case 1: P1 ^= ctx->mask; break;
    case 2: P2 ^= ctx->mask; break;
    case 3: P3 ^= ctx->mask; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ break;
    }
}
