#include "s08pt60-evk.h"
#include "picoRTOS_device.h"

#include "clock-mc9s08ptxx.h"
#include "mux-mc9s08ptxx.h"

#include "picoRTOS_port.h"
#include <generated/autoconf.h>

static void clock_init(void)
{
    struct clock_settings CLOCK_settings = {
#ifdef CONFIG_DEBUG
        /* otherwise BDM crashes */
        CLOCK_MC9S08PTXX_MODE_FEI,
#else
        CLOCK_MC9S08PTXX_MODE_FEE,
#endif
        8000000ul,  /* xosc */
        16000000ul, /* freq */
    };

    (void)clock_mc9s08ptxx_init(&CLOCK_settings);
}

static void mux_init(void)
{
    struct mux PORTA;
    struct mux PORTH;

    (void)mux_mc9s08ptxx_init(&PORTA, (size_t)MUX_MC9S08PTXX_PORT_A);
    (void)mux_mc9s08ptxx_init(&PORTH, (size_t)MUX_MC9S08PTXX_PORT_H);

    (void)mux_mc9s08ptxx_output(&PORTA, (size_t)3); /* TICK */
    (void)mux_mc9s08ptxx_output(&PORTH, (size_t)1); /* RED */
    (void)mux_mc9s08ptxx_output(&PORTH, (size_t)0); /* GREEN */
}

static int gpio_init(/*@partial@*/ struct s08pt60_evk *ctx)
{
    (void)gpio_mc9s08ptxx_init(&ctx->TICK, (size_t)GPIO_MC9S08PTXX_PORT_A, (size_t)3);
    (void)gpio_mc9s08ptxx_init(&ctx->RED, (size_t)GPIO_MC9S08PTXX_PORT_H, (size_t)1);
    (void)gpio_mc9s08ptxx_init(&ctx->GREEN, (size_t)GPIO_MC9S08PTXX_PORT_H, (size_t)0);

    gpio_write(&ctx->RED, true);
    gpio_write(&ctx->GREEN, true);

    return 0;
}

static int uart_init(/*@partial@*/ struct s08pt60_evk *ctx)
{
    struct uart_settings UART_settings = {
        38400ul,    /* baudrate */
        (size_t)8,  /* character size */
        UART_PAR_NONE,
        UART_CSTOPB_1BIT
    };

    (void)uart_mc9s08ptxx_sci_init(&ctx->UART2, ADDR_SCI2, CLOCK_NXP_MC9S08PTXX_BUS);
    (void)uart_setup(&ctx->UART2, &UART_settings);

    return 0;
}

static int spi_init(/*@partial@*/ struct s08pt60_evk *ctx)
{
    struct spi_settings SPI_settings = {
        1000000ul,                           /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)8,                           /* frame size */
        SPI_CS_POL_IGNORE,
        (size_t)0,
    };

    (void)spi_mc9s08ptxx_init(&ctx->SPI1, ADDR_SPI1, CLOCK_NXP_MC9S08PTXX_BUS);
    (void)spi_setup(&ctx->SPI1, &SPI_settings);

    return 0;
}

static int pwm_init(/*@partial@*/ struct s08pt60_evk *ctx)
{
    static struct pwm_mc9s08ptxx_ftm FTM2;

    struct pwm_mc9s08ptxx_ftm_settings FTM_settings = {
        PWM_MC9S08PTXX_FTM_CPWMS_UP,
        PWM_MC9S08PTXX_FTM_CLKS_SYS,
    };

    (void)pwm_mc9s08ptxx_ftm_init(&FTM2, ADDR_FTM2, CLOCK_NXP_MC9S08PTXX_BUS);
    (void)pwm_mc9s08ptxx_ftm_setup(&FTM2, &FTM_settings);

    struct pwm_mc9s08ptxx_ftm_pwm_settings PWM_settings = {
        PWM_MC9S08PTXX_FTM_PWM_MODE_HIGH_TRUE,
    };

    (void)pwm_mc9s08ptxx_ftm_pwm_init(&ctx->ANA.FTM2CH5, &FTM2, (size_t)5);
    (void)pwm_mc9s08ptxx_ftm_pwm_setup(&ctx->ANA.FTM2CH5, &PWM_settings);

    return 0;
}

static int adc_init(/*@partial@*/ struct s08pt60_evk *ctx)
{
    static struct adc_mc9s08ptxx ADC;

    struct adc_mc9s08ptxx_settings ADC_settings = {
        ADC_MC9S08PTXX_MODE_12BIT,
    };

    (void)adc_mc9s08ptxx_init(&ADC, ADDR_ADC);
    (void)adc_mc9s08ptxx_setup(&ADC, &ADC_settings);

    /* poetentionmeter, 1-10ms */
    struct adc_settings ADP1_settings = {
        1, 410, 1
    };

    (void)adc_mc9s08ptxx_adc_init(&ctx->ANA.ADP1, &ADC, (size_t)1);
    (void)adc_setup(&ctx->ANA.ADP1, &ADP1_settings);

    return 0;
}

static int wd_init(/*@partial@*/ struct s08pt60_evk *ctx)
{
    (void)wd_mc9s08ptxx_init(&ctx->WD, CLOCK_NXP_MC9S08PTXX_LPO);
    return 0;
}

int s08pt60_evk_init(struct s08pt60_evk *ctx)
{
    /* wd */
    (void)wd_init(ctx);
    (void)wd_stop(&ctx->WD);

    /* clocks */
    clock_init();
    mux_init();

    /* peripherals */
    (void)gpio_init(ctx);
    (void)uart_init(ctx);
    (void)spi_init(ctx);
    (void)pwm_init(ctx);
    (void)adc_init(ctx);

    return 0;
}
