#include "devkit-mpc5748g.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "clock-mpc574xx.h"
#include "mux-nxp_siul2.h"

static void clock_init(void)
{
    struct clock_settings CLOCK_settings = {
        CLOCK_MPC574XX_SYSCLK_FXOSC,
        CLOCK_MPC574XX_FMPLL_CLKIN_FXOSC,
        40000000ul,     /* fxosc */
        160000000ul,    /* phi0 */
        1ul,            /* s160_div */
        2ul,            /* s80_div */
        2ul,            /* fs80_div */
        4ul             /* s40_div */
    };

    (void)clock_mpc574xx_init(&CLOCK_settings);
}

static void mux_init(void)
{
    struct mux MUX;

    (void)mux_nxp_siul2_init(&MUX, ADDR_SIUL2);

    /* adc */
    (void)mux_nxp_siul2_analog(&MUX, (size_t)20);                                   /* PB[4] ADC1_P[0] */
    /* lin */
    (void)mux_nxp_siul2_output(&MUX, (size_t)18, MUX_NXP_SIUL2_ALT1);               /* PB[2] LIN0TX */
    (void)mux_nxp_siul2_input(&MUX, (size_t)19, MUX_NXP_SIUL2_ALT2, (size_t)200);   /* PB[3] LIN0RX */
    /* gpio */
    (void)mux_nxp_siul2_output(&MUX, (size_t)144, MUX_NXP_SIUL2_GPIO);              /* PJ[0] TICK */
    (void)mux_nxp_siul2_output(&MUX, (size_t)4, MUX_NXP_SIUL2_GPIO);                /* PA[4] LED0 */
    (void)mux_nxp_siul2_output(&MUX, (size_t)0, MUX_NXP_SIUL2_GPIO);                /* PA[0] LED1 */
    (void)mux_nxp_siul2_output(&MUX, (size_t)148, MUX_NXP_SIUL2_GPIO);              /* PJ[4] LED2 */
    (void)mux_nxp_siul2_output(&MUX, (size_t)117, MUX_NXP_SIUL2_GPIO);              /* PH[5] LED3 */
    (void)mux_nxp_siul2_output(&MUX, (size_t)36, MUX_NXP_SIUL2_GPIO);               /* PC[4] LED4 */
    (void)mux_nxp_siul2_output(&MUX, (size_t)125, MUX_NXP_SIUL2_GPIO);              /* PH[13] LED5 */
    (void)mux_nxp_siul2_output(&MUX, (size_t)7, MUX_NXP_SIUL2_GPIO);                /* PA[7] LED6 */
    (void)mux_nxp_siul2_output(&MUX, (size_t)10, MUX_NXP_SIUL2_GPIO);               /* PA[10] LED7 */
    /* spi */
    (void)mux_nxp_siul2_input(&MUX, (size_t)12, MUX_NXP_SIUL2_ALT1, (size_t)288);   /* PA[12] dSIN_0 */
    (void)mux_nxp_siul2_output(&MUX, (size_t)13, MUX_NXP_SIUL2_ALT1);               /* PA[13] dSOUT_0 */
    (void)mux_nxp_siul2_output(&MUX, (size_t)14, MUX_NXP_SIUL2_ALT1);               /* PA[14] dSCLK_0 */
    (void)mux_nxp_siul2_output(&MUX, (size_t)15, MUX_NXP_SIUL2_ALT1);               /* PA[15] dCS0_0 */
    /* can */
    (void)mux_nxp_siul2_output(&MUX, (size_t)16, MUX_NXP_SIUL2_ALT1);               /* PB[0] CAN0_TX */
    (void)mux_nxp_siul2_input(&MUX, (size_t)17, MUX_NXP_SIUL2_ALT2, (size_t)188);   /* PB[1] CAN0_RX */
    /* uart */
    (void)mux_nxp_siul2_output(&MUX, (size_t)40, MUX_NXP_SIUL2_ALT1);               /* PC[8] LIN2_TX */
    (void)mux_nxp_siul2_input(&MUX, (size_t)41, MUX_NXP_SIUL2_ALT3, (size_t)202);   /* PC[9] LIN2_RX */
}

static int adc_init(/*@partial@*/ struct devkit_mpc5748g *ctx)
{
    static struct adc_nxp_sar ADC1;
    struct adc_nxp_sar_settings ADC1_settings = {
        false,  /* overwrite */
        false,  /* align left */
        false,  /* scan mode */
        false,  /* injection_ext_trigger_en */
        false,  /* injection_trigger_edge_sel */
        false,  /* injection_start */
        false,  /* crosstrigger_unit_en */
        false,  /* crosstrigger_unit_trigger_mode */
        false,  /* analog_clock_freq_equals_bus_clock */
        false,  /* auto_clock_off_en */
    };

    (void)adc_nxp_sar_init(&ADC1, ADDR_ADC1);
    (void)adc_nxp_sar_setup(&ADC1, &ADC1_settings);

    /* channel */
    (void)adc_nxp_sar_adc_init(&ctx->ADC1_P0, &ADC1, (size_t)0);

    return 0;
}

static int can_init(/*@partial@*/ struct devkit_mpc5748g *ctx)
{
    struct can_settings CAN_settings = {
        1000000ul,  /* bitrate */
        (size_t)2,  /* tx_mailbox_count */
        true,       /* tx_auto_abort */
        false,      /* rx_overwrite */
        true        /* loopback */
    };

    (void)can_nxp_flexcan_init(&ctx->CAN0, ADDR_FLEXCAN0, CLOCK_MPC574XX_F40, (size_t)96);
    (void)can_setup(&ctx->CAN0, &CAN_settings);

    return 0;
}

static int flash_init(/*@partial@*/ struct devkit_mpc5748g *ctx)
{
    return flash_nxp_c55fmc_init(&ctx->FLASH, ADDR_C55FMC);
}

static int gpio_init(/*@partial@*/ struct devkit_mpc5748g *ctx)
{
    (void)gpio_nxp_siul2_init(&ctx->TICK, ADDR_GPIO_PTJ, (size_t)0);
    (void)gpio_nxp_siul2_init(&ctx->LED[0], ADDR_GPIO_PTA, (size_t)4);
    (void)gpio_nxp_siul2_init(&ctx->LED[1], ADDR_GPIO_PTA, (size_t)0);
    (void)gpio_nxp_siul2_init(&ctx->LED[2], ADDR_GPIO_PTJ, (size_t)4);
    (void)gpio_nxp_siul2_init(&ctx->LED[3], ADDR_GPIO_PTH, (size_t)5);
    (void)gpio_nxp_siul2_init(&ctx->LED[4], ADDR_GPIO_PTC, (size_t)4);
    (void)gpio_nxp_siul2_init(&ctx->LED[5], ADDR_GPIO_PTH, (size_t)13);
    (void)gpio_nxp_siul2_init(&ctx->LED[6], ADDR_GPIO_PTA, (size_t)7);
    (void)gpio_nxp_siul2_init(&ctx->LED[7], ADDR_GPIO_PTA, (size_t)10);

    return 0;
}

static int lin_init(/*@partial@*/ struct devkit_mpc5748g *ctx)
{
    struct lin_settings LIN0_settings = {
        115200ul, /* bitrate */
        LIN_MODE_MASTER,
    };

    (void)lin_nxp_linflexd_init(&ctx->LIN0, ADDR_LINFLEXD0, CLOCK_MPC574XX_F20);
    return lin_setup(&ctx->LIN0, &LIN0_settings);
}

static int pwm_init(/*@partial@*/ struct devkit_mpc5748g *ctx)
{
    static struct pwm_nxp_emios C99_EMIOS0;

    struct pwm_nxp_emios_settings C99_EMIOS0_settings = {
        (size_t)1, /* gpre */
    };

    (void)pwm_nxp_emios_init(&C99_EMIOS0, ADDR_EMIOS0, CLOCK_MPC574XX_FS80);
    (void)pwm_nxp_emios_setup(&C99_EMIOS0, &C99_EMIOS0_settings);

    struct pwm_nxp_emios_pwm_settings PWM_settings = {
        PWM_NXP_EMIOS_UCPRE_1,  /* ucpre */
        false,                  /* polarity */
    };

    /* TBD */
    (void)pwm_nxp_emios_pwm_init(&ctx->PWM, &C99_EMIOS0, (size_t)0);
    return pwm_nxp_emios_pwm_setup(&ctx->PWM, &PWM_settings);
}

static int spi_init(/*@partial@*/ struct devkit_mpc5748g *ctx)
{
    struct spi_settings SPI_settings = {
        1000000ul,     /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)16, /* frame size */
        SPI_CS_POL_ACTIVE_LOW,
        0           /* cs */
    };

    (void)spi_nxp_dspi_init(&ctx->SPI, ADDR_DSPI0, CLOCK_MPC574XX_F40);
    (void)spi_setup(&ctx->SPI, &SPI_settings);

    return 0;
}

static int twi_init(/*@partial@*/ struct devkit_mpc5748g *ctx)
{
    struct twi_settings TWI_settings = {
        TWI_BITRATE_STANDARD,
        TWI_MODE_MASTER,
        (twi_addr_t)0x55
    };

    (void)twi_nxp_i2c_init(&ctx->I2C0, ADDR_I2C0, CLOCK_MPC574XX_F40);
    (void)twi_setup(&ctx->I2C0, &TWI_settings);

    TWI_settings.mode = TWI_MODE_SLAVE;
    (void)twi_nxp_i2c_init(&ctx->I2C1, ADDR_I2C1, CLOCK_MPC574XX_F40);
    (void)twi_setup(&ctx->I2C1, &TWI_settings);

    return 0;
}

static int uart_init(/*@partial@*/ struct devkit_mpc5748g *ctx)
{
    struct uart_settings UART_settings = {
        115200ul,   /* baudrate */
        (size_t)8,  /* cs */
        false,      /* parenb */
        false,      /* parodd */
        true        /* cstopb */
    };

    (void)uart_nxp_linflexd_init(&ctx->UART, ADDR_LINFLEXD2, CLOCK_MPC574XX_F80);
    (void)uart_setup(&ctx->UART, &UART_settings);

    return 0;
}

int devkit_mpc5748g_init(struct devkit_mpc5748g *ctx)
{
    clock_init();
    mux_init();

    (void)adc_init(ctx);
    (void)can_init(ctx);
    (void)flash_init(ctx);
    (void)gpio_init(ctx);
    (void)lin_init(ctx);
    (void)pwm_init(ctx);
    (void)spi_init(ctx);
    (void)twi_init(ctx);
    (void)uart_init(ctx);

    return 0;
}
