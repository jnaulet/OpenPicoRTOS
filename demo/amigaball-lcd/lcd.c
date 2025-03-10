#include "lcd.h"
#include "picoRTOS.h"

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
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    /* byte mode */
    (void)lcd_8bit(ctx);

    /* xfer byte, should take less than 1 tick */
    while (!PICORTOS_DELAY_ELAPSED(ref, (picoRTOS_tick_t)2)) {
        if ((res = spi_xfer(ctx->spi, &dummy, &x, sizeof(x))) != -EAGAIN)
            break;
        /* give it some air */
        picoRTOS_postpone();
    }

    picoRTOS_assert(!PICORTOS_DELAY_ELAPSED(ref, (picoRTOS_tick_t)2), return -EBUSY);
    return res;
}

static int lcd_xfer16(struct lcd *ctx, uint16_t x)
{
    int res = 0;
    size_t xfered = 0;
    uint16_t dummy = 0;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    uint_least8_t *x8 = (uint_least8_t*)&x;

    /* word mode */
    (void)lcd_16bit(ctx);

    /* FIXME */
#if defined(CONFIG_AVR) && !defined(S_SPLINT_S)
    x = __builtin_bswap16(x);
#endif

    /* xfer word, should be almost immediate */
    while (xfered != sizeof(x) &&
           !PICORTOS_DELAY_ELAPSED(ref, (picoRTOS_tick_t)2)) {
        /* xfer */
        if ((res = spi_xfer(ctx->spi, &dummy, &x8[xfered], sizeof(x) - xfered)) == -EAGAIN) {
            picoRTOS_postpone();
            continue;
        }

        xfered += (size_t)res;
    }

    picoRTOS_assert(!PICORTOS_DELAY_ELAPSED(ref, (picoRTOS_tick_t)2), return -EBUSY);
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
    LCD_CS_ENABLE(ctx);

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

    LCD_CS_DISABLE(ctx);
}

static void lcd_clear(/*@partial@*/ struct lcd *ctx, uint16_t color)
{
    int n = LCD_FB_DIVIDER;

    lcd_zero(ctx);

    while (n-- != 0) {

        size_t i = (size_t)LCD_FB_PIXELS;

        /* set fb to zero */
        while (i-- != 0)
            ctx->fb[i] = color;

        (void)lcd_refresh(ctx);
    }
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
#ifdef CONFIG_AVR
        0x12, 0xff,         /* partial mode */
#endif
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

    LCD_CS_DISABLE(ctx);

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

void lcd_zero(struct lcd *ctx)
{
    lcd_set_addr(ctx, 0, 0, LCD_WIDTH, LCD_HEIGHT);

    LCD_MODE_DATA(ctx);
    (void)lcd_16bit(ctx);
}

int lcd_refresh(struct lcd *ctx)
{
    /* max 30 fps */
#define LCD_REFRESH_TIMEOUT PICORTOS_DELAY_MSEC(34)

    size_t xfered = 0;
    picoRTOS_tick_t ref = picoRTOS_get_tick();
    uint_least8_t *fb8 = (uint_least8_t*)ctx->fb;

    LCD_CS_ENABLE(ctx);

    while (xfered < sizeof(ctx->fb) &&
           !PICORTOS_DELAY_ELAPSED(ref, LCD_REFRESH_TIMEOUT)) {

        int res;
        if ((res = spi_xfer(ctx->spi, fb8, &fb8[xfered],
                            sizeof(ctx->fb) - xfered)) == -EAGAIN) {
            /* wait */
            picoRTOS_postpone();
            continue;
        }

        xfered += (size_t)res;
    }

    LCD_CS_DISABLE(ctx);
    picoRTOS_assert(!PICORTOS_DELAY_ELAPSED(ref, LCD_REFRESH_TIMEOUT), return -EBUSY);
    return 0;
}
