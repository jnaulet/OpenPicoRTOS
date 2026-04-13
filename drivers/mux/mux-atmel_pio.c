#include "mux-atmel_pio.h"
#include "picoRTOS.h"

#include <stdint.h>

struct MUX_ATMEL_PIO {
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
    volatile uint32_t PIO_ABCDSR0;
    volatile uint32_t PIO_ABCDSR1;
    uint32_t RESERVED5[2];
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

/* Function: mux_atmel_pio_init
 * Initializes a mux port
 *
 * Parameters:
 *  ctx - The mux to init
 *  base - The mux port base address
 *
 * Returns:
 * Always 0
 */
int mux_atmel_pio_init(struct mux *ctx, int base)
{
    ctx->base = (struct MUX_ATMEL_PIO*)base;
    return 0;
}

static int mux_atmel_pio(struct mux *ctx, mux_atmel_pio_t mux, uint32_t mask)
{
    picoRTOS_assert(mux < MUX_ATMEL_PIO_COUNT, return -EINVAL);

    switch (mux) {
    case MUX_ATMEL_PIO_DISABLE:
        ctx->base->PIO_PDR = mask;
        break;

    case MUX_ATMEL_PIO_GPIO:
        ctx->base->PIO_PER = mask;
        break;

    case MUX_ATMEL_PIO_A:
        ctx->base->PIO_PDR = mask;
        ctx->base->PIO_ABCDSR0 &= ~mask;
        ctx->base->PIO_ABCDSR1 &= ~mask;
        break;

    case MUX_ATMEL_PIO_B:
        ctx->base->PIO_PDR = mask;
        ctx->base->PIO_ABCDSR0 |= mask;
        ctx->base->PIO_ABCDSR1 &= ~mask;
        break;

    case MUX_ATMEL_PIO_C:
        ctx->base->PIO_PDR = mask;
        ctx->base->PIO_ABCDSR0 &= ~mask;
        ctx->base->PIO_ABCDSR1 |= mask;
        break;

    case MUX_ATMEL_PIO_D:
        ctx->base->PIO_PDR = mask;
        ctx->base->PIO_ABCDSR0 |= mask;
        ctx->base->PIO_ABCDSR1 |= mask;
        break;

    default:
        picoRTOS_assert_void(false);
        /*@notreached@*/
        return -EIO;
    }

    return 0;
}

/* Function: mux_atmel_pio_input
 * Sets a pin to input only
 *
 * Parameters:
 *  ctx - The mux port
 *  pin - The pin to set
 *  mux - The function of the pin
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_atmel_pio_input(struct mux *ctx, size_t pin, mux_atmel_pio_t mux)
{
    picoRTOS_assert(pin < (size_t)MUX_ATMEL_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(mux < MUX_ATMEL_PIO_COUNT, return -EINVAL);

    int res;
    uint32_t mask = (uint32_t)(1ul << pin);

    if ((res = mux_atmel_pio(ctx, mux, mask)) < 0)
        return res;

    ctx->base->PIO_ODR = mask;
    return 0;
}

/* Function: mux_atmel_pio_output
 * Sets a pin to output only
 *
 * Parameters:
 *  ctx - The mux port
 *  pin - The pin to set
 *  mux - The function of the pin
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_atmel_pio_output(struct mux *ctx, size_t pin, mux_atmel_pio_t mux)
{
    picoRTOS_assert(pin < (size_t)MUX_ATMEL_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(mux < MUX_ATMEL_PIO_COUNT, return -EINVAL);

    int res;
    uint32_t mask = (uint32_t)(1ul << pin);

    if ((res = mux_atmel_pio(ctx, mux, mask)) < 0)
        return res;

    ctx->base->PIO_OER = mask;
    return 0;
}

/* Function: mux_atmel_pio_pull_up
 * Pulls up a specific pin
 *
 * Parameters:
 *  ctx - The mux port
 *  pin - The pin to pull up
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_atmel_pio_pull_up(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_ATMEL_PIN_COUNT, return -EINVAL);

    ctx->base->PIO_PUER = (uint32_t)(1ul << pin);
    return 0;
}
