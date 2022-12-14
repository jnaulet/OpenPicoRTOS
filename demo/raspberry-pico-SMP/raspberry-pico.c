#include "raspberry-pico.h"
#include "picoRTOSConfig.h"

#include "adc-rp2040.h"
#include "clock-rp2040.h"
#include "gpio-sio.h"
#include "mux-rp2040.h"
#include "spi-pl022.h"
#include "uart-pl011.h"
#include "pwm-rp2040.h"
#include "twi-dw_apb_i2c.h"
#include "wd-rp2040.h"

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

    (void)mux_rp2040_init(&MUX, (struct MUX_RP2040_IO*)ADDR_IO_BANK0,
                          (struct MUX_RP2040_PADS *)ADDR_PADS_BANK0);

    /* UART */
    (void)mux_rp2040_output(&MUX, (size_t)0, MUX_RP2040_F2);    /* UART0 TX */
    (void)mux_rp2040_input(&MUX, (size_t)1, MUX_RP2040_F2);     /* UART0 RX */

    /* I2C */
    (void)mux_rp2040_io(&MUX, (size_t)6, MUX_RP2040_F3);    /* I2C1 SDA */
    (void)mux_rp2040_pull_up(&MUX, (size_t)6);
    (void)mux_rp2040_io(&MUX, (size_t)7, MUX_RP2040_F3);    /* I2C1 SCL */
    (void)mux_rp2040_pull_up(&MUX, (size_t)7);

    /* SPI */
    (void)mux_rp2040_output(&MUX, (size_t)9, MUX_RP2040_F1);    /* SPI1 CSn */
    (void)mux_rp2040_pull_up(&MUX, (size_t)9);
    (void)mux_rp2040_output(&MUX, (size_t)10, MUX_RP2040_F1);   /* SPI1 SCK */
    (void)mux_rp2040_output(&MUX, (size_t)11, MUX_RP2040_F1);   /* SPI1 TX */
    (void)mux_rp2040_input(&MUX, (size_t)12, MUX_RP2040_F1);    /* SPI1 RX */

    /* GPIOs */
    (void)mux_rp2040_input(&MUX, (size_t)2, MUX_RP2040_F5);
    (void)mux_rp2040_input(&MUX, (size_t)3, MUX_RP2040_F5);
    (void)mux_rp2040_output(&MUX, (size_t)4, MUX_RP2040_F5);
    (void)mux_rp2040_output(&MUX, (size_t)8, MUX_RP2040_F5);
    (void)mux_rp2040_output(&MUX, (size_t)13, MUX_RP2040_F5);
    (void)mux_rp2040_output(&MUX, (size_t)14, MUX_RP2040_F5);
    (void)mux_rp2040_input(&MUX, (size_t)15, MUX_RP2040_F5);
    (void)mux_rp2040_input(&MUX, (size_t)16, MUX_RP2040_F5);
    (void)mux_rp2040_input(&MUX, (size_t)17, MUX_RP2040_F5);
    (void)mux_rp2040_input(&MUX, (size_t)18, MUX_RP2040_F5);
    (void)mux_rp2040_output(&MUX, (size_t)19, MUX_RP2040_F5);
    (void)mux_rp2040_input(&MUX, (size_t)20, MUX_RP2040_F5);
    (void)mux_rp2040_output(&MUX, (size_t)21, MUX_RP2040_F5);

    /* PWMs */
    (void)mux_rp2040_input(&MUX, (size_t)5, MUX_RP2040_F4);
    (void)mux_rp2040_output(&MUX, (size_t)22, MUX_RP2040_F4);
    (void)mux_rp2040_output(&MUX, (size_t)23, MUX_RP2040_F4);
    (void)mux_rp2040_output(&MUX, (size_t)24, MUX_RP2040_F4);
    (void)mux_rp2040_output(&MUX, (size_t)25, MUX_RP2040_F4);

    /* ADCs */
    /* NOthing to do */
}

static void raspberry_pico_init_uart(/*@partial@*/ struct raspberry_pico *ctx)
{
    static struct uart UART;
    struct uart_settings UART_settings = {
        9600ul,     /* baudrate */
        (size_t)8,  /* character size */
        false,      /* parenb */
        false,      /* parodd */
        false       /* cstopb */
    };

    (void)uart_pl011_init(&UART, (struct UART_PL011*)ADDR_UART0, CLOCK_RP2040_PERI);
    (void)uart_setup(&UART, &UART_settings);

    ctx->UART = &UART;
}

static void raspberry_pico_init_i2c(/*@partial@*/ struct raspberry_pico *ctx)
{
    static struct twi I2C;
    struct twi_settings TWI_settings = {
        TWI_BITRATE_STANDARD,   /* bitrate */
        TWI_MODE_MASTER,        /* mode */
        (twi_addr_t)0x68        /* slave address */
    };

    (void)twi_dw_apb_i2c_init(&I2C, (struct TWI_DW_APB_I2C*)ADDR_I2C1, CLOCK_RP2040_SYS);
    (void)twi_setup(&I2C, &TWI_settings);

    ctx->I2C = &I2C;
}

static void raspberry_pico_init_spi(/*@partial@*/ struct raspberry_pico *ctx)
{
    static struct spi SPI;
    struct spi_settings SPI_settings = {
        1000000ul,                           /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)16,                           /* frame size */
        SPI_CS_POL_IGNORE,
        (size_t)0,
    };

    (void)spi_pl022_init(&SPI, (struct SPI_PL022*)ADDR_SPI1, CLOCK_RP2040_PERI);
    (void)spi_setup(&SPI, &SPI_settings);

    /* loopback mode for tests */
    (void)spi_pl022_set_loopback(&SPI, true);

    ctx->SPI = &SPI;
}

static void raspberry_pico_init_gpio(/*@partial@*/ struct raspberry_pico *ctx)
{
    static struct gpio GPIO2;
    static struct gpio GPIO3;
    static struct gpio GPIO4;
    static struct gpio GPIO8;
    static struct gpio GPIO13;
    static struct gpio GPIO14;
    static struct gpio GPIO15;
    static struct gpio GPIO16;
    static struct gpio GPIO17;
    static struct gpio GPIO18;
    static struct gpio GPIO19;
    static struct gpio GPIO20;
    static struct gpio GPIO21;

    (void)gpio_sio_init(&GPIO2, (struct GPIO_SIO*)ADDR_SIO_GPIO, (size_t)2, GPIO_SIO_DIR_INPUT);
    (void)gpio_sio_init(&GPIO3, (struct GPIO_SIO*)ADDR_SIO_GPIO, (size_t)3, GPIO_SIO_DIR_INPUT);
    (void)gpio_sio_init(&GPIO4, (struct GPIO_SIO*)ADDR_SIO_GPIO, (size_t)3, GPIO_SIO_DIR_OUTPUT);
    (void)gpio_sio_init(&GPIO8, (struct GPIO_SIO*)ADDR_SIO_GPIO, (size_t)8, GPIO_SIO_DIR_INPUT);
    (void)gpio_sio_init(&GPIO13, (struct GPIO_SIO*)ADDR_SIO_GPIO, (size_t)13, GPIO_SIO_DIR_OUTPUT);
    (void)gpio_sio_init(&GPIO14, (struct GPIO_SIO*)ADDR_SIO_GPIO, (size_t)14, GPIO_SIO_DIR_OUTPUT);
    (void)gpio_sio_init(&GPIO15, (struct GPIO_SIO*)ADDR_SIO_GPIO, (size_t)15, GPIO_SIO_DIR_INPUT);
    (void)gpio_sio_init(&GPIO16, (struct GPIO_SIO*)ADDR_SIO_GPIO, (size_t)16, GPIO_SIO_DIR_INPUT);
    (void)gpio_sio_init(&GPIO17, (struct GPIO_SIO*)ADDR_SIO_GPIO, (size_t)17, GPIO_SIO_DIR_INPUT);
    (void)gpio_sio_init(&GPIO18, (struct GPIO_SIO*)ADDR_SIO_GPIO, (size_t)18, GPIO_SIO_DIR_INPUT);
    (void)gpio_sio_init(&GPIO19, (struct GPIO_SIO*)ADDR_SIO_GPIO, (size_t)19, GPIO_SIO_DIR_OUTPUT);
    (void)gpio_sio_init(&GPIO20, (struct GPIO_SIO*)ADDR_SIO_GPIO, (size_t)20, GPIO_SIO_DIR_INPUT);
    (void)gpio_sio_init(&GPIO21, (struct GPIO_SIO*)ADDR_SIO_GPIO, (size_t)21, GPIO_SIO_DIR_OUTPUT);

    ctx->GPIO2 = &GPIO2;
    ctx->GPIO3 = &GPIO3;
    ctx->GPIO4 = &GPIO4;
    ctx->GPIO8 = &GPIO8;
    ctx->GPIO13 = &GPIO13;
    ctx->GPIO14 = &GPIO14;
    ctx->GPIO15 = &GPIO15;
    ctx->GPIO16 = &GPIO16;
    ctx->GPIO17 = &GPIO17;
    ctx->GPIO18 = &GPIO18;
    ctx->GPIO19 = &GPIO19;
    ctx->GPIO20 = &GPIO20;
    ctx->GPIO21 = &GPIO21;
}

static void raspberry_pico_init_pwm(/*@partial@*/ struct raspberry_pico *ctx)
{
    static struct pwm_rp2040 PWM;

    static struct pwm PWM3A;
    static struct pwm PWM3B;
    static struct pwm PWM4A;
    static struct pwm PWM4B;
    static struct ipwm PWM2B;

    struct pwm_rp2040_ipwm_settings IPWM_settings = {
        (size_t)1,  /* div */
        true        /* polarity */
    };

    (void)pwm_rp2040_init(&PWM, (struct PWM_RP2040*)ADDR_PWM, CLOCK_RP2040_PERI);

    /* PWMs */
    (void)pwm_rp2040_pwm_init(&PWM3A, &PWM, (size_t)3, PWM_RP2040_PWM_OUTPUT_A);
    (void)pwm_rp2040_pwm_init(&PWM3B, &PWM, (size_t)3, PWM_RP2040_PWM_OUTPUT_B);
    (void)pwm_rp2040_pwm_init(&PWM4A, &PWM, (size_t)4, PWM_RP2040_PWM_OUTPUT_A);
    (void)pwm_rp2040_pwm_init(&PWM4B, &PWM, (size_t)4, PWM_RP2040_PWM_OUTPUT_B);

    /* IPWMs */
    (void)pwm_rp2040_ipwm_init(&PWM2B, &PWM, (size_t)2);
    (void)pwm_rp2040_ipwm_setup(&PWM2B, &IPWM_settings);

    ctx->PWM3A = &PWM3A;
    ctx->PWM3B = &PWM3B;
    ctx->PWM4A = &PWM4A;
    ctx->PWM4B = &PWM4B;
    ctx->PWM2B = &PWM2B;
}

static void raspberry_pico_init_adc(/*@partial@*/ struct raspberry_pico *ctx)
{
    static struct adc_rp2040 ADC;

    static struct adc ADC0;
    static struct adc ADC1;
    static struct adc ADC2;
    static struct adc TEMP_SENSOR;

    (void)adc_rp2040_init(&ADC, (struct ADC_RP2040*)ADDR_ADC);

    /* temp sensor calibration, in degrees */
    struct adc_settings TEMP_settings = {
        1,  /* multiplier */
        33, /* divider */
        0,  /* offset */
    };

    (void)adc_rp2040_adc_init(&ADC0, &ADC, (size_t)0);
    (void)adc_rp2040_adc_init(&ADC1, &ADC, (size_t)1);
    (void)adc_rp2040_adc_init(&ADC2, &ADC, (size_t)2);
    (void)adc_rp2040_adc_init(&TEMP_SENSOR, &ADC, (size_t)ADC_RP2040_CHANNEL_TEMP_SENSOR);
    (void)adc_setup(&TEMP_SENSOR, &TEMP_settings);

    ctx->ADC0 = &ADC0;
    ctx->ADC1 = &ADC1;
    ctx->ADC2 = &ADC2;
    ctx->TEMP_SENSOR = &TEMP_SENSOR;
}

static void raspberry_pico_init_wdt(/*@partial@*/ struct raspberry_pico *ctx)
{
    static struct wd WDT;
    struct wd_rp2040_settings WDT_settings = {
        /* timeout_us */
        (1000000ul / (unsigned long)CONFIG_TICK_HZ) + 1ul
    };

    (void)wd_rp2040_init(&WDT, (struct WD_RP2040*)ADDR_WATCHDOG, CLOCK_RP2040_REF);
    (void)wd_rp2040_setup(&WDT, &WDT_settings);

    ctx->WDT = &WDT;
}

int raspberry_pico_init(/*@out@*/ struct raspberry_pico *ctx)
{
    /* unreset all peripherals (FIXME) */
    *(uint32_t*)ADDR_RESET = 0;

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
