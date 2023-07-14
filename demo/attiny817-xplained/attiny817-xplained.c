#include "attiny817-xplained.h"
#include "picoRTOS_device.h"

#include "clock-tinyavr.h"
#include "mux-tinyavr.h"

static void clock_init(void)
{
    struct clock_settings CLOCK_settings = {
        CLOCK_TINYAVR_CLKSEL_OSC20M,
        false,                      /* clkout_enable */
        false,                      /* prescaler_enable */
        CLOCK_TINYAVR_PDIV_COUNT,   /* ignore */
        false,                      /* lock_enable */
    };

    (void)clock_tinyavr_init(&CLOCK_settings);
}

static void mux_init(void)
{
    static struct mux PORTA;
    static struct mux PORTB;

    /* ports */
    (void)mux_tinyavr_init(&PORTA, ADDR_PORTA);
    (void)mux_tinyavr_init(&PORTB, ADDR_PORTB);

    /* SPI */
    (void)mux_tinyavr_output(&PORTA, (size_t)1);    /* MOSI */
    (void)mux_tinyavr_input(&PORTA, (size_t)2);     /* MISO */
    (void)mux_tinyavr_output(&PORTA, (size_t)3);    /* SCK */

    /* UART */
    (void)mux_tinyavr_output(&PORTB, (size_t)2);    /* TxD */
    (void)mux_tinyavr_input(&PORTB, (size_t)3);     /* RxD */
    (void)mux_tinyavr_pull_up(&PORTB, (size_t)2);

    /* ADC */
    (void)mux_tinyavr_input(&PORTA, (size_t)4);    /* AIN04 */

    /* PWMs */
    (void)mux_tinyavr_output(&PORTB, (size_t)0);    /* WO0 */
    (void)mux_tinyavr_output(&PORTB, (size_t)1);    /* WO1 */
}

static int gpio_init(/*@partial@*/ struct attiny817_xplained *ctx)
{
    (void)gpio_tinyavr_init(&ctx->USER_LED, ADDR_PORTC, (size_t)0);
    (void)gpio_tinyavr_init(&ctx->USER_BUTTON, ADDR_PORTC, (size_t)5);

    return 0;
}

static int uart_init(/*@partial@*/ struct attiny817_xplained *ctx)
{
    const struct uart_settings UART_settings = {
        9600ul,
        (size_t)8,
        UART_PAR_NONE,
        UART_CSTOPB_1BIT
    };

    (void)uart_tinyavr_usart_init(&ctx->UART, ADDR_USART0, CLOCK_TINYAVR_CLK_PER);
    return uart_setup(&ctx->UART, &UART_settings);
}

static int spi_init(/*@partial@*/ struct attiny817_xplained *ctx)
{
    struct spi_settings SPI_settings = {
        1000000ul, /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)0,  /* ignore frame size */
        SPI_CS_POL_IGNORE,
        (size_t)0,  /* ignore SS */
    };

    (void)spi_tinyavr_init(&ctx->SPI, ADDR_SPI0, CLOCK_TINYAVR_CLK_PER);
    return spi_setup(&ctx->SPI, &SPI_settings);
}

static int adc_init(/*@partial@*/ struct attiny817_xplained *ctx)
{
    static struct adc_tinyavr ADC0;
    struct adc_tinyavr_settings ADC0_settings = {
        ADC_TINYAVR_RESSEL_10BIT,
        ADC_TINYAVR_REFSEL_VDD,
        1500000ul, /* 1.5Mhz max */
    };

    (void)adc_tinyavr_init(&ADC0, ADDR_ADC0, CLOCK_TINYAVR_CLK_PER);
    (void)adc_tinyavr_setup(&ADC0, &ADC0_settings);

    /* AIN4 settings */
    struct adc_settings ADC_settings_mV = {
        9,  /* mult */
        2,  /* div */
        0   /* offset */
    };

    (void)adc_tinyavr_adc_init(&ctx->AIN04, &ADC0, ADC_TINYAVR_MUXPOS_AIN4);
    return adc_setup(&ctx->AIN04, &ADC_settings_mV);
}

static int wdt_init(/*@partial@*/ struct attiny817_xplained *ctx)
{
    struct wd_tinyavr_settings WDT_settings = {
        WD_TINYAVR_PERIOD_8MS,  /* window */
        WD_TINYAVR_PERIOD_8MS,  /* period */
    };

    (void)wd_tinyavr_init(&ctx->WDT, ADDR_WDT);
    (void)wd_tinyavr_setup(&ctx->WDT, &WDT_settings);

    /* don't start here for memory management reasons */
    return 0;
}

static int pwm_init(/*@partial@*/ struct attiny817_xplained *ctx)
{
    static struct pwm_tinyavr_tca TCA;
    struct pwm_tinyavr_tca_settings TCA_settings = {
        PWM_TINYAVR_TCA_CLKSEL_DIV1,
        PWM_TINYAVR_TCA_WGMODE_SINGLESLOPE,
    };

    (void)pwm_tinyavr_tca_init(&TCA, ADDR_TCA0, CLOCK_TINYAVR_CLK_PER);
    (void)pwm_tinyavr_tca_setup(&TCA, &TCA_settings);

    (void)pwm_tinyavr_tca_pwm_init(&ctx->PWM.WO0, &TCA, PWM_TINYAVR_TCA_CMP0);
    (void)pwm_tinyavr_tca_pwm_init(&ctx->PWM.WO1, &TCA, PWM_TINYAVR_TCA_CMP1);

    return 0;
}

int attiny817_xplained_init(struct attiny817_xplained *ctx)
{
    /* clocks + mux */
    clock_init();
    mux_init();

    /* peripherals */
    (void)gpio_init(ctx);
    (void)uart_init(ctx);
    (void)spi_init(ctx);
    (void)adc_init(ctx);
    (void)wdt_init(ctx);
    (void)pwm_init(ctx);

    return 0;
}
