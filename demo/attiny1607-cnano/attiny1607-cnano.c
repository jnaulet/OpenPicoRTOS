#include "attiny1607-cnano.h"
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
    static struct mux PORTC;

    /* ports */
    (void)mux_tinyavr_init(&PORTA, (struct MUX_TINYAVR*)ADDR_PORTA);
    (void)mux_tinyavr_init(&PORTB, (struct MUX_TINYAVR*)ADDR_PORTB);
    (void)mux_tinyavr_init(&PORTC, (struct MUX_TINYAVR*)ADDR_PORTC);

    /* SWITCH */
    (void)mux_tinyavr_input(&PORTC, (size_t)4);    /* SW0 */

    /* SPI */
    (void)mux_tinyavr_output(&PORTC, (size_t)2);    /* MOSI */
    (void)mux_tinyavr_input(&PORTC, (size_t)1);     /* MISO */
    (void)mux_tinyavr_output(&PORTC, (size_t)0);    /* SCK */
    (void)mux_tinyavr_output(&PORTC, (size_t)3);    /* SS */
    (void)mux_tinyavr_select_alt_pin(MUX_TINYAVR_ALT_SPI0);

    /* UART */
    (void)mux_tinyavr_output(&PORTB, (size_t)2);    /* TxD */
    (void)mux_tinyavr_input(&PORTB, (size_t)3);     /* RxD */
    (void)mux_tinyavr_pull_up(&PORTB, (size_t)2);

    /* ADC */
    (void)mux_tinyavr_input(&PORTA, (size_t)4);    /* AIN04 */

    /* PWMs */
    (void)mux_tinyavr_input(&PORTB, (size_t)7);     /* LED0 */
    (void)mux_tinyavr_output(&PORTB, (size_t)5);    /* LED0 (wired) */
    (void)mux_tinyavr_select_alt_pin(MUX_TINYAVR_ALT_TCA02);
}

static int gpio_init(/*@partial@*/ struct attiny1607_cnano *ctx)
{
    (void)gpio_tinyavr_init(&ctx->SW0, (struct GPIO_TINYAVR*)ADDR_PORTC, (size_t)4);
    return 0;
}

static int uart_init(/*@partial@*/ struct attiny1607_cnano *ctx)
{
    const struct uart_settings UART_settings = {
        9600ul,
        (size_t)8,
        false,  /* N */
        false,  /* - */
        false,  /* 1 */
    };

    (void)uart_tinyavr_usart_init(&ctx->UART, (struct USART_TINYAVR_UART*)ADDR_USART0,
                                  CLOCK_TINYAVR_CLK_PER);

    return uart_setup(&ctx->UART, &UART_settings);
}

static int spi_init(/*@partial@*/ struct attiny1607_cnano *ctx)
{
    struct spi_settings SPI_settings = {
        1000000ul, /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)0,  /* ignore frame size */
        SPI_CS_POL_IGNORE,
        (size_t)0,  /* ignore SS */
    };

    (void)spi_tinyavr_init(&ctx->SPI, (struct SPI_TINYAVR*)ADDR_SPI0, CLOCK_TINYAVR_CLK_PER);
    return spi_setup(&ctx->SPI, &SPI_settings);
}

static int adc_init(/*@partial@*/ struct attiny1607_cnano *ctx)
{
    static struct adc_tinyavr ADC0;
    struct adc_tinyavr_settings ADC0_settings = {
        ADC_TINYAVR_RESSEL_10BIT,
        ADC_TINYAVR_REFSEL_VDD,
        1500000ul, /* 1.5Mhz max */
    };

    (void)adc_tinyavr_init(&ADC0, (struct ADC_TINYAVR*)ADDR_ADC0, CLOCK_TINYAVR_CLK_PER);
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

static int wdt_init(/*@partial@*/ struct attiny1607_cnano *ctx)
{
    struct wd_tinyavr_settings WDT_settings = {
        WD_TINYAVR_PERIOD_8MS,  /* window */
        WD_TINYAVR_PERIOD_8MS,  /* period */
    };

    (void)wd_tinyavr_init(&ctx->WDT, (struct WDT_TINYAVR*)ADDR_WDT);
    (void)wd_tinyavr_setup(&ctx->WDT, &WDT_settings);

    /* don't start here for memory management reasons */
    return 0;
}

static int pwm_init(/*@partial@*/ struct attiny1607_cnano *ctx)
{
    static struct pwm_tinyavr_tca TCA;
    struct pwm_tinyavr_tca_settings TCA_settings = {
        PWM_TINYAVR_TCA_CLKSEL_DIV1,
        PWM_TINYAVR_TCA_WGMODE_SINGLESLOPE,
    };

    (void)pwm_tinyavr_tca_init(&TCA, (struct PWM_TINYAVR_TCA*)ADDR_TCA0, CLOCK_TINYAVR_CLK_PER);
    (void)pwm_tinyavr_tca_setup(&TCA, &TCA_settings);

    (void)pwm_tinyavr_tca_pwm_init(&ctx->LED0, &TCA, PWM_TINYAVR_TCA_CMP2);
    return 0;
}

int attiny1607_cnano_init(struct attiny1607_cnano *ctx)
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
