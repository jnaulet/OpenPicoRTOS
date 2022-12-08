#include "mux-avr.h"
#include "picoRTOS.h"

#include <stdint.h>

struct MUX_AVR {
    volatile uint8_t RESERVED;
    volatile uint8_t DDR;
    volatile uint8_t PORT;
};

/* Function: mux_avr_init
 * Initializes multiplexing on a port
 *
 * Parameters:
 *  ctx - The mux to init
 *  base - The base address of the port
 *
 * Returns:
 * Always 0
 *
 * Remarks:
 * Manual multiplexing is not really required on AVR platforms as they self-mux,
 * except for some SPI pins as far as i know
 */
int mux_avr_init(struct mux *ctx, struct MUX_AVR *base)
{
    ctx->base = base;
    return 0;
}

/* Function: mux_avr_input
 * Sets the selected pin to input
 *
 * Parameters:
 *  ctx - The mux context
 *  pin - The pin to set
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_avr_input(struct mux *ctx, size_t pin)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_AVR_PIN_COUNT)) return -EINVAL;

    ctx->base->DDR &= ~(uint8_t)(1u << pin);

    return 0;
}

/* Function: mux_avr_output
 * Sets the selected pin to output
 *
 * Parameters:
 *  ctx - The mux context
 *  pin - The pin to set
 *  value - The original value of the pin
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_avr_output(struct mux *ctx, size_t pin, bool value)
{
    if (!picoRTOS_assert(pin < (size_t)MUX_AVR_PIN_COUNT)) return -EINVAL;

    ctx->base->DDR |= (uint8_t)(1u << pin);

    if (value) ctx->base->PORT |= (1 << pin);
    else ctx->base->PORT &= ~(1 << pin);

    return 0;
}
