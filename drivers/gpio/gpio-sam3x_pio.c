#include "gpio-sam3x_pio.h"
#include "picoRTOS.h"

struct GPIO_SAM3X_PIO {
    volatile uint32_t PIO_PER;
    volatile uint32_t PIO_PDR;
    volatile uint32_t PIO_PSR;
    uint32_t RESERVED0;
    volatile uint32_t PIO_OER;
    volatile uint32_t PIO_ODR;
    volatile uint32_t PIO_OSR;
    uint32_t RESERVED1;
    volatile uint32_t PIO_IFER;
    volatile uint32_t PIO_IFDR;
    volatile uint32_t PIO_IFSR;
    uint32_t RESERVED2;
    volatile uint32_t PIO_SODR;
    volatile uint32_t PIO_CODR;
    volatile uint32_t PIO_ODSR;
    volatile uint32_t PIO_PDSR;
    volatile uint32_t PIO_IER;
    volatile uint32_t PIO_IDR;
    volatile uint32_t PIO_IMR;
    volatile uint32_t PIO_ISR;
    volatile uint32_t PIO_MDER;
    volatile uint32_t PIO_MDDR;
    volatile uint32_t PIO_MDSR;
    uint32_t RESERVED3;
    volatile uint32_t PIO_PUDR;
    volatile uint32_t PIO_PUER;
    volatile uint32_t PIO_PUSR;
    uint32_t RESERVED4;
    volatile uint32_t PIO_ABSR;
    uint32_t RESERVED5[3];
    volatile uint32_t PIO_SCIFSR;
    volatile uint32_t PIO_DIFSR;
    volatile uint32_t PIO_IFDGSR;
    volatile uint32_t PIO_SCDR;
    uint32_t RESERVED6[4];
    volatile uint32_t PIO_OWER;
    volatile uint32_t PIO_OWDR;
    volatile uint32_t PIO_OWSR;
    uint32_t RESERVED7;
    volatile uint32_t PIO_AIMER;
    volatile uint32_t PIO_AIMDR;
    volatile uint32_t PIO_AIMMR;
    uint32_t RESERVED8;
    volatile uint32_t PIO_ESR;
    volatile uint32_t PIO_LSR;
    volatile uint32_t PIO_ELSR;
    uint32_t RESERVED9;
    volatile uint32_t PIO_FELLSR;
    volatile uint32_t PIO_REHLSR;
    volatile uint32_t PIO_FRLHSR;
    uint32_t RESERVED10;
    volatile uint32_t PIO_LOCKSR;
    volatile uint32_t PIO_WPMR;
    volatile uint32_t PIO_WPSR;
};

/* Function: gpio_sam3x_pio_init
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
int gpio_sam3x_pio_init(struct gpio *ctx, int base, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)GPIO_SAM3X_PIO_PIN_COUNT)) return -EINVAL;

    ctx->base = (struct GPIO_SAM3X_PIO*)base;
    ctx->mask = (uint32_t)(1ul << pin);

    return 0;
}

void gpio_write(struct gpio *ctx, bool value)
{
    if (value) ctx->base->PIO_SODR = ctx->mask;
    else ctx->base->PIO_CODR = ctx->mask;
}

bool gpio_read(struct gpio *ctx)
{
    return (ctx->base->PIO_PDSR & ctx->mask) != 0;
}

void gpio_toggle(struct gpio *ctx)
{
    if ((ctx->base->PIO_ODSR & ctx->mask) != 0)
        ctx->base->PIO_CODR = ctx->mask;
    else
        ctx->base->PIO_SODR = ctx->mask;
}
