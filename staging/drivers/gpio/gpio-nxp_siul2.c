#include "gpio-nxp_siul2.h"
#include "picoRTOS.h"

#include <stdint.h>

struct GPIO_NXP_SIUL2 {
    volatile uint16_t PGPDO;
    uint16_t RESERVED0[31];
    volatile uint16_t PGPDI;
    uint16_t RESERVED1[31];
    volatile uint32_t MPGPDO;
};

int gpio_nxp_siul2_init(struct gpio *ctx, int base, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)GPIO_NXP_SIUL2_PIN_COUNT)) return -EINVAL;

    ctx->base = (struct GPIO_NXP_SIUL2*)base;
    ctx->mask = (uint16_t)(0x8000u >> pin);

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
