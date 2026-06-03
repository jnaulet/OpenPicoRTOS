#include "raspberry-pico.h"
#include "picoRTOS_device.h"

/* reset subsystem */
static struct rp2040_reset RESET;

static void raspberry_pico_init_clock(void)
{
    struct clock_settings CLOCK_settings = {
        12000000ul,                             /* xosc */
        125000000ul,                            /* pll_sys */
        /* dividers */
        1ul,                                    /* ref_div */
        1ul,                                    /* sys_div */
        1ul,                                    /* clk_gpout0_div */
        1ul,                                    /* clk_gpout1_div */
        1ul,                                    /* clk_gpout2_div */
        1ul,                                    /* clk_gpout3_div */
    };

    (void)clock_rp2040_init(&CLOCK_settings);

    /* enable peripherals */
    (void)clock_rp2040_enable(CLOCK_RP2040_PERI);
    (void)clock_rp2040_enable(CLOCK_RP2040_ADC);
}

static void raspberry_pico_init_mux(void)
{
    struct mux MUX;

    /* clock subsystem */
    (void)rp2040_unreset(&RESET, RP2040_RESET_IO_BANK0);
    (void)rp2040_unreset(&RESET, RP2040_RESET_PADS_BANK0);

    (void)mux_rp2040_init(&MUX, ADDR_IO_BANK0, ADDR_PADS_BANK0);

    /* UART */
    (void)mux_rp2040_output(&MUX, (size_t)0, MUX_RP2040_F2);    /* UART0 TX */
    (void)mux_rp2040_input(&MUX, (size_t)1, MUX_RP2040_F2);     /* UART0 RX */

    /* I2C */
    (void)mux_rp2040_io(&MUX, (size_t)20, MUX_RP2040_F3);   /* I2C0 SDA */
    (void)mux_rp2040_io(&MUX, (size_t)21, MUX_RP2040_F3);   /* I2C0 SCL */
    (void)mux_rp2040_io(&MUX, (size_t)6, MUX_RP2040_F3);    /* I2C1 SDA */
    (void)mux_rp2040_io(&MUX, (size_t)7, MUX_RP2040_F3);    /* I2C1 SCL */

    (void)mux_rp2040_pull_up(&MUX, (size_t)6);
    (void)mux_rp2040_pull_up(&MUX, (size_t)7);
    (void)mux_rp2040_pull_up(&MUX, (size_t)20);
    (void)mux_rp2040_pull_up(&MUX, (size_t)21);

    /* SPI */
    (void)mux_rp2040_output(&MUX, (size_t)9, MUX_RP2040_F1);    /* SPI1 CSn */
    (void)mux_rp2040_pull_up(&MUX, (size_t)9);
    (void)mux_rp2040_output(&MUX, (size_t)10, MUX_RP2040_F1);   /* SPI1 SCK */
    (void)mux_rp2040_output(&MUX, (size_t)11, MUX_RP2040_F1);   /* SPI1 TX */
    (void)mux_rp2040_input(&MUX, (size_t)12, MUX_RP2040_F1);    /* SPI1 RX */

    /* GPIOs */
    (void)mux_rp2040_output(&MUX, (size_t)4, MUX_RP2040_F5);
    (void)mux_rp2040_output(&MUX, (size_t)8, MUX_RP2040_F5);
    (void)mux_rp2040_input(&MUX, (size_t)15, MUX_RP2040_F5);
    (void)mux_rp2040_input(&MUX, (size_t)16, MUX_RP2040_F5);
    (void)mux_rp2040_input(&MUX, (size_t)17, MUX_RP2040_F5);
    (void)mux_rp2040_input(&MUX, (size_t)18, MUX_RP2040_F5);
    (void)mux_rp2040_output(&MUX, (size_t)19, MUX_RP2040_F5);

    /* PWMs */
    (void)mux_rp2040_input(&MUX, (size_t)5, MUX_RP2040_F4);     /* PWM2B */
    (void)mux_rp2040_output(&MUX, (size_t)24, MUX_RP2040_F4);   /* PWM4A */
    (void)mux_rp2040_output(&MUX, (size_t)25, MUX_RP2040_F4);   /* PWM4B */
    (void)mux_rp2040_output(&MUX, (size_t)26, MUX_RP2040_F4);   /* PWM5A */
    (void)mux_rp2040_output(&MUX, (size_t)27, MUX_RP2040_F4);   /* PWM5B */

    /* ADCs */
    /* NOthing to do */
}

static void raspberry_pico_init_uart(/*@partial@*/ struct raspberry_pico *ctx)
{
    struct uart_settings UART_settings = {
        9600ul,     /* baudrate */
        (size_t)8,  /* character size */
        UART_PAR_NONE,
        UART_CSTOPB_1BIT
    };

    (void)rp2040_unreset(&RESET, RP2040_RESET_UART0);

    (void)uart_arm_pl011_init(&ctx->UART, ADDR_UART0, CLOCK_RP2040_PERI);
    (void)uart_setup(&ctx->UART, &UART_settings);
}

static void raspberry_pico_init_i2c(/*@partial@*/ struct raspberry_pico *ctx)
{
    struct twi_settings TWI_settings = {
        TWI_BITRATE_STANDARD,   /* bitrate */
        TWI_MODE_MASTER,        /* mode */
        (twi_addr_t)0x55        /* slave address */
    };

    (void)rp2040_unreset(&RESET, RP2040_RESET_I2C0);
    (void)rp2040_unreset(&RESET, RP2040_RESET_I2C1);

    (void)twi_dw_apb_i2c_init(&ctx->I2C0, ADDR_I2C0, CLOCK_RP2040_SYS);
    (void)twi_setup(&ctx->I2C0, &TWI_settings);

    TWI_settings.mode = TWI_MODE_SLAVE;
    (void)twi_dw_apb_i2c_init(&ctx->I2C1, ADDR_I2C1, CLOCK_RP2040_SYS);
    (void)twi_setup(&ctx->I2C1, &TWI_settings);
}

static void raspberry_pico_init_spi(/*@partial@*/ struct raspberry_pico *ctx)
{
    struct spi_settings SPI_settings = {
        1000000ul,                           /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)16,                           /* frame size */
        SPI_CS_POL_IGNORE,
        (size_t)0,
    };

    struct spi_arm_pl022_settings loopback = {
        true
    };

    (void)rp2040_unreset(&RESET, RP2040_RESET_SPI1);

    (void)spi_arm_pl022_init(&ctx->SPI, ADDR_SPI1, CLOCK_RP2040_PERI);
    (void)spi_setup(&ctx->SPI, &SPI_settings);

    /* loopback mode for tests */
    (void)spi_arm_pl022_setup(&ctx->SPI, &loopback);
}

static void raspberry_pico_init_gpio(/*@partial@*/ struct raspberry_pico *ctx)
{
    (void)gpio_rp2040_sio_init(&ctx->GPIO4, ADDR_SIO_GPIO, (size_t)3, GPIO_RP2040_SIO_DIR_OUTPUT);
    (void)gpio_rp2040_sio_init(&ctx->GPIO8, ADDR_SIO_GPIO, (size_t)8, GPIO_RP2040_SIO_DIR_INPUT);
    (void)gpio_rp2040_sio_init(&ctx->GPIO15, ADDR_SIO_GPIO, (size_t)15, GPIO_RP2040_SIO_DIR_INPUT);
    (void)gpio_rp2040_sio_init(&ctx->GPIO16, ADDR_SIO_GPIO, (size_t)16, GPIO_RP2040_SIO_DIR_INPUT);
    (void)gpio_rp2040_sio_init(&ctx->GPIO17, ADDR_SIO_GPIO, (size_t)17, GPIO_RP2040_SIO_DIR_INPUT);
    (void)gpio_rp2040_sio_init(&ctx->GPIO18, ADDR_SIO_GPIO, (size_t)18, GPIO_RP2040_SIO_DIR_INPUT);
    (void)gpio_rp2040_sio_init(&ctx->GPIO19, ADDR_SIO_GPIO, (size_t)19, GPIO_RP2040_SIO_DIR_OUTPUT);
}

static void raspberry_pico_init_pwm(/*@partial@*/ struct raspberry_pico *ctx)
{
    static struct pwm_rp2040 PWM;
    struct pwm_rp2040_ipwm_settings IPWM_settings = {
        (size_t)1,  /* div */
        true        /* polarity */
    };

    (void)rp2040_unreset(&RESET, RP2040_RESET_PWM);
    (void)pwm_rp2040_init(&PWM, ADDR_PWM, CLOCK_RP2040_PERI);

    /* PWMs */
    (void)pwm_rp2040_pwm_init(&ctx->PWM4A, &PWM, (size_t)4, PWM_RP2040_PWM_OUTPUT_A);
    (void)pwm_rp2040_pwm_init(&ctx->PWM4B, &PWM, (size_t)4, PWM_RP2040_PWM_OUTPUT_B);
    (void)pwm_rp2040_pwm_init(&ctx->PWM5A, &PWM, (size_t)5, PWM_RP2040_PWM_OUTPUT_A);
    (void)pwm_rp2040_pwm_init(&ctx->PWM5B, &PWM, (size_t)5, PWM_RP2040_PWM_OUTPUT_B);

    /* IPWMs */
    (void)pwm_rp2040_ipwm_init(&ctx->PWM2B, &PWM, (size_t)2);
    (void)pwm_rp2040_ipwm_setup(&ctx->PWM2B, &IPWM_settings);

    /* force period to 100us & duty cycle to 40% on PWM3B for ipwm tests */
    (void)pwm_set_period(&ctx->PWM5B, (pwm_period_us_t)100);
    (void)pwm_set_duty_cycle(&ctx->PWM5B, PWM_DUTY_CYCLE_PCENT(40));
    pwm_start(&ctx->PWM5B);
}

static void raspberry_pico_init_adc(/*@partial@*/ struct raspberry_pico *ctx)
{
    static struct adc_rp2040 ADC;

    (void)rp2040_unreset(&RESET, RP2040_RESET_ADC);
    (void)adc_rp2040_init(&ADC, ADDR_ADC);

    /* temp sensor calibration, in degrees */
    struct adc_settings TEMP_settings = {
        1,  /* multiplier */
        33, /* divider */
        0,  /* offset */
    };

    (void)adc_rp2040_adc_init(&ctx->ADC0, &ADC, (size_t)0);
    (void)adc_rp2040_adc_init(&ctx->ADC1, &ADC, (size_t)1);
    (void)adc_rp2040_adc_init(&ctx->ADC2, &ADC, (size_t)2);
    (void)adc_rp2040_adc_init(&ctx->TEMP_SENSOR, &ADC, (size_t)ADC_RP2040_CHANNEL_TEMP_SENSOR);
    (void)adc_setup(&ctx->TEMP_SENSOR, &TEMP_settings);
}

static void raspberry_pico_init_wdt(/*@partial@*/ struct raspberry_pico *ctx)
{
    struct wd_rp2040_settings WDT_settings = {
        /* timeout_us */
        (1000000ul / (unsigned long)CONFIG_TICK_HZ) + 1ul
    };

    (void)wd_rp2040_init(&ctx->WDT, ADDR_WATCHDOG, CLOCK_RP2040_REF);
    (void)wd_rp2040_setup(&ctx->WDT, &WDT_settings);
    (void)wd_start(&ctx->WDT);
}

int raspberry_pico_init(/*@out@*/ struct raspberry_pico *ctx)
{
    /* reset subsystem (required) */
    (void)rp2040_reset_init(&RESET, ADDR_RESET);

    raspberry_pico_init_clock();
    raspberry_pico_init_mux();

    raspberry_pico_init_uart(ctx);
    raspberry_pico_init_i2c(ctx);
    raspberry_pico_init_spi(ctx);
    raspberry_pico_init_gpio(ctx);
    raspberry_pico_init_pwm(ctx);
    raspberry_pico_init_adc(ctx);
    raspberry_pico_init_wdt(ctx);

    return 0;
}
