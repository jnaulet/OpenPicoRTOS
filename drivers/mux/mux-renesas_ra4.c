#include "mux-renesas_ra4.h"
#include "picoRTOS.h"

#include <stdint.h>

#define PFS_COUNT                               \
    (MUX_RENESAS_RA4_PORT_COUNT *                 \
     MUX_RENESAS_RA4_PIN_COUNT)

struct MUX_RENESAS_RA4 {
    volatile uint32_t PFS[PFS_COUNT];
    uint8_t RESERVED0[643];
    volatile uint8_t PWPR;
};

#define PFS_PSEL_M  0x1fu
#define PFS_PSEL(x) (((x) & PFS_PSEL_M) << 24)
#define PFS_PMR     (1 << 16)
#define PFS_ASEL    (1 << 15)
#define PFS_ISEL    (1 << 14)
#define PFS_EOF     (1 << 13)
#define PFS_EOR     (1 << 12)
#define PFS_DSCR1   (1 << 11)
#define PFS_DSCR    (1 << 10)
#define PFS_NCODR   (1 << 6)
#define PFS_PCR     (1 << 4)
#define PFS_PDR     (1 << 2)
#define PFS_PIDR    (1 << 1)
#define PFS_PODR    (1 << 0)

#define PWPR_B0WI  (1 << 7)
#define PWPR_PFSWE (1 << 6)

#define INDEX_FROM_PIN(ctx, pin) (((ctx)->port << 4) | (pin))

static void enable_write(struct mux *ctx)
{
    ctx->base->PWPR = (uint8_t)0;
    ctx->base->PWPR = (uint8_t)PWPR_PFSWE;
}

static void disable_write(struct mux *ctx)
{
    ctx->base->PWPR = (uint8_t)PWPR_B0WI;
}

/* Function: mux_renesas_ra4_init
 * Initializes multiplexing on a port
 *
 * Parameters:
 *  ctx - The mux to init
 *  base - The base address of the port
 *  port - The port number
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_renesas_ra4_init(struct mux *ctx, int base, size_t port)
{
    picoRTOS_assert(port < (size_t)MUX_RENESAS_RA4_PORT_COUNT, return -EINVAL);

    ctx->base = (struct MUX_RENESAS_RA4*)base;
    ctx->port = port;

    return 0;
}

/* Function: mux_renesas_ra4_output
 * Set port pin as GPIO output
 *
 * Parameters:
 *  ctx - The previously initialized mux
 *  pin - The port pin number
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_renesas_ra4_output(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_RENESAS_RA4_PIN_COUNT, return -EINVAL);

    size_t index = INDEX_FROM_PIN(ctx, pin);

    enable_write(ctx);
    ctx->base->PFS[index] = (uint32_t)PFS_PDR;
    disable_write(ctx);

    return 0;
}

/* Function: mux_renesas_ra4_input
 * Set port pin as GPIO input
 *
 * Parameters:
 *  ctx - The previously initialized mux
 *  pin - The port pin number
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_renesas_ra4_input(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_RENESAS_RA4_PIN_COUNT, return -EINVAL);

    size_t index = INDEX_FROM_PIN(ctx, pin);

    enable_write(ctx);
    ctx->base->PFS[index] = 0;
    disable_write(ctx);

    return 0;
}

/* Function: mux_renesas_ra4_analog
 * Set port pin as analog input
 *
 * Parameters:
 *  ctx - The previously initialized mux
 *  pin - The port pin number
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_renesas_ra4_analog(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_RENESAS_RA4_PIN_COUNT, return -EINVAL);

    size_t index = INDEX_FROM_PIN(ctx, pin);

    enable_write(ctx);
    ctx->base->PFS[index] = (uint32_t)PFS_ASEL;
    disable_write(ctx);

    return 0;
}

/* Function: mux_renesas_ra4_periph
 * Set port pin as special peripheral
 *
 * Parameters:
 *  ctx - The previously initialized mux
 *  pin - The port pin number
 *  psel - The pin function selection (see doc)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_renesas_ra4_periph(struct mux *ctx, size_t pin, size_t psel)
{
    picoRTOS_assert(pin < (size_t)MUX_RENESAS_RA4_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(psel < (size_t)MUX_RENESAS_RA4_PSEL_COUNT, return -EINVAL);

    size_t index = INDEX_FROM_PIN(ctx, pin);

    enable_write(ctx);
    ctx->base->PFS[index] = (uint32_t)(PFS_PMR | PFS_PSEL(psel));
    disable_write(ctx);

    return 0;
}

/* Function: mux_renesas_ra4_pull_up
 * Pulls up input pin
 *
 * Parameters:
 *  ctx - The previously initialized mux
 *  pin - The port pin number
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_renesas_ra4_pull_up(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_RENESAS_RA4_PIN_COUNT, return -EINVAL);

    size_t index = INDEX_FROM_PIN(ctx, pin);

    enable_write(ctx);
    ctx->base->PFS[index] |= PFS_PCR;
    disable_write(ctx);

    return 0;
}
