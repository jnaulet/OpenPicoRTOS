#include "board.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "clock-stm32h7xx.h"
#include "dma-stm32h7xx.h"
#include "gpio-stm32h7xx.h"
#include "misc-stm32h7xx_dmamux.h"
#include "mux-stm32h7xx.h"
#include "spi-stm32h7xx.h"
#include "wd-stm32h7xx_iwdg.h"

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
                700000000ul,    /* pll1 vco */
                2ul,            /* pll1_p */
                4ul,            /* pll1_q */
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
    (void)clock_stm32h7xx_enable(CLOCK_STM32H7XX_APB1L_SPI2);
    (void)clock_stm32h7xx_ker_sel(CLOCK_STM32H7XX_KER_SPI123SEL, 0u); /* pll1_q_ck */

    /* dma */
    (void)clock_stm32h7xx_enable(CLOCK_STM32H7XX_AHB1_DMA1);
}

static void mux_init(void)
{
    static struct mux PORTA;
    static struct mux PORTB;

    (void)mux_stm32h7xx_init(&PORTA, ADDR_GPIOA);
    (void)mux_stm32h7xx_init(&PORTB, ADDR_GPIOB);

    (void)mux_stm32h7xx_output(&PORTA, (size_t)1);          /* LED */
    (void)mux_stm32h7xx_output(&PORTB, (size_t)0);          /* BLK */
    (void)mux_stm32h7xx_output(&PORTB, (size_t)1);          /* DC */
    (void)mux_stm32h7xx_output(&PORTB, (size_t)12);         /* CS */
    (void)mux_stm32h7xx_output(&PORTB, (size_t)14);         /* RES */

    (void)mux_stm32h7xx_alt(&PORTB, (size_t)13, (size_t)5); /* SPI2_SCK */
    (void)mux_stm32h7xx_alt(&PORTB, (size_t)15, (size_t)5); /* SPI2_MOSI */
}

static void gpio_init(/*@partial@*/ struct board *ctx)
{
    static struct gpio LED;
    static struct gpio BLK;
    static struct gpio DC;
    static struct gpio CS;
    static struct gpio RES;

    /* led */
    (void)gpio_stm32h7xx_init(&LED, ADDR_GPIOA, (size_t)1);
    /* lcd */
    (void)gpio_stm32h7xx_init(&BLK, ADDR_GPIOB, (size_t)0);
    (void)gpio_stm32h7xx_init(&DC, ADDR_GPIOB, (size_t)1);
    (void)gpio_stm32h7xx_init(&CS, ADDR_GPIOB, (size_t)12);
    (void)gpio_stm32h7xx_init(&RES, ADDR_GPIOB, (size_t)14);

    /* leds */
    ctx->LED = &LED;

    /* physical layer */
    ctx->lcd_phys.dc = &DC;
    ctx->lcd_phys.rst = &RES;
    ctx->lcd_phys.cs = &CS;

    /* turn blk on forever */
    gpio_write(&BLK, true);
}

static void spi_init(/*@partial@*/ struct board *ctx)
{
    static struct spi SPI;
    struct spi_settings SPI_settings = {
        25000000ul, /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)8,  /* frame_size */
        SPI_CS_POL_ACTIVE_LOW,
        (size_t)0,  /* cs */
    };

    static struct dmamux DMAMUX1_0;
    static struct dmamux DMAMUX1_1;

    (void)dmamux_stm32h7xx_init(&DMAMUX1_0, ADDR_DMAMUX1, (size_t)0);
    (void)dmamux_stm32h7xx_init(&DMAMUX1_1, ADDR_DMAMUX1, (size_t)1);
    (void)dmamux_stm32h7xx_set_dmareq_id(&DMAMUX1_0, 39ul); /* spi2_rx_dma */
    (void)dmamux_stm32h7xx_set_dmareq_id(&DMAMUX1_1, 40ul); /* spi2_tx_dma */

    static struct dma DMA_SPI2_RX;
    static struct dma DMA_SPI2_TX;
    struct spi_stm32h7xx_settings DMA_settings;

    (void)spi_stm32h7xx_init(&SPI, ADDR_SPI2_I2S2, CLOCK_STM32H7XX_PLL1_Q_CK);
    (void)spi_setup(&SPI, &SPI_settings);

    (void)dma_stm32h7xx_init(&DMA_SPI2_RX, ADDR_DMA1, (size_t)0);
    (void)dma_stm32h7xx_init(&DMA_SPI2_TX, ADDR_DMA1, (size_t)1);

    /* DMA config */
    DMA_settings.fill = &DMA_SPI2_TX;
    DMA_settings.drain = &DMA_SPI2_RX;
    DMA_settings.threshold = (size_t)16;
    (void)spi_stm32h7xx_setup(&SPI, &DMA_settings);

    /*  physical layer */
    ctx->lcd_phys.spi = &SPI;
}

static void wd_init(/*@partial@*/ struct board *ctx)
{
    static struct wd WD;
    struct wd_stm32h7xx_iwdg_settings WD_settings = {
        12ul,   /* timeout_us */
        8ul,    /* window_us */
    };

    (void)wd_stm32h7xx_iwdg_init(&WD, ADDR_IWDG1, CLOCK_STM32H7XX_LSI_CK);
    (void)wd_stm32h7xx_iwdg_setup(&WD, &WD_settings);

    ctx->WD = &WD;
    (void)wd_start(&WD);
}

int board_init(struct board *ctx)
{
    clock_init();
    mux_init();

    gpio_init(ctx);
    spi_init(ctx);
    wd_init(ctx);

    return 0;
}
