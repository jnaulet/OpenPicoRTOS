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

    /* pwm */
    (void)mux_nxp_siu_output(&MUX, (size_t)181, MUX_NXP_SIU_ALT1);  /* PWM:  eMIOS2 */

    /* spi */
    (void)mux_nxp_siu_input(&MUX, (size_t)190, MUX_NXP_SIU_ALT2);   /* SPI_D:  eMIOS11 */
    (void)mux_nxp_siu_output(&MUX, (size_t)192, MUX_NXP_SIU_ALT2);  /* SPI_D:  eMIOS13 */
    (void)mux_nxp_siu_imux_muxsel(&MUX, (size_t)2, (size_t)10, (size_t)2);
}

static int adc_init(/*@partial@*/ struct mpc5775e_evb *ctx)
{
    static struct adc_nxp_eqadc ANA;
    static struct dma_nxp_edma DMA_A;
    /* dma channels */
    static struct dma DMA_A_CH0;    /* fill */
    static struct dma DMA_A_CH1;    /* drain */
    struct adc_nxp_eqadc_settings ANA_settings;

    /* setup dma (see doc for appropriate channel) */
    (void)dma_nxp_edma_init(&DMA_A, ADDR_EDMAA);
    (void)dma_nxp_edma_dma_init(&DMA_A_CH0, &DMA_A, (size_t)0);
    (void)dma_nxp_edma_dma_init(&DMA_A_CH1, &DMA_A, (size_t)1);

    /* setup eqadc (warning: incomplete) */
    ANA_settings.mode0 = ADC_NXP_EQADC_MODE0_SINGLE_SCAN;
    ANA_settings.fill = &DMA_A_CH0;
    ANA_settings.drain = &DMA_A_CH1;

    (void)adc_nxp_eqadc_init(&ANA, ADDR_EQADCA);
    (void)adc_nxp_eqadc_setup(&ANA, &ANA_settings);

    /* channel */
    (void)adc_nxp_eqadc_adc_init(&ctx->ADC, &ANA, (size_t)3);
    return 0;
}

static int gpio_init(/*@partial@*/ struct mpc5775e_evb *ctx)
{
    (void)gpio_nxp_siu_init(&ctx->TICK, ADDR_GPIO_PTD, (size_t)18);
    (void)gpio_nxp_siu_init(&ctx->LED0, ADDR_GPIO_PTF, (size_t)19);
    (void)gpio_nxp_siu_init(&ctx->LED1, ADDR_GPIO_PTF, (size_t)20);

    return 0;
}

static int pwm_init(/*@partial@*/ struct mpc5775e_evb *ctx)
{
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

    (void)pwm_nxp_emios_pwm_init(&ctx->PWM, &eMIOS0, (size_t)2);
    (void)pwm_nxp_emios_pwm_setup(&ctx->PWM, &PWM_settings);

    return 0;
}

static int spi_init(/*@partial@*/ struct mpc5775e_evb *ctx)
{
    struct spi_settings SPI_settings = {
        1000000ul,     /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)16, /* frame size */
        SPI_CS_POL_ACTIVE_LOW,
        0           /* cs */
    };

    (void)spi_nxp_dspi_init(&ctx->SPI, ADDR_DSPID, CLOCK_NXP_SIU_XOSC);
    (void)spi_setup(&ctx->SPI, &SPI_settings);

    return 0;
}

static int can_init(/*@partial@*/ struct mpc5775e_evb *ctx)
{
    struct can_settings CAN_settings = {
        500000ul,               /* bitrate */
        (size_t)2,              /* tx_mailbox_count */
        CAN_TX_AUTO_ABORT_ON,   /* tx_auto_abort */
        CAN_RX_OVERWRITE_OFF,   /* rx_overwrite */
        CAN_LOOPBACK_ON         /* loopback */
    };

    (void)can_nxp_flexcan_init(&ctx->CAN, ADDR_FLEXCANA, CLOCK_NXP_SIU_XOSC, (size_t)64);
    (void)can_setup(&ctx->CAN, &CAN_settings);

    return 0;
}

int mpc5775e_evb_init(struct mpc5775e_evb *ctx)
{
    clock_init();
    mux_init();

    (void)adc_init(ctx);
    (void)gpio_init(ctx);
    (void)pwm_init(ctx);
    (void)spi_init(ctx);
    (void)can_init(ctx);

    return 0;
}
