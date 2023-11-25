#include "mux-nxp_siul2.h"
#include "picoRTOS.h"

#include <stdint.h>

struct MUX_NXP_SIUL2 {
    uint32_t RESERVED0;
    volatile uint32_t MIDR1;
    volatile uint32_t MIDR2;
    uint32_t RESERVED1;
    volatile uint32_t ISR0;
    uint32_t RESERVED2;
    volatile uint32_t IRER0;
    uint32_t RESERVED3;
    volatile uint32_t IRSR0;
    uint32_t RESERVED4;
    volatile uint32_t IREER0;
    uint32_t RESERVED5;
    volatile uint32_t IFEER0;
    uint32_t RESERVED6;
    volatile uint32_t IFER0;
    uint32_t RESERVED7;
    volatile uint32_t IFMCR[32];
    volatile uint32_t IFCPR;
    uint32_t RESERVED8[95];
    volatile uint32_t MSCR[264]; /* max: 512 */
    uint32_t RESERVED9[248];
    volatile uint32_t IMCR[512];
    uint32_t RESERVED10[48];
    volatile uint32_t GPDO[66]; /* max: 128 */
    uint32_t RESERVED11[62];
    volatile uint32_t GPDI[66]; /* max: 128 */
    uint32_t RESERVED12[62];
    volatile uint16_t PGPDO[32];
    volatile uint16_t PGPDI[32];
    volatile uint32_t MPGPDO[32];
};

#define MSCR_SRC_M  0x3u
#define MSCR_SRC(x) (((x) & MSCR_SRC_M) << 28)
#define MSCR_OBE    (1 << 25)
#define MSCR_APC    (1 << 22)
#define MSCR_IBE    (1 << 19)
#define MSCR_PUE    (1 << 16)
#define MSCR_PUS    (1 << 17)
#define MSCR_SSS_M  0xfu
#define MSCR_SSS(x) ((x) & MSCR_SSS_M)

#define IMCR_SSS_M  0xfu
#define IMCR_SSS(x) ((x) & IMCR_SSS_M)

/* Function: mux_nxp_siul2_init
 * Initializes multiplexing on a port
 *
 * Parameters:
 *  ctx - The mux to init
 *  base - The base address of the port
 *
 * Returns:
 * Always 0
 */
int mux_nxp_siul2_init(struct mux *ctx, int base)
{
    ctx->base = (struct MUX_NXP_SIUL2*)base;
    return 0;
}

/* Function: mux_nxp_siul2_output
 * Sets the selected pin to output
 *
 * Parameters:
 *  ctx - The mux context
 *  pin - The pin to set
 *  mux - The alternative mux to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_nxp_siul2_output(struct mux *ctx, size_t pin, mux_nxp_siul2_t mux)
{
    picoRTOS_assert(pin < (size_t)MUX_NXP_SIUL2_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(mux < MUX_NXP_SIUL2_COUNT, return -EINVAL);

    /* sss */
    ctx->base->MSCR[pin] &= ~MSCR_SSS_M;
    ctx->base->MSCR[pin] |= MSCR_SSS(mux);
    /* obe */
    ctx->base->MSCR[pin] |= MSCR_OBE;
    /* src */
    ctx->base->MSCR[pin] |= MSCR_SRC(MSCR_SRC_M); /* default to max */

    return 0;
}

/* Function: mux_nxp_siul2_input
 * Sets the selected pin to input
 *
 * Parameters:
 *  ctx - The mux context
 *  pin - The pin to set
 *  mux - The alternative mux to apply
 *  imcr - The input mux register to set (see IO signals description doc)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_nxp_siul2_input(struct mux *ctx, size_t pin, mux_nxp_siul2_t mux, size_t imcr)
{
    picoRTOS_assert(pin < (size_t)MUX_NXP_SIUL2_PIN_COUNT, return -EINVAL);
    picoRTOS_assert(mux < MUX_NXP_SIUL2_COUNT, return -EINVAL);
    picoRTOS_assert(imcr < (size_t)MUX_NXP_SIUL2_IMCR_COUNT, return -EINVAL);

    /* ibe */
    ctx->base->MSCR[pin] |= MSCR_IBE;

    /* imcr */
    ctx->base->IMCR[imcr] &= ~IMCR_SSS_M;
    ctx->base->IMCR[imcr] = (uint32_t)IMCR_SSS(mux);

    return 0;
}

/* Function: mux_nxp_siul2_analog
 * Sets the selected pin to analog input
 *
 * Parameters:
 *  ctx - The mux context
 *  pin - The pin to set
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_nxp_siul2_analog(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_NXP_SIUL2_PIN_COUNT, return -EINVAL);

    ctx->base->MSCR[pin] = (uint32_t)(MSCR_APC | MSCR_IBE);
    return 0;
}

/* Function: mux_nxp_siul2_pull_up
 * Pulls the selected pin up
 *
 * Parameters:
 *  ctx - The mux context
 *  pin - The pin to set
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_nxp_siul2_pull_up(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_NXP_SIUL2_PIN_COUNT, return -EINVAL);

    /* pull enable & up */
    ctx->base->MSCR[pin] |= (MSCR_PUE | MSCR_PUS);
    return 0;
}

/* Function: mux_nxp_siul2_pull_down
 * Pulls the selected pin down
 *
 * Parameters:
 *  ctx - The mux context
 *  pin - The pin to set
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int mux_nxp_siul2_pull_down(struct mux *ctx, size_t pin)
{
    picoRTOS_assert(pin < (size_t)MUX_NXP_SIUL2_PIN_COUNT, return -EINVAL);

    /* pull enable & down */
    ctx->base->MSCR[pin] |= MSCR_PUE;
    ctx->base->MSCR[pin] &= ~MSCR_PUS;

    return 0;
}
