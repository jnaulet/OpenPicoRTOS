#include "gpio-8051.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#define GPIO_8051_PORT_COUNT 4 /* default */

#ifdef ADDR_P4
# undef GPIO_8051_PORT_COUNT
# define GPIO_8051_PORT_COUNT 5
#endif
#ifdef ADDR_P5
# undef GPIO_8051_PORT_COUNT
# define GPIO_8051_PORT_COUNT 6
#endif

#ifndef S_SPLINT_S
__sfr __at(ADDR_P0) P0;
__sfr __at(ADDR_P1) P1;
__sfr __at(ADDR_P2) P2;
__sfr __at(ADDR_P3) P3;
# if GPIO_8051_PORT_COUNT > 4
__sfr __at(ADDR_P4) P4;
# if GPIO_8051_PORT_COUNT > 5
__sfr __at(ADDR_P5) P5;
# endif
# endif
#else
static uint8_t P0;
static uint8_t P1;
static uint8_t P2;
static uint8_t P3;
# if GPIO_8051_PORT_COUNT > 4
static uint8_t P4;
# if GPIO_8051_PORT_COUNT > 5
static uint8_t P5;
# endif
# endif
#endif

int gpio_8051_init(struct gpio *ctx, size_t port, size_t pin)
{
    picoRTOS_assert(port < (size_t)GPIO_8051_PORT_COUNT, return -EINVAL);
    picoRTOS_assert(pin < (size_t)GPIO_8051_PIN_COUNT, return -EINVAL);

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

#ifdef ADDR_P4
    case 4:
        if (value ^ ctx->invert) P4 |= ctx->mask;
        else P4 &= ~ctx->mask;
        break;
#endif
#ifdef ADDR_P5
    case 5:
        if (value ^ ctx->invert) P5 |= ctx->mask;
        else P5 &= ~ctx->mask;
        break;
#endif
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
#ifdef ADDR_P4
    case 4: return ((P4 & ctx->mask) != 0) ^ ctx->invert;
#endif
#ifdef ADDR_P5
    case 5: return ((P5 & ctx->mask) != 0) ^ ctx->invert;
#endif
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
#ifdef ADDR_P4
    case 4: P4 ^= ctx->mask; break;
#endif
#ifdef ADDR_P5
    case 5: P5 ^= ctx->mask; break;
#endif
    default:
        picoRTOS_break();
        /*@notreached@*/ break;
    }
}
