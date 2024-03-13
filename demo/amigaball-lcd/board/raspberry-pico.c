#include "board.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "clock-rp2040.h"
#include "dma-rp2040.h"
#include "gpio-rp2040_sio.h"
#include "misc-rp2040_reset.h"
#include "mux-rp2040.h"
#include "spi-arm_pl022.h"
#include "wd-rp2040.h"

/* reset subsystem */
static struct rp2040_reset RESET;

static void init_clock(void)
{
    struct clock_settings CLOCK_settings = {
        12000000ul,                             /* xosc */
        125000000ul,                            /* pll_sys */
        /* dividers */
        1ul,                                    /* ref_div */
        1ul,                                    /* sys_div */
        1ul,                                    /* clk_gpout0_div */
        1ul,                                    /* clk_gpout1_div */
        1ul,                                    /* clk_gpout2_div */
        1ul,                                    /* clk_gpout3_div */
    };

    (void)clock_rp2040_init(&CLOCK_settings);

    /* enable peripherals */
    (void)clock_rp2040_enable(CLOCK_RP2040_PERI);
}

static void init_mux(void)
{
    struct mux MUX;

    /* clock subsystem */
    (void)rp2040_unreset(&RESET, RP2040_RESET_IO_BANK0);
    (void)rp2040_unreset(&RESET, RP2040_RESET_PADS_BANK0);

    (void)mux_rp2040_init(&MUX, ADDR_IO_BANK0, ADDR_PADS_BANK0);

    /* SPI */
    (void)mux_rp2040_output(&MUX, (size_t)10, MUX_RP2040_F1);   /* SPI1 SCK */
    (void)mux_rp2040_output(&MUX, (size_t)11, MUX_RP2040_F1);   /* SPI1 TX */

    /* LEDs */
    (void)mux_rp2040_output(&MUX, (size_t)25, MUX_RP2040_F5);

    /* LCD */
    (void)mux_rp2040_output(&MUX, (size_t)8, MUX_RP2040_F5);
    (void)mux_rp2040_output(&MUX, (size_t)9, MUX_RP2040_F5);
    (void)mux_rp2040_output(&MUX, (size_t)12, MUX_RP2040_F5);
    (void)mux_rp2040_output(&MUX, (size_t)13, MUX_RP2040_F5);
}

static void init_spi(/*@partial@*/ struct board *ctx)
{
    static struct spi SPI;
    struct spi_settings SPI_settings = {
        62500000ul,                           /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)16,                           /* frame size */
        SPI_CS_POL_IGNORE,
        (size_t)0,
    };

    static struct dma DMA_SPI1_TX;
    static struct dma DMA_SPI1_RX;
    struct spi_arm_pl022_dma_settings DMA_settings;

    (void)rp2040_unreset(&RESET, RP2040_RESET_DMA);
    (void)rp2040_unreset(&RESET, RP2040_RESET_SPI1);

    (void)dma_r2040_init(&DMA_SPI1_TX, ADDR_DMA, (size_t)0, DMA_RP2040_TREQ_SEL_DREQ_SPI1_TX);
    (void)dma_r2040_init(&DMA_SPI1_RX, ADDR_DMA, (size_t)1, DMA_RP2040_TREQ_SEL_DREQ_SPI1_RX);

    (void)spi_arm_pl022_init(&SPI, ADDR_SPI1, CLOCK_RP2040_PERI);
    (void)spi_setup(&SPI, &SPI_settings);

    /* DMA config */
    DMA_settings.fill = &DMA_SPI1_TX;
    DMA_settings.drain = &DMA_SPI1_RX;
    DMA_settings.threshold = (size_t)16;
    (void)spi_arm_pl022_setup_dma(&SPI, &DMA_settings);

    /* physical layer */
    ctx->lcd_phys.spi = &SPI;
}

static void init_gpio(/*@partial@*/ struct board *ctx)
{
    static struct gpio LED;

    struct gpio_settings LED_settings = {
        GPIO_INVERT_ENABLE,
    };

    static struct gpio DC;
    static struct gpio RST;
    static struct gpio CS;
    static struct gpio BL;

    /* LEDs */
    (void)gpio_rp2040_sio_init(&LED, ADDR_SIO_GPIO, (size_t)25, GPIO_RP2040_SIO_DIR_OUTPUT);
    (void)gpio_setup(&LED, &LED_settings);
    /* LCD */
    (void)gpio_rp2040_sio_init(&DC, ADDR_SIO_GPIO, (size_t)8, GPIO_RP2040_SIO_DIR_OUTPUT);
    (void)gpio_rp2040_sio_init(&CS, ADDR_SIO_GPIO, (size_t)9, GPIO_RP2040_SIO_DIR_OUTPUT);
    (void)gpio_rp2040_sio_init(&RST, ADDR_SIO_GPIO, (size_t)12, GPIO_RP2040_SIO_DIR_OUTPUT);
    (void)gpio_rp2040_sio_init(&BL, ADDR_SIO_GPIO, (size_t)13, GPIO_RP2040_SIO_DIR_OUTPUT);

    /* leds */
    ctx->LED = &LED;

    /* physical layer */
    ctx->lcd_phys.dc = &DC;
    ctx->lcd_phys.rst = &RST;
    ctx->lcd_phys.cs = &CS;

    /* force BL on */
    gpio_write(&BL, true);
}

static void init_wd(/*@partial@*/ struct board *ctx)
{
    static struct wd WD;
    struct wd_rp2040_settings WD_settings = {
        16000ul, /* timeout_us */
    };

    (void)wd_rp2040_init(&WD, ADDR_WATCHDOG, CLOCK_RP2040_REF);
    (void)wd_rp2040_setup(&WD, &WD_settings);
    (void)wd_start(&WD);

    ctx->WD = &WD;
}

int board_init(/*@out@*/ struct board *ctx)
{
    /* reset subsystem (required) */
    (void)rp2040_reset_init(&RESET, ADDR_RESET);

    init_clock();
    init_mux();

    init_spi(ctx);
    init_gpio(ctx);
    init_wd(ctx);

    return 0;
}
