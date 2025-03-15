#include "gpio-stm32f10xxx.h"
#include "picoRTOS.h"

#include <stdint.h>

struct GPIO_STM32F10XXX {
    volatile uint32_t GPIOx_CRL;
    volatile uint32_t GPIOx_CRH;
    volatile uint32_t GPIOx_IDR;
    volatile uint32_t GPIOx_ODR;
    volatile uint32_t GPIOx_BSRR;
    volatile uint32_t GPIOx_BRR;
    volatile uint32_t GPIOx_LCKR;
};

#define GPIOx_CRL_CNF0_M   0x3u
#define GPIOx_CRL_CNF0(x)  (((x) & GPIOx_CRL_CNF0_M) << 2)
#define GPIOx_CRL_MODE0_M  0x3u
#define GPIOx_CRL_MODE0(x) ((x) & GPIOx_CRL_MODE0_M)

#define GPIOx_CRH_CNF8_M   0x3u
#define GPIOx_CRH_CNF8(x)  (((x) & GPIOx_CRH_MODE8_M) << 2)
#define GPIOx_CRH_MODE8_M  0x3u
#define GPIOx_CRH_MODE8(x) ((x) & GPIOx_CRH_MODE8_M)

#define GPIOx_BSRR_BR0 (1 << 16)
#define GPIOx_BSRR_BS0 (1 << 0)

#define GPIOx_BRR_BR0 (1 << 0)

#define GPIOx_LCKR_LCKK (1 << 16)
#define GPIOx_LCKR_LC0  (1 << 0)

int gpio_stm32f10xxx_init(struct gpio *ctx, int base, size_t pin)
{
    picoRTOS_assert(pin < (size_t)GPIO_STM32F10XXX_PIN_COUNT, return -EINVAL);

    ctx->base = (struct GPIO_STM32F10XXX*)base;
    ctx->mask = (uint32_t)(1 << pin);
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
    if (value ^ ctx->invert) ctx->base->GPIOx_BSRR = ctx->mask;
    else ctx->base->GPIOx_BSRR = ctx->mask << 16;
}

bool gpio_read(struct gpio *ctx)
{
    return ((ctx->base->GPIOx_IDR & ctx->mask) != 0) ^ ctx->invert;
}

void gpio_toggle(struct gpio *ctx)
{
    ctx->base->GPIOx_ODR ^= ctx->mask;
}
