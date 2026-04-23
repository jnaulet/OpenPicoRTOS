#include "same70-xplained-ultra.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include "clock-atmel_pmc.h"
#include "wd-atmel_wdt.h"
#include "mux-atmel_pio.h"
#include "misc-atsamx7x_matrix.h"

static int clock_init(/*@partial@*/ struct same70_xplained_ultra *ctx)
{
    static struct wd WDT;
    struct clock_settings CLOCK_settings = {
        CLOCK_ATMEL_PMC_MAINCK_XTAL,
        12000000ul,
        150000000ul,    /* plla */
        CLOCK_ATMEL_PMC_UPLL_DIV2,
        CLOCK_ATMEL_PMC_MCKR_CSS_PLLACK,
        1ul,    /* prescaler */
        2ul     /* mdiv */
    };

    /* disable automatic watchdog */
    (void)wd_atmel_wdt_init(&WDT, ADDR_WDT0, CLOCK_ATMEL_PMC_SCLK);
    (void)wd_stop(&WDT); /* can only be done once */

    (void)flash_atmel_eefc_init(&ctx->FLASH, ADDR_EFC);
    (void)flash_atmel_eefc_set_fws(&ctx->FLASH, (size_t)7);  /* for 150mhz */

    (void)clock_atmel_pmc_init(&CLOCK_settings);

    (void)clock_atmel_pmc_pck_enable((size_t)4, 130u, CLOCK_ATMEL_PMC_PCKR_CSS_UPLLCKDIV);
    (void)clock_atmel_pmc_pck_enable((size_t)5, 12u, CLOCK_ATMEL_PMC_PCKR_CSS_UPLLCKDIV);

    (void)clock_atmel_pmc_periph_clk_enable(PID_MCAN1);
    (void)clock_atmel_pmc_periph_clk_enable(PID_AFEC0);
    (void)clock_atmel_pmc_periph_clk_enable(PID_TC1_CHANNEL2);
    (void)clock_atmel_pmc_periph_clk_enable(PID_USART1);
    (void)clock_atmel_pmc_periph_clk_enable(PID_QSPI);

    return 0;
}

static int mux_init(void)
{
    static struct mux PIOA;
    static struct mux PIOB;
    static struct mux PIOC;
    static struct mux PIOD;

    (void)mux_atmel_pio_init(&PIOA, ADDR_PIOA);
    (void)mux_atmel_pio_init(&PIOB, ADDR_PIOB);
    (void)mux_atmel_pio_init(&PIOC, ADDR_PIOC);
    (void)mux_atmel_pio_init(&PIOD, ADDR_PIOD);

    (void)mux_atmel_pio_output(&PIOA, (size_t)5, MUX_ATMEL_PIO_GPIO);       /* LED */
    (void)mux_atmel_pio_output(&PIOC, (size_t)30, MUX_ATMEL_PIO_B);         /* TIOB5 */
    (void)mux_atmel_pio_output(&PIOD, (size_t)26, MUX_ATMEL_PIO_GPIO);      /* GND */
    (void)mux_atmel_pio_output(&PIOC, (size_t)31, MUX_ATMEL_PIO_GPIO);      /* VCC */
    (void)mux_atmel_pio_output(&PIOD, (size_t)30, MUX_ATMEL_PIO_DISABLE);   /* AFE0 AD0 */
    (void)mux_atmel_pio_input(&PIOA, (size_t)21, MUX_ATMEL_PIO_A);          /* RXD1 */
    (void)mux_atmel_pio_output(&PIOB, (size_t)4, MUX_ATMEL_PIO_D);          /* TXD1 */
    (void)mux_atmel_pio_output(&PIOA, (size_t)13, MUX_ATMEL_PIO_A);         /* QIO0 */
    (void)mux_atmel_pio_output(&PIOA, (size_t)12, MUX_ATMEL_PIO_A);         /* QIO1 */
    (void)mux_atmel_pio_output(&PIOA, (size_t)17, MUX_ATMEL_PIO_A);         /* QIO2 */
    (void)mux_atmel_pio_output(&PIOD, (size_t)31, MUX_ATMEL_PIO_A);         /* QIO3 */
    (void)mux_atmel_pio_output(&PIOA, (size_t)14, MUX_ATMEL_PIO_A);         /* QSCK */
    (void)mux_atmel_pio_output(&PIOA, (size_t)11, MUX_ATMEL_PIO_A);         /* QCS */

    return 0;
}

static int gpio_init(/*@partial@*/ struct same70_xplained_ultra *ctx)
{
    static struct gpio VCC;
    static struct gpio GND;

    (void)gpio_atmel_pio_init(&ctx->USER_LED0, ADDR_PIOA, (size_t)5);
    (void)gpio_atmel_pio_init(&VCC, ADDR_PIOC, (size_t)31);
    (void)gpio_atmel_pio_init(&GND, ADDR_PIOD, (size_t)26);

    /* setup */
    gpio_write(&VCC, true);
    gpio_write(&GND, false);

    return 0;
}

static int can_init(/*@partial@*/ struct same70_xplained_ultra *ctx)
{
    static struct atsamx7x_matrix MATRIX;
    /* cppcheck-suppress [unassignedVariable] */
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

static int uart_init(/*@partial@*/ struct same70_xplained_ultra *ctx)
{
    struct uart_settings UART_settings = {
        115200ul,
        (size_t)8,
        UART_PAR_NONE,
        UART_CSTOPB_1BIT
    };

    (void)uart_atsamx7x_usart_init(&ctx->UART, ADDR_USART1, CLOCK_ATMEL_PMC_PCK(4));
    (void)uart_atsamx7x_usart_set_usclks(&ctx->UART, UART_ATSAMX7X_USART_USCLKS_PCK);
    (void)uart_setup(&ctx->UART, &UART_settings);

    return 0;
}

static int flash_init(/*@partial@*/ struct same70_xplained_ultra *ctx)
{
    struct flash_attributes attr;
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while (flash_probe(&ctx->FLASH) < 0 && deadlock-- != 0) arch_delay_us(100ul);
    picoRTOS_assert_void_fatal(deadlock != -1);

    (void)flash_get_attributes(&ctx->FLASH, &attr);
    return 0;
}

static int qspi_init(/*@partial@*/ struct same70_xplained_ultra *ctx)
{
    struct spi_settings SPI_settings = {
        75000000ul,
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)8,
        SPI_CS_POL_ACTIVE_LOW,
        0,
    };

    (void)spi_atsamx7x_qspi_init(&ctx->QSPI, ADDR_QSPI, CLOCK_ATMEL_PMC_MCK);
    (void)spi_setup(&ctx->QSPI, &SPI_settings);

    return 0;
}

static int flash_ext_init(/*@partial@*/ struct same70_xplained_ultra *ctx)
{
    struct flash_attributes attr;
    int deadlock = CONFIG_DEADLOCK_COUNT;

    (void)flash_ext_sst26vf_init(&ctx->FLASH_EXT, &ctx->QSPI);

    while (flash_ext_probe(&ctx->FLASH_EXT) < 0 && deadlock-- != 0) arch_delay_us(100ul);
    picoRTOS_assert_void_fatal(deadlock != -1);

    (void)flash_ext_get_attributes(&ctx->FLASH_EXT, &attr);
    return 0;
}

int same70_xplained_ultra_init(/*@out@*/ struct same70_xplained_ultra *ctx)
{
    (void)clock_init(ctx);
    (void)mux_init();

    (void)gpio_init(ctx);
    (void)can_init(ctx);
    (void)adc_init(ctx);
    (void)pwm_init(ctx);
    (void)uart_init(ctx);
    (void)flash_init(ctx);
    (void)qspi_init(ctx);
    (void)flash_ext_init(ctx);

    return 0;
}
