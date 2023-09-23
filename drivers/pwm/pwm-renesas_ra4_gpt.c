#include "pwm-renesas_ra4_gpt.h"
#include "picoRTOS.h"

#include <stdint.h>

struct PWM_RENESAS_RA4_GPT {
    volatile uint32_t GTWP;
    volatile uint32_t GTSTR;
    volatile uint32_t GTSTP;
    volatile uint32_t GTCLR;
    volatile uint32_t GTSSR;
    volatile uint32_t GTPSR;
    volatile uint32_t GTCSR;
    volatile uint32_t GTUPSR;
    volatile uint32_t GTDNSR;
    volatile uint32_t GTICASR;
    volatile uint32_t GTICBSR;
    volatile uint32_t GTCR;
    volatile uint32_t GTUDDTYC;
    volatile uint32_t GTIOR;
    volatile uint32_t GTINTAD;
    volatile uint32_t GTST;
    volatile uint32_t GTBER;
    uint32_t RESERVED0;
    volatile uint32_t GTCNT;
    volatile uint32_t GTCCRA;
    volatile uint32_t GTCCRB;
    volatile uint32_t GTCCRC;
    volatile uint32_t GTCCRE;
    volatile uint32_t GTCCRD;
    volatile uint32_t GTCCRF;
    volatile uint32_t GTPR;
    volatile uint32_t GTPBR;
    uint32_t RESERVED1[7];
    volatile uint32_t GTDTCR;
    volatile uint32_t GTDVU;
};

#define GTWP_PRKEY_M  0xffu
#define GTWP_PRKEY(x) (((x) & GTWP_PRKEY_M) << 8)
#define GTWP_WP       (1 << 0)

#define GTSSR_CSTRT    (1u << 31)
#define GTSSR_SSELCH   (1 << 23)
#define GTSSR_SSELCG   (1 << 22)
#define GTSSR_SSELCF   (1 << 21)
#define GTSSR_SSELCE   (1 << 20)
#define GTSSR_SSELCD   (1 << 19)
#define GTSSR_SSELCC   (1 << 18)
#define GTSSR_SSELCB   (1 << 17)
#define GTSSR_SSELCA   (1 << 16)
#define GTSSR_SSCBFAH  (1 << 15)
#define GTSSR_SSCBFAL  (1 << 14)
#define GTSSR_SSCBRAH  (1 << 13)
#define GTSSR_SSCBRAL  (1 << 12)
#define GTSSR_SSCAFBH  (1 << 11)
#define GTSSR_SSCAFBL  (1 << 10)
#define GTSSR_SSCARBH  (1 << 9)
#define GTSSR_SSCARBL  (1 << 8)
#define GTSSR_SSGTRGBF (1 << 3)
#define GTSSR_SSGTRGBR (1 << 2)
#define GTSSR_SSGTRGAF (1 << 1)
#define GTSSR_SSGTRGAR (1 << 0)

/* ... */

#define GTCR_TPCS_M  0x7u
#define GTCR_TPCS(x) (((x) & GTCR_TPCS_M) << 24)
#define GTCR_MD_M    0x7u
#define GTCR_MD(x)   (((x) & GTCR_MD_M) << 16)
#define GTCR_CST     (1 << 0)

#define GTUDDTYC_OBDTYR   (1 << 27)
#define GTUDDTYC_OBDTYF   (1 << 26)
#define GTUDDTYC_OBDTY_M  0x3u
#define GTUDDTYC_OBDTY(x) (((x) & GTUDDTYC_OBDTY_M) << 24)
#define GTUDDTYC_OADTYR   (1 << 19)
#define GTUDDTYC_OADTYF   (1 << 18)
#define GTUDDTYC_OADTY_M  0x3u
#define GTUDDTYC_OADTY(x) (((x) & GTUDDTYC_OADTY_M) << 16)
#define GTUDDTYC_UDF      (1 << 1)
#define GTUDDTYC_UD       (1 << 0)

#define GTIOR_NFCSB_M  0x3u
#define GTIOR_NFCSB(x) (((x) & GTIOR_NFCSB_M) << 30)
#define GTIOR_NFBEN    (1 << 29)
#define GTIOR_OBDF_M   0x3u
#define GTIOR_OBDF(x)  (((x) & GTIOR_OBDF_M) << 25)
#define GTIOR_OBE      (1 << 24)
#define GTIOR_OBHLD    (1 << 23)
#define GTIOR_OBDFLT   (1 << 22)
#define GTIOR_GTIOB_M  0x1fu
#define GTIOR_GTIOB(x) (((x) & GTIOR_GTIOB_M) << 16)
#define GTIOR_NFCSA_M  0x3u
#define GTIOR_NFCSA(x) (((x) & GTIOR_NFCSA_M) << 14)
#define GTIOR_NFAEN    (1 << 13)
#define GTIOR_OADF_M   0x3u
#define GTIOR_OADF(x)  (((x) & GTIOR_OADF_M) << 9)
#define GTIOR_OAE      (1 << 8)
#define GTIOR_OAHLD    (1 << 7)
#define GTIOR_OADFLT   (1 << 6)
#define GTIOR_GTIOA_M  0x1fu
#define GTIOR_GTIOA(x) ((x) & GTIOR_GTIOA_M)

/* TODO: OPS */

static void unlock(struct pwm_renesas_ra4_gpt *ctx)
{
    ctx->base->GTWP = (uint32_t)GTWP_PRKEY(0xa5);
}

static void lock(struct pwm_renesas_ra4_gpt *ctx)
{
    ctx->base->GTWP = (uint32_t)GTWP_PRKEY(0xa5) | GTWP_WP;
}

/* Function: pwm_renesas_ra4_gpt_init
 * Inits an Reneas RA4 GPT PWM timer
 *
 * Parameters:
 *  ctx - The GPT timer to init
 *  base - The GPT timer base address
 *  clkid - The GPT timer input clock id
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_renesas_ra4_gpt_init(struct pwm_renesas_ra4_gpt *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct PWM_RENESAS_RA4_GPT*)base;
    ctx->freq = clock_get_freq(clkid);

    unlock(ctx);
    ctx->base->GTSSR = (uint32_t)GTSSR_CSTRT;
    lock(ctx);

    return 0;
}

/* OUTPUTs */

/* Function: pwm_renesas_ra4_gpt_pwm_init
 * Creates a PWM output from a GPT timer
 *
 * Parameters:
 *  ctx - The ti_epwm block to configure
 *  parent - The parent GPT timer
 *  output - The output pin selection
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_renesas_ra4_gpt_pwm_init(struct pwm *ctx,
                                 struct pwm_renesas_ra4_gpt *parent,
                                 pwm_renesas_ra4_gpt_output_t output)
{
    picoRTOS_assert(output < PWM_RENESAS_RA4_GPT_OUTPUT_COUNT, return -EINVAL);

    ctx->parent = parent;
    ctx->output = output;
    ctx->ncycles = 0;

    return 0;
}

/* Function: pwm_renesas_ra4_gpt_pwm_setup
 * Configures a PWM output
 *
 * Parameters:
 *  ctx - The pwm to init
 *  settings - The pwm output settings
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int pwm_renesas_ra4_gpt_pwm_setup(struct pwm *ctx,
                                  struct pwm_renesas_ra4_gpt_pwm_settings *settings)
{
    picoRTOS_assert(settings->oadf < PWM_RENESAS_RA4_GPT_OADF_COUNT, return -EINVAL);
    picoRTOS_assert(settings->pin_function_select < PWM_RENESAS_RA4_PWM_PIN_FUNCTION_SELECT_COUNT,
                    return -EINVAL);

    /* OUTPUT_A */
    size_t shift = 0;

    /* OUTPUT_B */
    if (ctx->output == PWM_RENESAS_RA4_GPT_OUTPUT_B)
        shift = (size_t)16;

    unlock(ctx->parent);

    /* reset output */
    ctx->parent->base->GTIOR &= ~(uint32_t)(0xffffu << shift);
    ctx->parent->base->GTIOR |= (GTIOR_OADF(settings->oadf) |
                                 GTIOR_OAE |
                                 (settings->output_setting_at_start_stop ? GTIOR_OAHLD : 0) |
                                 (settings->output_value_at_stop ? GTIOR_OADFLT : 0) |
                                 GTIOR_GTIOA(settings->pin_function_select)) << shift;

    lock(ctx->parent);
    return 0;
}

int pwm_set_period(struct pwm *ctx, pwm_period_us_t period)
{
    /* ignore 32bit for the moment */
#define GTPR_MAX 0xffff

    picoRTOS_assert(period != 0, return -EINVAL);

    size_t i;
    static const unsigned long tpcs[] = { 1ul, 4ul, 16ul, 64ul, 256ul, 1024ul };

    /* real channel input frequency */
    for (i = 0; i < (size_t)6; i++) {

        unsigned long hz = (unsigned long)ctx->parent->freq / tpcs[i];

        ctx->ncycles = (size_t)(hz / 1000000ul) * (size_t)period;
        if (ctx->ncycles <= (size_t)GTPR_MAX) {
            /* everything is ok */
            unlock(ctx->parent);
            ctx->parent->base->GTPR = (uint32_t)ctx->ncycles;
            ctx->parent->base->GTCR &= ~GTCR_TPCS(GTCR_TPCS_M);
            ctx->parent->base->GTCR |= GTCR_TPCS(i);
            lock(ctx->parent);
            return 0;
        }
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

int pwm_set_duty_cycle(struct pwm *ctx, pwm_duty_cycle_t duty_cycle)
{
    size_t gtccrx = ((size_t)duty_cycle * ctx->ncycles) >> 16;

    unlock(ctx->parent);

    if (ctx->output == PWM_RENESAS_RA4_GPT_OUTPUT_A)
        ctx->parent->base->GTCCRA = (uint32_t)gtccrx;
    else
        ctx->parent->base->GTCCRB = (uint32_t)gtccrx;

    lock(ctx->parent);
    return 0;
}

void pwm_start(struct pwm *ctx)
{
    unlock(ctx->parent);
    ctx->parent->base->GTCR |= GTCR_CST;
    lock(ctx->parent);
}

void pwm_stop(struct pwm *ctx)
{
    unlock(ctx->parent);
    ctx->parent->base->GTCR &= ~GTCR_CST;
    lock(ctx->parent);
}
