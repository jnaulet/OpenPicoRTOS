#include "stm32h7xx_m.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "clock-stm32h7xx.h"
#include "mux-stm32h7xx.h"
#include "misc-stm32h7xx_dmamux.h"
#include "dma-stm32h7xx.h"

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
                72000000ul, /* pll2 vco */
                2ul,        /* pll2_p */
                0,          /* pll2_q */
                0           /* pll2_r */
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

    /* can */
    (void)clock_stm32h7xx_enable(CLOCK_STM32H7XX_APB1H_FDCAN);
    (void)clock_stm32h7xx_ker_sel(CLOCK_STM32H7XX_KER_FDCANSEL, 0u); /* hse_ck */

    /* rng */
    (void)clock_stm32h7xx_enable(CLOCK_STM32H7XX_AHB2_RNG);
    (void)clock_stm32h7xx_ker_sel(CLOCK_STM32H7XX_KER_RNGSEL, 1u); /* pll1_q_ck */

    /* adc */
    (void)clock_stm32h7xx_enable(CLOCK_STM32H7XX_AHB4_ADC3);
    (void)clock_stm32h7xx_ker_sel(CLOCK_STM32H7XX_KER_ADCSEL, 0u); /* pll2_q_clk */

    /* dma */
    (void)clock_stm32h7xx_enable(CLOCK_STM32H7XX_AHB1_DMA1);
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

    (void)mux_stm32h7xx_alt(&PORTA, (size_t)11, (size_t)9); /* FDCAN1_RX */
    (void)mux_stm32h7xx_alt(&PORTA, (size_t)12, (size_t)9); /* FDCAN1_TX */
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

static void can_init(/*@partial@*/ struct stm32h7xx_m *ctx)
{
    struct can_settings CAN_settings = {
        125000ul,   /* bitrate */
        (size_t)32, /* tx_mailbox_count */
        CAN_TX_AUTO_ABORT_OFF,
        CAN_RX_OVERWRITE_ON,
        CAN_LOOPBACK_ON
    };

    (void)can_stm32h7xx_fdcan_init(&ctx->can.CAN, ADDR_FDCAN1,
                                   CLOCK_STM32H7XX_HSE_CK,
                                   (uint32_t)0, (size_t)896);

    (void)can_setup(&ctx->can.CAN, &CAN_settings);
}

static void rng_init(/*@partial@*/ struct stm32h7xx_m *ctx)
{
    (void)rng_stm32h7xx_trng_init(&ctx->can.TRNG, ADDR_RNG, CLOCK_STM32H7XX_PLL1_Q_CK);
}

static void adc_init(/*@partial@*/ struct stm32h7xx_m *ctx)
{
    static struct adc_stm32h7xx_sar ADC3;
    static struct dmamux DMAMUX1_0;

    /* ADC & DMA */
    (void)adc_stm32h7xx_sar_init(&ADC3, ADDR_ADC3, CLOCK_STM32H7XX_PLL2_Q_CK,
                                 ADC_STM32H7XX_SAR_MASTER);
    (void)dmamux_stm32h7xx_init(&DMAMUX1_0, ADDR_DMAMUX1, (size_t)0);
    (void)dmamux_stm32h7xx_set_dmareq_id(&DMAMUX1_0, 115ul); /* adc3_dma */

    static struct dma DMA_ADC3;
    struct dma *DMA_ADC3_runtime = &DMA_ADC3;

    (void)dma_stm32h7xx_init(&DMA_ADC3, ADDR_DMA1, (size_t)0);

    /* plug everything in */
    struct adc_stm32h7xx_sar_settings ADC_settings = {
        true, /* tsen */
        ADC_STM32H7XX_SAR_PRESC_DIV1,
        ADC_STM32H7XX_SAR_CKMODE_CK_ADCX,
        ADC_STM32H7XX_SAR_CONT_SINGLE,
        ADC_STM32H7XX_SAR_RES_12BITS,
        1u, /* oversampling */
        DMA_ADC3_runtime
    };

    static struct adc TEMP_DUP;

    (void)adc_stm32h7xx_sar_setup(&ADC3, &ADC_settings);
    (void)adc_stm32h7xx_sar_adc_init(&ctx->TEMP, &ADC3, (size_t)17);
    (void)adc_stm32h7xx_sar_adc_init(&TEMP_DUP, &ADC3, (size_t)17);
}

int stm32h7xx_m_init(struct stm32h7xx_m *ctx)
{
    clock_init();
    mux_init();

    gpio_init(ctx);
    spi_init(ctx);
    uart_init(ctx);
    pwm_init(ctx);
    can_init(ctx);
    rng_init(ctx);
    adc_init(ctx);

    return 0;
}
