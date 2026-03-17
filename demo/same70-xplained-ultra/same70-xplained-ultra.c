#include "same70-xplained-ultra.h"
#include "picoRTOS_device.h"

#include "clock-atmel_pmc.h"
#include "flash-atmel_eefc.h"
#include "wd-atmel_wdt.h"
#include "mux-atmel_pio.h"
#include "uart-atsamx7x.h"
#include "misc-atsamx7x_matrix.h"

static int clock_init(void)
{
    static struct wd WDT;
    static struct flash FLASH;

    struct clock_settings CLOCK_settings = {
        CLOCK_ATMEL_PMC_MAINCK_XTAL,
        12000000ul,
        300000000ul,    /* plla */
        CLOCK_ATMEL_PMC_UPLL_DIV2,
        CLOCK_ATMEL_PMC_MCKR_CSS_PLLACK,
        1ul,    /* prescaler */
        2ul     /* mdiv */
    };

    /* disable automatic watchdog */
    (void)wd_atmel_wdt_init(&WDT, ADDR_WDT0, CLOCK_ATMEL_PMC_SCLK);
    (void)wd_stop(&WDT); /* can only be done once */

    (void)flash_atmel_eefc_init(&FLASH, ADDR_EFC);
    (void)flash_atmel_eefc_set_fws(&FLASH, (size_t)7);  /* for 300mhz */

    (void)clock_atmel_pmc_init(&CLOCK_settings);

    (void)clock_atmel_pmc_pck_enable((size_t)5, 12u, CLOCK_ATMEL_PMC_PCKR_CSS_UPLLCKDIV);
    // (void)clock_atmel_pmc_pck_enable((size_t)6, 12u, CLOCK_ATMEL_PMC_PCKR_CSS_UPLLCKDIV);
    // (void)clock_atmel_pmc_pck_enable((size_t)7, 12u, CLOCK_ATMEL_PMC_PCKR_CSS_UPLLCKDIV);

    (void)clock_atmel_pmc_periph_clk_enable(PID_MCAN1);
    (void)clock_atmel_pmc_periph_clk_enable(PID_AFEC0);
    // (void)clock_atmel_pmc_periph_clk_enable(PID_TC1_CHANNEL0);
    // (void)clock_atmel_pmc_periph_clk_enable(PID_TC1_CHANNEL1);
    (void)clock_atmel_pmc_periph_clk_enable(PID_TC1_CHANNEL2);

    return 0;
}

static int mux_init(void)
{
    static struct mux PIOA;
    static struct mux PIOC;
    static struct mux PIOD;

    (void)mux_atmel_pio_init(&PIOA, ADDR_PIOA);
    (void)mux_atmel_pio_init(&PIOC, ADDR_PIOC);
    (void)mux_atmel_pio_init(&PIOD, ADDR_PIOD);

    (void)mux_atmel_pio_output(&PIOA, (size_t)5, MUX_ATMEL_PIO_GPIO);       /* LED */
    (void)mux_atmel_pio_output(&PIOA, (size_t)17, MUX_ATMEL_PIO_GPIO);      /* GND */
    (void)mux_atmel_pio_output(&PIOC, (size_t)30, MUX_ATMEL_PIO_B);         /* TIOB5 */
    (void)mux_atmel_pio_output(&PIOD, (size_t)26, MUX_ATMEL_PIO_GPIO);      /* GND */
    (void)mux_atmel_pio_output(&PIOC, (size_t)31, MUX_ATMEL_PIO_GPIO);      /* VCC */
    (void)mux_atmel_pio_output(&PIOD, (size_t)30, MUX_ATMEL_PIO_DISABLE);   /* AFE0 AD0 */

    return 0;
}

static int gpio_init(/*@partial@*/ struct same70_xplained_ultra *ctx)
{
    static struct gpio GND;
    static struct gpio VCC;
    static struct gpio GND2;

    (void)gpio_atmel_pio_init(&ctx->USER_LED0, ADDR_PIOA, (size_t)5);
    (void)gpio_atmel_pio_init(&GND, ADDR_PIOD, (size_t)26);
    (void)gpio_atmel_pio_init(&VCC, ADDR_PIOC, (size_t)31);
    (void)gpio_atmel_pio_init(&GND2, ADDR_PIOA, (size_t)17);

    /* setup */
    gpio_write(&GND, false);
    gpio_write(&VCC, true);
    gpio_write(&GND2, false);

    return 0;
}

static int can_init(/*@partial@*/ struct same70_xplained_ultra *ctx)
{
    static struct atsamx7x_matrix MATRIX;
    static uint32_t mram[DEVICE_MCAN_RAM_WORD_COUNT]
    __attribute__((aligned(ARCH_L1_DCACHE_LINESIZE)));

    struct can_settings CAN_settings = {
        500000ul,
        (size_t)32,
        CAN_TX_AUTO_ABORT_ON,
        CAN_RX_OVERWRITE_ON,
        CAN_LOOPBACK_OFF
    };

    (void)atsamx7x_matrix_init(&MATRIX, ADDR_MATRIX);
    (void)atsamx7x_matrix_set_can0dmaba(&MATRIX, (uint16_t)((uintptr_t)mram >> 16));
    (void)atsamx7x_matrix_set_can1dmaba(&MATRIX, (uint16_t)((uintptr_t)mram >> 16));

    (void)can_bosch_mcan_init(&ctx->CAN, ADDR_MCAN1, CLOCK_ATMEL_PMC_PCK(5),
                              (uint32_t*)((uint8_t*)mram - ((uintptr_t)mram & 0xffffu)),
                              (uint32_t)(((uint16_t)(uintptr_t)mram) >> 2) + 0u,
                              (size_t)640);

    (void)can_setup(&ctx->CAN, &CAN_settings);
    return 0;
}

static int adc_init(/*@partial@*/ struct same70_xplained_ultra *ctx)
{
    static struct adc_atmel_afec AFEC0;
    struct adc_settings ADC_settings = { 1, 40, -1 };

    (void)adc_atmel_afec_init(&AFEC0, ADDR_AFEC0, CLOCK_ATMEL_PMC_MCK);
    (void)adc_atmel_afec_adc_init(&ctx->POT, &AFEC0, (size_t)0);
    (void)adc_setup(&ctx->POT, &ADC_settings);

    return 0;
}

static int pwm_init(/*@partial@*/ struct same70_xplained_ultra *ctx)
{
    static struct pwm_atsamx7x_tc TC1;
    struct pwm_atsamx7x_tc_pwm_settings PWM_settings = {
        PWM_ATSAMX7X_TC_TCCLKS_TIMER_CLOCK2,
        PWM_ATSAMX7X_TC_WAVSEL_UP_RC,
#if 0
        PWM_ATSAMX7X_TC_CP_CLEAR,   /* xCPx */
        PWM_ATSAMX7X_TC_CP_SET      /* xCPC */
#else
        PWM_ATSAMX7X_TC_CP_SET,     /* xCPx */
        PWM_ATSAMX7X_TC_CP_CLEAR    /* xCPC */
#endif
    };

    (void)pwm_atsamx7x_tc_init(&TC1, ADDR_TC1, CLOCK_ATMEL_PMC_MCK);
    (void)pwm_atsamx7x_tc_pwm_init(&ctx->PWM, &TC1, (size_t)2, PWM_ATSAMX7X_TC_TIOB);
    (void)pwm_atsamx7x_tc_pwm_setup(&ctx->PWM, &PWM_settings);

    return 0;
}

int same70_xplained_ultra_init(/*@out@*/ struct same70_xplained_ultra *ctx)
{
    (void)clock_init();
    (void)mux_init();

    (void)gpio_init(ctx);
    (void)can_init(ctx);
    (void)adc_init(ctx);
    (void)pwm_init(ctx);

    return 0;
}
