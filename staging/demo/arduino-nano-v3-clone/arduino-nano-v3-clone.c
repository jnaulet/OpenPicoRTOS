#include "arduino-nano-v3-clone.h"

static void init_clock(void)
{
    struct clock_settings CLOCK_settings = {
        16000000ul, /* xtal */
        CLOCK_LGT8FX8P_MCLK_HFRC,
        CLOCK_LGT8FX8P_PS_1,
        CLOCK_LGT8FX8P_WDCLKS_LFRC,
    };

    (void)clock_lgt8fx8p_init(&CLOCK_settings);
}

static void init_mux(void)
{
    struct mux PORTB;
    struct mux PORTD;

    (void)mux_avr_init(&PORTB, ADDR_PORTB);
    (void)mux_avr_init(&PORTD, ADDR_PORTD);

    /* DIGITAL: UART */
    (void)mux_avr_output(&PORTD, (size_t)1, true);  /* UART_TX */
    (void)mux_avr_input(&PORTD, (size_t)0);         /* UART_RX */

    /* DIGITAL: GPIO */
    (void)mux_avr_output(&PORTB, (size_t)5, true);  /* L */
    (void)mux_avr_output(&PORTB, (size_t)4, false); /* TICK */

    /* DIGITAL: PWM */
    (void)mux_avr_output(&PORTD, (size_t)3, false);
    (void)mux_avr_output(&PORTB, (size_t)1, false);

#if 0
    /* DIGITAL: SPI */
    (void)mux_avr_input(&PORTB, (size_t)4);             /* MISO */
    (void)mux_avr_output(&PORTB, (size_t)2, false);     /* MOSI */
    (void)mux_avr_output(&PORTB, (size_t)3, false);     /* SCK */
    (void)mux_avr_output(&PORTB, (size_t)2, true);      /* _SS */
#endif
}

static void init_uart(/*@partial@*/ struct arduino_nano_v3_clone *ctx)
{
    /* SERIAL */
    struct uart_settings UART_settings = {
        115200ul,
        (size_t)8,
        UART_PAR_NONE,
        UART_CSTOPB_1BIT
    };

    (void)uart_avr_init(&ctx->SERIAL, ADDR_USART, CLOCK_LGT8FX8P_PERI_CLK);
    (void)uart_setup(&ctx->SERIAL, &UART_settings);
}

static void init_digital(/*@partial@*/ struct arduino_nano_v3_clone *ctx)
{
    (void)gpio_avr_init(&ctx->L, ADDR_PORTB, (size_t)5);
    (void)gpio_avr_init(&ctx->TICK, ADDR_PORTB, (size_t)4);
}

static void init_spi(/*@partial@*/ struct arduino_nano_v3_clone *ctx)
{
    struct spi_settings SPI_settings = {
        1000000ul, /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)8,  /* frame size */
        SPI_CS_POL_IGNORE,
        (size_t)0   /* CS */
    };

    (void)spi_avr_init(&ctx->SPI, ADDR_SPI, CLOCK_LGT8FX8P_PERI_CLK);
    (void)spi_setup(&ctx->SPI, &SPI_settings);
}

static void init_pwm(/*@partial@*/ struct arduino_nano_v3_clone *ctx)
{
    static struct pwm_avr TC1;
    static struct pwm_avr TC2;

    /* Variable period set by ICRn */
    struct pwm_avr_settings TCn_settings_ICRn = {
        PWM_AVR_CS_CLKIO_1,
        PWM_AVR_WAVEFORM_FAST_PWM_ICRn
    };

    (void)pwm_avr_init(&TC1, ADDR_TC1, CLOCK_LGT8FX8P_PERI_CLK);
    (void)pwm_avr_setup(&TC1, &TCn_settings_ICRn);
    (void)pwm_avr_init(&TC2, ADDR_TC2, CLOCK_LGT8FX8P_PERI_CLK);
    (void)pwm_avr_setup(&TC2, &TCn_settings_ICRn);

    /* pwm */
    struct pwm_settings PWM_settings = {
        PWM_AVR_MODE_CLEAR
    };

    (void)pwm_avr_pwm_init(&ctx->PWM3, &TC2, PWM_AVR_OCnB);
    (void)pwm_avr_pwm_setup(&ctx->PWM3, &PWM_settings);
    (void)pwm_avr_pwm_init(&ctx->PWM9, &TC1, PWM_AVR_OCnA);
    (void)pwm_avr_pwm_setup(&ctx->PWM9, &PWM_settings);
}

static void init_wdt(/*@partial@*/ struct arduino_nano_v3_clone *ctx)
{
    struct wd_avr_settings WD_settings = {
        WD_AVR_CYCLES_2K
    };

    (void)wd_avr_init(&ctx->WDT, ADDR_WDT);
    (void)wd_avr_setup(&ctx->WDT, &WD_settings);
}

int arduino_nano_v3_clone_init(struct arduino_nano_v3_clone *ctx)
{
    init_clock();
    init_mux();

    init_uart(ctx);
    init_digital(ctx);
    // init_spi(ctx);
    // init_pwm(ctx);
    init_wdt(ctx);

    return 0;
}
