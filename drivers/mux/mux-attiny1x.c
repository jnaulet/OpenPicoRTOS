#include "mux-attiny1x.h"
#include "picoRTOS.h"

#include <stdint.h>

struct MUX_ATTINY1X {
    volatile uint8_t DIR;
    volatile uint8_t DIRSET;
    volatile uint8_t DIRCLR;
    volatile uint8_t DIRTGL;
    volatile uint8_t OUT;
    volatile uint8_t OUTSET;
    volatile uint8_t OUTCLR;
    volatile uint8_t OUTTGL;
    volatile uint8_t IN;
    volatile uint8_t INTFLAGS;
    uint8_t RESERVED0[6];
    volatile uint8_t PINnCTRL[MUX_ATTINY1X_PIN_COUNT];
};

#define PINnCTRL_PULLUPEN (1 << 3)

/* Function: mux_attiny1x_init
 * Initializes a port for muxing
 *
 * Parameters:
 *  ctx - The mux object to init
 *  base - The IO port base address
 *
 * Returns:
 * Always 0
 */
int mux_attiny1x_init(struct mux *ctx, struct MUX_ATTINY1X *base)
{
    ctx->base = base;
    return 0;
}

/* Function: mux_attiny1x_input
 * Sets a pin as input only
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to setup on that particular port
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_attiny1x_input(struct mux *ctx, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_ATTINY1X_PIN_COUNT)) return -EINVAL;

    ctx->base->DIRCLR = (uint8_t)(1 << pin);
    return 0;
}

/* Function: mux_attiny1x_output
 * Sets a pin as output only
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to setup on that particular port
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_attiny1x_output(struct mux *ctx, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_ATTINY1X_PIN_COUNT)) return -EINVAL;

    ctx->base->DIRSET = (uint8_t)(1 << pin);
    return 0;
}

/* Function: mux_attiny1x_pull_up
 * Pulls up a specific mux pin
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to pull up
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_attiny1x_pull_up(struct mux *ctx, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_ATTINY1X_PIN_COUNT)) return -EINVAL;

    ctx->base->PINnCTRL[pin] = (uint8_t)PINnCTRL_PULLUPEN;
    return 0;
}
