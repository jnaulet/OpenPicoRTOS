#include "board.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "clock-gd32vf103.h"
#include "dma-gd32vf103.h"
#include "gpio-gd32vf103.h"
#include "mux-gd32vf103.h"
#include "spi-gd32vf103.h"
#include "wd-gd32vf103_fwdgt.h"

static void clock_init(void)
{
    struct clock_settings CLOCK_settings = {
        0ul, /* disable hxtal */
        CLOCK_GD32VF103_CK_SYS_PLL,
        CLOCK_GD32VF103_PLLSEL_IRC8M,
        108000000ul,    /* pll */
        1ul,            /* ahb_div */
        2ul,            /* apb1_div (54Mhz max) */
        1ul,            /* apb2_div */
        8ul,            /* adc_div */
    };

    (void)clock_gd32vf103_init(&CLOCK_settings);
}

static void mux_init(void)
{
    struct mux MUXA;
    struct mux MUXB;

    /* enable clocks */
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_PA);
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_PB);
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_AF);

    (void)mux_gd32vf103_init(&MUXA, ADDR_GPIOA);
    (void)mux_gd32vf103_init(&MUXB, ADDR_GPIOB);

    /* LED */
    (void)mux_gd32vf103_output(&MUXA, (size_t)2,     /* LED (Blue) */
                               MUX_GD32VF103_OUTPUT_PUSH_PULL,
                               MUX_GD32VF103_SPEED_2MHZ);
    /* SPI0 */
    (void)mux_gd32vf103_afio(&MUXA, (size_t)5,        /* SCK0 */
                             MUX_GD32VF103_AFIO_PUSH_PULL,
                             MUX_GD32VF103_SPEED_50MHZ);
    (void)mux_gd32vf103_afio(&MUXA, (size_t)7,      /* MOSI0 */
                             MUX_GD32VF103_AFIO_PUSH_PULL,
                             MUX_GD32VF103_SPEED_50MHZ);
    /* LCD */
    (void)mux_gd32vf103_output(&MUXB, (size_t)0,     /* RS/DC */
                               MUX_GD32VF103_OUTPUT_PUSH_PULL,
                               MUX_GD32VF103_SPEED_2MHZ);
    (void)mux_gd32vf103_output(&MUXB, (size_t)1,     /* RST */
                               MUX_GD32VF103_OUTPUT_PUSH_PULL,
                               MUX_GD32VF103_SPEED_2MHZ);
    (void)mux_gd32vf103_output(&MUXB, (size_t)2,     /* CS */
                               MUX_GD32VF103_OUTPUT_PUSH_PULL,
                               MUX_GD32VF103_SPEED_2MHZ);
}

static int gpio_init(/*@partial@*/ struct board *ctx)
{
    static struct gpio LED;

    static struct gpio DC;
    static struct gpio RST;
    static struct gpio CS;

    /* LEDs */
    (void)gpio_gd32vf103_init(&LED, ADDR_GPIOA, (size_t)2);
    /* LCD */
    (void)gpio_gd32vf103_init(&DC, ADDR_GPIOB, (size_t)0);
    (void)gpio_gd32vf103_init(&RST, ADDR_GPIOB, (size_t)1);
    (void)gpio_gd32vf103_init(&CS, ADDR_GPIOB, (size_t)2);

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
    static struct spi SPI0;
    static struct dma DMA0_CH1;
    static struct dma DMA0_CH2;

    struct spi_gd32vf103_dma_settings DMA_settings;
    struct spi_settings SPI0_settings = {
        54000000ul, /* bitrate */
        SPI_MODE_MASTER,
        SPI_CLOCK_MODE_0,
        (size_t)8,  /* frame size */
        SPI_CS_POL_ACTIVE_LOW,
        (size_t)0   /* cs */
    };

    /* SPI */
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_SPI0);
    (void)spi_gd32vf103_init(&SPI0, ADDR_SPI0, CLOCK_GD32VF103_CK_SYS);
    (void)spi_setup(&SPI0, &SPI0_settings);

    /* DMA */
    (void)clock_gd32vf103_enable(CLOCK_GD32VF103_CLK_DMA0);
    (void)dma_gd32vf103_init(&DMA0_CH1, ADDR_DMA0, (size_t)1);
    (void)dma_gd32vf103_init(&DMA0_CH2, ADDR_DMA0, (size_t)2);

    /* runtime DMA setup */
    DMA_settings.fill = &DMA0_CH2;
    DMA_settings.drain = &DMA0_CH1;
    DMA_settings.threshold = (size_t)16; /* FIXME */
    (void)spi_gd32vf103_setup_dma(&SPI0, &DMA_settings);

    /* physical layer */
    ctx->lcd_phys.spi = &SPI0;
    return 0;
}

static int wd_init(/*@partial@*/ struct board *ctx)
{
    static struct wd WD;
    struct wd_gd32vf103_fwdgt_settings WD_settings = {
        16000ul, /* timeout_us */
    };

    (void)wd_gd32vf103_fwdgt_init(&WD, ADDR_FWDGT, CLOCK_GD32VF103_CK_FWDGT);
    (void)wd_gd32vf103_fwdgt_setup(&WD, &WD_settings);

    ctx->WD = &WD;

    return wd_start(&WD);
}

int board_init(struct board *ctx)
{
    clock_init();
    mux_init();

    (void)gpio_init(ctx);
    (void)spi_init(ctx);
    (void)wd_init(ctx);

    return 0;
}
