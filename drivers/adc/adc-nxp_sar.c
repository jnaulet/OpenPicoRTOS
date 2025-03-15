#include "adc-nxp_sar.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

struct ADC_NXP_SAR {
    volatile uint32_t MCR;
    volatile uint32_t MSR;
    uint32_t RESERVED0[2];
    volatile uint32_t ISR;
    volatile uint32_t CEOCFR[3];
    volatile uint32_t IMR;
    volatile uint32_t CIMR[3];
    volatile uint32_t WTISR;
    volatile uint32_t WTIMR;
    uint32_t RESERVED1[2];
    volatile uint32_t DMAE;
    volatile uint32_t DMAR[3];
    uint32_t RESERVED2[4];
    volatile uint32_t THRHLR0;
    volatile uint32_t THRHLR1;
    volatile uint32_t THRHLR2;
    volatile uint32_t THRHLR3;
    int32_t RESERVED3[4];
    volatile uint32_t PSCR;
    volatile uint32_t PSR[3];
    uint32_t RESERVED4;
    volatile uint32_t CTR[3];
    uint32_t RESERVED5;
    volatile uint32_t NCMR[3];
    uint32_t RESERVED6;
    volatile uint32_t JCMR[3];
    uint32_t RESERVED7;
    volatile uint32_t DSDR;
    volatile uint32_t PDEDR;
    uint32_t RESERVED8[13];
    volatile uint32_t CDR[96];
    volatile uint32_t THRHLR4;
    volatile uint32_t THRHLR5;
    uint32_t RESERVED9[10];
    volatile uint32_t CWSELR[12];
    volatile uint32_t CWENR[3];
    uint32_t RESERVED10;
    volatile uint32_t AWORR[3];
    uint32_t RESERVED11[41];
    volatile uint32_t CALBISTREG;
    uint32_t RESERVED12;
    volatile uint32_t OFSGNUSR;
};

#define MCR_OWREN  (1u << 31)
#define MCR_MODE   (1u << 29)
#define MCR_NSTART (1u << 24)
#define MCR_PWDN   (1u << 0)

#define CDR_CDATA_M 0xffffu
#define CDR_VALID   (1 << 19)

/* Function: adc_nxp_sar_init
 * Inits a NXP ADC SAR block
 *
 * Parameters:
 *  ctx - The ADC block to init
 *  base - The ADC block base address
 *
 * Returns:
 * Always 0
 */
int adc_nxp_sar_init(struct adc_nxp_sar *ctx, int base)
{
    ctx->base = (struct ADC_NXP_SAR*)base;

    /* turn on */
    ctx->base->MCR &= ~MCR_PWDN;

    return 0;
}

/* Function: adc_nxp_sar_setup
 * Configures a NXP ADC SAR block
 *
 * Parameters:
 *  ctx - The ADC block to init
 *  base - The ADC block base address
 *
 * Returns:
 * Always 0
 */
int adc_nxp_sar_setup(struct adc_nxp_sar *ctx, const struct adc_nxp_sar_settings *settings)
{
    if (settings->overwrite) ctx->base->MCR |= MCR_OWREN;
    else ctx->base->MCR &= ~MCR_OWREN;

    if (settings->scan_mode) ctx->base->MCR |= MCR_MODE;
    else ctx->base->MCR &= ~MCR_MODE;

    return 0;
}

/* Function: adc_nxp_sar_adc_init
 * Inits an ADC channel
 *
 * Parameters:
 *  ctx - The ADC channel to init
 *  parent - The parent ADC block
 *  channel - The ADC channel number
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int adc_nxp_sar_adc_init(struct adc *ctx, struct adc_nxp_sar *parent, size_t channel)
{
    picoRTOS_assert(channel < (size_t)ADC_NXP_SAR_CHANNEL_COUNT, return -EINVAL);

    ctx->parent = parent;
    ctx->channel = channel;
    ctx->index = channel / (size_t)32;
    ctx->mask = (uint32_t)(1u << (0x1fu & channel));
    /* calibration */
    ctx->multiplier = 1;
    ctx->divider = 1;
    ctx->offset = 0;

    /* enable channel */
    parent->base->NCMR[ctx->index] |= ctx->mask;
    return 0;
}

int adc_setup(struct adc *ctx, const struct adc_settings *settings)
{
    ctx->multiplier = settings->multiplier;
    ctx->divider = settings->divider;
    ctx->offset = settings->offset;

    return 0;
}

int adc_read(struct adc *ctx, int *data)
{
    uint16_t cdr;
    struct adc_nxp_sar *parent = ctx->parent;

    if ((parent->base->CEOCFR[ctx->index] & ctx->mask) == 0) {
        /* one-shot mode: force new conversion */
        if ((parent->base->MCR & MCR_MODE) == 0)
            parent->base->MCR |= MCR_NSTART;

        return -EAGAIN;
    }

    cdr = (uint16_t)parent->base->CDR[ctx->channel];
    *data = (((int)cdr * ctx->multiplier) / ctx->divider) + ctx->offset;

    parent->base->CEOCFR[ctx->index] = ctx->mask; /* ack */
    return 1;
}

int adc_read_multiple(struct adc *ctx, int *data, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    return adc_read(ctx, data);
}
