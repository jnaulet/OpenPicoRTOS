#include "wd-renesas_ra4_wdt.h"
#include "picoRTOS.h"

#include <stdint.h>

struct WD_RENESAS_RA4_WDT {
    volatile uint8_t WDTRR;     /* 4200h */
    uint8_t RESERVED0;
    volatile uint16_t WDTCR;    /* 4202h */
    volatile uint16_t WDTSR;    /* 4204h */
    volatile uint8_t WDTRCR;    /* 4206h */
    uint8_t RESERVED1;
    volatile uint8_t WDTCSTPR;  /* 4208h */
};

#define WDTCR_RPSS_M  0x3u
#define WDTCR_RPSS(x) (((x) & WDTCR_RPSS_M) << 12)
#define WDTCR_RPES_M  0x3u
#define WDTCR_RPES(x) (((x) & WDTCR_RPES_M) << 8)
#define WDTCR_CKS_M   0xfu
#define WDTCR_CKS(x)  (((x) & WDTCR_CKS_M) << 4)
#define WDTCR_TOPS_M  0x3u
#define WDTCR_TOPS(x) ((x) & WDTCR_TOPS_M)

#define WDTSR_REFEF     (1 << 15)
#define WDTSR_UNDFF     (1 << 14)
#define WDTSR_CNTVAL_M  0x3fffu
#define WDTSR_CNTVAL(x) ((x) & WDTSR_CNTVAL_M)

#define WDTRCR_RSTIRQS (1 << 7)

#define WDTCSTPR_SLCSTP (1 << 7)

int wd_renesas_ra4_wdt_init(struct wd *ctx, clock_id_t clkid)
{
    ctx->base = (struct WD_RENESAS_RA4_WDT*)base;
    ctx->clkid = clkid;

    return 0;
}

int wd_start(struct wd *ctx)
{
}

int wd_stop(struct wd *ctx)
{
}

void wd_refresh(struct wd *ctx)
{
    ctx->WDTRR = (uint8_t)0x00;
    ctx->WDTRR = (uint8_t)0xff;
}
