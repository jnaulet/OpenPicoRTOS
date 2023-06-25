#include "board.h"
#include "picoRTOS_device.h"

#include "clock-same5x.h"
#include "dma-same5x.h"
#include "gpio-same5x.h"
#include "mux-same5x.h"
#include "spi-atmel_sercom.h"
#include "wd-same5x.h"

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
    (void)clock_same5x_setup(CLOCK_SAME5X_SERCOM1_CORE, GCLK_MAIN);
    (void)clock_same5x_enable(CLOCK_SAME5X_SERCOM1_CORE);

    /* DMA */
    (void)clock_same5x_mclk_enable(CLOCK_SAME5X_MCLK_AHB_DMAC);
}

static void mux_init(void)
{
    struct mux_same5x PORTA;
    struct mux_same5x PORTB;

    (void)mux_same5x_init(&PORTA, ADDR_PORTA);
    (void)mux_same5x_init(&PORTB, ADDR_PORTB);

    /* LED */
    (void)mux_same5x_output(&PORTA, (size_t)22, MUX_PMUX_GPIO); /* D13 */
    /* LCD */
    (void)mux_same5x_output(&PORTA, (size_t)6, MUX_PMUX_GPIO);  /* A5 */
    (void)mux_same5x_output(&PORTA, (size_t)4, MUX_PMUX_GPIO);  /* A4 */
    (void)mux_same5x_output(&PORTB, (size_t)9, MUX_PMUX_GPIO);  /* A3 */

    /* SPI */
    (void)mux_same5x_output(&PORTA, (size_t)0, MUX_PMUX_D);     /* MOSI */
    (void)mux_same5x_output(&PORTA, (size_t)1, MUX_PMUX_D);     /* SCK */
}

static int gpio_init(/*@partial@*/ struct board *ctx)
{
    static struct gpio LED;

    struct gpio_settings LED_settings = {
        true, /* invert */
    };

    static struct gpio RST;
    static struct gpio DC;
    static struct gpio CS;

    /* LED */
    (void)gpio_same5x_init(&LED, ADDR_PORTA, (size_t)22);
    (void)gpio_setup(&LED, &LED_settings);
    /* LCD */
    (void)gpio_same5x_init(&RST, ADDR_PORTA, (size_t)6);
    (void)gpio_same5x_init(&DC, ADDR_PORTA, (size_t)4);
    (void)gpio_same5x_init(&CS, ADDR_PORTB, (size_t)9);

    /* led */
    ctx->LED = &LED;

    /* physical layer */
    ctx->lcd_phys.dc = &DC;
    ctx->lcd_phys.rst = &RST;
    ctx->lcd_phys.cs = &CS;

    return 0;
}

static int spi_init(/*@partial@*/ struct board *ctx)
{
    static struct spi SPI;
    static struct dma SPI_TX;
    static struct dma SPI_RX;
    static struct dma_same5x DMAC;

    struct spi_atmel_sercom_settings DMA_settings;
    struct spi_settings SPI_settings = {
        60000000ul, /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)8,  /* frame size */
        SPI_CS_POL_IGNORE,
        (size_t)0,  /* ignore SS */
    };

    /* DMAC */
    (void)dma_same5x_init(&DMAC, ADDR_DMAC);
    (void)dma_same5x_dma_init(&SPI_TX, &DMAC, (size_t)0, DMA_SAME5X_TRIGSRC_SERCOM1_TX);
    (void)dma_same5x_dma_init(&SPI_RX, &DMAC, (size_t)1, DMA_SAME5X_TRIGSRC_SERCOM1_RX);

    /* DMA */
    DMA_settings.fill = &SPI_TX;
    DMA_settings.drain = &SPI_RX;
    DMA_settings.threshold = (size_t)16;
    DMA_settings.loopback = false;

    /* SPI */
    (void)spi_atmel_sercom_init(&SPI, ADDR_SERCOM1, CLOCK_SAME5X_SERCOM1_CORE);
    (void)spi_atmel_sercom_setup(&SPI, &DMA_settings);
    (void)spi_setup(&SPI, &SPI_settings);

    /* physical layer */
    ctx->lcd_phys.spi = &SPI;
    return 0;
}

static int wd_init(/*@partial@*/ struct board *ctx)
{
    static struct wd WD;
    struct wd_same5x_settings WD_settings = {
        WD_SAME5X_PERIOD_16CYC,
        false, /* window_mode */
        WD_SAME5X_PERIOD_16CYC
    };

    (void)wd_same5x_init(&WD, ADDR_WDT);
    (void)wd_same5x_setup(&WD, &WD_settings);

    ctx->WD = &WD;

    return wd_start(&WD);
}

int board_init(struct board *ctx)
{
    clock_init();
    mux_init();

    /* peripherals */
    (void)gpio_init(ctx);
    (void)spi_init(ctx);
    (void)wd_init(ctx);

    return 0;
}
