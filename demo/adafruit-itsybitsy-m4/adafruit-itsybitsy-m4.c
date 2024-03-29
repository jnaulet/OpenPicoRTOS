#include "adafruit-itsybitsy-m4.h"
#include "picoRTOS_device.h"

#include "clock-same5x.h"
#include "mux-same5x.h"

#define GCLK_MAIN     (size_t)0
#define GCLK_DFLL48M  (size_t)1
#define GCLK_DFLL12M  (size_t)2
#define GCLK_XOSC32K  (size_t)3

/* Borrowed from Arduino IDE startup */
static void clock_init(void)
{
    /* Prepare XOSC32 clock generator */
    struct clock_same5x_gclk_settings GCLK_OSCULP32K_settings = {
        CLOCK_SAME5X_GCLK_SRC_OSCULP32K,
        1ul, /* div */
    };

    /* general reset */
    (void)clock_same5x_gclk_reset();

    /* XOSC32 */
    (void)clock_same5x_gclk_generator_setup(GCLK_XOSC32K, &GCLK_OSCULP32K_settings);
    (void)clock_same5x_gclk_generator_enable(GCLK_XOSC32K);

    /* GCLK_DPLLn */
    (void)clock_same5x_setup(CLOCK_SAME5X_FDPLL0, GCLK_XOSC32K);
    (void)clock_same5x_enable(CLOCK_SAME5X_FDPLL0);

    /* FDPLL200Mn */
    struct clock_same5x_dpll_settings DPLL_settings = {
        CLOCK_SAME5X_DPLL_REFCLK_GCLK,
        120000000ul, /* 120Mhz from GCLK */
    };

    (void)clock_same5x_dpll_setup((size_t)0, &DPLL_settings);
    (void)clock_same5x_dpll_enable((size_t)0);

    /* GCLK_MAIN */
    struct clock_same5x_gclk_settings GCLK_DPLL0_settings = {
        CLOCK_SAME5X_GCLK_SRC_DPLL0,
        1ul, /* div */
    };

    (void)clock_same5x_gclk_generator_setup(GCLK_MAIN, &GCLK_DPLL0_settings);
    /* by default, CPUDIV is 1 so it's ok, sort of */

    /* PERIPHERALS */

    /* DFLL48M */
    struct clock_same5x_gclk_settings GCLK_DFLL48M_settings = {
        CLOCK_SAME5X_GCLK_SRC_DFLL,
        1ul, /* div */
    };

    (void)clock_same5x_gclk_generator_setup(GCLK_DFLL48M, &GCLK_DFLL48M_settings);
    (void)clock_same5x_gclk_generator_enable(GCLK_DFLL48M);

    /* DFLL12M */
    struct clock_same5x_gclk_settings GCLK_DFLL12M_settings = {
        CLOCK_SAME5X_GCLK_SRC_DFLL,
        4ul, /* div */
    };

    (void)clock_same5x_gclk_generator_setup(GCLK_DFLL12M, &GCLK_DFLL12M_settings);
    (void)clock_same5x_gclk_generator_enable(GCLK_DFLL12M);

    /* All SERCOMs */
    (void)clock_same5x_setup(CLOCK_SAME5X_SERCOMn_SLOW, GCLK_DFLL12M);
    (void)clock_same5x_enable(CLOCK_SAME5X_SERCOMn_SLOW);

    /* SPI */
    (void)clock_same5x_mclk_enable(CLOCK_SAME5X_MCLK_APBA_SERCOM1);
    (void)clock_same5x_setup(CLOCK_SAME5X_SERCOM1_CORE, GCLK_DFLL48M);
    (void)clock_same5x_enable(CLOCK_SAME5X_SERCOM1_CORE);

    /* UART */
    (void)clock_same5x_mclk_enable(CLOCK_SAME5X_MCLK_APBB_SERCOM3);
    (void)clock_same5x_setup(CLOCK_SAME5X_SERCOM3_CORE, GCLK_DFLL48M);
    (void)clock_same5x_enable(CLOCK_SAME5X_SERCOM3_CORE);

    /* PWM */
    (void)clock_same5x_mclk_enable(CLOCK_SAME5X_MCLK_APBB_TC3);
    (void)clock_same5x_setup(CLOCK_SAME5X_TC3, GCLK_DFLL48M);
    (void)clock_same5x_enable(CLOCK_SAME5X_TC3);

    /* ADC */
    (void)clock_same5x_mclk_enable(CLOCK_SAME5X_MCLK_APBD_ADC0);
    (void)clock_same5x_setup(CLOCK_SAME5X_ADC0, GCLK_DFLL12M);
    (void)clock_same5x_enable(CLOCK_SAME5X_ADC0);

    /* WDT */
    (void)clock_same5x_mclk_enable(CLOCK_SAME5X_MCLK_APBA_WDT);

    /* FLASH */
    (void)clock_same5x_mclk_enable(CLOCK_SAME5X_MCLK_AHB_NVMCTRL);
    (void)clock_same5x_mclk_enable(CLOCK_SAME5X_MCLK_APBB_NVMCTRL);

    /* I2C */
    (void)clock_same5x_mclk_enable(CLOCK_SAME5X_MCLK_APBB_SERCOM2);
    (void)clock_same5x_setup(CLOCK_SAME5X_SERCOM2_CORE, GCLK_DFLL48M);
    (void)clock_same5x_enable(CLOCK_SAME5X_SERCOM2_CORE);
}

static void mux_init(void)
{
    struct mux_same5x PORTA;
    struct mux_same5x PORTB;

    (void)mux_same5x_init(&PORTA, ADDR_PORTA);
    (void)mux_same5x_init(&PORTB, ADDR_PORTB);

    /* RED */
    (void)mux_same5x_output(&PORTA, (size_t)22, MUX_PMUX_GPIO); /* D13 */

    /* SPI */
    (void)mux_same5x_output(&PORTA, (size_t)0, MUX_PMUX_D);     /* MOSI */
    (void)mux_same5x_output(&PORTA, (size_t)1, MUX_PMUX_D);     /* SCK */
    (void)mux_same5x_output(&PORTB, (size_t)23, MUX_PMUX_C);    /* MISO */

    /* UART */
    (void)mux_same5x_input(&PORTA, (size_t)16, MUX_PMUX_D);         /* RX_D0 */
    (void)mux_same5x_output(&PORTA, (size_t)17, MUX_PMUX_D);        /* TX_D1 */
    (void)mux_same5x_pull_up(&PORTA, (size_t)16);

    /* PWM */
    (void)mux_same5x_output(&PORTA, (size_t)15, MUX_PMUX_E);         /* D5 */

    /* ADC */
    (void)mux_same5x_input(&PORTA, (size_t)2, MUX_PMUX_B);         /* A0 */

    /* I2C */
    (void)mux_same5x_output(&PORTA, (size_t)12, MUX_PMUX_C);    /* SDA */
    (void)mux_same5x_output(&PORTA, (size_t)13, MUX_PMUX_C);    /* SCL */

}

static int gpio_init(/*@partial@*/ struct adafruit_itsybitsy_m4 *ctx)
{
    return gpio_same5x_init(&ctx->RED, ADDR_PORTA, (size_t)22);
}

static int spi_init(/*@partial@*/ struct adafruit_itsybitsy_m4 *ctx)
{
    struct spi_settings SPI_settings = {
        1000000ul, /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)8,  /* frame size */
        SPI_CS_POL_IGNORE,
        (size_t)0,  /* ignore SS */
    };

    /* external */
    (void)spi_atmel_sercom_init(&ctx->SPI, ADDR_SERCOM1, CLOCK_SAME5X_SERCOM1_CORE);
    return spi_setup(&ctx->SPI, &SPI_settings);
}

static int uart_init(/*@partial@*/ struct adafruit_itsybitsy_m4 *ctx)
{
    /* 115200 8N1 */
    struct uart_settings UART_settings = {
        115200ul,
        (size_t)8,
        UART_PAR_NONE,
        UART_CSTOPB_1BIT
    };

    (void)uart_atmel_sercom_init(&ctx->UART, ADDR_SERCOM3, CLOCK_SAME5X_SERCOM3_CORE);
    return uart_setup(&ctx->UART, &UART_settings);
}

static int pwm_init(/*@partial@*/ struct adafruit_itsybitsy_m4 *ctx)
{
    static struct pwm_same5x_tc TC3;
    struct pwm_same5x_tc_settings TC3_settings = {
        PWM_SAME5X_TC_PRESCALER_DIV1,
        PWM_SAME5X_TC_MODE_16,
        PWM_SAME5X_TC_WAVEGEN_MPWM,
    };

    (void)pwm_same5x_tc_init(&TC3, ADDR_TC3, CLOCK_SAME5X_TC3);
    (void)pwm_same5x_tc_setup(&TC3, &TC3_settings);

    (void)pwm_same5x_tc_pwm_init(&ctx->D5, &TC3, (size_t)1);
    return 0;
}

static int adc_init(/*@partial@*/ struct adafruit_itsybitsy_m4 *ctx)
{
    struct adc_settings ADC_settings_mV = {
        4,  /* multiplier */
        5,  /* divider */
        0   /* offset */
    };

    (void)adc_same5x_init(&ctx->ADC, ADDR_ADC0, ADC_SAME5X_CHANNEL_SCALEDCOREVCC);
    return adc_setup(&ctx->ADC, &ADC_settings_mV);
}

static int wd_init(/*@partial@*/ struct adafruit_itsybitsy_m4 *ctx)
{
    struct wd_same5x_settings WD_settings = {
        WD_SAME5X_PERIOD_64CYC,
        true, /* window_mode */
        WD_SAME5X_PERIOD_16CYC
    };

    (void)wd_same5x_init(&ctx->WDT, ADDR_WDT);
    (void)wd_same5x_setup(&ctx->WDT, &WD_settings);

    return wd_start(&ctx->WDT);
}

static int flash_init(/*@partial@*/ struct adafruit_itsybitsy_m4 *ctx)
{
    (void)flash_same5x_init(&ctx->FLASH, ADDR_NVMCTRL);
    return 0;
}

static int twi_init(/*@partial@*/ struct adafruit_itsybitsy_m4 *ctx)
{
    struct twi_settings TWI_settings = {
        TWI_BITRATE_STANDARD,   /* bitrate */
        TWI_MODE_SLAVE,         /* mode */
        (twi_addr_t)0x55        /* slave address */
    };

    (void)twi_atmel_sercom_init(&ctx->I2C, ADDR_SERCOM2, CLOCK_SAME5X_SERCOM2_CORE);
    (void)twi_setup(&ctx->I2C, &TWI_settings);

    return 0;
}

int adafruit_itsybitsy_m4_init(struct adafruit_itsybitsy_m4 *ctx)
{
    clock_init();
    mux_init();

    /* peripherals */
    (void)gpio_init(ctx);
    (void)spi_init(ctx);
    (void)uart_init(ctx);
    (void)pwm_init(ctx);
    (void)adc_init(ctx);
    (void)wd_init(ctx);
    (void)flash_init(ctx);
    (void)twi_init(ctx);

    return 0;
}
