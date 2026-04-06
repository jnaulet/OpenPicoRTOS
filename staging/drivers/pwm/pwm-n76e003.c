#include "pwm-n76e003.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#ifndef S_SPLINT_S
__sfr __at(ADDR_PWMCON0) PWMCON0;
__sfr __at(ADDR_PWMCON1) PWMCON1;
__sfr __at(ADDR_CKCON)   CKCON;
__sfr __at(ADDR_PWMPL)   PWMPL;
__sfr __at(ADDR_PWMPH)   PWMPH;
__sfr __at(ADDR_PWM0L)   PWM0L;
__sfr __at(ADDR_PWM0H)   PWM0H;
__sfr __at(ADDR_PWM1L)   PWM1L;
__sfr __at(ADDR_PWM1H)   PWM1H;
__sfr __at(ADDR_PWM2L)   PWM2L;
__sfr __at(ADDR_PWM2H)   PWM2H;
__sfr __at(ADDR_PWM3L)   PWM3L;
__sfr __at(ADDR_PWM3H)   PWM3H;
__sfr __at(ADDR_PWM4L)   PWM4L;     /* page 1 */
__sfr __at(ADDR_PWM4H)   PWM4H;     /* page 1 */
__sfr __at(ADDR_PWM5L)   PWM5L;     /* page 1 */
__sfr __at(ADDR_PWM5H)   PWM5H;     /* page 1 */
__sfr __at(ADDR_PIOCON0) PIOCON0;
__sfr __at(ADDR_PIOCON1) PIOCON1;   /* page 1 */
__sfr __at(ADDR_PMEN)    PMEN;
__sfr __at(ADDR_PMD)     PMD;
__sfr __at(ADDR_PNP)     PNP;
__sfr __at(ADDR_PDTEN)   PDTEN;
__sfr __at(ADDR_PDTCNT)  PDTCNT;
#else
static unsigned char PWMCON0;
/*@unused@*/ static unsigned char PWMCON1;
/*@unused@*/ static unsigned char CKCON;
static unsigned char PWMPL;
static unsigned char PWMPH;
static unsigned char PWM0L;
static unsigned char PWM0H;
static unsigned char PWM1L;
static unsigned char PWM1H;
static unsigned char PWM2L;
static unsigned char PWM2H;
static unsigned char PWM3L;
static unsigned char PWM3H;
static unsigned char PWM4L;
static unsigned char PWM4H;
static unsigned char PWM5L;
static unsigned char PWM5H;
/*@unused@*/ static unsigned char PIOCON0;
/*@unused@*/ static unsigned char PIOCON1;
static unsigned char PMEN;
static unsigned char PMD;
/*@unused@*/ static unsigned char PNP;
/*@unused@*/ static unsigned char PDTEN;
/*@unused@*/ static unsigned char PDTCNT;
#endif

#define PWMCON0_PWMRUN (1u << 7)
#define PWMCON0_LOAD   (1 << 6)
#define PWMCON0_PWMF   (1 << 5)
#define PWMCON0_CLRPWM (1 << 4)

#define PWMCON1_PWMMOD_M  0x3u
#define PWMCON1_PWMMOD(x) (((x) & PWMCON1_PWMMOD_M) << 6)
#define PWMCON1_GP        (1 << 5)
#define PWMCON1_PWMTYP    (1 << 4)
#define PWMCON1_FBINEN    (1 << 3)
#define PWMCON1_PWMDIV_M  0x7u
#define PWMCON1_PWMDIV(x) ((x) & PWMCON1_PWMDIV_M)

#define PIOCON0_PIO05 (1 << 5)
#define PIOCON0_PIO04 (1 << 4)
#define PIOCON0_PIO03 (1 << 3)
#define PIOCON0_PIO02 (1 << 2)
#define PIOCON0_PIO01 (1 << 1)
#define PIOCON0_PIO00 (1 << 0)

#define PIOCON1_PIO15 (1 << 5)
#define PIOCON1_PIO13 (1 << 3)
#define PIOCON1_PIO12 (1 << 2)
#define PIOCON1_PIO11 (1 << 1)

#define PDTEN_PDTCTN8 (1 << 4)
#define PDTEN_PDT45EN (1 << 2)
#define PDTEN_PDT23EN (1 << 1)
#define PDTEN_PDT01EN (1 << 0)

#define TA_UNPROTECT()                          \
  do {                                          \
    ASM(" mov 0xc7, #0xaa");                    \
    ASM(" mov 0xc7, #0x55");                    \
  } while(false)

int pwm_n76e003_init(struct pwm_n76e003 *ctx, clock_id_t clkid)
{
    ctx->clkid = clkid;
    ctx->pwmp = 0;

    /* 1st step, according to rm */
    PWMCON0 |= PWMCON0_CLRPWM;

    /* mask everything & 0 */
    PMEN = (unsigned char)0x3f;
    PMD = (unsigned char)0x0;

    return 0;
}

int pwm_n76e003_set_period(struct pwm_n76e003 *ctx, unsigned long pwmp)
{
    picoRTOS_assert(pwmp > 0, return -EINVAL);

    /* set PWMPx */
    PWMPL = (unsigned char)pwmp;
    PWMPH = (unsigned char)(pwmp >> 8);

    ctx->pwmp = pwmp;
    return 0;
}

int pwm_n76e003_pwm_init(struct pwm *ctx, struct pwm_n76e003 *parent, size_t channel)
{
    picoRTOS_assert(channel < (size_t)PWM_N76E003_CHANNEL_COUNT, return -EINVAL);

    ctx->parent = parent;
    ctx->channel = (unsigned int)channel;
    ctx->mask = (unsigned char)1;

    /* TODO: multiplexing */
    PIOCON0 |= (unsigned char)(1u << ctx->channel);
    return 0;
}

int pwm_n76e003_pwm_setup(struct pwm *ctx, const struct pwm_n76e003_settings *settings)
{
    /* only PWMMOD yet */
    picoRTOS_assert(settings->pwmmod < PWM_N76E003_PWMMOD_COUNT, return -EINVAL);

    if (settings->dead_time != 0) {
        switch (ctx->channel) {
        case 0: /*@fallthrough@*/
        case 1:
            TA_UNPROTECT();
            ASM(" orl 0xf9, #0x1");
            break;
        case 2: /*@fallthrough@*/
        case 3:
            TA_UNPROTECT();
            ASM(" orl 0xf9, #0x2");
            break;

        case 4: /*@fallthrough@*/
        case 5:
            TA_UNPROTECT();
            ASM(" orl 0xf9, #0x4");
            break;

        default:
            picoRTOS_assert_void(false);
            /*@notreached@*/ return -EIO;
        }

        /* set PDTCNT */
        TA_UNPROTECT();
        PDTCNT = (unsigned char)settings->dead_time;
    }

    PWMCON1 &= ~PWMCON1_PWMMOD(PWMCON1_PWMMOD_M);
    PWMCON1 |= PWMCON1_PWMMOD(settings->pwmmod);

    ctx->mask = (unsigned char)((settings->pwmmod == PWM_N76E003_PWMMOD_INDEPENDENT) ? 1 : 3);
    PIOCON0 |= (unsigned char)(ctx->mask << ctx->channel);
    return 0;
}

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
{
    picoRTOS_assert(period > 0, return -EINVAL);

    clock_freq_t freq = clock_get_freq(ctx->parent->clkid);

    picoRTOS_assert(freq > 0, return -EIO);
    unsigned long pwmp = ((unsigned long)freq / 1000000ul) * period;

    picoRTOS_assert(pwmp < 65536ul, return -EINVAL);

    /* set PWMPx, at last */
    PWMPL = (unsigned char)pwmp;
    PWMPH = (unsigned char)(pwmp >> 8);
    /* load */
    PWMCON0 |= PWMCON0_LOAD;

    ctx->parent->pwmp = pwmp;
    return 0;
}

static void sfr_page1(void)
{
    picoRTOS_suspend();
    TA_UNPROTECT();
    ASM(" orl 0x91, #0x1");
}

static void sfr_page0(void)
{
    TA_UNPROTECT();
    ASM(" anl 0x91, #0xfe");
    picoRTOS_resume();
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    unsigned long pwm = (ctx->parent->pwmp * (unsigned long)duty_cycle) >> 16;

    switch (ctx->channel) {
    case 0:
        PWM0L = (unsigned char)pwm;
        PWM0H = (unsigned char)(pwm >> 8);
        break;

    case 1:
        PWM1L = (unsigned char)pwm;
        PWM1H = (unsigned char)(pwm >> 8);
        break;

    case 2:
        PWM2L = (unsigned char)pwm;
        PWM2H = (unsigned char)(pwm >> 8);
        break;

    case 3:
        PWM3L = (unsigned char)pwm;
        PWM3H = (unsigned char)(pwm >> 8);
        break;

    case 4:
        sfr_page1();
        PWM4L = (unsigned char)pwm;
        PWM4H = (unsigned char)(pwm >> 8);
        sfr_page0();
        break;

    case 5:
        sfr_page1();
        PWM5L = (unsigned char)pwm;
        PWM5H = (unsigned char)(pwm >> 8);
        sfr_page0();
        break;

    default:
        picoRTOS_assert_void(false);
        /*@notreached@*/ return -EIO;
    }

    /* load */
    PWMCON0 |= PWMCON0_LOAD;
    return 0;
}

/* cppcheck-suppress [constParameterPointer] */
void pwm_start(struct pwm *ctx)
{
    PMEN &= ~(ctx->mask << ctx->channel);
    /* start counter */
    PWMCON0 |= PWMCON0_PWMRUN;
}

/* cppcheck-suppress [constParameterPointer] */
void pwm_stop(struct pwm *ctx)
{
    PMEN |= (ctx->mask << ctx->channel);
}
