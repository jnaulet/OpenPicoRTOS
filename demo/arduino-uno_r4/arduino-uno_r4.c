#include "arduino-uno_r4.h"
#include "picoRTOS_device.h"

#include "clock-renesas_ra4.h"
#include "mux-renesas_ra4.h"

static int clock_init(void)
{
    /* R4 Minima has no oscillator
     * Arduino bootloader sets HOCO at 48Mhz, let's use that
     */
    struct clock_settings CLOCK_settings = {
        CLOCK_RENESAS_RA4_MOSEL_RESONATOR,  /* not used */
        0ul,                                /* mosc */
        CLOCK_RENESAS_RA4_CKSEL_HOCO,       /* 48Mhz (?) */
        0ul,                                /* pll */
        CLOCK_RENESAS_RA4_SCKDIV_2,         /* fck (up to 32Mhz) */
        CLOCK_RENESAS_RA4_SCKDIV_1,         /* ick */
        CLOCK_RENESAS_RA4_SCKDIV_1,         /* pcka */
        CLOCK_RENESAS_RA4_SCKDIV_2,         /* pckb (up to 32Mhz) */
        CLOCK_RENESAS_RA4_SCKDIV_1,         /* pckc */
        CLOCK_RENESAS_RA4_SCKDIV_1,         /* pckd */
    };

    return clock_renesas_ra4_init(&CLOCK_settings);
}

static int mux_init(void)
{
    static struct mux PORT1;
    static struct mux PORT3;

    (void)mux_renesas_ra4_init(&PORT1, ADDR_PFS, (size_t)1);
    (void)mux_renesas_ra4_init(&PORT3, ADDR_PFS, (size_t)3);

    (void)mux_renesas_ra4_output(&PORT1, (size_t)5);                /* GPIO */
    (void)mux_renesas_ra4_periph(&PORT1, (size_t)11, (size_t)3);    /* PWM */
    (void)mux_renesas_ra4_periph(&PORT3, (size_t)1, (size_t)4);     /* RX */
    (void)mux_renesas_ra4_periph(&PORT3, (size_t)2, (size_t)4);     /* TX */
    (void)mux_renesas_ra4_periph(&PORT1, (size_t)0, (size_t)7);     /* SCL1 */
    (void)mux_renesas_ra4_periph(&PORT1, (size_t)1, (size_t)7);     /* SDA1 */

    (void)mux_renesas_ra4_pull_up(&PORT3, (size_t)1);

    return 0;
}

static int gpio_init(/*@partial@*/ struct arduino_uno_r4 *ctx)
{
    (void)gpio_renesas_ra4_init(&ctx->TICK, ADDR_PORT1, (size_t)5);
    return 0;
}

static int spi_init(/*@partial@*/ struct arduino_uno_r4 *ctx)
{
    struct spi_renesas_ra4_settings loopback = {
        true, /* loopback */
    };

    struct spi_settings SPI_settings = {
        1000000ul,                           /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)16,                           /* frame size */
        SPI_CS_POL_IGNORE,
        (size_t)0,
    };

    /* start module */
    (void)clock_renesas_ra4_module_start((size_t)MODULE_B(19));

    (void)spi_renesas_ra4_init(&ctx->SPI, ADDR_SPI0, CLOCK_RENESAS_RA4_PCLKA);
    (void)spi_renesas_ra4_setup(&ctx->SPI, &loopback);
    (void)spi_setup(&ctx->SPI, &SPI_settings);

    return 0;
}

static int pwm_init(/*@partial@*/ struct arduino_uno_r4 *ctx)
{
    static struct pwm_renesas_ra4_gpt GPT3;

    struct pwm_renesas_ra4_gpt_pwm_settings PWM_settings = {
        6u,     /* pin_function_select */
        false,  /* output_value_at_stop */
        false,  /* output_setting_at_start_stop */
        PWM_RENESAS_RA4_GPT_OADF_PROHIBITED,
    };

    /* start module */
    (void)clock_renesas_ra4_module_start((size_t)MODULE_D(6));

    (void)pwm_renesas_ra4_gpt_init(&GPT3, ADDR_GPT163, CLOCK_RENESAS_RA4_PCLKD);
    (void)pwm_renesas_ra4_gpt_pwm_init(&ctx->L, &GPT3, PWM_RENESAS_RA4_GPT_OUTPUT_A);
    (void)pwm_renesas_ra4_gpt_pwm_setup(&ctx->L, &PWM_settings);

    return 0;
}

static int uart_init(/*@partial@*/ struct arduino_uno_r4 *ctx)
{
    struct uart_settings UART_settings = {
        9600ul,     /* baudrate */
        (size_t)8,  /* character size */
        UART_PAR_NONE,
        UART_CSTOPB_1BIT
    };

    /* start module */
    (void)clock_renesas_ra4_module_start((size_t)MODULE_B(29));

    (void)uart_renesas_ra4_sci_init(&ctx->UART, ADDR_SCI2, CLOCK_RENESAS_RA4_PCLKA);
    (void)uart_setup(&ctx->UART, &UART_settings);

    return 0;
}

static int adc_init(/*@partial@*/ struct arduino_uno_r4 *ctx)
{
    static struct adc_renesas_ra4 ADC140;
    struct adc_renesas_ra4_settings ADC140_settings = {
        ADC_RENESAS_RA4_ADPRC_14BIT,
        ADC_RENESAS_RA4_ADCS_SINGLE,
    };

    struct adc_settings A0_settings_mV = {
        7,  /* multiplier */
        6,  /* divider */
        0,  /* offset */
    };

    /* start module */
    (void)clock_renesas_ra4_module_start((size_t)MODULE_D(16));

    (void)adc_renesas_ra4_init(&ADC140, ADDR_ADC140);
    (void)adc_renesas_ra4_setup(&ADC140, &ADC140_settings);

    (void)adc_renesas_ra4_adc_init(&ctx->A0, &ADC140, (size_t)9);
    (void)adc_setup(&ctx->A0, &A0_settings_mV);

    return 0;
}

static int i2c_init(/*@partial@*/ struct arduino_uno_r4 *ctx)
{
    struct twi_settings I2C_settings = {
        TWI_BITRATE_STANDARD,
        TWI_MODE_SLAVE,
        (twi_addr_t)0x69,
    };

    /* start module */
    (void)clock_renesas_ra4_module_start((size_t)MODULE_B(8));

    (void)twi_renesas_ra4_init(&ctx->I2C, ADDR_IIC1, CLOCK_RENESAS_RA4_PCLKA);
    (void)twi_setup(&ctx->I2C, &I2C_settings);

    return 0;
}

int arduino_uno_r4_init(struct arduino_uno_r4 *ctx)
{
    (void)clock_init();
    (void)mux_init();

    (void)gpio_init(ctx);
    (void)spi_init(ctx);
    (void)pwm_init(ctx);
    (void)uart_init(ctx);
    (void)adc_init(ctx);
    (void)i2c_init(ctx);

    return 0;
}
