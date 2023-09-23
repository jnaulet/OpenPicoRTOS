#include "gpio-renesas_ra4.h"
#include "picoRTOS.h"

#include <stdint.h>

struct GPIO_RENESAS_RA4 {
    volatile uint32_t PCNTR1;
    volatile uint32_t PCNTR2;
    volatile uint32_t PCNTR3;
    volatile uint32_t PCNTR4;
};

#define PCNTR1_PODR00 (1 << 16)
#define PCNTR1_PDR00  (1 << 0)

#define PCNTR2_EIDR00 (1 << 16)
#define PCNTR2_PIDR00 (1 << 0)

#define PCNTR3_PORR00 (1 << 16)
#define PCNTR3_POSR00 (1 << 0)

#define PCNTR4_EORR00 (1 << 16)
#define PCNTR4_EOSR00 (1 << 0)

/* Function: gpio_renesas_ra4_init
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
int gpio_renesas_ra4_init(struct gpio *ctx, int base, size_t pin)
{
    picoRTOS_assert(pin < (size_t)GPIO_RENESAS_RA4_PIN_COUNT, return -EINVAL);

    ctx->base = (struct GPIO_RENESAS_RA4*)base;
    ctx->mask = (uint32_t)(1 << pin);
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

void gpio_write(struct gpio *ctx, bool value)
{
    if (value ^ ctx->invert) ctx->base->PCNTR3 = ctx->mask;
    else ctx->base->PCNTR3 = ctx->mask << 16;
}

bool gpio_read(struct gpio *ctx)
{
    return ((ctx->base->PCNTR2 & ctx->mask) != 0) ^ ctx->invert;
}

void gpio_toggle(struct gpio *ctx)
{
    uint32_t mask = ctx->base->PCNTR1 >> 16;

    gpio_write(ctx, (mask & ctx->mask) == 0);
}
