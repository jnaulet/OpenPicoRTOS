#include "board.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "clock-f28379x.h"
#include "dma-ti_f28x.h"
#include "gpio-ti_f28x.h"
#include "mux-ti_f28x.h"
#include "spi-ti_f28x.h"
#include "wd-ti_f28x.h"

static void clock_init(void)
{
    struct clock_settings CLOCK_settings = {
        CLOCK_F28379X_CLKSRC_XTAL,
        10000000ul,     /* clksrc freq */
        200000000ul,    /* pll requested freq */
    };

    (void)clock_f28379x_init(&CLOCK_settings);
    (void)clock_f28379x_pclk_enable(CLOCK_F28379X_PCLK_DMA);
    (void)clock_f28379x_pclk_enable(CLOCK_F28379X_PCLK_SPIA);
}

static void mux_init(void)
{
    struct mux GPIOCTRL_A;
    struct mux GPIOCTRL_B;
    struct mux GPIOCTRL_D;

    (void)mux_ti_f28x_init(&GPIOCTRL_A, ADDR_GPIOCTRL_A);
    (void)mux_ti_f28x_init(&GPIOCTRL_B, ADDR_GPIOCTRL_B);
    (void)mux_ti_f28x_init(&GPIOCTRL_D, ADDR_GPIOCTRL_D);

    /* GPIOs */
    (void)mux_ti_f28x_output(&GPIOCTRL_A, (size_t)31, MUX_TI_F28X_GPIO, MUX_TI_F28X_GPIO);      /* LED */
    (void)mux_ti_f28x_output(&GPIOCTRL_A, (size_t)22, MUX_TI_F28X_GPIO, MUX_TI_F28X_GPIO);      /* RST */
    (void)mux_ti_f28x_output(&GPIOCTRL_D, (size_t)9, MUX_TI_F28X_GPIO, MUX_TI_F28X_GPIO);       /* DC */
    (void)mux_ti_f28x_output(&GPIOCTRL_D, (size_t)8, MUX_TI_F28X_GPIO, MUX_TI_F28X_GPIO);       /* CS */

    /* SPI */
    (void)mux_ti_f28x_output(&GPIOCTRL_B, (size_t)26, MUX_TI_F28X_ALT3, MUX_TI_F28X_ALT3);          /* SPIA MOSI */
    (void)mux_ti_f28x_output(&GPIOCTRL_B, (size_t)28, MUX_TI_F28X_ALT3, MUX_TI_F28X_ALT3);          /* SPIA CLK */
}

static void gpio_init(/*@partial@*/ struct board *ctx)
{
    static struct gpio LED;

    static struct gpio DC;
    static struct gpio RST;
    static struct gpio CS;

    /* LEDs */
    (void)gpio_ti_f28x_init(&LED, ADDR_GPIODATA_A, (size_t)31);
    /* LCD */
    (void)gpio_ti_f28x_init(&DC, ADDR_GPIODATA_D, (size_t)9);
    (void)gpio_ti_f28x_init(&RST, ADDR_GPIODATA_A, (size_t)22);
    (void)gpio_ti_f28x_init(&CS, ADDR_GPIODATA_D, (size_t)8);

    /* leds */
    ctx->LED = &LED;

    /* physical layer */
    ctx->lcd_phys.dc = &DC;
    ctx->lcd_phys.rst = &RST;
    ctx->lcd_phys.cs = &CS;
}

static void spi_init(/*@partial@*/ struct board *ctx)
{
    static struct spi SPI;
    struct spi_settings SPI_settings = {
        50000000ul,
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)8,
        SPI_CS_POL_IGNORE,
        (size_t)0
    };

    /* DMA */
    static struct dma SPI_TX;
    static struct dma SPI_RX;
    struct spi_ti_f28x_dma_settings DMA_settings;

    (void)dma_ti_f28x_init(&SPI_TX, ADDR_DMA, (size_t)0, DMA_TI_F28X_CHSRCSEL_SPITXDMAA);
    (void)dma_ti_f28x_init(&SPI_RX, ADDR_DMA, (size_t)1, DMA_TI_F28X_CHSRCSEL_SPIRXDMAA);

    /* DMA config */
    DMA_settings.fill = &SPI_TX;
    DMA_settings.drain = &SPI_RX;
    DMA_settings.threshold = (size_t)16;

    /* SPI */
    (void)spi_ti_f28x_init(&SPI, ADDR_SPIA, CLOCK_F28379X_LSPCLK);
    (void)spi_setup(&SPI, &SPI_settings);

    /* Despite all my efforts, DMA doesn't want to copy to SPITXBUF
     * and i'm not patient enough to look for a solution, so this is
     * disabled and FPS will be limited to 12 for the moment
     *
     * (void)spi_ti_f28x_dma_setup(&SPI, &DMA_settings);
     */

    /* physical layer */
    ctx->lcd_phys.spi = &SPI;
}

static void wd_init(/*@partial@*/ struct board *ctx)
{
    static struct wd WD;

    /* wd auto-starts */
    (void)wd_ti_f28x_init(&WD, ADDR_WD, CLOCK_F28379X_INTOSC1);
    wd_refresh(&WD);

    ctx->WD = &WD;
}

int board_init(struct board *ctx)
{
    /* before anything else */
    (void)wd_init(ctx);     /* WDT */

    /* clocks & mux */
    clock_init();
    mux_init();

    /* peripherals */

    (void)gpio_init(ctx);   /* GPIO */
    (void)spi_init(ctx);    /* SPI */

    return 0;
}
