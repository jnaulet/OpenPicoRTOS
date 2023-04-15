#include "adc-rp2040.h"
#include "picoRTOS.h"

struct ADC_RP2040 {
    volatile uint32_t CS;
    volatile uint32_t RESULT;
    volatile uint32_t FCS;
    volatile uint32_t FIFO;
    volatile uint32_t DIV;
    volatile uint32_t INTR;
    volatile uint32_t INTE;
    volatile uint32_t INTF;
    volatile uint32_t INTS;
};

#define CS_RROBIN_M   0x1fu
#define CS_RROBIN(x)  (((x) & CS_RROBIN_M) << 16)
#define CS_AINSEL_M   0x7u
#define CS_AINSEL(x)  (((x) & CS_AINSEL_M) << 12)
#define CS_ERR_STICKY (1 << 10)
#define CS_ERR        (1 << 9)
#define CS_READY      (1 << 8)
#define CS_START_MANY (1 << 3)
#define CS_START_ONCE (1 << 2)
#define CS_TS_EN      (1 << 1)
#define CS_EN         (1 << 0)

#define FCS_THRESH_M  0xfu
#define FCS_THRESH(x) (((x) & FCS_THRESH_M) << 16)
#define FCS_OVER      (1 << 11)
#define FCS_UNDER     (1 << 10)
#define FCS_FULL      (1 << 9)
#define FCS_EMPTY     (1 << 8)
#define FCS_DREQ_EN   (1 << 3)
#define FCS_ERR       (1 << 2)
#define FCS_SHIFT     (1 << 1)
#define FCS_EN        (1 << 0)

#define FIFO_ERR      (1 << 15)

#define DIV_INT_M   0xffffu
#define DIV_INT(x)  (((x) & DIV_INT_M) << 8)
#define DIV_FRAC_M  0xffu
#define DIV_FRAC(x) ((x) & DIV_FRAC_M)

/* Function: adc_rp2040_init
 * Initializes an ADC block
 *
 * Parameters:
 *  ctx - The ADC block to init
 *  base - The ADC base address
 *
 * Returns:
 * Always 0
 */
int adc_rp2040_init(struct adc_rp2040 *ctx,  struct ADC_RP2040 *base)
{
    ctx->base = base;

    /* No support for DMA and FIFOs yet */

    /* Turn on */
    ctx->base->CS |= CS_EN;

    return 0;
}

/* Function: adc_rp2040_adc_init
 * Initializes an ADC channel
 *
 * Parameters:
 *  ctx - The ADC channel to init
 *  parent - The parent ADC block
 *  channel - The selected channel
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int adc_rp2040_adc_init(struct adc *ctx, struct adc_rp2040 *parent, size_t channel)
{
    if (!picoRTOS_assert(channel < (size_t)ADC_RP2040_CHANNEL_COUNT)) return -EINVAL;

    ctx->parent = parent;
    ctx->channel = channel;
    ctx->state = ADC_RP2040_ADC_STATE_IDLE;
    ctx->multiplier = 1;
    ctx->divider = 1;
    ctx->offset = 0;

    /* turn on temp sensor if requested */
    if (channel == (size_t)ADC_RP2040_CHANNEL_TEMP_SENSOR)
        parent->base->CS |= CS_TS_EN;

    return 0;
}

int adc_setup(struct adc *ctx, struct adc_settings *settings)
{
    ctx->multiplier = settings->multiplier;
    ctx->divider = settings->divider;
    ctx->offset = settings->offset;

    return 0;
}

static int adc_read_idle(struct adc *ctx)
{
    struct adc_rp2040 *parent = ctx->parent;

    parent->base->CS &= ~CS_AINSEL(CS_AINSEL_M);
    parent->base->CS |= CS_AINSEL(ctx->channel);
    parent->base->CS |= CS_START_ONCE;

    ctx->state = ADC_RP2040_ADC_STATE_ACQ;
    return -EAGAIN;
}

static int adc_read_acq(struct adc *ctx, int *data)
{
    struct adc_rp2040 *parent = ctx->parent;

    if ((parent->base->CS & CS_READY) == 0)
        return -EAGAIN;

    /* get data */
    uint32_t result = parent->base->RESULT;

    /* apply calibration */
    *data = ((int)result * ctx->multiplier) / ctx->divider + ctx->offset;

    ctx->state = ADC_RP2040_ADC_STATE_IDLE;
    return 1;
}

int adc_read(struct adc *ctx, int *data)
{
    switch (ctx->state) {
    case ADC_RP2040_ADC_STATE_IDLE: return adc_read_idle(ctx);
    case ADC_RP2040_ADC_STATE_ACQ: return adc_read_acq(ctx, data);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

int adc_read_multiple(struct adc *ctx, int *data, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    return adc_read(ctx, data);
}
