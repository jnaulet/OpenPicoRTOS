#include "adc-atmel_afec.h"
#include "picoRTOS.h"

#include <stdint.h>

struct ADC_ATMEL_AFEC {
    volatile uint32_t AFEC_CR;
    volatile uint32_t AFEC_MR;
    volatile uint32_t AFEC_EMR;
    volatile uint32_t AFEC_SEQ1R;
    volatile uint32_t AFEC_SEQ2R;
    volatile uint32_t AFEC_CHER;
    volatile uint32_t AFEC_CHDR;
    volatile uint32_t AFEC_CHSR;
    volatile uint32_t AFEC_LCDR;
    volatile uint32_t AFEC_IER;
    volatile uint32_t AFEC_IDR;
    volatile uint32_t AFEC_IMR;
    volatile uint32_t AFEC_ISR;
    uint32_t RESERVED0[6];
    volatile uint32_t AFEC_OVER;
    volatile uint32_t AFEC_CWR;
    volatile uint32_t AFEC_CGR;
    uint32_t RESERVED1[2];
    volatile uint32_t AFEC_DIFFR;
    volatile uint32_t AFEC_CSELR;
    volatile uint32_t AFEC_CDR;
    volatile uint32_t AFEC_COCR;
    volatile uint32_t AFEC_TEMPMR;
    volatile uint32_t AFEC_TEMPCWR;
    uint32_t RESERVED2[7];
    volatile uint32_t AFEC_ACR;
    uint32_t RESERVED3[2];
    volatile uint32_t AFEC_SHMR;
    uint32_t RESERVED4[11];
    volatile uint32_t AFEC_COSR;
    volatile uint32_t AFEC_CVR;
    volatile uint32_t AFEC_CECR;
    uint32_t RESERVED5[2];
    volatile uint32_t AFEC_WPMR;
    volatile uint32_t AFEC_WPSR;
};

#define AFEC_CR_START (1 << 1)
#define AFEC_CR_SWRST (1 << 0)

#define AFEC_MR_USEQ        (1u << 31)
#define AFEC_MR_TRANSFER_M  0x3u
#define AFEC_MR_TRANSFER(x) (((x) & AFEC_MR_TRANSFER_M) << 28)
#define AFEC_MR_TRACKTIM_M  0xfu
#define AFEC_MR_TRACKTIM(x) (((x) & AFEC_MR_TRACKTIM_M) << 24)
#define AFEC_MR_ONE         (1 << 23)
#define AFEC_MR_STARTUP_M   0xfu
#define AFEC_MR_STARTUP(x)  (((x) & AFEC_MR_STARTUP_M) << 16)
#define AFEC_MR_PRESCAL_M   0xffu
#define AFEC_MR_PRESCAL(x)  (((x) & AFEC_MR_PRESCAL_M) << 8)
#define AFEC_MR_FREERUN     (1 << 7)
#define AFEC_MR_FWUP        (1 << 6)
#define AFEC_MR_SLEEP       (1 << 5)
#define AFEC_MR_TRGSEL_M    0x7u
#define AFEC_MR_TRGSEL(x)   (((x) & AFEC_MR_TRGSEL_M) << 1)
#define AFEC_MR_TRGEN       (1 << 0)

#define AFEC_EMR_SIGNMODE_M   0x3u
#define AFEC_EMR_SIGNMODE(x)  (((x) & AFEC_EMR_SIGNMODE_M) << 28)
#define AFEC_EMR_STM          (1 << 25)
#define AFEC_EMR_TAG          (1 << 24)
#define AFEC_EMR_RES_M        0x7u
#define AFEC_EMR_RES(x)       (((x) & AFEC_EMR_RES_M) << 16)
#define AFEC_EMR_CMPFILTER_M  0x3u
#define AFEC_EMR_CMPFILTER(x) (((x) & AFEC_EMR_CMPFILTER_M) << 12)
#define AFEC_EMR_CMPALL       (1 << 9)
#define AFEC_EMR_CMPSEL_M     0x1fu
#define AFEC_EMR_CMPSEL(x)    (((x) & AFEC_EMR_CMPSEL_M) << 3)
#define AFEC_EMR_CMPMODE_M    0x3u
#define AFEC_EMR_CMPMODE      ((x) & AFEC_EMR_CMPMODE_M)

#define AFEC_SEQ1R_USCH0_M  0xfu
#define AFEC_SEQ1R_USCH0(x) ((x) & AFEC_SEQ1R_USCH0_M)
/* ... */

#define AFEC_SEQ2R_USCH8_M  0xfu
#define AFEC_SEQ2R_USCH8(x) ((x) & AFEC_SEQ2R_USCH8_M)
/* ... */

#define AFEC_IxR_TEMPCHG (1 << 30)
#define AFEC_IxR_COMPE   (1 << 26)
#define AFEC_IxR_GOVRE   (1 << 25)
#define AFEC_IxR_DRDY    (1 << 24)
/* ... */

#define AFEC_CGR_GAIN0_M  0x3u
#define AFEC_CGR_GAIN0(x) ((x) & AFEC_CGR_GAIN0_M)
/* ... */

#define AFEC_CSELR_CSEL_M  0xfu
#define AFEC_CSELR_CSEL(x) ((x) & AFEC_CSELR_CSEL_M)

#define AFEC_COCR_AOFF_M  0x3ffu
#define AFEC_COCR_AOFF(x) ((x) & AFEC_COCR_AOFF_M)

#define AFEC_ACR_IBCTL_M  0x3u
#define AFEC_ACR_IBCTL(x) (((x) & AFEC_ACR_IBCTL_M) << 8)
#define AFEC_ACR_PGA1EN   (1 << 3)
#define AFEC_ACR_PGA0EN   (1 << 2)

#define AFEC_WPMR_WPKEY_M  0xffffffu
#define AFEC_WPMR_WPKEY(x) (((x) & AFEC_WPMR_WPKEY_M) << 8)
#define AFEC_WPMR_WPEN     (1 << 0)

#define AFE_CLOCK_MIN 4000000
#define AFE_CLOCK_TYP 20000000
#define AFE_CLOCK_MAX 40000000

#define AOFF_DEFAULT 0x200

/* Function: adc_atmel_afec_init
 * Initializes an ADC block
 *
 * Parameters:
 *  ctx - The ADC block to init
 *  base - The ADC block base address
 *
 * Returns:
 * Always 0
 */
int adc_atmel_afec_init(struct adc_atmel_afec *ctx, int base, clock_id_t clkid)
{
#define div_ceil(a, b) (((a) + ((b) - 1)) / (b))
#define div_floor(a, b) ((a) / (b))

    ctx->base = (struct ADC_ATMEL_AFEC*)base;
    ctx->clkid = clkid;
    ctx->seq = 0;

    /*
     * Ensure clock frequency is typical
     */
    int prescal;
    clock_freq_t f = clock_get_freq(clkid);

    picoRTOS_assert(f > 0, return -EIO);
    prescal = (int)div_floor(f, (clock_freq_t)AFE_CLOCK_TYP);

    /* TODO: reset */

    ctx->base->AFEC_MR = (uint32_t)(AFEC_MR_USEQ |
                                    AFEC_MR_TRACKTIM(2) |
                                    AFEC_MR_ONE |
                                    AFEC_MR_STARTUP(4) |
                                    AFEC_MR_PRESCAL(prescal - 1));

    /* gain amplifiers must be enabled prior to any conversion */
    ctx->base->AFEC_ACR = (uint32_t)(AFEC_ACR_IBCTL(1) |
                                     AFEC_ACR_PGA0EN |
                                     AFEC_ACR_PGA1EN);
#undef div_ceil
#undef div_floor
    return 0;
}

/* Function: adc_atmel_afec_protect
 * Enables write-protection on AFEC block
 *
 * Parameters:
 *  ctx - The ADC block to protect
 *
 * Returns:
 * Always 0
 */
int adc_atmel_afec_protect(struct adc_atmel_afec *ctx)
{
    /* enable write protection */
    ctx->base->AFEC_WPSR = (uint32_t)(AFEC_WPMR_WPKEY(0x4414443) |
                                      AFEC_WPMR_WPEN);

    return 0;
}

static int seq_add(struct adc_atmel_afec *ctx, size_t channel)
{
    picoRTOS_assert(ctx->seq < (size_t)ADC_ATMEL_AFEC_SEQ_COUNT,
                    return -EINVAL);

    if (ctx->seq < (size_t)8) {
        size_t shift = ctx->seq * 4;
        ctx->base->AFEC_SEQ1R &= ~(AFEC_SEQ1R_USCH0(AFEC_SEQ1R_USCH0_M) << shift);
        ctx->base->AFEC_SEQ1R |= (AFEC_SEQ1R_USCH0(channel) << shift);
    }else{
        size_t shift = (ctx->seq - (size_t)8) * 4;
        ctx->base->AFEC_SEQ2R &= ~(AFEC_SEQ2R_USCH8(AFEC_SEQ2R_USCH8_M) << shift);
        ctx->base->AFEC_SEQ2R |= (AFEC_SEQ2R_USCH8(channel) << shift);
    }

    /* return seq num */
    return (int)ctx->seq++;
}

/* Function: adc_atmel_afec_adc_init
 * Initializes an ADC channel
 *
 * Parameters:
 *  ctx - The Adc channel to init
 *  parent - The parent ADC block
 *  channel - The selected channel
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int adc_atmel_afec_adc_init(struct adc *ctx, struct adc_atmel_afec *parent, size_t channel)
{
    picoRTOS_assert(channel < (size_t)ADC_ATMEL_AFEC_CHANNEL_COUNT,
                    return -EINVAL);

    int res;

    ctx->parent = parent;
    ctx->channel = channel;
    ctx->multiplier = 1;
    ctx->divider = 1;
    ctx->offset = 0;

    /* add seq */
    if ((res = seq_add(parent, channel)) < 0)
        return res;

    /* configure AOFF */
    parent->base->AFEC_CSELR = (uint32_t)AFEC_CSELR_CSEL(channel);
    parent->base->AFEC_COCR = (uint32_t)AFEC_COCR_AOFF(AOFF_DEFAULT);

    /* enable channel */
    parent->base->AFEC_CHER = (uint32_t)(1 << (size_t)res);
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
    struct adc_atmel_afec *parent = ctx->parent;

    if ((parent->base->AFEC_ISR & (1u << ctx->channel)) == 0) {
        /* launch conversion */
        parent->base->AFEC_CR = (uint32_t)AFEC_CR_START;
        return -EAGAIN;
    }

    /* channel selection */
    parent->base->AFEC_CSELR = (uint32_t)AFEC_CSELR_CSEL(ctx->channel);
    cdr = (uint16_t)parent->base->AFEC_CDR;

    /* apply calibration */
    *data = ((int)cdr * ctx->multiplier) / ctx->divider + ctx->offset;
    return 1;
}

int adc_read_multiple(struct adc *ctx, int *data, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);
    return adc_read(ctx, data);
}
