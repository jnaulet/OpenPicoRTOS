#include "lcd.h"
#include "picoRTOS.h"

#include <generated/autoconf.h>

#define LCD_MODE_CMD(ctx)   gpio_write((ctx)->dc, false)
#define LCD_MODE_DATA(ctx)  gpio_write((ctx)->dc, true)
#define LCD_CS_ENABLE(ctx)  gpio_write((ctx)->cs, false)
#define LCD_CS_DISABLE(ctx) gpio_write((ctx)->cs, true)

static int lcd_8bit(struct lcd *ctx)
{
    static const struct spi_settings SPI_settings = {
        0ul, /* bitrate */
        SPI_MODE_IGNORE,
        SPI_CLOCK_MODE_IGNORE,
        (size_t)8,  /* frame size */
        SPI_CS_POL_IGNORE,
        (size_t)0   /* cs */
    };

    return spi_setup(ctx->spi, &SPI_settings);
}

static int lcd_16bit(struct lcd *ctx)
{
    static const struct spi_settings SPI_settings = {
        0ul, /* bitrate */
        SPI_MODE_IGNORE,
        SPI_CLOCK_MODE_IGNORE,
        (size_t)16, /* frame size */
        SPI_CS_POL_IGNORE,
        (size_t)0   /* cs */
    };

    return spi_setup(ctx->spi, &SPI_settings);
}

static int lcd_xfer8(struct lcd *ctx, uint_least8_t x)
{
    int res = 0;
    uint_least8_t dummy = 0;
    int timeout = (int)PICORTOS_DELAY_SEC(1);

    /* byte mode */
    (void)lcd_8bit(ctx);

    /* xfer byte */
    while (timeout-- != 0)
        if ((res = spi_xfer(ctx->spi, &dummy, &x, sizeof(x))) != -EAGAIN)
            break;

    picoRTOS_assert(timeout != -1, return -EBUSY);
    return res;
}

static int lcd_xfer16(struct lcd *ctx, uint16_t x)
{
    int res = 0;
    uint16_t dummy = 0;
    int timeout = (int)PICORTOS_DELAY_SEC(1);

    /* word mode */
    (void)lcd_16bit(ctx);

    /* xfer word */
    while (timeout-- != 0)
        if ((res = spi_xfer(ctx->spi, &dummy, &x, sizeof(x))) != -EAGAIN)
            break;

    picoRTOS_assert(timeout != -1, return -EBUSY);
    return res;
}

static int lcd_reg(struct lcd *ctx, uint_least8_t x)
{
    LCD_MODE_CMD(ctx);
    return lcd_xfer8(ctx, x);
}

static int lcd_u8(struct lcd *ctx, uint_least8_t x)
{
    LCD_MODE_DATA(ctx);
    return lcd_xfer8(ctx, x);
}

static int lcd_u16(struct lcd *ctx, uint16_t x)
{
    LCD_MODE_DATA(ctx);
    return lcd_xfer16(ctx, x);
}

static void lcd_set_addr(struct lcd *ctx, long x, long y, long w, long h)
{
    /* x */
    (void)lcd_reg(ctx, (uint_least8_t)0x2a);
    (void)lcd_u16(ctx, (uint16_t)(x + 1l));
    (void)lcd_u16(ctx, (uint16_t)(x + w));
    /* y */
    (void)lcd_reg(ctx, (uint_least8_t)0x2b);
    (void)lcd_u16(ctx, (uint16_t)(y + 26l));
    (void)lcd_u16(ctx, (uint16_t)(y + h + 25l));
    /* end */
    (void)lcd_reg(ctx, (uint_least8_t)0x2c);
}

static void lcd_clear(/*@partial@*/ struct lcd *ctx, uint16_t color)
{
    size_t n = (size_t)(LCD_WIDTH * LCD_HEIGHT);

    /* set fb to zero */
    while (n-- != 0)
        ctx->fb[n] = color;

    (void)lcd_refresh(ctx);
}

static int lcd_run_init_seq(/*@partial@*/ struct lcd *ctx)
{
    const int *p;
    static const int init_seq[] = {
        0x21, 0xff,                                     /* display inversion on */
        0xb1, 0x05, 0x3a, 0x3a, 0xff,                   /* framerate 1 (normal) */
        0xb2, 0x05, 0x3a, 0x3a, 0xff,                   /* framerate 2 (idle) */
        0xb3, 0x05, 0x3a, 0x3a, 0x05, 0x3a, 0x3a, 0xff, /* framerate 3 (full) */
        0xb4, 0x03, 0xff,                               /* inversion color */
        0xc0, 0x62, 0x02, 0x04, 0xff,                   /* power control 1 */
        0xc1, 0xc0, 0xff,                               /* power control 2 */
        0xc2, 0x0d, 0x00, 0xff,                         /* power control 3 */
        0xc3, 0x8d, 0x6a, 0xff,                         /* power control 4 */
        0xc4, 0x8d, 0xee, 0xff,                         /* power control 5 */
        0xc5, 0x0e, 0xff,                               /* VCOM control 1 */
        0xe0, 0x10, 0x0e, 0x02, 0x03, 0x0e, 0x07, 0x02, /* gamma + polarty corr */
        0x07, 0x0a, 0x12, 0x27, 0x37, 0x00, 0x0d, 0x0e,
        0x10, 0xff,
        0xe1, 0x10, 0x0e, 0x03, 0x03, 0x0f, 0x06, 0x02,    /* gamma + polarty corr */
        0x08, 0x0a, 0x13, 0x26, 0x36, 0x00, 0x0d, 0x0e,
        0x10, 0xff,
        0x3a, 0x55, 0xff,   /* interface pixel format */
        0x36, 0x78, 0xff,   /* memory data access control */
        0x29, 0xff,         /* display on */
        0x11, 0xff,         /* sleep out */
        0xff
    };

    LCD_CS_ENABLE(ctx);

    /* Initialize the display */
    for (p = init_seq; *p != 0xff; p++) {
        (void)lcd_reg(ctx, (uint_least8_t)*p++);
        if (*p == 0xff)
            continue;

        while (*p != 0xff)
            (void)lcd_u8(ctx, (uint_least8_t)*p++);
    }

    /* Clear display */
    lcd_clear(ctx, (uint16_t)0);
    return 0;
}

int lcd_init(struct lcd *ctx, struct lcd_phys *phys)
{
    ctx->spi = phys->spi;
    ctx->dc = phys->dc;
    ctx->rst = phys->rst;
    ctx->cs = phys->cs;

    /* init pins */
    LCD_MODE_CMD(ctx);
    LCD_CS_DISABLE(ctx);

    /* reset */
    gpio_write(ctx->rst, false);
    picoRTOS_sleep(PICORTOS_DELAY_MSEC(1l));
    gpio_write(ctx->rst, true);
    picoRTOS_sleep(PICORTOS_DELAY_MSEC(5l));

    return lcd_run_init_seq(ctx);
}

int lcd_refresh(struct lcd *ctx)
{
    size_t xfered = 0;
    int timeout = (int)PICORTOS_DELAY_MSEC(200l);
    uint_least8_t *fb8 = (uint_least8_t*)ctx->fb;

    lcd_set_addr(ctx, 0, 0, LCD_WIDTH, LCD_HEIGHT);

    LCD_MODE_DATA(ctx);
    (void)lcd_16bit(ctx);

    while (xfered < sizeof(ctx->fb) && timeout-- != 0) {
        int res;
        if ((res = spi_xfer(ctx->spi, fb8, &fb8[xfered],
                            sizeof(ctx->fb) - xfered)) == -EAGAIN) {
            /* wait */
            picoRTOS_schedule();
            continue;
        }

        xfered += (size_t)res;
    }

    picoRTOS_assert(timeout != -1, return -EBUSY);
    return 0;
}
