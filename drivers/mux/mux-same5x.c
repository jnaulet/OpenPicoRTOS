#include "mux-same5x.h"
#include "picoRTOS.h"

#include <stdint.h>

#define PMUX_COUNT 16

struct MUX_SAME5X {
    volatile uint32_t DIR;
    volatile uint32_t DIRCLR;
    volatile uint32_t DIRSET;
    volatile uint32_t DIRTGL;
    volatile uint32_t OUT;
    volatile uint32_t OUTCLR;
    volatile uint32_t OUTSET;
    volatile uint32_t OUTTGL;
    volatile uint32_t IN;
    volatile uint32_t CTRL;
    volatile uint32_t WRCONFIG;
    volatile uint32_t EVCTRL;
    volatile uint8_t PMUXn[PMUX_COUNT];
    volatile uint8_t PINCFGn[MUX_SAME5X_PIN_COUNT];
};

#define PMUXn_PMUXO_M  0xfu
#define PMUXn_PMUXO(x) (((x) & PMUXn_PMUXO_M) << 4)
#define PMUXn_PMUXE_M  0xfu
#define PMUXn_PMUXE(x) ((x) & PMUXn_PMUXE_M)

#define PINCFGn_DRVSTR (1 << 6)
#define PINCFGn_PULLEN (1 << 2)
#define PINCFGn_INEN   (1 << 1)
#define PINCFGn_PMUXEN (1 << 0)

/* Function: mux_same5x_init
 * Initializes a mux port
 *
 * Parameters:
 *  ctx - The mux to init
 *  base - The mux port base address
 *
 * Returns:
 * Always 0
 */
int mux_same5x_init(struct mux_same5x *ctx, struct MUX_SAME5X *base)
{
    ctx->base = base;
    return 0;
}

static int set_pmux(struct mux_same5x *ctx, size_t pin, mux_pmux_t pmux)
{
#define IS_ODD(x) (((x) & 0x1) != 0)

    if (!picoRTOS_assert(pin < (size_t)MUX_SAME5X_PIN_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(pmux < MUX_PMUX_COUNT)) return -EINVAL;

    size_t pmux_index = pin >> 1;

    if (IS_ODD(pin)) {
        ctx->base->PMUXn[pmux_index] &= ~PMUXn_PMUXO(PMUXn_PMUXO_M);
        ctx->base->PMUXn[pmux_index] |= PMUXn_PMUXO(pmux);
    }else{
        ctx->base->PMUXn[pmux_index] &= ~PMUXn_PMUXE(PMUXn_PMUXE_M);
        ctx->base->PMUXn[pmux_index] |= PMUXn_PMUXE(pmux);
    }

    return 0;
}

/* Function: mux_same5x_input
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
int mux_same5x_input(struct mux_same5x *ctx, size_t pin, mux_pmux_t pmux)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_SAME5X_PIN_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(pmux < MUX_PMUX_COUNT)) return -EINVAL;

    if (pmux != MUX_PMUX_GPIO) {
        (void)set_pmux(ctx, pin, pmux);
        ctx->base->PINCFGn[pin] = (uint8_t)(PINCFGn_INEN | PINCFGn_PMUXEN);
    }

    ctx->base->DIRCLR = (uint32_t)1 << pin;
    return 0;
}

/* Function: mux_same5x_output
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
int mux_same5x_output(struct mux_same5x *ctx, size_t pin, mux_pmux_t pmux)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_SAME5X_PIN_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(pmux < MUX_PMUX_COUNT)) return -EINVAL;

    if (pmux != MUX_PMUX_GPIO) {
        (void)set_pmux(ctx, pin, pmux);
        ctx->base->PINCFGn[pin] = (uint8_t)PINCFGn_PMUXEN;
    }

    ctx->base->DIRSET = (uint32_t)1 << pin;
    return 0;
}

/* Function: mux_same5x_pull_up
 * Pulls a specific pin up
 *
 * Parameters:
 *  ctx - The mux port
 *  pin - The pin to pull up
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_same5x_pull_up(struct mux_same5x *ctx, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_SAME5X_PIN_COUNT)) return -EINVAL;

    ctx->base->PINCFGn[pin] |= PINCFGn_PULLEN;
    return 0;
}

/* Function: mux_same5x_pull_down
 * Pulls a specific pin down
 *
 * Parameters:
 *  ctx - The mux port
 *  pin - The pin to pull down
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_same5x_pull_down(struct mux_same5x *ctx, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_SAME5X_PIN_COUNT)) return -EINVAL;

    ctx->base->PINCFGn[pin] |= PINCFGn_PULLEN;
    return 0;
}
