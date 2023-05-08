#include "adc-tinyavr.h"
#include "picoRTOS.h"

#include <stdint.h>

struct ADC_TINYAVR {
    volatile uint8_t CTRLA;
    volatile uint8_t CTRLB;
    volatile uint8_t CTRLC;
    volatile uint8_t CTRLD;
    volatile uint8_t CTRLE;
    volatile uint8_t SAMPCTRL;
    volatile uint8_t MUXPOS;
    uint8_t RESERVED0;
    volatile uint8_t COMMAND;
    volatile uint8_t EVCTRL;
    volatile uint8_t INTCTRL;
    volatile uint8_t INTFLAGS;
    volatile uint8_t DBGCTRL;
    volatile uint8_t TEMP;
    uint8_t RESERVED1[2];
    volatile uint16_t RES;
    volatile uint16_t WINLT;
    volatile uint16_t WINHT;
    volatile uint8_t CALIB;
};

#define CTRLA_RESSEL (1 << 2)
#define CTRLA_ENABLE (1 << 0)

#define CTRLC_REFSEL_M  0x3u
#define CTRLC_REFSEL(x) (((x) & CTRLC_REFSEL_M) << 4)
#define CTRLC_PRESC_M   0x7u
#define CTRLC_PRESC(x)  ((x) & CTRLC_PRESC_M)

#define MUXPOS_MUXPOS_M  0x1fu
#define MUXPOS_MUXPOS(x) ((x) & MUXPOS_MUXPOS_M)

#define COMMAND_STCONV (1 << 0)

#define INTFLAGS_RESRDY (1 << 0)

/* Function: adc_tinyavr_init
 * Initializes an ADC block
 *
 * Parameters:
 *  ctx - The adc_avr instance to initialize
 *  base - The ADC base address
 *  clkid - The ADC block clock ID
 *
 * Returns:
 * always 0
 */
int adc_tinyavr_init(struct adc_tinyavr *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct ADC_TINYAVR*)base;
    ctx->clkid = clkid;
    ctx->state = ADC_TINYAVR_STATE_IDLE;

    /* enable */
    ctx->base->CTRLA = (uint8_t)CTRLA_ENABLE;

    return 0;
}

/* Function: adc_tinyavr_setup
 * Configures an ADC block
 *
 * Parameters:
 *  ctx - The ADC block instance to configure
 *  settings - The settings to apply
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int adc_tinyavr_setup(struct adc_tinyavr *ctx, struct adc_tinyavr_settings *settings)
{
    if (!picoRTOS_assert(settings->ressel < ADC_TINYAVR_RESSEL_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->refsel < ADC_TINYAVR_REFSEL_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->clk_freq >= (unsigned long)ADC_TINYAVR_CLK_FREQ_MIN)) return -EINVAL;
    if (!picoRTOS_assert(settings->clk_freq <= (unsigned long)ADC_TINYAVR_CLK_FREQ_MAX)) return -EINVAL;

    if (settings->ressel == ADC_TINYAVR_RESSEL_8BIT) ctx->base->CTRLA |= CTRLA_RESSEL;
    else ctx->base->CTRLA &= ~CTRLA_RESSEL;

    ctx->base->CTRLC &= ~CTRLC_REFSEL(CTRLC_REFSEL_M);
    ctx->base->CTRLC |= CTRLC_REFSEL(settings->refsel);

    /* quick and dirty freq calculation */
    clock_freq_t clk_per = clock_get_freq(ctx->clkid);
    unsigned long pdiv = (unsigned long)clk_per / settings->clk_freq;

    if (!picoRTOS_assert(clk_per > 0))
        return -EIO;

    /* get the closest power of 2 */
    ctx->base->CTRLC &= ~CTRLC_PRESC(CTRLC_PRESC_M);

    if (pdiv > 128ul) ctx->base->CTRLC |= CTRLC_PRESC(0x7);
    else if (pdiv > 64ul) ctx->base->CTRLC |= CTRLC_PRESC(0x6);
    else if (pdiv > 32ul) ctx->base->CTRLC |= CTRLC_PRESC(0x5);
    else if (pdiv > 16ul) ctx->base->CTRLC |= CTRLC_PRESC(0x4);
    else if (pdiv > 8ul) ctx->base->CTRLC |= CTRLC_PRESC(0x3);
    else if (pdiv > 4ul) ctx->base->CTRLC |= CTRLC_PRESC(0x2);
    else if (pdiv > 2ul) ctx->base->CTRLC |= CTRLC_PRESC(0x1);
    else if (pdiv > 1ul) ctx->base->CTRLC |= CTRLC_PRESC(0x0);
    else{
        picoRTOS_break();
        /*@notreached@*/
        return -EINVAL;
    }

    return 0;
}

/* channels */

/* Function: adc_tinyavr_adc_init
 * Initializes an ADC channel
 *
 * Parameters:
 *  ctx - The ADC channel to init
 *  parent - The parent ADC block
 *  muxpos - The mux position of the selected channel
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int adc_tinyavr_adc_init(struct adc *ctx, struct adc_tinyavr *parent, adc_tinyavr_muxpos_t muxpos)
{
    if (!picoRTOS_assert(muxpos < ADC_TINYAVR_MUXPOS_COUNT)) return -EINVAL;

    ctx->parent = parent;
    ctx->muxpos = muxpos;

    /* default setup */
    ctx->multiplier = 1;
    ctx->divider = 1;
    ctx->offset = 0;

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
    struct adc_tinyavr *parent = ctx->parent;

    /* check status */
    if ((parent->base->COMMAND & COMMAND_STCONV) != 0)
        return -EAGAIN;

    /* set muxpos */
    parent->base->MUXPOS = (uint8_t)MUXPOS_MUXPOS(ctx->muxpos);

    /* start conversion */
    parent->base->COMMAND = (uint8_t)COMMAND_STCONV;

    parent->state = ADC_TINYAVR_STATE_ACQ;
    return -EAGAIN;
}

static int adc_read_acq(struct adc *ctx, int *data)
{
    struct adc_tinyavr *parent = ctx->parent;

    if ((parent->base->INTFLAGS & INTFLAGS_RESRDY) == 0)
        return -EAGAIN;

    /* read data */
    uint16_t result = parent->base->RES;

    /* apply calibration */
    *data = ((int)result * ctx->multiplier) / ctx->divider + ctx->offset;

    /* clear flag */
    parent->base->INTFLAGS = (uint8_t)INTFLAGS_RESRDY;

    parent->state = ADC_TINYAVR_STATE_IDLE;
    return 1;
}

int adc_read(struct adc *ctx, int *data)
{
    struct adc_tinyavr *parent = ctx->parent;

    switch (parent->state) {
    case ADC_TINYAVR_STATE_IDLE: return adc_read_idle(ctx);
    case ADC_TINYAVR_STATE_ACQ: return adc_read_acq(ctx, data);
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
