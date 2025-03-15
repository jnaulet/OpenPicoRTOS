#include "twi-gd32vf103.h"
#include "picoRTOS.h"

#include <stdint.h>

struct TWI_GD32VF103_I2C {
    volatile uint32_t I2C_CTL0;
    volatile uint32_t I2C_CTL1;
    volatile uint32_t I2C_SADDR0;
    volatile uint32_t I2C_SADDR1;
    volatile uint32_t I2C_DATA;
    volatile uint32_t I2C_STAT0;
    volatile uint32_t I2C_STAT1;
    volatile uint32_t I2C_CKCFG;
    volatile uint32_t I2C_RT;
    uint32_t RESERVED0[27];
    volatile uint32_t I2C_FMPCFG;
};

#define I2C_CTL0_SRESET   (1 << 15)
#define I2C_CTL0_SALT     (1 << 13)
#define I2C_CTL0_PECTRANS (1 << 12)
#define I2C_CTL0_POAP     (1 << 11)
#define I2C_CTL0_ACKEN    (1 << 10)
#define I2C_CTL0_STOP     (1 << 9)
#define I2C_CTL0_START    (1 << 8)
#define I2C_CTL0_SS       (1 << 7)
#define I2C_CTL0_GCEN     (1 << 6)
#define I2C_CTL0_PECEN    (1 << 5)
#define I2C_CTL0_ARPEN    (1 << 4)
#define I2C_CTL0_SMBSEL   (1 << 3)
#define I2C_CTL0_SMBEN    (1 << 1)
#define I2C_CTL0_I2CEN    (1 << 0)

#define I2C_CTL1_DMALST    (1 << 12)
#define I2C_CTL1_DMAON     (1 << 1)
#define I2C_CTL1_BUFIE     (1 << 10)
#define I2C_CTL1_EVIE      (1 << 9)
#define I2C_CTL1_ERRIE     (1 << 8)
#define I2C_CTL1_I2CCLK_M  0x3fu
#define I2C_CTL1_I2CCLK(x) ((x) & I2C_CTL1_I2CCLK_M)

#define I2C_SADDR0_ADDFORMAT    (1 << 15)
#define I2C_SADDR0_ADDRESS10_M  0x3ffu
#define I2C_SADDR0_ADDRESS10(x) ((x) & I2C_SADDR0_ADDRESS10_M)
#define I2C_SADDR0_ADDRESS7_M   0x7fu
#define I2C_SADDR0_ADDRESS7(x)  (((x) & I2C_SADDR0_ADDRESS7_M) << 1)

#define I2C_SADDR1_ADDRESS2_M  0x7fu
#define I2C_SADDR1_ADDRESS2(x) (((x) & I2C_SADDR1_ADDRESS2_M) << 1)
#define I2C_SADDR1_DUADEN      (1 << 0)

#define I2C_STAT0_SMBALT    (1 << 15)
#define I2C_STAT0_SMBTO     (1 << 14)
#define I2C_STAT0_PECERR    (1 << 12)
#define I2C_STAT0_OUERR     (1 << 11)
#define I2C_STAT0_AERR      (1 << 10)
#define I2C_STAT0_LOSTARB   (1 << 9)
#define I2C_STAT0_BERR      (1 << 8)
#define I2C_STAT0_TBE       (1 << 7)
#define I2C_STAT0_RBNE      (1 << 6)
#define I2C_STAT0_STPDET    (1 << 4)
#define I2C_STAT0_ADD10SEND (1 << 3)
#define I2C_STAT0_BTC       (1 << 2)
#define I2C_STAT0_ADDSEND   (1 << 1)
#define I2C_STAT0_SBSEND    (1 << 0)

#define I2C_STAT1_PECV_M  0xffu
#define I2C_STAT1_PECV(x) (((x) & I2C_STAT1_PECV_M) << 8)
#define I2C_STAT1_DUMODF  (1 << 7)
#define I2C_STAT1_HSTSMB  (1 << 6)
#define I2C_STAT1_DEFSMB  (1 << 5)
#define I2C_STAT1_RXGC    (1 << 4)
#define I2C_STAT1_TR      (1 << 2)
#define I2C_STAT1_I2CBSY  (1 << 1)
#define I2C_STAT1_MASTER  (1 << 0)

#define I2C_CKCFG_FAST    (1 << 15)
#define I2C_CKCFG_DCTY    (1 << 14)
#define I2C_CKCFG_CLKC_M  0xfffu
#define I2C_CKCFG_CLKC(x) ((x) & I2C_CKCFG_CLKC_M)

#define I2C_RT_RISETIME_M  0x3fu
#define I2C_RT_RISETIME(x) ((x) & I2C_RT_RISETIME_M)

#define I2C_FMPCFG_FMPEN (1 << 0)

int twi_gd32vf103_init(struct twi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct TWI_GD32VF103_I2C*)base;
    ctx->clkid = clkid;

    return 0;
}

static int set_bitrate(struct twi *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    unsigned long clkc;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq >= (clock_freq_t)2000000, return -EIO);
    picoRTOS_assert(freq <= (clock_freq_t)60000000, return -EIO);

    /* set i2cclk */
    ctx->base->I2C_CTL1 &= I2C_CTL1_I2CCLK(I2C_CTL1_I2CCLK_M);
    ctx->base->I2C_CTL1 |= I2C_CTL1_I2CCLK((unsigned long)freq / 1000000ul);

    if (bitrate <= (unsigned long)TWI_BITRATE_STANDARD)
        ctx->base->I2C_CKCFG &= ~I2C_CKCFG_FAST;
    else
        ctx->base->I2C_CKCFG |= I2C_CKCFG_FAST;

    clkc = ((unsigned long)freq / bitrate) / 2ul;
    picoRTOS_assert(clkc <= (unsigned long)I2C_CKCFG_CLKC_M,
                    return -EINVAL);

    ctx->base->I2C_CKCFG &= ~I2C_CKCFG_CLKC(I2C_CKCFG_CLKC_M);
    ctx->base->I2C_CKCFG |= I2C_CKCFG_CLKC(clkc);

    /* TODO: fast & fast plus */
    return 0;
}

static int set_mode(struct twi *ctx, twi_mode_t mode)
{
    picoRTOS_assert(mode != TWI_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < TWI_MODE_COUNT, return -EINVAL);

    ctx->mode = mode;
    return 0;
}

int twi_setup(struct twi *ctx, const struct twi_settings *settings)
{
    picoRTOS_assert(settings->mode < TWI_MODE_COUNT, return -EINVAL);
    picoRTOS_assert(settings->slave_addr < (twi_addr_t)TWI_ADDR_COUNT, return -EINVAL);

    int res;

    /* disable */
    ctx->base->I2C_CTL0 &= ~I2C_CTL0_I2CEN;

    /* bitrate */
    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    /* mode */
    if (settings->mode != TWI_MODE_IGNORE &&
        (res = set_mode(ctx, settings->mode)) < 0)
        return res;

    if (ctx->mode == TWI_MODE_SLAVE)
        ctx->base->I2C_SADDR0 = (uint32_t)I2C_SADDR0_ADDRESS7(settings->slave_addr);

    /* enable */
    ctx->base->I2C_CTL0 |= I2C_CTL0_I2CEN;

    ctx->slave_addr = settings->slave_addr;
    return 0;
}

int twi_poll(struct twi *ctx)
{
    if ((ctx->base->I2C_STAT0 & I2C_STAT0_ADDSEND) != 0) {
        if ((ctx->base->I2C_STAT1 & I2C_STAT1_TR) != 0) return TWI_READ;
        else return TWI_WRITE;
    }

    return -EAGAIN;
}

/* active operations */

static int twi_rw_as_master_idle(struct twi *ctx)
{
    if ((ctx->base->I2C_STAT1 & I2C_STAT1_I2CBSY) != 0)
        return -EAGAIN;

    /* send start condition */
    ctx->base->I2C_CTL0 |= (uint32_t)I2C_CTL0_START;

    ctx->state = TWI_GD32VF103_STATE_SLA;
    ctx->last = 0;

    return -EAGAIN;
}

static int twi_write_as_master_sla(struct twi *ctx)
{
    if ((ctx->base->I2C_STAT0 & I2C_STAT0_SBSEND) == 0)
        return -EAGAIN;

    /* send address + r */
    ctx->base->I2C_DATA = (uint32_t)(ctx->slave_addr << 1);
    /* clean acken */
    ctx->base->I2C_CTL0 &= ~I2C_CTL0_ACKEN;

    ctx->state = TWI_GD32VF103_STATE_ACK;
    return -EAGAIN;
}

static int twi_rw_as_master_ack(struct twi *ctx)
{
    if ((ctx->base->I2C_STAT0 & I2C_STAT0_ADDSEND) == 0) {
        /* ack error */
        if ((ctx->base->I2C_STAT0 & I2C_STAT0_AERR) != 0) {
            ctx->base->I2C_STAT0 &= ~I2C_STAT0_AERR;
            ctx->state = TWI_GD32VF103_STATE_IDLE;
            return -EIO;
        }

        return -EAGAIN;
    }

    /* clear ADDSEND flag */
    /*@i@*/ (void)ctx->base->I2C_STAT1;

    ctx->state = TWI_GD32VF103_STATE_DATA;
    return -EAGAIN;
}

static int twi_write_as_master_data(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    /* check if tx buf is available */
    if ((ctx->base->I2C_STAT0 & I2C_STAT0_TBE) == 0)
        return -EAGAIN;

    /* load data */
    ctx->base->I2C_DATA = (uint32_t)*(uint8_t*)buf;

    /* last byte */
    if (n == (size_t)1) {
        ctx->last = (int)n;
        ctx->state = TWI_GD32VF103_STATE_STOP;
        return -EAGAIN;
    }

    return 1;
}

static int twi_rw_as_master_stop(struct twi *ctx)
{
    /* no pending transmission */
    if ((ctx->base->I2C_STAT0 & I2C_STAT0_BTC) == 0)
        return -EAGAIN;

    /* send stop condition */
    ctx->base->I2C_CTL0 |= (uint32_t)I2C_CTL0_STOP;

    ctx->state = TWI_GD32VF103_STATE_IDLE;
    return ctx->last;
}

static int twi_write_as_master(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_GD32VF103_STATE_IDLE: return twi_rw_as_master_idle(ctx);
    case TWI_GD32VF103_STATE_SLA: return twi_write_as_master_sla(ctx);
    case TWI_GD32VF103_STATE_ACK: return twi_rw_as_master_ack(ctx);
    case TWI_GD32VF103_STATE_DATA: return twi_write_as_master_data(ctx, buf, n);
    case TWI_GD32VF103_STATE_STOP: return twi_rw_as_master_stop(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_rw_as_slave_idle(struct twi *ctx)
{
    if ((ctx->base->I2C_STAT0 & I2C_STAT0_ADDSEND) != 0) {
        ctx->state = TWI_GD32VF103_STATE_DATA;
        /* clear ADDSEND flag */
        /*@i@*/ (void)ctx->base->I2C_STAT1;
    }

    return -EAGAIN;
}

static int twi_write_as_slave_data(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    /* check if tx buf is available */
    if ((ctx->base->I2C_STAT0 & I2C_STAT0_TBE) == 0)
        return -EAGAIN;

    /* load data */
    ctx->base->I2C_DATA = (uint32_t)*(uint8_t*)buf;

    /* end of transmission */
    if (n == (size_t)1) {
        ctx->state = TWI_GD32VF103_STATE_STOP;
        ctx->last = 1;
        return -EAGAIN;
    }

    return 1;
}

static int twi_write_as_slave_stop(struct twi *ctx)
{
    if ((ctx->base->I2C_STAT0 & I2C_STAT0_AERR) != 0)
        return -EAGAIN;

    /* clear AERR */
    ctx->base->I2C_STAT0 &= ~I2C_STAT0_AERR;

    ctx->state = TWI_GD32VF103_STATE_IDLE;
    return ctx->last;
}

static int twi_write_as_slave(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_GD32VF103_STATE_IDLE: return twi_rw_as_slave_idle(ctx);
    case TWI_GD32VF103_STATE_DATA: return twi_write_as_slave_data(ctx, buf, n);
    case TWI_GD32VF103_STATE_STOP: return twi_write_as_slave_stop(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

int twi_write(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if (ctx->mode == TWI_MODE_MASTER)
        return twi_write_as_master(ctx, buf, n);

    if (ctx->mode == TWI_MODE_SLAVE)
        return twi_write_as_slave(ctx, buf, n);

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_read_as_master_sla(struct twi *ctx)
{
    if ((ctx->base->I2C_STAT0 & I2C_STAT0_SBSEND) == 0)
        return -EAGAIN;

    /* send address + r */
    ctx->base->I2C_DATA = (uint32_t)((ctx->slave_addr << 1) | 0x1);
    /* enable ack */
    ctx->base->I2C_CTL0 |= I2C_CTL0_ACKEN;

    ctx->state = TWI_GD32VF103_STATE_ACK;
    return -EAGAIN;
}

static int twi_read_as_master_data(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    /* check if rx buf is available */
    if ((ctx->base->I2C_STAT0 & I2C_STAT0_RBNE) == 0)
        return -EAGAIN;

    /* one to last byte */
    if (n == (size_t)2)
        ctx->base->I2C_CTL0 &= ~I2C_CTL0_ACKEN;

    /* read data */
    *(uint8_t*)buf = (uint8_t)ctx->base->I2C_DATA;

    /* last byte */
    if (n == (size_t)1) {
        ctx->last = (int)n;
        ctx->state = TWI_GD32VF103_STATE_STOP;
        return -EAGAIN;
    }

    return 1;
}

static int twi_read_as_master(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_GD32VF103_STATE_IDLE: return twi_rw_as_master_idle(ctx);
    case TWI_GD32VF103_STATE_SLA: return twi_read_as_master_sla(ctx);
    case TWI_GD32VF103_STATE_ACK: return twi_rw_as_master_ack(ctx);
    case TWI_GD32VF103_STATE_DATA: return twi_read_as_master_data(ctx, buf, n);
    case TWI_GD32VF103_STATE_STOP: return twi_rw_as_master_stop(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_read_as_slave_data(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->I2C_STAT0 & I2C_STAT0_RBNE) == 0)
        return -EAGAIN;

    /* stop detection */
    if ((ctx->base->I2C_STAT0 & I2C_STAT0_STPDET) != 0) {
        /* wirte to I2C_CTL0 to clear STPDET */
        ctx->base->I2C_CTL0 &= ~I2C_CTL0_SRESET;
        ctx->state = TWI_GD32VF103_STATE_IDLE;
        return 0;
    }

    *(uint8_t*)buf = (uint8_t)ctx->base->I2C_DATA;

    return 1;
}

static int twi_read_as_slave(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_GD32VF103_STATE_IDLE: return twi_rw_as_slave_idle(ctx);
    case TWI_GD32VF103_STATE_DATA: return twi_read_as_slave_data(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

int twi_read(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if (ctx->mode == TWI_MODE_MASTER)
        return twi_read_as_master(ctx, buf, n);

    if (ctx->mode == TWI_MODE_SLAVE)
        return twi_read_as_slave(ctx, buf, n);

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}
