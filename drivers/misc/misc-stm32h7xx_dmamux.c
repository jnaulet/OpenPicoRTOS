#include "misc-stm32h7xx_dmamux.h"
#include "picoRTOS.h"

#include <stdint.h>

#define DMAMUX_STM32H7XX_RGxCR_COUNT 8

struct DMAMUX_STM32H7XX {
    volatile uint32_t CxCR[DMAMUX_STM32H7XX_CHANNEL_MAX];
    uint32_t RESERVED0[16];
    volatile uint32_t CSR;
    volatile uint32_t CFR;
    uint32_t RESERVED1[30];
    volatile uint32_t RGxCR[DMAMUX_STM32H7XX_RGxCR_COUNT];
    uint32_t RESERVED2[8];
    volatile uint32_t RGSR;
    volatile uint32_t RGCFR;
};

#define CxCR_SYNC_ID_M    0x1fu
#define CxCR_SYNC_ID(x)   (((x) & CxCR_SYNC_ID_M) << 24)
#define CxCR_NBREQ_M      0x1fu
#define CxCR_NBREQ(x)     (((x) & CxCR_NBREQ_M) << 19)
#define CxCR_SPOL_M       0x3u
#define CxCR_SPOL(x)      (((x) & CxCR_SPOL_M) << 17)
#define CxCR_SE           (1 << 16)
#define CxCR_EGE          (1 << 9)
#define CxCR_SOIE         (1 << 8)
#define CxCR_DMAREQ_ID_M  0x7fu
#define CxCR_DMAREQ_ID(x) ((x) & CxCR_DMAREQ_ID_M)

#define RGxCR_GNBREQ_M  0x1fu
#define RGxCR_GNBREQ(x) (((x) & RGxCR_GNBREQ_M) << 19)
#define RGxCR_GPOL_M    0x3u
#define RGxCR_GPOL(x)   (((x) & RGxCR_GPOL_M) << 17)
#define RGxCR_GE        (1 << 16)
#define RGxCR_OIE       (1 << 8)
#define RGxCR_SIG_ID_M  0x1fu
#define RGxCR_SIG_ID(x) ((x) & RGxCR_SIG_ID_M)

/* Function: dmamux_stm32h7xx_init
 * Inits the DMAMUX channel
 *
 * Parameters:
 *  ctx - The DMAMUX channel to init
 *  base - The DMAMUX base address
 *  channel - The DMAMUX channel
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int dmamux_stm32h7xx_init(struct dmamux *ctx, int base, size_t channel)
{
    if (!picoRTOS_assert(channel < (size_t)DMAMUX_STM32H7XX_CHANNEL_MAX)) return -EINVAL;

    ctx->base = (struct DMAMUX_STM32H7XX*)base;
    ctx->channel = channel;

    return 0;
}

/* Function: dmamux_stm32h7xx_set_dmareq_id
 * Configures and DMAXMUX DMAREQ_ID
 *
 * Parameters:
 *  ctx - The DMAMUX channel to configure
 *  id - The DMAREQ_ID to setup
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int dmamux_stm32h7xx_set_dmareq_id(struct dmamux *ctx, unsigned long id)
{
    if (!picoRTOS_assert(id <= (unsigned long)CxCR_DMAREQ_ID_M)) return -EINVAL;

    ctx->base->CxCR[ctx->channel] = (uint32_t)CxCR_DMAREQ_ID(id);
    return 0;
}
