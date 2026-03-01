#include "spi-ch552.h"
#include "picoRTOS_device.h"

#include <picoRTOS.h>

#ifndef S_SPLINT_S
__sfr __at(ADDR_SPI0_STAT) SPI0_STAT;
__sfr __at(ADDR_SPI0_DATA) SPI0_DATA;
__sfr __at(ADDR_SPI0_CTRL) SPI0_CTRL;
__sfr __at(ADDR_SPI0_CK_SE) SPI0_CK_SE;
__sfr __at(ADDR_SPI0_S_PRE) SPI0_S_PRE;
__sfr __at(ADDR_SPI0_SETUP) SPI0_SETUP;
#else
static unsigned char SPI0_STAT;
static unsigned char SPI0_DATA;
static unsigned char SPI0_CTRL;
static unsigned char SPI0_CK_SE;
static unsigned char SPI0_S_PRE;
static unsigned char SPI0_SETUP;
#endif

#define SPI0_STAT_S0_FST_ACT  (1 << 7)
#define SPI0_STAT_S0_IF_OV    (1 << 6)
#define SPI0_STAT_S0_IF_FIRST (1 << 5)
#define SPI0_STAT_S0_IF_BYTE  (1 << 4)
#define SPI0_STAT_S0_FREE     (1 << 3)
#define SPI0_STAT_S0_T_FIFO   (1 << 2)
#define SPI0_STAT_S0_R_FIFO   (1 << 0)

#define SPI0_CTRL_bS0_MISO_OE  (1 << 7)
#define SPI0_CTRL_bS0_MOSI_OE  (1 << 6)
#define SPI0_CTRL_bS0_SCK_OE   (1 << 5)
#define SPI0_CTRL_bS0_DATA_DIR (1 << 4)
#define SPI0_CTRL_bS0_MST_CLK  (1 << 3)
#define SPI0_CTRL_bS0_2_WIRE   (1 << 2)
#define SPI0_CTRL_bS0_CLR_ALL  (1 << 1)
#define SPI0_CTRL_bS0_AUTO_IF  (1 << 0)

#define SPI0_SETUP_bS0_MODE_SLV     (1 << 7)
#define SPI0_SETUP_bS0_IE_FIFO_OV   (1 << 6)
#define SPI0_SETUP_bS0_IE_FIRST     (1 << 5)
#define SPI0_SETUP_bS0_IE_BYTE      (1 << 4)
#define SPI0_SETUP_bS0_BIT_ORDER    (1 << 3)
#define SPI0_SETUP_bS0_SLV_SELT     (1 << 1)
#define SPI0_SETUP_bS0_SLV_PRELOAD  (1 << 0)

int spi_ch552_init(struct spi *ctx, clock_id_t clkid)
{
    ctx->clkid = clkid;
    SPI0_CTRL &= ~SPI0_CTRL_bS0_CLR_ALL;
    return 0;
}

int spi_ch552_set_half_duplex(struct spi *ctx)
{
    /*@i@*/ (void)ctx;

    SPI0_CTRL |= SPI0_CTRL_bS0_2_WIRE;
    return 0;
}

static int set_bitrate(const struct spi *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq > 0, return (int)freq);

    SPI0_CK_SE = (unsigned char)((unsigned long)freq / bitrate);
    return 0;
}

static int set_mode(spi_mode_t mode)
{
    picoRTOS_assert(mode != SPI_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < SPI_MODE_COUNT, return -EINVAL);

    if (mode == SPI_MODE_MASTER) {
        SPI0_SETUP &= ~SPI0_SETUP_bS0_MODE_SLV;
        SPI0_CTRL |= (SPI0_CTRL_bS0_MOSI_OE | SPI0_CTRL_bS0_SCK_OE);
        SPI0_CTRL &= ~SPI0_CTRL_bS0_MISO_OE;
    }else{
        SPI0_SETUP |= SPI0_SETUP_bS0_MODE_SLV;
        SPI0_CTRL &= ~(SPI0_CTRL_bS0_MOSI_OE | SPI0_CTRL_bS0_SCK_OE);
        SPI0_CTRL |= SPI0_CTRL_bS0_MISO_OE;
    }

    return 0;
}

static int set_clkmode(spi_clock_mode_t clkmode)
{
    picoRTOS_assert(clkmode != SPI_CLOCK_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(clkmode < SPI_CLOCK_MODE_COUNT, return -EINVAL);

    switch (clkmode) {
    case SPI_CLOCK_MODE_0:
        SPI0_CTRL &= ~SPI0_CTRL_bS0_MST_CLK;
        break;

    case SPI_CLOCK_MODE_3:
        SPI0_CTRL |= SPI0_CTRL_bS0_MST_CLK;
        break;

    default:
        picoRTOS_assert_void(false);
        /*@notreached@*/
        return -EINVAL;
    }

    return 0;
}

int spi_setup(struct spi *ctx, const struct spi_settings *settings)
{
    int res;

    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    if (settings->clkmode != SPI_CLOCK_MODE_IGNORE &&
        (res = set_clkmode(settings->clkmode)) < 0)
        return res;

    /* ignore frame size */

    if (settings->mode != SPI_MODE_IGNORE &&
        (res = set_mode(settings->mode)) < 0)
        return res;

    /* ignore cs_pol */
    /* ignore cs */

    return 0;
}

int spi_xfer(struct spi *ctx, void *rx, const void *tx, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    /*@i@*/ (void)ctx;

    if ((SPI0_STAT & SPI0_STAT_S0_FREE) != (unsigned char)0) {
        /* rx */
        if ((SPI0_STAT & SPI0_STAT_S0_R_FIFO) != (unsigned char)0) {
            *(unsigned char*)rx = SPI0_DATA;
            return 1;
        }
        /* tx */
        SPI0_DATA = *(unsigned char*)tx;
    }

    return -EAGAIN;
}
