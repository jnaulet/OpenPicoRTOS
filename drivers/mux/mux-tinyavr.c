#include "mux-tinyavr.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

struct MUX_TINYAVR {
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
    volatile uint8_t PINnCTRL[MUX_TINYAVR_PIN_COUNT];
};

#define PINnCTRL_PULLUPEN (1 << 3)

struct TINYAVR_PORTMUX {
    volatile uint8_t CTRLA;
    volatile uint8_t CTRLB;
    volatile uint8_t CTRLC;
    volatile uint8_t CTRLD;
};

static struct TINYAVR_PORTMUX *PORTMUX =
    (struct TINYAVR_PORTMUX*)ADDR_PORTMUX;

/* Function: mux_tinyavr_init
 * Initializes a port for muxing
 *
 * Parameters:
 *  ctx - The mux object to init
 *  base - The IO port base address
 *
 * Returns:
 * Always 0
 */
int mux_tinyavr_init(struct mux *ctx, int base)
{
    ctx->base = (struct MUX_TINYAVR*)base;
    return 0;
}

/* Function: mux_tinyavr_input
 * Sets a pin as input only
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to setup on that particular port
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_tinyavr_input(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_TINYAVR_PIN_COUNT, return -EINVAL);

    ctx->base->DIRCLR = (uint8_t)(1 << pin);
    return 0;
}

/* Function: mux_tinyavr_output
 * Sets a pin as output only
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to setup on that particular port
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_tinyavr_output(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_TINYAVR_PIN_COUNT, return -EINVAL);

    ctx->base->DIRSET = (uint8_t)(1 << pin);
    return 0;
}

/* Function: mux_tinyavr_pull_up
 * Pulls up a specific mux pin
 *
 * Parameters:
 *  ctx - A mux context/object previously initialized
 *  pin - The pin number to pull up
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_tinyavr_pull_up(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_TINYAVR_PIN_COUNT, return -EINVAL);

    ctx->base->PINnCTRL[pin] = (uint8_t)PINnCTRL_PULLUPEN;
    return 0;
}

int mux_tinyavr_select_alt_pin(mux_tinyavr_alt_t alt)
{
    picoRTOS_assert(alt < MUX_TINYAVR_ALT_COUNT, return -EINVAL); /* ! */

    size_t shift = (size_t)0xfu & alt;

    switch (alt >> 4) {
    case 0: PORTMUX->CTRLA |= (1 << shift); break;
    case 1: PORTMUX->CTRLB |= (1 << shift); break;
    case 2: PORTMUX->CTRLC |= (1 << shift); break;
    case 3: PORTMUX->CTRLD |= (1 << shift); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}
