#include "pwm-stm32h7xx_tim.h"
#include "picoRTOS.h"

#include <stdint.h>

struct PWM_STM32H7XX_TIM {
    volatile uint32_t CR1;  /* basic */
    volatile uint32_t CR2;  /* basic + general-purpose timers2-5 */
    volatile uint32_t SMCR; /* general-purpose timers2-5 */
    volatile uint32_t DIER; /* basic */
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2; /* general-purpose timers2-5 */
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;      /* general-purpose timers2-5 */
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;     /* general-purpose timers2-5 */
    volatile uint32_t CCR3;     /* general-purpose timers2-5 */
    volatile uint32_t CCR4;     /* general-purpose timers2-5 */
    volatile uint32_t BDTR;     /* advanced */
    volatile uint32_t DCR;      /* general-purpose timers2-5 */
    volatile uint32_t DMAR;     /* general-purpose timers2-5 */
    uint32_t RESERVED0;
    volatile uint32_t CCMR3;    /* advanced */
    volatile uint32_t CCR5;     /* advanced */
    volatile uint32_t CCR6;     /* advanced */
    volatile uint32_t AF1;      /* general-purpose timers2-5 */
    volatile uint32_t AF2;      /* advanced */
    volatile uint32_t TISEL;
};

#define CR1_UIFREMAP (1 << 11)
#define CR1_CKD_M    0x3u
#define CR1_CKD(x)   (((x) & CR1_CKD_M) << 8)
#define CR1_ARPE     (1 << 7)
#define CR1_CMS_M    0x3u
#define CR1_CMS(x)   (((x) & CR1_CMS_M) << 5)
#define CR1_DIR      (1 << 4)
#define CR1_OPM      (1 << 3)
#define CR1_URS      (1 << 2)
#define CR1_UDIS     (1 << 1)
#define CR1_CEN      (1 << 0)

#define CR2_MMS2_M  0xfu
#define CR2_MMS2(x) (((x) & CR2_MMS2_M) << 20)
#define CR2_OIS6    (1 << 18)
#define CR2_OIS5    (1 << 16)
#define CR2_OIS4    (1 << 14)
#define CR2_OIS3N   (1 << 13)
#define CR2_OIS3    (1 << 12)
#define CR2_OIS2N   (1 << 11)
#define CR2_OIS2    (1 << 10)
#define CR2_OIS1N   (1 << 9)
#define CR2_OIS1    (1 << 8)
#define CR2_TI1S    (1 << 7)
#define CR2_MMS_M   0x7u
#define CR2_MMS(x)  (((x) & CR2_MMS_M) << 4)
#define CR2_CCDS    (1 << 3)
#define CR2_CCUS    (1 << 2)
#define CR2_CCPC    (1 << 0)

#define SMCR_TS43_M   0x3u
#define SMCR_TS43(x)  (((x) & SMCR_TS43_M) << 20)
#define SMCR_SMS3     (1 << 16)
#define SMCR_ETP      (1 << 15)
#define SMCR_ECE      (1 << 14)
#define SMCR_ETPS_M   0x3u
#define SMCR_ETPS(x)  (((x) & SMCR_ETPS_M) << 12)
#define SMCR_ETF_M    0xfu
#define SMCR_ETF(x)   (((x) & SMCR_ETF_M) << 8)
#define SMCR_MSM      (1 << 7)
#define SMCR_TS20_M   0x7u
#define SMCR_TS20(x)  (((x) & SMCR_TS20_M) << 4)
#define SMCR_SMS20_M  0x7u
#define SMCR_SMS20(x) ((x) & SMCR_SMS20_M)

#define DIER_TDE   (1 << 14)
#define DIER_COMDE (1 << 13)
#define DIER_CC4DE (1 << 12)
#define DIER_CC3DE (1 << 11)
#define DIER_CC2DE (1 << 10)
#define DIER_CC1DE (1 << 9)
#define DIER_UDE   (1 << 8)
#define DIER_BIE   (1 << 7)
#define DIER_TIE   (1 << 6)
#define DIER_COMIE (1 << 5)
#define DIER_CC4IE (1 << 4)
#define DIER_CC3IE (1 << 3)
#define DIER_CC2IE (1 << 2)
#define DIER_CC1IE (1 << 1)
#define DIER_UIE   (1 << 0)

#define SR_CC6IF (1 << 17)
#define SR_CC5IF (1 << 16)
#define SR_SBIF  (1 << 13)
#define SR_CC4OF (1 << 12)
#define SR_CC3OF (1 << 11)
#define SR_CC2OF (1 << 10)
#define SR_CC1OF (1 << 9)
#define SR_B2IF  (1 << 8)
#define SR_BIF   (1 << 7)
#define SR_TIF   (1 << 6)
#define SR_COMIF (1 << 5)
#define SR_CC4IF (1 << 4)
#define SR_CC3IF (1 << 3)
#define SR_CC2IF (1 << 1)
#define SR_UIF   (1 << 0)

#define C99_EGR_B2G  (1 << 8)
#define C99_EGR_BG   (1 << 7)
#define C99_EGR_TG   (1 << 6)
#define C99_EGR_COMG (1 << 5)
#define C99_EGR_CC4G (1 << 4)
#define C99_EGR_CC3G (1 << 3)
#define C99_EGR_CC2G (1 << 2)
#define C99_EGR_CC1G (1 << 1)
#define C99_EGR_UG   (1 << 0)

/* input */
#define CCMRx_ICyF_M    0xfu
#define CCMRx_ICyF(x)   (((x) & CCMRx_ICyF_M) << 4)
#define CCMRx_ICyPSC_M  0x3u
#define CCMRx_ICyPSC(x) (((x) & CCMRx_ICyPSC_M) << 2)
#define CCMRx_CCyS_M    0x3u
#define CCMRx_CCyS(x)   ((x) & CCMRx_CCyS_M)
/* capture */
#define CCMRx_OCyM3     (1 << 16)
#define CCMRx_OCyCE     (1 << 7)
#define CCMRx_OCyM20_M  0x7u
#define CCMRx_OCyM20(x) (((x) & CCMRx_OCyM20_M) << 4)
#define CCMRx_OCyPE     (1 << 3)
#define CCMRx_OCyFE     (1 << 2)
#define CCMRx_OCyS_M    0x3u
#define CCMRx_OCyS(x)   ((x) & CCMRx_OCyS_M)

#define CCER_CC6P  (1 << 21)
#define CCER_CC6E  (1 << 20)
#define CCER_CC5P  (1 << 17)
#define CCER_CC5E  (1 << 16)
#define CCER_CC4NP (1 << 15)
#define CCER_CC4P  (1 << 13)
#define CCER_CC4E  (1 << 12)
#define CCER_CC3NP (1 << 11)
#define CCER_CC3NE (1 << 10)
#define CCER_CC2NP (1 << 7)
#define CCER_CC2NE (1 << 6)
#define CCER_CC2P  (1 << 5)
#define CCER_CC2E  (1 << 4)
#define CCER_CC1NP (1 << 3)
#define CCER_CC1NE (1 << 2)
#define CCER_CC1P  (1 << 1)
#define CCER_CC1E  (1 << 0)

#define CNT_UIFCPY (1u << 31)
#define CNT_CNT_M  0xffffu
#define CNT_CNT(x) ((x) & CNT_CNT_M)

#define BDTR_BK2P    (1 << 25)
#define BDTR_BK2E    (1 << 24)
#define BDTR_BK2F_M  0xfu
#define BDTR_BK2F(x) (((x) & BDTR_BK2F_M) << 20)
#define BDTR_BKF_M   0xfu
#define BDTR_BKF(x)  (((x) & BDTR_BKF_M) << 16)
#define BDTR_MOE     (1 << 15)
#define BDTR_AOE     (1 << 14)
#define BDTR_BKP     (1 << 13)
#define BDTR_BKE     (1 << 12)
#define BDTR_OSSR    (1 << 11)
#define BDTR_OSSI    (1 << 10)
#define BDTR_LOCK_M  0x3u
#define BDTR_LOCK(x) (((x) & BDTR_LOCK_M) << 8)
#define BDTR_DTG_M   0xffu
#define BDTR_DTG(x)  ((x) & BDTR_DTG_M)

#define DCR_DBL_M  0x1fu
#define DCR_DBL(x) (((x) & DCR_DBL_M) << 8)
#define DCR_DBA_M  0x1fu
#define DCR_DBA(x) ((x) & DCR_DBA_M)

#define CCR5_GC5C3 (1u << 31)
#define CCR5_GC5C2 (1 << 30)
#define CCR5_GC5C1 (1 << 29)

#define AF1_ETRSEL_M  0xfu
#define AF1_ETRSEL(x) (((x) & AF1_ETRSEL_M) << 14)
#define AF1_BKCMP2P   (1 << 11)
#define AF1_BKCMP1P   (1 << 10)
#define AF1_BKINP     (1 << 9)
#define AF1_BKDF1BK2E (1 << 8)
#define AF1_BKCMP2E   (1 << 2)
#define AF1_BKCMP1E   (1 << 1)
#define AF1_BKNE      (1 << 0)

#define AF2_BK2CMP2P   (1 << 11)
#define AF2_BK2CMP1P   (1 << 10)
#define AF2_BK2INP     (1 << 9)
#define AF2_BK2DF1BK3E (1 << 8)
#define AF2_BK2CMP2E   (1 << 2)
#define AF2_BK2CMP1E   (1 << 1)
#define AF2_BK2NE      (1 << 0)

#define TISEL_TI1SEL_M  0xfu
#define TISEL_TI1SEL(x) ((x) & TISEL_TI1SEL_M)

/* Function: pwm_stm32h7xx_tim_init
 * Initializes a STM32H7xx TIM for PWM
 *
 * Parameters:
 *  ctx - The TIM/PWM to init
 *  base - The TIM base address
 *  clkid - The clock id for this TIM
 *  channel_count - The number of channels supported by this TIM
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_stm32h7xx_tim_init(struct pwm_stm32h7xx_tim *ctx, int base, clock_id_t clkid,
                           size_t channel_count)
{
    picoRTOS_assert(channel_count < (size_t)PWM_STM32H7XX_TIM_CHANNEL_MAX,
                    return -EINVAL);

    ctx->base = (struct PWM_STM32H7XX_TIM*)base;
    ctx->clkid = clkid;
    ctx->channel_count = channel_count;
    ctx->freq = clock_get_freq(clkid);

    picoRTOS_assert(ctx->freq > 0, return (int)ctx->freq);
    return 0;
}

/* Function: pwm_stm32h7xx_tim_setup
 * Configures a STM32H7xx TIM used as PWM
 *
 * Parameters:
 *  ctx - The TIM/PWM to configure
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_stm32h7xx_tim_setup(struct pwm_stm32h7xx_tim *ctx,
                            const struct pwm_stm32h7xx_tim_settings *settings)
{

    picoRTOS_assert(settings->align < PWM_STM32H7XX_TIM_ALIGN_COUNT,
                    return -EINVAL);

    /* zero field anyway */
    ctx->base->CR1 &= ~CR1_CMS(CR1_CMS_M);

    switch (settings->align) {
    case PWM_STM32H7XX_TIM_ALIGN_EDGE_UP: ctx->base->CR1 &= ~CR1_DIR;  break;
    case PWM_STM32H7XX_TIM_ALIGN_EDGE_DOWN: ctx->base->CR1 |= CR1_DIR; break;
    case PWM_STM32H7XX_TIM_ALIGN_CENTER_MODE_1: ctx->base->CR1 |= CR1_CMS(1); break;
    case PWM_STM32H7XX_TIM_ALIGN_CENTER_MODE_2: ctx->base->CR1 |= CR1_CMS(2); break;
    case PWM_STM32H7XX_TIM_ALIGN_CENTER_MODE_3: ctx->base->CR1 |= CR1_CMS(3); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

/* Function: pwm_stm32h7xx_tim_pwm_init
 * Initializes a STM32H7xx PWM channel
 *
 * Parameters:
 *  ctx - The PWM channel to init
 *  parent - The parent STM32H7xx TIM base address
 *  channel - The PWM channel number
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_stm32h7xx_tim_pwm_init(struct pwm *ctx,
                               struct pwm_stm32h7xx_tim *parent,
                               size_t channel)
{
    picoRTOS_assert(channel < (size_t)parent->channel_count, return -EINVAL);

    ctx->parent = parent;
    ctx->channel = channel;
    ctx->ncycles = 0;

    return 0;
}

static int set_oc(struct pwm *ctx, pwm_stm32h7xx_tim_oc_t oc)
{
    picoRTOS_assert(oc < PWM_STM32H7XX_TIM_OC_COUNT, return -EINVAL);

    volatile uint32_t *CCMRx;
    struct pwm_stm32h7xx_tim *parent = ctx->parent;
    size_t lshift = (size_t)((ctx->channel & 0x1) != 0 ? 8 : 0);

    switch (ctx->channel) {
    case 0: /*@fallthrough@*/
    case 1: CCMRx = &parent->base->CCMR1; break;
    case 2: /*@fallthrough@*/
    case 3: CCMRx = &parent->base->CCMR2; break;
    case 4: /*@fallthrough@*/
    case 5: CCMRx = &parent->base->CCMR3; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    /* configure channel as output */
    *CCMRx &= ~(CCMRx_OCyS(CCMRx_OCyS_M) << lshift);
    /* set mode */
    *CCMRx &= ~(CCMRx_OCyM20(CCMRx_OCyM20_M) << lshift);
    *CCMRx &= ~((unsigned long)CCMRx_OCyM3 << lshift);

    switch (oc) {
    case PWM_STM32H7XX_TIM_OC_PWM_MODE_1:
        *CCMRx |= (CCMRx_OCyM20(6) << lshift);
        break;

    case PWM_STM32H7XX_TIM_OC_PWM_MODE_2:
        *CCMRx |= (CCMRx_OCyM20(7) << lshift);
        break;

    case PWM_STM32H7XX_TIM_OC_COMBINED_PWM_MODE_1:
        *CCMRx |= (CCMRx_OCyM20(4) << lshift);
        *CCMRx |= ((unsigned long)CCMRx_OCyM3 << lshift);
        break;

    case PWM_STM32H7XX_TIM_OC_COMBINED_PWM_MODE_2:
        *CCMRx |= (CCMRx_OCyM20(5) << lshift);
        *CCMRx |= ((unsigned long)CCMRx_OCyM3 << lshift);
        break;

    case PWM_STM32H7XX_TIM_OC_ASYMETRIC_PWM_MODE_1:
        *CCMRx |= (CCMRx_OCyM20(6) << lshift);
        *CCMRx |= ((unsigned long)CCMRx_OCyM3 << lshift);
        break;

    case PWM_STM32H7XX_TIM_OC_ASYMETRIC_PWM_MODE_2:
        *CCMRx |= (CCMRx_OCyM20(7) << lshift);
        *CCMRx |= ((unsigned long)CCMRx_OCyM3 << lshift);
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    /* enable preload */
    *CCMRx |= ((size_t)CCMRx_OCyPE << lshift);
    parent->base->EGR = (uint32_t)C99_EGR_UG;

    return 0;
}

/* Function: pwm_stm32h7xx_tim_pwm_setup
 * Configures a STM32H7xx PWM channel
 *
 * Parameters:
 *  ctx - The PWM channel to configure
 *  settings - The settings to apply
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_stm32h7xx_tim_pwm_setup(struct pwm *ctx, const struct pwm_stm32h7xx_tim_pwm_settings *settings)
{
    int res;

    if ((res = set_oc(ctx, settings->oc)) < 0)
        return res;

    return 0;
}

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
{
#define PWM_STM32H7XX_PWM_ARR_MAX 0xffff

    picoRTOS_assert(period > 0, return -EINVAL);

    size_t psc = 0;
    struct pwm_stm32h7xx_tim *parent = ctx->parent;

    /* compute psc & arr */
    do {
        size_t hz = (size_t)parent->freq / ++psc;
        ctx->ncycles = (hz / (size_t)1000000) * (size_t)period;
    } while(ctx->ncycles > (size_t)PWM_STM32H7XX_PWM_ARR_MAX);

    picoRTOS_assert(ctx->ncycles > 0, return -EINVAL);

    parent->base->PSC = (uint32_t)psc - 1;
    parent->base->ARR = (uint32_t)ctx->ncycles;

    return 0;
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    struct pwm_stm32h7xx_tim *parent = ctx->parent;
    size_t CCRx = ((size_t)duty_cycle * ctx->ncycles) >> 16;

    switch (ctx->channel) {
    case 0: parent->base->CCR1 = (uint32_t)CCRx; break;
    case 1: parent->base->CCR2 = (uint32_t)CCRx; break;
    case 2: parent->base->CCR3 = (uint32_t)CCRx; break;
    case 3: parent->base->CCR4 = (uint32_t)CCRx; break;
    case 4: parent->base->CCR5 = (uint32_t)CCRx; break;
    case 5: parent->base->CCR6 = (uint32_t)CCRx; break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

void pwm_start(struct pwm *ctx)
{
    size_t lshift = ctx->channel * 4;
    struct pwm_stm32h7xx_tim *parent = ctx->parent;

    parent->base->CCER |= (1 << lshift);
    parent->base->CR1 |= CR1_CEN;
}

void pwm_stop(struct pwm *ctx)
{
    size_t lshift = ctx->channel * 4;
    struct pwm_stm32h7xx_tim *parent = ctx->parent;

    parent->base->CCER &= ~(1 << lshift);
    parent->base->CR1 &= ~CR1_CEN;
}
