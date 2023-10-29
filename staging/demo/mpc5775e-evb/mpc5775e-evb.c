#include "mpc5775e-evb.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "clock-nxp_siu.h"
#include "dma-nxp_edma.h"
#include "misc-nxp_igf.h"
#include "mux-nxp_siu.h"

static void clock_init(void)
{
    static struct clock_settings CLOCK_settings = {
        40000000ul,     /* xosc is 40Mhz */
        384000000ul,    /* pll0 is 384Mhz */
        2ul,            /* pll0_phi is 192Mhz */
        8ul,            /* pll0_phi1 is 48Mhz */
        264000000ul,    /* pll1 is 264Mhz */
        1ul,            /* sysclkdiv */
        2ul,            /* perdiv */
        2ul,            /* fmperdiv */
        1ul,            /* etpudiv */
    };

    (void)clock_nxp_siu_init(&CLOCK_settings);
}

static void mux_init(void)
{
    struct mux MUX;

    (void)mux_nxp_siu_init(&MUX, ADDR_SIU);

    /* gpios */
    (void)mux_nxp_siu_output(&MUX, (size_t)114, MUX_NXP_SIU_GPIO);  /* TICK: M_eTPUA0 */
    (void)mux_nxp_siu_output(&MUX, (size_t)179, MUX_NXP_SIU_GPIO);  /* LED0: eMIOS0 */
    (void)mux_nxp_siu_output(&MUX, (size_t)180, MUX_NXP_SIU_GPIO);  /* LED1: eMIOS1 */
    (void)mux_nxp_siu_output(&MUX, (size_t)181, MUX_NXP_SIU_GPIO);  /* LED2: eMIOS2 */
    (void)mux_nxp_siu_output(&MUX, (size_t)182, MUX_NXP_SIU_GPIO);  /* LED3: eMIOS3 */
}

static int adc_init(/*@partial@*/ struct mpc5775e_evb *ctx)
{
    static struct adc_nxp_eqadc ANB;
    static struct dma_nxp_edma DMA_B;
    /* dma channels */
    static struct dma DMA_B_CH0;    /* fill */
    static struct dma DMA_B_CH1;    /* drain */

    struct adc_nxp_eqadc_settings ANB_settings;

    /* setup dma (see doc for appropriate channel) */
    (void)dma_nxp_edma_init(&DMA_B, ADDR_EDMA1);
    (void)dma_nxp_edma_dma_init(&DMA_B_CH0, &DMA_B, (size_t)0);
    (void)dma_nxp_edma_dma_init(&DMA_B_CH1, &DMA_B, (size_t)1);

    /* setup eqadc (warning: incomplete) */
    ANB_settings.mode0 = ADC_NXP_EQADC_MODE0_SINGLE_SCAN;
    ANB_settings.fill = &DMA_B_CH0;
    ANB_settings.drain = &DMA_B_CH1;

    (void)adc_nxp_eqadc_init(&ANB, ADDR_EQADC1);
    (void)adc_nxp_eqadc_setup(&ANB, &ANB_settings);

    /* channel */
    (void)adc_nxp_eqadc_adc_init(&ctx->ADC, &ANB, (size_t)20);

    return 0;
}

static int gpio_init(/*@partial@*/ struct mpc5775e_evb *ctx)
{
    (void)gpio_nxp_siu_init(&ctx->TICK, ADDR_GPIO_PTD, (size_t)18);
    (void)gpio_nxp_siu_init(&ctx->LED[0], ADDR_GPIO_PTF, (size_t)19);
    (void)gpio_nxp_siu_init(&ctx->LED[1], ADDR_GPIO_PTF, (size_t)20);
    (void)gpio_nxp_siu_init(&ctx->LED[2], ADDR_GPIO_PTF, (size_t)21);
    (void)gpio_nxp_siu_init(&ctx->LED[3], ADDR_GPIO_PTF, (size_t)22);

    return 0;
}

static int pwm_init(/*@partial@*/ struct mpc5775e_evb *ctx)
{
    static struct nxp_igf IGF0;
    static struct pwm_nxp_emios eMIOS0;
    struct pwm_nxp_emios_settings eMIOS_settings = {
        (size_t)1, /* GPRE */
    };

    (void)pwm_nxp_emios_init(&eMIOS0, ADDR_EMIOS0, CLOCK_NXP_SIU_PER_CLK);
    (void)pwm_nxp_emios_setup(&eMIOS0, &eMIOS_settings);

    struct pwm_nxp_emios_pwm_settings PWM_settings = {
        PWM_NXP_EMIOS_UCPRE_1,  /* ucpre */
        false,                  /* polarity */
    };

    (void)pwm_nxp_emios_pwm_init(&ctx->PWM, &eMIOS0, (size_t)0);
    (void)pwm_nxp_emios_pwm_setup(&ctx->PWM, &PWM_settings);

    struct pwm_nxp_emios_ipwm_settings IPWM_settings = {
        PWM_NXP_EMIOS_UCPRE_1,      /* ucpre */
        false,                      /* polarity */
        IPWM_NXP_EMIOS_IF_BYPASSED, /* filter */
    };

    (void)pwm_nxp_emios_ipwm_init(&ctx->IPWM, &eMIOS0, (size_t)1);
    (void)pwm_nxp_emios_ipwm_setup(&ctx->IPWM, &IPWM_settings);

    (void)nxp_igf_init(&IGF0, ADDR_IGF0, (size_t)1);

    return 0;
}

int mpc5775e_evb_init(struct mpc5775e_evb *ctx)
{
    clock_init();
    mux_init();

    //(void)adc_init(ctx);
    (void)gpio_init(ctx);
    //(void)pwm_init(ctx);

    return 0;
}
