#include "adc-avr.h"
#include "picoRTOS.h"

struct ADC_AVR {
    volatile uint16_t ADC;
    volatile uint8_t ADCSRA;
    volatile uint8_t ADCSRB;
    volatile uint8_t ADMUX;
    volatile uint8_t DIDR2;
    volatile uint8_t DIDR0;
};

#define ADCSRA_ADEN    (1 << 7)
#define ADCSRA_ADSC    (1 << 6)
#define ADCSRA_ADATE   (1 << 5)
#define ADCSRA_ADIF    (1 << 4)
#define ADCSRA_ADIE    (1 << 3)
#define ADCSRA_ADPS_M  0x7u
#define ADCSRA_ADPS(x) ((x) & ADCSRA_ADPS_M)

#define ADCSRB_ACME    (1 << 6)
#define ADCSRB_MUX5    (1 << 3)
#define ADCSRA_ADTS_M  0x7u
#define ADCSRB_ADTS(x) ((x) & ADCSRA_ADTS_M)

#define ADMUX_REFS_M  0x3u
#define ADMUX_REFS(x) (((x) & ADMUX_REFS_M) << 6)
#define ADMUX_ADLAR   (1 << 5)
#define ADMUX_MUX_M   0xfu
#define ADMUX_MUX(x)  ((x) & ADMUX_MUX_M)

/* Function: adc_avr_init
 * Initializes an ADC block
 *
 * Parameters:
 *  ctx - The adc_avr instance to initialize
 *  base - The ADC base address
 *
 * Returns:
 * always 0
 */
int adc_avr_init(struct adc_avr *ctx, struct ADC_AVR *base)
{
    ctx->base = base;
    ctx->state = ADC_AVR_STATE_IDLE;

    /* enable */
    ctx->base->ADCSRA = (uint8_t)ADCSRA_ADEN;

    return 0;
}

/* Function: adc_avr_setup
 * Configures an ADC block
 *
 * Parameters:
 *  ctx - The ADC block instance to configure
 *  settings - The settings
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int adc_avr_setup(struct adc_avr *ctx, struct adc_avr_settings *settings)
{
    if (!picoRTOS_assert(settings->prescaler < ADC_AVR_PRESCALER_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->refs < ADC_AVR_REFS_COUNT)) return -EINVAL;

    ctx->base->ADCSRA &= ~ADCSRA_ADPS_M;
    ctx->base->ADCSRA |= (uint8_t)ADCSRA_ADPS(settings->prescaler);
    ctx->base->ADMUX &= ~ADMUX_REFS(ADMUX_REFS_M);
    ctx->base->ADMUX |= ADMUX_REFS(settings->refs);

    if (settings->trig != ADC_AVR_TRIG_MANUAL) {
        ctx->base->ADCSRB &= ~ADCSRA_ADTS_M;
        ctx->base->ADCSRB |= ADCSRB_ADTS(settings->trig);
        ctx->base->ADCSRA |= ADCSRA_ADATE;
    }

    if (settings->trig == ADC_AVR_TRIG_FREE_RUNNING_MODE)
        ctx->base->ADCSRA |= ADCSRA_ADSC; /* start free-running mode */

    if (settings->left_adjust_result)
        ctx->base->ADMUX |= ADMUX_ADLAR;
    else
        ctx->base->ADMUX &= ~ADMUX_ADLAR;

    return 0;
}

static int adc_avr_set_mux(struct adc_avr *ctx, size_t channel)
{
    if (!picoRTOS_assert(channel <= (size_t)ADC_AVR_CHANNEL_P13_N10)) return -EINVAL;

    ctx->base->ADMUX &= ~ADMUX_MUX_M;
    ctx->base->ADMUX |= (uint8_t)ADMUX_MUX(channel);
    ctx->base->ADCSRB &= ~ADCSRB_MUX5;
    ctx->base->ADCSRB |= ADCSRB_MUX5 & channel;

    return 0;
}

/* channels */

/* Function: adc_avr_adc_init
 * Initializes an ADC channel
 *
 * Parameters:
 *  ctx - The ADC channel to init
 *  adc - The parent ADC block
 *  channel - The number of the selected channel
 *
 * Returns:
 * 0 in case of success, -errno otherwise
 */
int adc_avr_adc_init(struct adc *ctx, struct adc_avr *adc, size_t channel)
{
    if (!picoRTOS_assert(channel <= (size_t)ADC_AVR_CHANNEL_P13_N10)) return -EINVAL;

    ctx->adc = adc;
    ctx->channel = channel;
    ctx->multiplier = 1;
    ctx->divider = 1;
    ctx->offset = 0;

    return 0;
}

static int adc_read_idle(struct adc *ctx)
{
    struct adc_avr *adc = ctx->adc;

    if (adc_avr_set_mux(adc, ctx->channel) < 0)
        return -EINVAL;

    /* start conversion */
    if ((adc->base->ADCSRA & ADCSRA_ADATE) == 0)
        adc->base->ADCSRA |= ADCSRA_ADSC;

    adc->state = ADC_AVR_STATE_ACQ;
    return -EAGAIN;
}

static int adc_read_acq(struct adc *ctx, int *data)
{
    struct adc_avr *adc = ctx->adc;

    if ((adc->base->ADCSRA & ADCSRA_ADIF) == 0)
        return -EAGAIN;

    /* read data */
    uint16_t result = adc->base->ADC;

    /* apply calibration */
    *data = ((int)result * ctx->multiplier) / ctx->divider + ctx->offset;

    /* clear flag */
    adc->base->ADCSRA |= ADCSRA_ADIF;

    adc->state = ADC_AVR_STATE_IDLE;
    return 1;
}

int adc_setup(struct adc *ctx, struct adc_settings *settings)
{
    ctx->multiplier = settings->multiplier;
    ctx->divider = settings->divider;
    ctx->offset = settings->offset;

    return 0;
}

int adc_read(struct adc *ctx, int *data)
{
    struct adc_avr *adc = ctx->adc;

    switch (adc->state) {
    case ADC_AVR_STATE_IDLE: return adc_read_idle(ctx);
    case ADC_AVR_STATE_ACQ: return adc_read_acq(ctx, data);
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
