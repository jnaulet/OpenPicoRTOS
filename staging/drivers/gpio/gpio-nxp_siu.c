#include "gpio-nxp_siu.h"
#include "picoRTOS.h"

struct GPIO_NXP_SIU {
    volatile uint32_t PGPDO;
    uint32_t RESERVED0[15];
    volatile uint32_t PGPDI;
    uint32_t RESERVED1[15];
};

int gpio_nxp_siu_init(/*@out@*/ struct gpio *ctx, int base, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)GPIO_NXP_SIU_PIN_COUNT)) return -EINVAL;

    ctx->base = (struct GPIO_NXP_SIU*)base;
    ctx->mask = (uint32_t)(0x80000000u >> pin);

    return 0;
}

void gpio_write(struct gpio *ctx, bool value)
{
    if (value) ctx->base->PGPDO |= ctx->mask;
    else ctx->base->PGPDO &= ~ctx->mask;
}

bool gpio_read(struct gpio *ctx)
{
    return (ctx->base->PGPDI & ctx->mask) != 0;
}

void gpio_toggle(__attribute__((unused)) /*@unused@*/ struct gpio *ctx)
{
    picoRTOS_break(); /* unsupported */
}
