#include "launchxl-f28379d.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "clock-f28379x.h"
#include "mux-ti_f28x.h"

static void clock_init(void)
{
    struct clock_settings CLOCK_settings = {
        CLOCK_F28379X_CLKSRC_XTAL,
        10000000ul,     /* clksrc freq */
        200000000ul,    /* pll requested freq */
    };

    (void)clock_f28379x_init(&CLOCK_settings);

    (void)clock_f28379x_pclk_enable(CLOCK_F28379X_PCLK_SPIA);
    (void)clock_f28379x_pclk_enable(CLOCK_F28379X_PCLK_EPWM1);
    (void)clock_f28379x_pclk_enable(CLOCK_F28379X_PCLK_TBCLKSYNC);
    (void)clock_f28379x_pclk_enable(CLOCK_F28379X_PCLK_ADCA);
    (void)clock_f28379x_pclk_enable(CLOCK_F28379X_PCLK_CANB);
    (void)clock_f28379x_pclk_enable(CLOCK_F28379X_PCLK_I2CA);
    (void)clock_f28379x_pclk_enable(CLOCK_F28379X_PCLK_I2CB);
}

static void mux_init(void)
{
    struct mux GPIOCTRL_A;
    struct mux GPIOCTRL_B;
    struct mux GPIOCTRL_D;

    (void)mux_ti_f28x_init(&GPIOCTRL_A, ADDR_GPIOCTRL_A);
    (void)mux_ti_f28x_init(&GPIOCTRL_B, ADDR_GPIOCTRL_B);
    (void)mux_ti_f28x_init(&GPIOCTRL_D, ADDR_GPIOCTRL_D);

    /* GPIOs */
    (void)mux_ti_f28x_output(&GPIOCTRL_A, (size_t)31, MUX_TI_F28X_GPIO, MUX_TI_F28X_GPIO);      /* LEDB */
    (void)mux_ti_f28x_output(&GPIOCTRL_B, (size_t)0, MUX_TI_F28X_GPIO, MUX_TI_F28X_GPIO);       /* TICK */
    (void)mux_ti_f28x_output(&GPIOCTRL_B, (size_t)2, MUX_TI_F28X_GPIO, MUX_TI_F28X_GPIO);       /* LEDR */

    /* SPI */
    (void)mux_ti_f28x_output(&GPIOCTRL_B, (size_t)27, MUX_TI_F28X_ALT3, MUX_TI_F28X_ALT3);          /* SPIA MOSI */
    (void)mux_ti_f28x_input(&GPIOCTRL_B, (size_t)28, MUX_TI_F28X_ALT3, MUX_TI_F28X_ALT3);           /* SPIA MISO */
    (void)mux_ti_f28x_output(&GPIOCTRL_B, (size_t)29, MUX_TI_F28X_ALT3, MUX_TI_F28X_ALT3);          /* SPIA CLK */
    (void)mux_ti_f28x_output(&GPIOCTRL_B, (size_t)30, MUX_TI_F28X_ALT3, MUX_TI_F28X_ALT3);          /* SPIA CS */

    (void)mux_ti_f28x_output(&GPIOCTRL_A, (size_t)0, MUX_TI_F28X_GPIO, MUX_TI_F28X_ALT1);           /* EPWM1A */
    (void)mux_ti_f28x_output(&GPIOCTRL_A, (size_t)1, MUX_TI_F28X_GPIO, MUX_TI_F28X_ALT1);           /* EPWM1B */

    /* CAN */
    (void)mux_ti_f28x_output(&GPIOCTRL_A, (size_t)12, MUX_TI_F28X_GPIO, MUX_TI_F28X_ALT2);          /* CANB TX */
    (void)mux_ti_f28x_input(&GPIOCTRL_A, (size_t)17, MUX_TI_F28X_GPIO, MUX_TI_F28X_ALT2);           /* CANB RX */

    /* I2CA */
    (void)mux_ti_f28x_output(&GPIOCTRL_D, (size_t)8, MUX_TI_F28X_GPIO, MUX_TI_F28X_ALT1);       /* SDAA */
    (void)mux_ti_f28x_output(&GPIOCTRL_D, (size_t)9, MUX_TI_F28X_GPIO, MUX_TI_F28X_ALT1);       /* SCLA */
    (void)mux_ti_f28x_set_qsel(&GPIOCTRL_D, (size_t)8, MUX_TI_F28X_QSEL_ASYNC);
    (void)mux_ti_f28x_set_qsel(&GPIOCTRL_D, (size_t)9, MUX_TI_F28X_QSEL_ASYNC);

    /* I2CB */
    (void)mux_ti_f28x_output(&GPIOCTRL_B, (size_t)8, MUX_TI_F28X_ALT1, MUX_TI_F28X_ALT2);       /* SDAB */
    (void)mux_ti_f28x_output(&GPIOCTRL_B, (size_t)9, MUX_TI_F28X_ALT1, MUX_TI_F28X_ALT2);       /* SCLB */
    (void)mux_ti_f28x_set_qsel(&GPIOCTRL_B, (size_t)8, MUX_TI_F28X_QSEL_ASYNC);
    (void)mux_ti_f28x_set_qsel(&GPIOCTRL_B, (size_t)9, MUX_TI_F28X_QSEL_ASYNC);

    (void)mux_ti_f28x_pull_up(&GPIOCTRL_B, (size_t)8);
    (void)mux_ti_f28x_pull_up(&GPIOCTRL_B, (size_t)9);
    (void)mux_ti_f28x_pull_up(&GPIOCTRL_D, (size_t)8);
    (void)mux_ti_f28x_pull_up(&GPIOCTRL_D, (size_t)9);
}

static void gpio_init(/*@partial@*/ struct launchxl_f28379d *ctx)
{
    (void)gpio_ti_f28x_init(&ctx->LED.B, ADDR_GPIODATA_A, (size_t)31);
    (void)gpio_ti_f28x_init(&ctx->TICK, ADDR_GPIODATA_B, (size_t)0);
    (void)gpio_ti_f28x_init(&ctx->LED.R, ADDR_GPIODATA_B, (size_t)2);
}

static void spi_init(/*@partial@*/ struct launchxl_f28379d *ctx)
{
    struct spi_settings SPI_settings = {
        1000000ul,
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)8,
        SPI_CS_POL_IGNORE,
        (size_t)0
    };

    (void)spi_ti_f28x_init(&ctx->SPI, ADDR_SPIA, CLOCK_F28379X_LSPCLK);
    (void)spi_setup(&ctx->SPI, &SPI_settings);
    (void)spi_ti_f28x_set_loopback(&ctx->SPI, true);
}

static void pwm_init(/*@partial@*/ struct launchxl_f28379d *ctx)
{
    static struct pwm_ti_epwm C99_EPWM1;
    struct pwm_ti_epwm_settings C99_EPWM1_settings = {
        100000000ul,     /* frequency */
        PWM_TI_EPWM_WAVEFORM_UP,
    };

    struct pwm_ti_epwm_pwm_settings CMPA_settings = {
        PWM_TI_EPWM_AQ_CLEAR,       /* zero */
        PWM_TI_EPWM_AQ_DISABLE,     /* period */
        PWM_TI_EPWM_AQ_SET,         /* CMPA, up */
        PWM_TI_EPWM_AQ_DISABLE,     /* CMPA, down */
        PWM_TI_EPWM_AQ_DISABLE,     /* CMPB, up */
        PWM_TI_EPWM_AQ_DISABLE,     /* CMPB, down */
    };

    (void)pwm_ti_epwm_init(&C99_EPWM1, ADDR_EPWM1, CLOCK_F28379X_EPWMCLK);
    (void)pwm_ti_epwm_setup(&C99_EPWM1, &C99_EPWM1_settings);

    (void)pwm_ti_epwm_pwm_init(&ctx->PWM, &C99_EPWM1, PWM_TI_EPWM_CMPA);
    (void)pwm_ti_epwm_pwm_setup(&ctx->PWM, &CMPA_settings);
    (void)pwm_set_period(&ctx->PWM, (pwm_period_us_t)100);
}

static void adc_init(/*@partial@*/ struct launchxl_f28379d *ctx)
{
    /* reduce ADCA scope */
    static struct adc_ti_type4 ADCA;
    struct adc_ti_type4_settings ADCA_settings = {
        ADC_TI_TYPE4_RESOLUTION_12BIT,
        ADC_TI_TYPE4_SIGMODE_SINGLE,
        ADC_TI_TYPE4_PRESCALE_2_0,
        ADC_TI_TYPE4_TRIM_OFF_INDEX_ADCA
    };

    struct adc_ti_type4_adc_settings ADCIN13_settings = {
        ADC_TI_TYPE4_TRIG_SOFT,
        (uint16_t)ADC_TI_TYPE4_ACQPS_MIN_12BIT
    };

    (void)adc_ti_type4_init(&ADCA, ADDR_ADCA, ADDR_ADCARESULT, ADCA_CAL_INL_ADDR);
    (void)adc_ti_type4_setup(&ADCA, &ADCA_settings);

    (void)adc_ti_type4_adc_init(&ctx->ADCIN13, &ADCA, (size_t)13, (size_t)3, ADC_TI_TYPE4_INTFLG_1);
    (void)adc_ti_type4_adc_setup(&ctx->ADCIN13, &ADCIN13_settings);

    struct adc_settings TS_settings_deg_x10 = {
        1,  /* multiplier */
        2,  /* divider */
        0,  /* offset */
    };

    (void)adc_setup(&ctx->ADCIN13, &TS_settings_deg_x10);

    /* turn on temperature sensor (FIXME) */
    ASM(" eallow");
    *((uint32_t*)(ADDR_ANALOGSUBSYS + 0x26)) = (uint32_t)0x1;
    ASM(" edis");
}

static void can_init(/*@partial@*/ struct launchxl_f28379d *ctx)
{
    struct can_settings CAN_settings = {
        500000ul,   /* bitrate */
        (size_t)1,  /* tx_mailbox_count */
        false,      /* tx_auto_abort */
        false,      /* rx_overwrite */
        true        /* loopback */
    };

    (void)can_ti_dcan_init(&ctx->CAN, ADDR_CANB, CLOCK_F28379X_SYSCLK);
    (void)can_setup(&ctx->CAN, &CAN_settings);
}

static void i2c_init(/*@partial@*/ struct launchxl_f28379d *ctx)
{
    struct twi_settings I2C_settings = {
        100000ul,           /* bitrate */
        TWI_MODE_MASTER,
        (twi_addr_t)0x55,   /* slave_addr */
    };

    (void)twi_ti_f28x_init(&ctx->I2CA, ADDR_I2CA, CLOCK_F28379X_SYSCLK);
    (void)twi_setup(&ctx->I2CA, &I2C_settings);

    I2C_settings.mode = TWI_MODE_SLAVE;
    (void)twi_ti_f28x_init(&ctx->I2CB, ADDR_I2CB, CLOCK_F28379X_SYSCLK);
    (void)twi_setup(&ctx->I2CB, &I2C_settings);
}

static void wd_init(/*@partial@*/ struct launchxl_f28379d *ctx)
{
    /* wd auto-starts */
    (void)wd_ti_f28x_init(&ctx->WDT, ADDR_WD, CLOCK_F28379X_INTOSC1);
    wd_refresh(&ctx->WDT);
}

int launchxl_f28379d_init(struct launchxl_f28379d *ctx)
{
    /* before anything else */
    (void)wd_init(ctx);     /* WDT */

    /* clocks & mux */
    clock_init();
    mux_init();

    /* peripherals */

    (void)gpio_init(ctx);   /* GPIO */
    (void)spi_init(ctx);    /* SPI */
    (void)pwm_init(ctx);    /* PWM */
    (void)adc_init(ctx);    /* ADC */
    (void)can_init(ctx);    /* CAN */
    (void)i2c_init(ctx);    /* I2C */

    return 0;
}
