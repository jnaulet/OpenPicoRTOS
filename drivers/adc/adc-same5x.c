#include "adc-same5x.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"

#include <stdint.h>

struct ADC_SAME5X {
    volatile uint16_t CTRLA;
    volatile uint8_t EVCTRL;
    volatile uint8_t DBGCTRL;
    volatile uint16_t INPUTCTRL;
    volatile uint16_t CTRLB;
    volatile uint8_t REFCTRL;
    uint8_t RESERVED0;
    volatile uint8_t AVGCTRL;
    volatile uint8_t SAMPCTRL;
    volatile uint16_t WINLT;
    volatile uint16_t WINUT;
    volatile uint16_t GAINCORR;
    volatile uint16_t OFFSETCORR;
    volatile uint8_t SWTRIG;
    uint8_t RESERVED1[23];
    volatile uint8_t INTENCLR;
    volatile uint8_t INTENSET;
    volatile uint8_t INTFLAG;
    volatile uint8_t STATUS;
    volatile uint32_t SYNCBUSY;
    volatile uint32_t DSEQDATA;
    volatile uint32_t DSEQCTRL;
    volatile uint32_t DSEQSTAT;
    volatile uint16_t RESULT;
    uint16_t RESERVED2;
    volatile uint16_t RESS;
    uint16_t RESERVED3;
    volatile uint16_t CALIB;
};

#define CTRLA_R2R          (1 << 15)
#define CTRLA_PRESCALER_M  0x7u
#define CTRLA_PRESCALER(x) (((x) & CTRLA_PRESCALER_M) << 8)
#define CTRLA_ONDEMAND     (1 << 7)
#define CTRLA_RUNSTDBY     (1 << 6)
#define CTRLA_SLAVEEN      (1 << 5)
#define CTRLA_DUALSEL_M    0x3u
#define CTRLA_DUALSEL(x)   (((x) & CTRLA_DUALSEL_M) << 3)
#define CTRLA_ENABLE       (1 << 1)
#define CTRLA_SWRST        (1 << 0)

#define INPUTCTRL_DSEQSTOP  (1 << 15)
#define INPUTCTRL_MUXNEG_M  0x1fu
#define INPUTCTRL_MUXNEG(x) (((x) & INPUTCTRL_MUXNEG_M) << 8)
#define INPUTCTRL_DIFFMODE  (1 << 7)
#define INPUTCTRL_MUXPOS_M  0x1fu
#define INPUTCTRL_MUXPOS(x) ((x) & INPUTCTRL_MUXPOS_M)

#define CTRLB_WINSS      (1 << 11)
#define CTRLB_WINMODE_M  0x7u
#define CTRLB_WINMODE(x) (((x) & CTRLB_WINMODE_M) << 8)
#define CTRLB_RESSEL_M   0x7u
#define CTRLB_RESSEL(x)  (((x) & CTRLB_RESSEL_M) << 3)
#define CTRLB_CORREN     (1 << 2)
#define CTRLB_FREERUN    (1 << 1)
#define CTRLB_LEFTADJ    (1 << 0)

#define REFCTRL_REFCOMP   (1 << 7)
#define REFCTRL_REFSEL_M  0xfu
#define REFCTRL_REFSEL(x) ((x) & REFCTRL_REFSEL_M)

#define AVGCTRL_ADJRES_M     0x7u
#define AVGCTRL_ADJRES(x)    (((x) & AVGCTRL_ADJRES_M) << 4)
#define AVGCTRL_SAMPLENUM_M  0xfu
#define AVGCTRL_SAMPLENUM(x) ((x) & AVGCTRL_SAMPLENUM_M)

#define SAMPCTRL_OFFCOMP    (1 << 7)
#define SAMPCTRL_SAMPLEN_M  0x3fu
#define SAMPCTRL_SAMPLEN(x) ((x) & SAMPCTRL_SAMPLEN_M)

#define SWTRIG_START (1 << 1)
#define SWTRIG_FLUSH (1 << 0)

#define INTFLAG_WINMON  (1 << 2)
#define INTFLAG_OVERRUN (1 << 1)
#define INTFLAG_RESRDY  (1 << 0)

#define STATUS_WCC_M   0x3fu
#define STATUS_WCC(x)  (((x) & STATUS_WCC_M) << 2)
#define STATUS_ADCBUSY (1 << 0)

#define SYNCBUSY_SWTRIG     (1 << 11)
#define SYNCBUSY_OFFSETCORR (1 << 10)
#define SYNCBUSY_GAINCORR   (1 << 9)
#define SYNCBUSY_WINUT      (1 << 8)
#define SYNCBUSY_WINLT      (1 << 7)
#define SYNCBUSY_SAMPCTRL   (1 << 6)
#define SYNCBUSY_AVGCTL     (1 << 5)
#define SYNCBUSY_REFCTL     (1 << 4)
#define SYNCBUSY_CTRLB      (1 << 3)
#define SYNCBUSY_INPUTCTRL  (1 << 2)
#define SYNCBUSY_ENABLE     (1 << 1)
#define SYNCBUSY_SWRST      (1 << 0)

static int sync_busywait(const struct adc *ctx, uint32_t mask)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while ((ctx->base->SYNCBUSY & mask) != 0 && deadlock-- != 0)
        arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);
    return 0;
}

/* Function: adc_same5x_init
 * Initializes an ADC channel
 *
 * Parameters:
 *  ctx - The ADC channel to init
 *  base - The ADC block base address
 *  channel - The selected ADC channel
 *
 * Returns:
 * Always 0
 */
int adc_same5x_init(struct adc *ctx, int base, adc_same5x_channel_t channel)
{
    picoRTOS_assert(channel < ADC_SAME5X_CHANNEL_COUNT, return -EINVAL);

    ctx->base = (struct ADC_SAME5X*)base;
    ctx->channel = channel;
    ctx->state = ADC_SAME5X_STATE_IDLE;
    ctx->multiplier = 1;
    ctx->divider = 1;
    ctx->offset = 0;

    /* enable */
    ctx->base->CTRLA = (uint16_t)CTRLA_ENABLE;
    return sync_busywait(ctx, (uint32_t)SYNCBUSY_ENABLE);
}

int adc_setup(struct adc *ctx, const struct adc_settings *settings)
{
    ctx->multiplier = settings->multiplier;
    ctx->divider = settings->divider;
    ctx->offset = settings->offset;

    return 0;
}

static int adc_read_idle(struct adc *ctx)
{
    int res;

    if ((ctx->base->STATUS & STATUS_ADCBUSY) != 0)
        return -EAGAIN;

    ctx->base->INPUTCTRL = (uint16_t)INPUTCTRL_MUXPOS(ctx->channel);
    ctx->base->SWTRIG = (uint8_t)SWTRIG_START;

    if ((res = sync_busywait(ctx, (uint32_t)(SYNCBUSY_INPUTCTRL | SYNCBUSY_SWTRIG))) < 0)
        return res;

    ctx->state = ADC_SAME5X_STATE_ACQ;
    return -EAGAIN;
}

static int adc_read_acq(struct adc *ctx, int *data)
{
    if ((ctx->base->STATUS & STATUS_ADCBUSY) != 0 ||
        (ctx->base->INTFLAG & INTFLAG_RESRDY) == 0)
        return -EAGAIN;

    /* get data */
    uint16_t result = ctx->base->RESULT;

    /* apply calibration */
    *data = ((int)result * ctx->multiplier) / ctx->divider + ctx->offset;

    /* ack */
    ctx->base->INTFLAG = (uint8_t)INTFLAG_RESRDY;
    ctx->state = ADC_SAME5X_STATE_IDLE;
    return 1;
}

int adc_read(struct adc *ctx, int *data)
{
    switch (ctx->state) {
    case ADC_SAME5X_STATE_IDLE: return adc_read_idle(ctx);
    case ADC_SAME5X_STATE_ACQ: return adc_read_acq(ctx, data);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

int adc_read_multiple(struct adc *ctx, int *data, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    return adc_read(ctx, data);
}
