#include "pwm-ti_epwm.h"
#include "picoRTOS.h"

#include <stdint.h>

struct C99_EPWM_REGS {
    volatile uint16_t TBCTL;
    volatile uint16_t TBCTL2;
    uint16_t RESERVED0[2];
    volatile uint16_t TBCTR;
    volatile uint16_t TBSTS;
    uint16_t RESERVED1[2];
    volatile uint16_t CMPCTL;
    volatile uint16_t CMPCTL2;
    uint16_t RESERVED2[2];
    volatile uint16_t DBCTL;
    volatile uint16_t DBCTL2;
    uint16_t RESERVED3[2];
    volatile uint16_t AQCTL;
    volatile uint16_t AQTSRCSEL;
    uint16_t RESERVED4[2];
    volatile uint16_t PCCTL;
    uint16_t RESERVED5[3];
    volatile uint16_t VCAPCTL;
    volatile uint16_t VCNTCFG;
    uint16_t RESERVED6[6];
    volatile uint16_t HRCNFG;
    volatile uint16_t HRPWR;
    uint16_t RESERVED7[4];
    volatile uint16_t HRMSTEP;
    volatile uint16_t HRNCFG2;
    uint16_t RESERVED8[5];
    volatile uint16_t HRPCTL;
    volatile uint16_t TRREM;
    uint16_t RESERVED9[5];
    volatile uint16_t GLDCTL;
    volatile uint16_t GLDCFG;
    uint16_t RESERVED10[2];
    volatile uint32_t TI_EPWMXLINK;
    uint16_t RESERVED11[6];
    volatile uint16_t AQCTLA;
    volatile uint16_t AQCTLA2;
    volatile uint16_t AQCTLB;
    volatile uint16_t AQCTLB2;
    uint16_t RESERVED12[3];
    volatile uint16_t AQSFRC;
    uint16_t RESERVED13;
    volatile uint16_t AQCSFRC;
    uint16_t RESERVED14[6];
    volatile uint16_t DBREDHR;
    volatile uint16_t DBRED;
    volatile uint16_t DBFEDHR;
    volatile uint16_t DBFED;
    uint16_t RESERVED15[12];
    volatile uint32_t TBPHS;
    volatile uint16_t TBPRDHR;
    volatile uint16_t TBPRD;
    uint16_t RESERVED16[5];
    volatile uint32_t CMPA;
    volatile uint32_t CMPB;
    uint16_t RESERVED17;
    volatile uint16_t CMPC;
    uint16_t RESERVED18;
    volatile uint16_t CMPD;
    uint16_t RESERVED19[2];
    volatile uint16_t GLDCTL2;
    uint16_t RESERVED20[2];
    volatile uint16_t SWVDELVAL;
    uint16_t RESERVED21[8];
    volatile uint16_t TZSEL;
    uint16_t RESERVED22;
    volatile uint16_t TZDCSEL;
    uint16_t RESERVED23;
    volatile uint16_t TZCTL;
    volatile uint16_t TZCTL2;
    volatile uint16_t TZCTLDCA;
    volatile uint16_t TZCTLDCB;
    uint16_t RESERVED24[5];
    volatile uint16_t TZEINT;
    uint16_t RESERVED25[5];
    volatile uint16_t TZFLG;
    volatile uint16_t TZBCFLG;
    volatile uint16_t TZOSTFLG;
    uint16_t RESERVED26;
    volatile uint16_t TZCLR;
    volatile uint16_t TZCBCCLR;
    volatile uint16_t TZOSCCLR;
    uint16_t RESERVED27;
    volatile uint16_t TZFRC;
    uint16_t RESERVED28[8];
    volatile uint16_t ETSEL;
    uint16_t RESERVED29;
    volatile uint16_t ETPS;
    uint16_t RESERVED30;
    volatile uint16_t ETFLG;
    uint16_t RESERVED31;
    volatile uint16_t ETCLR;
    uint16_t RESERVED32;
    volatile uint16_t ETFRC;
    uint16_t RESERVED33;
    volatile uint16_t ETINTPS;
    uint16_t RESERVED34;
    volatile uint16_t ETSOCPS;
    uint16_t RESERVED35;
    volatile uint16_t ETCNTINITCTL;
    uint16_t RESERVED36;
    volatile uint16_t ETCNTINIT;
    uint16_t RESERVED37[11];
    volatile uint16_t DCTRIPSEL;
    uint16_t RESERVED38[2];
    volatile uint16_t DCACTL;
    volatile uint16_t DCBCTL;
    uint16_t RESERVED39[2];
    volatile uint16_t DCFCTL;
    volatile uint16_t DCCAPCTL;
    volatile uint16_t DCFOFFSET;
    volatile uint16_t DCFOFFSETCNT;
    volatile uint16_t DCFWINDOW;
    volatile uint16_t DCFWINDOWCNT;
    uint16_t RESERVED40[2];
    volatile uint16_t DCCAP;
    uint16_t RESERVED41[2];
    volatile uint16_t DCAHTRIPSEL;
    volatile uint16_t DCALTRIPSEL;
    volatile uint16_t DCBHTRIPSEL;
    volatile uint16_t DCBLTRIPSEL;
    uint16_t RESERVED42[39];
    volatile uint16_t HWVDELVAL;
    volatile uint16_t VCNTVAL;
};

#define TBCTL_CLKDIV_M     0x7u
#define TBCTL_CLKDIV(x)    (((x) & TBCTL_CLKDIV_M) << 10)
#define TBCTL_HSPCLKDIV_M  0x7u
#define TBCTL_HSPCLKDIV(x) (((x) & TBCTL_HSPCLKDIV_M) << 7)
#define TBCTL_CTRMODE_M    0x3u
#define TBCTL_CTRMODE(x)   ((x) & TBCTL_CTRMODE_M)

#define AQCTLn_ZRO_M  0x3u
#define AQCTLn_ZRO(x) ((x) & AQCTLn_ZRO_M)
#define AQCTLn_PRD_M  0x3u
#define AQCTLn_PRD(x) (((x) & AQCTLn_PRD_M) << 2)
#define AQCTLn_CAU_M  0x3u
#define AQCTLn_CAU(x) (((x) & AQCTLn_CAU_M) << 4)
#define AQCTLn_CAD_M  0x3u
#define AQCTLn_CAD(x) (((x) & AQCTLn_CAD_M) << 6)
#define AQCTLn_CBU_M  0x3u
#define AQCTLn_CBU(x) (((x) & AQCTLn_CBU_M) << 8)
#define AQCTLn_CBD_M  0x3u
#define AQCTLn_CBD(x) (((x) & AQCTLn_CBD_M) << 10)

#define CMPA_CMPA_M  0xffffu
#define CMPA_CMPA(x) (((x) & CMPA_CMPA_M) << 16)

#define CMPB_CMPB_M  0xffffu
#define CMPB_CMPB(x) (((x) & CMPB_CMPB_M) << 16)

typedef enum {
    AQCTLn_ACTION_ZRO,
    AQCTLn_ACTION_PRD,
    AQCTLn_ACTION_CAU,
    AQCTLn_ACTION_CAD,
    AQCTLn_ACTION_CBU,
    AQCTLn_ACTION_CBD,
    AQCTLn_ACTION_COUNT
} aqctl_action_t;

/* Function: pwm_ti_epwm_init
 * Inits an TI_EPWM block
 *
 * Parameters:
 *  ctx - The ti_epwm block to init
 *  base - The TI_EPWM block base address
 *  clkid - The TI_EPWM input clock id
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_ti_epwm_init(struct pwm_ti_epwm *ctx, struct C99_EPWM_REGS *base, clock_id_t clkid)
{
    ctx->base = base;
    ctx->clk_freq = clock_get_freq(clkid);
    ctx->waveform = PWM_TI_EPWM_WAVEFORM_COUNT;
    ctx->freq = 0;

    if (!picoRTOS_assert(ctx->clk_freq > 0))
        return (int)ctx->freq;

    ctx->base->TBCTR = 0; /* zero-out counter */

    return 0;
}

static int set_tbclk(struct pwm_ti_epwm *ctx, unsigned long hz)
{
    if (!picoRTOS_assert(hz > 0)) return -EINVAL;

    uint16_t i, j;
    uint32_t clkdiv = (uint32_t)1;
    uint32_t clk = (uint32_t)ctx->clk_freq;

    for (i = 0; i < (uint16_t)TBCTL_CLKDIV_M; i++) {

        uint32_t hspclkdiv = 0;

        for (j = 0; j < (uint16_t)TBCTL_HSPCLKDIV_M; j++) {

            uint32_t tbclk;

            /* exception when hspclkdiv is 0 */
            if (hspclkdiv == 0)
                tbclk = (uint32_t)((float)clk / clkdiv);
            else
                tbclk = (uint32_t)((float)clk / (clkdiv * hspclkdiv));

            if (tbclk == (uint32_t)hz) {
                /* frequencies match */
                ctx->base->TBCTL &= ~TBCTL_HSPCLKDIV(TBCTL_HSPCLKDIV_M);
                ctx->base->TBCTL |= TBCTL_HSPCLKDIV(j);
                ctx->base->TBCTL &= ~TBCTL_CLKDIV(TBCTL_CLKDIV_M);
                ctx->base->TBCTL |= TBCTL_CLKDIV(i);
                return 0;
            }

            /* increment (+2) hspclkdiv */
            hspclkdiv += 2ul;
        }

        /* increase (*2) clkdiv */
        clkdiv *= 2ul;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int set_waveform(struct pwm_ti_epwm *ctx, pwm_ti_epwm_waveform_t wf)
{
    if (!picoRTOS_assert(wf < PWM_TI_EPWM_WAVEFORM_COUNT)) return -EINVAL;

    switch (wf) {
    case PWM_TI_EPWM_WAVEFORM_UP:       /*@fallthrough@*/
    case PWM_TI_EPWM_WAVEFORM_DOWN:     /*@fallthrough@*/
    case PWM_TI_EPWM_WAVEFORM_UPDOWN:
        ctx->waveform = wf;
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EINVAL;
    }

    return 0;
}

static int set_output_a(struct pwm_ti_epwm *ctx, aqctl_action_t act, pwm_ti_epwm_aq_t aq)
{
    if (!picoRTOS_assert(act < AQCTLn_ACTION_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(aq < PWM_TI_EPWM_AQ_COUNT)) return -EINVAL;

    switch (act) {
    case AQCTLn_ACTION_ZRO:
        ctx->base->AQCTLA &= ~AQCTLn_ZRO(AQCTLn_ZRO_M);
        ctx->base->AQCTLA |= AQCTLn_ZRO(aq);
        break;

    case AQCTLn_ACTION_PRD:
        ctx->base->AQCTLA &= ~AQCTLn_PRD(AQCTLn_PRD_M);
        ctx->base->AQCTLA |= AQCTLn_PRD(aq);
        break;

    case AQCTLn_ACTION_CAU:
        ctx->base->AQCTLA &= ~AQCTLn_CAU(AQCTLn_CAU_M);
        ctx->base->AQCTLA |= AQCTLn_CAU(aq);
        break;

    case AQCTLn_ACTION_CAD:
        ctx->base->AQCTLA &= ~AQCTLn_CAD(AQCTLn_CAD_M);
        ctx->base->AQCTLA |= AQCTLn_CAD(aq);
        break;

    case AQCTLn_ACTION_CBU:
        ctx->base->AQCTLA &= ~AQCTLn_CBU(AQCTLn_CBU_M);
        ctx->base->AQCTLA |= AQCTLn_CBU(aq);
        break;

    case AQCTLn_ACTION_CBD:
        ctx->base->AQCTLA &= ~AQCTLn_CBD(AQCTLn_CBD_M);
        ctx->base->AQCTLA |= AQCTLn_CBD(aq);
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

static int set_output_b(struct pwm_ti_epwm *ctx, aqctl_action_t act, pwm_ti_epwm_aq_t aq)
{
    if (!picoRTOS_assert(act < AQCTLn_ACTION_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(aq < PWM_TI_EPWM_AQ_COUNT)) return -EINVAL;

    switch (act) {
    case AQCTLn_ACTION_ZRO:
        ctx->base->AQCTLB &= ~AQCTLn_ZRO(AQCTLn_ZRO_M);
        ctx->base->AQCTLB |= AQCTLn_ZRO(aq);
        break;

    case AQCTLn_ACTION_PRD:
        ctx->base->AQCTLB &= ~AQCTLn_PRD(AQCTLn_PRD_M);
        ctx->base->AQCTLB |= AQCTLn_PRD(aq);
        break;

    case AQCTLn_ACTION_CAU:
        ctx->base->AQCTLB &= ~AQCTLn_CAU(AQCTLn_CAU_M);
        ctx->base->AQCTLB |= AQCTLn_CAU(aq);
        break;

    case AQCTLn_ACTION_CAD:
        ctx->base->AQCTLB &= ~AQCTLn_CAD(AQCTLn_CAD_M);
        ctx->base->AQCTLB |= AQCTLn_CAD(aq);
        break;

    case AQCTLn_ACTION_CBU:
        ctx->base->AQCTLB &= ~AQCTLn_CBU(AQCTLn_CBU_M);
        ctx->base->AQCTLB |= AQCTLn_CBU(aq);
        break;

    case AQCTLn_ACTION_CBD:
        ctx->base->AQCTLB &= ~AQCTLn_CBD(AQCTLn_CBD_M);
        ctx->base->AQCTLB |= AQCTLn_CBD(aq);
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/
        return -EIO;
    }

    return 0;
}

/* Function: pwm_ti_epwm_setup
 * Configures an TI_EPWM block
 *
 * Parameters:
 *  ctx - The ti_epwm block to configure
 *  settings - The ti_epwm specific parameters
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_ti_epwm_setup(struct pwm_ti_epwm *ctx, struct pwm_ti_epwm_settings *settings)
{
    int res;

    if ((res = set_tbclk(ctx, settings->frequency)) < 0 ||
        (res = set_waveform(ctx, settings->waveform)) < 0)
        return res;

    ctx->freq = settings->frequency;
    return 0;
}

/* Function: pwm_ti_epwm_pwm_init
 * Creates a PWM output from an TI_EPWM block
 *
 * Parameters:
 *  ctx - The pwm to init
 *  parent - The parent ePWM block
 *  cmp - The pwm output (A/B)
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_ti_epwm_pwm_init(struct pwm *ctx, struct pwm_ti_epwm *parent, pwm_ti_epwm_cmp_t cmp)
{
    if (!picoRTOS_assert(cmp < PWM_TI_EPWM_CMP_COUNT)) return -EINVAL;

    ctx->parent = parent;
    ctx->cmp = cmp;
    ctx->ncycles = 0;

    return 0;
}

static int setup_output_a(struct pwm *ctx, struct pwm_ti_epwm_pwm_settings *settings)
{
    int res;

    if ((res = set_output_a(ctx->parent, AQCTLn_ACTION_ZRO, settings->zero_aq)) < 0 ||
        (res = set_output_a(ctx->parent, AQCTLn_ACTION_PRD, settings->prd_aq)) < 0 ||
        (res = set_output_a(ctx->parent, AQCTLn_ACTION_CAU, settings->cmpa_up_aq)) < 0 ||
        (res = set_output_a(ctx->parent, AQCTLn_ACTION_CAD, settings->cmpa_down_aq)) < 0 ||
        (res = set_output_a(ctx->parent, AQCTLn_ACTION_CBU, settings->cmpb_up_aq)) < 0 ||
        (res = set_output_a(ctx->parent, AQCTLn_ACTION_CBD, settings->cmpb_down_aq)) < 0)
        return res;

    return 0;
}

static int setup_output_b(struct pwm *ctx, struct pwm_ti_epwm_pwm_settings *settings)
{
    int res;

    if ((res = set_output_b(ctx->parent, AQCTLn_ACTION_ZRO, settings->zero_aq)) < 0 ||
        (res = set_output_b(ctx->parent, AQCTLn_ACTION_PRD, settings->prd_aq)) < 0 ||
        (res = set_output_b(ctx->parent, AQCTLn_ACTION_CAU, settings->cmpa_up_aq)) < 0 ||
        (res = set_output_b(ctx->parent, AQCTLn_ACTION_CAD, settings->cmpa_down_aq)) < 0 ||
        (res = set_output_b(ctx->parent, AQCTLn_ACTION_CBU, settings->cmpb_up_aq)) < 0 ||
        (res = set_output_b(ctx->parent, AQCTLn_ACTION_CBD, settings->cmpb_down_aq)) < 0)
        return res;

    return 0;
}

/* Function: pwm_ti_epwm_pwm_setup
 * Configures a PWM output from an TI_EPWM block
 *
 * Parameters:
 *  ctx - The pwm to init
 *  settings - The pwm output settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_ti_epwm_pwm_setup(struct pwm *ctx, struct pwm_ti_epwm_pwm_settings *settings)
{
    switch (ctx->cmp) {
    case PWM_TI_EPWM_CMPA: return setup_output_a(ctx, settings);
    case PWM_TI_EPWM_CMPB: return setup_output_b(ctx, settings);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
{
    if (!picoRTOS_assert(period > 0)) return -EINVAL;

    struct pwm_ti_epwm *parent = ctx->parent;

    uint32_t hz = (uint32_t)parent->freq;

    ctx->ncycles = (hz / (uint32_t)1000000ul) * (uint32_t)period;

    if (!picoRTOS_assert(ctx->ncycles > 0))
        return -EINVAL;

    parent->base->TBPRD = (uint16_t)ctx->ncycles;

    return 0;
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    struct pwm_ti_epwm *parent = ctx->parent;
    uint32_t value = (uint32_t)duty_cycle * ctx->ncycles / (uint32_t)PWM_DUTY_CYCLE_MAX;

    switch (ctx->cmp) {
    case PWM_TI_EPWM_CMPA: parent->base->CMPA = (uint32_t)CMPA_CMPA(value); break;
    case PWM_TI_EPWM_CMPB: parent->base->CMPB = (uint32_t)CMPB_CMPB(value); break;
    default:
        picoRTOS_break();
        /*@notreached@*/ return -EIO;
    }

    return 0;
}

void pwm_start(struct pwm *ctx)
{
    struct pwm_ti_epwm *parent = ctx->parent;

    parent->base->TBCTL &= ~TBCTL_CTRMODE(TBCTL_CTRMODE_M);
    parent->base->TBCTL |= TBCTL_CTRMODE(parent->waveform);
}

void pwm_stop(struct pwm *ctx)
{
    struct pwm_ti_epwm *parent = ctx->parent;

    parent->base->TBCTL &= ~TBCTL_CTRMODE(TBCTL_CTRMODE_M);
}
