#include "mux-ti_f28x.h"
#include "picoRTOS.h"

#include <stdint.h>
#include <stddef.h>

struct GPIO_CTRL_REGS {
    uint32_t RESERVED0;
    volatile uint32_t GPnQSEL1;
    volatile uint32_t GPnQSEL2;
    volatile uint32_t GPnMUX1;
    volatile uint32_t GPnMUX2;
    volatile uint32_t GPnDIR;
    volatile uint32_t GPnPUD;
    uint32_t RESERVED1[9];
    volatile uint32_t GPnGMUX1;
    volatile uint32_t GPnGMUX2;
    uint32_t RESERVED2[14];
};

static int set_mux(struct mux *ctx, size_t pin, mux_ti_f28x_t gmux, mux_ti_f28x_t mux)
{
    picoRTOS_assert(pin < (size_t)MUX_TI_F28X_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(gmux < MUX_TI_F28X_COUNT, return -EINVAL);
    picoRTOS_assert(mux < MUX_TI_F28X_COUNT, return -EINVAL);

    if (pin < (size_t)(MUX_TI_F28X_PIN_COUNT / 2)) {
        size_t shift = (size_t)(pin * 2);
        uint32_t mask = (uint32_t)0x3ul << shift;

        ctx->base->GPnGMUX1 &= ~mask;
        ctx->base->GPnGMUX1 |= (uint32_t)gmux << shift;
        ctx->base->GPnMUX1 &= ~mask;
        ctx->base->GPnMUX1 |= (uint32_t)mux << shift;

    }else{
        size_t shift = (size_t)((pin - 16) * 2);
        uint32_t mask = (uint32_t)0x3ul << shift;

        ctx->base->GPnGMUX2 &= ~mask;
        ctx->base->GPnGMUX2 |= (uint32_t)gmux << shift;
        ctx->base->GPnMUX2 &= ~mask;
        ctx->base->GPnMUX2 |= (uint32_t)mux << shift;
    }

    return 0;
}

/* Function: mux_ti_f28x_init
 * Initialises a multiplexer block
 *
 * Parameters:
 *  ctx - The mux block to init
 *  base - The mux block base address
 *
 * Returns:
 * always 0
 */
int mux_ti_f28x_init(struct mux *ctx, int base)
{
    ctx->base = (struct GPIO_CTRL_REGS*)base;
    return 0;
}

/* Function: mux_ti_f28x_output
 * Configures a pin as output
 *
 * Parameters:
 *  ctx - The mux block this pin is part of
 *  pin - The pin number
 *  gmux - The GMUX function (see doc)
 *  mux - The MUX function (see doc)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_ti_f28x_output(struct mux *ctx, size_t pin, mux_ti_f28x_t gmux, mux_ti_f28x_t mux)
{
    picoRTOS_assert(pin < (size_t)MUX_TI_F28X_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(gmux < MUX_TI_F28X_COUNT, return -EINVAL);
    picoRTOS_assert(mux < MUX_TI_F28X_COUNT, return -EINVAL);

    int ret;

    ASM(" eallow");
    ctx->base->GPnDIR |= (uint32_t)1ul << pin;
    ret = set_mux(ctx, pin, gmux, mux);
    ASM(" edis");

    return ret;
}

/* Function: mux_ti_f28x_input
 * Configures a pin as input
 *
 * Parameters:
 *  ctx - The mux block this pin is part of
 *  pin - The pin number
 *  gmux - The GMUX function (see doc)
 *  mux - The MUX function (see doc)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_ti_f28x_input(struct mux *ctx, size_t pin, mux_ti_f28x_t gmux, mux_ti_f28x_t mux)
{
    picoRTOS_assert(pin < (size_t)MUX_TI_F28X_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(gmux < MUX_TI_F28X_COUNT, return -EINVAL);
    picoRTOS_assert(mux < MUX_TI_F28X_COUNT, return -EINVAL);

    int ret;

    ASM(" eallow");
    ctx->base->GPnDIR &= ~((uint32_t)1ul << pin);
    ret = set_mux(ctx, pin, gmux, mux);
    ASM(" edis");

    return ret;
}

/* Function: mux_ti_f28x_pull_up
 * Pulls up a specific pin
 *
 * Parameters:
 *  ctx - The mux block this pin is part of
 *  pin - The pin number
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_ti_f28x_pull_up(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_TI_F28X_PIN_COUNT, return -EINVAL);

    ASM(" eallow");
    ctx->base->GPnPUD &= ~((uint32_t)1ul << pin);
    ASM(" edis");

    return 0;
}

/* Function: mux_ti_f28x_set_qsel
 * Configures the QSEL value of a pin
 *
 * Parameters:
 *  ctx - The mux block this pin is part of
 *  pin - The pin number
 *  qsel - The qualification for the pin
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_ti_f28x_set_qsel(struct mux *ctx, size_t pin, mux_ti_f28x_qsel_t qsel)
{
    picoRTOS_assert(pin < (size_t)MUX_TI_F28X_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(qsel < MUX_TI_F28X_QSEL_COUNT, return -EINVAL);

    ASM(" eallow");

    if (pin < (size_t)16) {
        size_t shift = (size_t)(pin * 2);
        uint32_t mask = (uint32_t)0x3ul << shift;

        ctx->base->GPnQSEL1 &= ~mask;
        ctx->base->GPnQSEL1 |= ((uint32_t)qsel << shift);

    }else{
        size_t shift = (size_t)((pin - 16) * 2);
        uint32_t mask = (uint32_t)0x3ul << shift;

        ctx->base->GPnQSEL2 &= ~mask;
        ctx->base->GPnQSEL2 |= ((uint32_t)qsel << shift);
    }

    ASM(" edis");
    return 0;
}
