#include "adc-ti_type4.h"
#include "picoRTOS.h"

typedef void (*cal_inl_fn)(void);
typedef uint16_t (*get_offset_trim_fn)(uint16_t offset);

#define GET_ADC_OFFSET_TRIM_OTP 0x0703acu
#define GET_ADC_OFFSET_TRIM_OTP_FN              \
    (get_offset_trim_fn)GET_ADC_OFFSET_TRIM_OTP

#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct ADC_REGS {
    volatile uint16_t ADCCTL1;
    volatile uint16_t ADCCTL2;
    volatile uint16_t ADCBURSTCTL;
    volatile uint16_t ADCINTFLG;
    volatile uint16_t ADCINTFLGCLR;
    volatile uint16_t ADCINTOVF;
    volatile uint16_t ADCINTOVFCLR;
    volatile uint16_t ADCINTSEL1N2;
    volatile uint16_t ADCINTSEL3N4;
    volatile uint16_t ADCSOCPRICTL;
    volatile uint16_t ADCINTSOCSEL1;
    volatile uint16_t ADCINTSOCSEL2;
    volatile uint16_t ADCSOCFLG1;
    volatile uint16_t ADCSOCFRC1;
    volatile uint16_t ADCSOCOVF1;
    volatile uint16_t ADCSOCOVFCLR1;
    volatile uint32_t ADCSOCnCTL[16];
    volatile uint32_t ADCEVTSTAT;
    volatile uint32_t ADCEVTCLR;
    volatile uint32_t ADCEVTSEL;
    volatile uint32_t ADCEVTINTSEL;
    volatile uint16_t ADCOSDETECT;
    volatile uint16_t ADCCOUNTER;
    volatile uint16_t ADCREV;
    volatile uint16_t ADCOFFTRIM;
    uint16_t RESERVED0[4];

    struct {
        volatile uint16_t CONFIG;
        volatile uint16_t STAMP;
        volatile uint16_t OFFCAL;
        volatile uint16_t OFFREF;
        volatile uint32_t TRIPHI;
        volatile uint32_t TRIPLO;
    } ADCPPBn[4];

    volatile uint32_t ADCINLTRIM1;
    volatile uint32_t ADCINLTRIM2;
    volatile uint32_t ADCINLTRIM3;
    volatile uint32_t ADCINLTRIM4;
    volatile uint32_t ADCINLTRIM5;
    volatile uint32_t ADCINLTRIM6;
};

#define ADCPPBnRESULT_COUNT 4

struct ADC_RESULT_REGS {
    volatile uint16_t ADCRESULTn[ADC_TI_TYPE4_CHANNEL_COUNT];
    volatile uint16_t ADCPPBnRESULT[ADCPPBnRESULT_COUNT];
};

#define ADCCTL1_ADCPWDNZ    (1 << 7)
#define ADCCTL1_INTPULSEPOS (1 << 2)

#define ADCCTL2_SIGNALMODE  (1 << 7)
#define ADCCTL2_RESOLUTION  (1 << 6)
#define ADCCTL2_PRESCALE_M  0xfu
#define ADCCTL2_PRESCALE(x) ((x) & ADCCTL2_PRESCALE_M)

#define ADCINTSEL1N2_INT2E      (1 << 13)
#define ADCINTSEL1N2_INT2SEL_M  0xfu
#define ADCINTSEL1N2_INT2SEL(x) (((x) & ADCINTSEL1N2_INT2SEL_M) << 8)
#define ADCINTSEL1N2_INT1E      (1 << 5)
#define ADCINTSEL1N2_INT1SEL_M  0xfu
#define ADCINTSEL1N2_INT1SEL(x) ((x) & ADCINTSEL1N2_INT1SEL_M)

#define ADCINTSEL3N4_INT4E      (1 << 13)
#define ADCINTSEL3N4_INT4SEL_M  0xfu
#define ADCINTSEL3N4_INT4SEL(x) (((x) & ADCINTSEL3N4_INT4SEL_M) << 8)
#define ADCINTSEL3N4_INT3E      (1 << 5)
#define ADCINTSEL3N4_INT3SEL_M  0xfu
#define ADCINTSEL3N4_INT3SEL(x) ((x) & ADCINTSEL3N4_INT3SEL_M)

#define ADCSOCnCTL_TRIGSEL_M  0x3ful
#define ADCSOCnCTL_TRIGSEL(x) (((x) & ADCSOCnCTL_TRIGSEL_M) << 20)
#define ADCSOCnCTL_CHSEL_M    0xful
#define ADCSOCnCTL_CHSEL(x)   (((x) & ADCSOCnCTL_CHSEL_M) << 15)
#define ADCSOCnCTL_ACQPS_M    0x1ffu
#define ADCSOCnCTL_ACQPS(x)   ((x) & ADCSOCnCTL_ACQPS_M)

static int calibrate_inl(unsigned long cal_inl_addr)
{
    if (*(uint16_t*)cal_inl_addr != (uint16_t)0xffff) {
        (*(cal_inl_fn)cal_inl_addr)();
        return 0;
    }

    return -EFAULT;
}

static int request_samples(struct adc_ti_type4 *ctx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    if (!picoRTOS_assert(n <= (size_t)ADC_TI_TYPE4_SAMPLES_MAX)) return -EINVAL;

    int ret = 0;

    /* check if we have enough left */
    if (!picoRTOS_assert((ctx->sample_count + n) <= (size_t)ADC_TI_TYPE4_SAMPLES_MAX))
        return -EIO;

    ret = (int)ctx->sample_count;
    ctx->sample_count += n;

    return ret;
}

static int request_intflg(struct adc_ti_type4 *ctx, size_t soc,
                          adc_ti_type4_intflg_t intflg)
{
    if (!picoRTOS_assert(soc < (size_t)ADC_TI_TYPE4_CHANNEL_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(intflg < ADC_TI_TYPE4_INTFLG_COUNT)) return -EINVAL;

    int res = 0;

    ASM(" eallow");

    switch (intflg) {
    case ADC_TI_TYPE4_INTFLG_1:
        ctx->base->ADCINTSEL1N2 |= ADCINTSEL1N2_INT1E;
        ctx->base->ADCINTSEL1N2 &= ~ADCINTSEL1N2_INT1SEL(ADCINTSEL1N2_INT1SEL_M);
        ctx->base->ADCINTSEL1N2 |= ADCINTSEL1N2_INT1SEL(soc);
        break;

    case ADC_TI_TYPE4_INTFLG_2:
        ctx->base->ADCINTSEL1N2 |= ADCINTSEL1N2_INT2E;
        ctx->base->ADCINTSEL1N2 &= ~ADCINTSEL1N2_INT2SEL(ADCINTSEL1N2_INT2SEL_M);
        ctx->base->ADCINTSEL1N2 |= ADCINTSEL1N2_INT2SEL(soc);
        break;

    case ADC_TI_TYPE4_INTFLG_3:
        ctx->base->ADCINTSEL3N4 |= ADCINTSEL3N4_INT3E;
        ctx->base->ADCINTSEL3N4 &= ~ADCINTSEL3N4_INT3SEL(ADCINTSEL3N4_INT3SEL_M);
        ctx->base->ADCINTSEL3N4 |= ADCINTSEL3N4_INT3SEL(soc);
        break;

    case ADC_TI_TYPE4_INTFLG_4:
        ctx->base->ADCINTSEL3N4 |= ADCINTSEL3N4_INT4E;
        ctx->base->ADCINTSEL3N4 &= ~ADCINTSEL3N4_INT4SEL(ADCINTSEL3N4_INT4SEL_M);
        ctx->base->ADCINTSEL3N4 |= ADCINTSEL3N4_INT4SEL(soc);
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/
        res = -EIO;
    }

    ASM(" edis");

    return res;
}

/* Function: adc_ti_type4_init
 * Inits an ADC block
 *
 * Parameters:
 *  ctx - The ADC block to init
 *  base - The ADC block base address
 *  result_base - The ADC block result register base address
 *  cal_inl_addr - The INL calibration address
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int adc_ti_type4_init(struct adc_ti_type4 *ctx,
                      int base, int result_base,
                      unsigned long cal_inl_addr)
{
    int res;

    ctx->base = (struct ADC_REGS*)base;
    ctx->result = (struct ADC_RESULT_REGS*)result_base;
    ctx->acqps_min = (size_t)ADC_TI_TYPE4_ACQPS_MIN_16BIT;
    ctx->sample_count = 0;

    /* calibrate INL from provided address */
    if ((res = calibrate_inl(cal_inl_addr)) < 0)
        return res;

    ASM(" eallow");

    /* interrupt pulse is at end of conversion */
    ctx->base->ADCCTL1 |= ADCCTL1_INTPULSEPOS;
    /* turn block on */
    ctx->base->ADCCTL1 |= ADCCTL1_ADCPWDNZ;

    ASM(" edis");
    return 0;
}

static int set_offset_trim_otp(struct adc_ti_type4 *ctx,
                               adc_ti_type4_sigmode_t sigmode,
                               adc_ti_type4_resolution_t resolution,
                               adc_ti_type4_trim_off_index_t trim_off_index)
{
    if (!picoRTOS_assert(resolution < ADC_TI_TYPE4_RESOLUTION_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(sigmode < ADC_TI_TYPE4_SIGMODE_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(trim_off_index < ADC_TI_TYPE4_TRIM_OFF_INDEX_COUNT)) return -EINVAL;

    uint16_t index = (uint16_t)(trim_off_index << 2);

    if (*(uint16_t*)GET_ADC_OFFSET_TRIM_OTP == (uint16_t)0xffff)
        return 0;

    index += (uint16_t)sigmode;
    if (resolution == ADC_TI_TYPE4_RESOLUTION_16BIT)
        index += 2;

    ctx->base->ADCOFFTRIM = (*GET_ADC_OFFSET_TRIM_OTP_FN)(index);
    return 0;
}

/* Function: adc_ti_type4_setup
 * Configures an ADC block
 *
 * Parameters:
 *  ctx - The ADC block to configure
 *  settings - The ADC block specific settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int adc_ti_type4_setup(struct adc_ti_type4 *ctx, struct adc_ti_type4_settings *settings)
{
    if (!picoRTOS_assert(settings->resolution < ADC_TI_TYPE4_RESOLUTION_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->sigmode < ADC_TI_TYPE4_SIGMODE_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->prescale < ADC_TI_TYPE4_PRESCALE_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->prescale != ADC_TI_TYPE4_PRESCALE_INVALID)) return -EINVAL;

    int res;

    ASM(" eallow");

    if (settings->sigmode == ADC_TI_TYPE4_SIGMODE_DIFFERENTIAL)
        ctx->base->ADCCTL2 |= ADCCTL2_SIGNALMODE;
    else
        ctx->base->ADCCTL2 &= ~ADCCTL2_SIGNALMODE;

    /* set offset trim from factory otp */
    if ((res = set_offset_trim_otp(ctx, settings->sigmode,
                                   settings->resolution,
                                   settings->trim_off_index)) < 0)
        return res;

    if (settings->resolution == ADC_TI_TYPE4_RESOLUTION_16BIT) {
        ctx->base->ADCCTL2 |= ADCCTL2_RESOLUTION;
        ctx->acqps_min = (size_t)ADC_TI_TYPE4_ACQPS_MIN_16BIT; /* 320ns */
    }else{
        /* 12bit linearity workaround (according to TI) */
        ctx->base->ADCINLTRIM1 &= ~(uint32_t)0xffff;
        ctx->base->ADCINLTRIM2 &= ~(uint32_t)0xffff;
        ctx->base->ADCINLTRIM4 &= ~(uint32_t)0xffff;
        ctx->base->ADCINLTRIM5 &= ~(uint32_t)0xffff;
        ctx->acqps_min = (size_t)ADC_TI_TYPE4_ACQPS_MIN_12BIT; /* 75ns */
    }

    ctx->base->ADCCTL2 &= ~ADCCTL2_PRESCALE(ADCCTL2_PRESCALE_M);
    ctx->base->ADCCTL2 |= ADCCTL2_PRESCALE(settings->prescale);

    ASM(" edis");

    return 0;
}

/* Function: adc_ti_type4_adc_init
 * Inits an ADC channel
 *
 * Parameters:
 *  ctx - The ADC channel to init
 *  parent - The parent ADC block
 *  channel - The ADC channel number
 *  nsamples - The number of samples for this channel
 *  intflg - The interrupt flag to associate to this channel
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int adc_ti_type4_adc_init(struct adc *ctx, struct adc_ti_type4 *parent,
                          size_t channel, size_t sample_count,
                          adc_ti_type4_intflg_t intflg)
{
    if (!picoRTOS_assert(channel < (size_t)ADC_TI_TYPE4_CHANNEL_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(sample_count <= (size_t)ADC_TI_TYPE4_SAMPLES_MAX)) return -EINVAL;
    if (!picoRTOS_assert(intflg < ADC_TI_TYPE4_INTFLG_COUNT)) return -EINVAL;

    int res;
    size_t index;

    ctx->parent = parent;
    ctx->intflg = intflg;
    ctx->channel = channel;
    ctx->trig = ADC_TI_TYPE4_TRIG_SOFT;

    /* default setup */
    ctx->multiplier = 1;
    ctx->divider = 1;
    ctx->offset = 0;

    if ((res = request_samples(ctx->parent, sample_count)) < 0)
        return res;

    index = (size_t)res;
    ctx->soc_start = index;
    ctx->soc_end = ctx->soc_start + sample_count - 1;

    /* init */
    ASM(" eallow");

    for (; (size_t)index <= ctx->soc_end; index++) {
        parent->base->ADCSOCnCTL[index] = (uint32_t)ADCSOCnCTL_CHSEL((uint32_t)ctx->channel);
        parent->base->ADCSOCnCTL[index] |= ctx->parent->acqps_min;
    }

    ASM(" edis");

    /* associate intflg to this channel */
    return request_intflg(ctx->parent, ctx->soc_end, intflg);
}

/* Function: adc_ti_type4_adc_setup
 * Configures an ADC channel
 *
 * Parameters:
 *  ctx - The ADC channel to configure
 *  settings - The ADC channel settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int adc_ti_type4_adc_setup(struct adc *ctx,
                           struct adc_ti_type4_adc_settings *settings)
{
    if (!picoRTOS_assert(settings->trig < ADC_TI_TYPE4_TRIG_COUNT)) return -EINVAL;
    if (!picoRTOS_assert((size_t)settings->acqps >= ctx->parent->acqps_min)) return -EINVAL;

    size_t i;
    struct adc_ti_type4 *parent = ctx->parent;

    ctx->trig = settings->trig;

    ASM(" eallow");
    for (i = ctx->soc_start; i <= ctx->soc_end; i++)
        parent->base->ADCSOCnCTL[i] = (uint32_t)(ADCSOCnCTL_CHSEL((uint32_t)ctx->channel) |
                                                 ADCSOCnCTL_TRIGSEL((uint32_t)settings->trig) |
                                                 ADCSOCnCTL_ACQPS((uint32_t)settings->acqps));
    ASM(" edis");

    return 0;
}

static int trig_soft(struct adc *ctx)
{
    struct adc_ti_type4 *parent = ctx->parent;
    uint16_t mask = (uint16_t)(1 << (ctx->soc_end + 1)) - 1;

    mask &= ~(uint16_t)((1 << ctx->soc_start) - 1);
    parent->base->ADCSOCFRC1 = mask;

    return -EAGAIN;
}

/* hooks */

int adc_setup(struct adc *ctx, struct adc_settings *settings)
{
    ctx->multiplier = settings->multiplier;
    ctx->divider = settings->divider;
    ctx->offset = settings->offset;

    return 0;
}

int adc_read(struct adc *ctx, int *data)
{
    return adc_read_multiple(ctx, data, (size_t)1);
}

int adc_read_multiple(struct adc *ctx, int *data, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    if (!picoRTOS_assert(n <= (size_t)ADC_TI_TYPE4_SAMPLES_MAX)) return -EINVAL;

    size_t i;
    struct adc_ti_type4 *parent = ctx->parent;
    uint16_t intmask = (uint16_t)1 << ctx->intflg;
    size_t max = MIN(ctx->soc_end + 1, ctx->soc_start + n);

    if ((parent->base->ADCINTFLG & intmask) == 0) {
        if (ctx->trig == ADC_TI_TYPE4_TRIG_SOFT)
            return trig_soft(ctx);

        return -EAGAIN;
    }

    for (i = ctx->soc_start; i < max; i++) {
        uint16_t result = parent->result->ADCRESULTn[i];
        *data++ = ((int)result * ctx->multiplier) / ctx->divider + ctx->offset;
    }

    /* clear flags */
    parent->base->ADCINTFLGCLR = intmask;
    return (int)i;
}
