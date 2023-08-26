#include "stm32h7xx_m.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "clock-stm32h7xx.h"
#include "mux-stm32h7xx.h"

static void clock_init(void)
{
    struct clock_settings RCC_settings = {
        CLOCK_STM32H7XX_HSI_16MHZ,
        25000000ul,                     /* hse_hz */
        CLOCK_STM32H7XX_CSI_OFF,        /* csi off */
        CLOCK_STM32H7XX_HSI48_OFF,      /* hsi48 off */
        CLOCK_STM32H7XX_PLLSRC_HSE_CK,
        {
            {
                400000000ul,    /* pll1 vco */
                2ul,            /* pll1_p */
                2ul,            /* pll1_q */
                0               /* pll1_r */
            },
            {
                0,      /* pll2 vco */
                0,      /* pll2_p */
                0,      /* pll2_q */
                0       /* pll2_r */
            },
            {
                0,      /* pll3 vco */
                0,      /* pll3_p */
                0,      /* pll3_q */
                0       /* pll3_r */
            }
        },
        CLOCK_STM32H7XX_SW_PLL1_P_CK,
    };

    /* main */
    (void)clock_stm32h7xx_init(&RCC_settings);
    /* per_ck is hse_ck */
    (void)clock_stm32h7xx_ker_sel(CLOCK_STM32H7XX_KER_CKPERSEL, 2u);

    /* gpio */
    (void)clock_stm32h7xx_enable(CLOCK_STM32H7XX_AHB4_GPIOA);
    (void)clock_stm32h7xx_enable(CLOCK_STM32H7XX_AHB4_GPIOB);

    /* spi */
    (void)clock_stm32h7xx_enable(CLOCK_STM32H7XX_APB2_SPI1);
    (void)clock_stm32h7xx_ker_sel(CLOCK_STM32H7XX_KER_SPI123SEL, 0u); /* pll1_q_ck */

    /* uart */
    (void)clock_stm32h7xx_enable(CLOCK_STM32H7XX_APB1L_UART4);
    (void)clock_stm32h7xx_ker_sel(CLOCK_STM32H7XX_KER_USART234578SEL, 3u); /* hsi_ker_ck */

    /* tim */
    (void)clock_stm32h7xx_enable(CLOCK_STM32H7XX_APB1L_TIM2);
}

static void mux_init(void)
{
    static struct mux PORTA;
    static struct mux PORTB;

    (void)mux_stm32h7xx_init(&PORTA, ADDR_GPIOA);
    (void)mux_stm32h7xx_init(&PORTB, ADDR_GPIOB);

    (void)mux_stm32h7xx_output(&PORTB, (size_t)12);         /* TICK */

    (void)mux_stm32h7xx_alt(&PORTA, (size_t)1, (size_t)1);  /* LED */

    (void)mux_stm32h7xx_alt(&PORTA, (size_t)4, (size_t)5);  /* SPI1_NSS */
    (void)mux_stm32h7xx_alt(&PORTA, (size_t)5, (size_t)5);  /* SPI1_SCK */
    (void)mux_stm32h7xx_alt(&PORTA, (size_t)6, (size_t)5);  /* SPI1_MISO */
    (void)mux_stm32h7xx_alt(&PORTA, (size_t)7, (size_t)5);  /* SPI1_MOSI */

    (void)mux_stm32h7xx_alt(&PORTB, (size_t)8, (size_t)8);  /* UART4_RX */
    (void)mux_stm32h7xx_alt(&PORTB, (size_t)9, (size_t)8);  /* UART4_TX */
}

static void gpio_init(/*@partial@*/ struct stm32h7xx_m *ctx)
{
    (void)gpio_stm32h7xx_init(&ctx->TICK, ADDR_GPIOB, (size_t)12);
    // (void)gpio_stm32h7xx_init(&ctx->LED, ADDR_GPIOA, (size_t)1);
}

static void spi_init(/*@partial@*/ struct stm32h7xx_m *ctx)
{
    struct spi_settings SPI1_settings = {
        1000000ul, /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)8,  /* frame_size */
        SPI_CS_POL_ACTIVE_LOW,
        (size_t)0,  /* cs */
    };

    (void)spi_stm32h7xx_init(&ctx->SPI1, ADDR_SPI1_I2S1, CLOCK_STM32H7XX_PLL1_Q_CK);
    (void)spi_setup(&ctx->SPI1, &SPI1_settings);
}

static void uart_init(/*@partial@*/ struct stm32h7xx_m *ctx)
{
    struct uart_settings UART4_settings = {
        115200ul,
        (size_t)8,
        UART_PAR_NONE,
        UART_CSTOPB_1BIT,
    };

    (void)uart_stm32h7xx_init(&ctx->UART4, ADDR_UART4, CLOCK_STM32H7XX_HSI_KER_CK);
    (void)uart_setup(&ctx->UART4, &UART4_settings);
}

static void pwm_init(/*@partial@*/ struct stm32h7xx_m *ctx)
{
    static struct pwm_stm32h7xx_tim TIM2;
    struct pwm_stm32h7xx_tim_settings TIM2_settings = {
        PWM_STM32H7XX_TIM_ALIGN_EDGE_UP,
    };

    (void)pwm_stm32h7xx_tim_init(&TIM2, ADDR_TIM2, CLOCK_STM32H7XX_SYS_CK, (size_t)4);
    (void)pwm_stm32h7xx_tim_setup(&TIM2, &TIM2_settings);

    /* PWM */
    struct pwm_stm32h7xx_tim_pwm_settings TIM2_CH2_settings = {
        PWM_STM32H7XX_TIM_OC_PWM_MODE_1,
    };

    (void)pwm_stm32h7xx_tim_pwm_init(&ctx->LED, &TIM2, (size_t)1);
    (void)pwm_stm32h7xx_tim_pwm_setup(&ctx->LED, &TIM2_CH2_settings);
}

int stm32h7xx_m_init(struct stm32h7xx_m *ctx)
{
    clock_init();
    mux_init();

    gpio_init(ctx);
    spi_init(ctx);
    uart_init(ctx);
    pwm_init(ctx);

    return 0;
}
