#include "twi-nxp_i2c.h"
#include "picoRTOS.h"

struct TWI_NXP_I2C {
    volatile uint8_t IBAD;
    volatile uint8_t IBFD;
    volatile uint8_t IBCR;
    volatile uint8_t IBSR;
    volatile uint8_t IBDR;
    volatile uint8_t IBIC;
    volatile uint8_t IBDBG;
};

#define IBAD_ADDR_M  0x7fu
#define IBAD_ADDR(x) (((x) & IBAD_ADDR_M) << 1)

#define IBFD_IBC_M  0xffu
#define IBFD_IBC(x) ((x) & IBFD_IBC_M)

#define IBCR_MDIS  (1 << 7)
#define IBCR_IBIE  (1 << 6)
#define IBCR_MSSL  (1 << 5)
#define IBCR_TXRX  (1 << 4)
#define IBCR_NOACK (1 << 3)
#define IBCR_RSTA  (1 << 2)
#define IBCR_DMAEN (1 << 1)

#define IBSR_TCF  (1 << 7)
#define IBSR_IAAS (1 << 6)
#define IBSR_IBB  (1 << 5)
#define IBSR_IBAL (1 << 4)
#define IBSR_SRW  (1 << 2)
#define IBSR_IBIF (1 << 1)
#define IBSR_RXAK (1 << 0)

#define IBIC_BIIE     (1 << 7)
#define IBIC_BYTERXIE (1 << 6)

#define IBDBG_BYTE_RX          (1 << 2)
#define IBDBG_IPG_DEBUG_HALTED (1 << 1)
#define IBDBG_IPG_DEBUG_EN     (1 << 0)

int twi_nxp_i2c_init(struct twi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct TWI_NXP_I2C*)base;
    ctx->clkid = clkid;
    ctx->state = TWI_NXP_I2C_STATE_IDLE;
    ctx->last = 0;

    return 0;
}

static int set_bitrate(struct twi *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    unsigned long ibc;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq > 0, return -EIO);

    ibc = (unsigned long)freq / bitrate;
    picoRTOS_assert(ibc <= (unsigned long)IBFD_IBC_M,
                    return -EINVAL);

    ctx->base->IBFD = (uint8_t)IBFD_IBC(ibc);
    return 0;
}

static int set_mode(struct twi *ctx, twi_mode_t mode)
{
    picoRTOS_assert(mode != TWI_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < TWI_MODE_COUNT, return -EINVAL);

    if (mode == TWI_MODE_MASTER) ctx->base->IBCR |= IBCR_MSSL;
    else ctx->base->IBCR &= ~IBCR_MSSL;

    return 0;
}

int twi_setup(struct twi *ctx, struct twi_settings *settings)
{
    int res;

    /* turn off */
    ctx->base->IBCR |= IBCR_MDIS;

    /* bitrate */
    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    /* mode */
    if (settings->mode != TWI_MODE_IGNORE &&
        (res = set_mode(ctx, settings->mode)) < 0)
        return res;

    if (ctx->mode == TWI_MODE_SLAVE)
        ctx->base->IBAD = (uint8_t)IBAD_ADDR(settings->slave_addr);

    /* turn on */
    ctx->base->IBCR &= ~IBCR_MDIS;

    ctx->slave_addr = settings->slave_addr;
    return 0;
}

int twi_poll(struct twi *ctx)
{
    if ((ctx->base->IBSR & IBSR_IAAS) != 0) {
        if ((ctx->base->IBSR & IBSR_SRW) == 0) return TWI_WRITE;
        else return TWI_READ;
    }

    return -EAGAIN;
}

/* MASTER STOP */

static int twi_send_stop_as_master(struct twi *ctx)
{
    if ((ctx->base->IBSR & IBSR_IBIF) == 0)
        return -EAGAIN;

    ctx->base->IBSR |= IBSR_IBIF; /* ack */
    ctx->base->IBCR &= ~IBCR_MSSL;

    return ctx->last;
}

static int twi_stop_as_master(struct twi *ctx, int ret)
{
    ctx->state = TWI_NXP_I2C_STATE_STOP;
    ctx->last = ret;

    return twi_send_stop_as_master(ctx);
}

/* MASTER TRANSMIT */

static int twi_write_as_master_data(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    const uint8_t *buf8 = (const uint8_t*)buf;

    if ((ctx->base->IBSR & IBSR_IBIF) == 0)
        return -EAGAIN;

    /* check for errors */
    if ((ctx->base->IBSR & IBSR_RXAK) != 0)
        return twi_stop_as_master(ctx, 0);

    if ((ctx->base->IBSR & IBSR_IBB) != 0)
        return twi_stop_as_master(ctx, -EPIPE);

    /* byte trasmitted */
    if ((ctx->base->IBSR & IBSR_TCF) == 0)
        return -EAGAIN;

    ctx->base->IBSR |= IBSR_IBIF; /* ack */
    ctx->base->IBDR = (uint8_t)*buf8;

    if (n == (size_t)1)
        return twi_stop_as_master(ctx, (int)n);

    return 1;
}

static int twi_write_as_master_idle(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->IBSR & IBSR_IBB) != 0)
        return -EAGAIN;

    ctx->base->IBSR |= IBSR_IBIF;
    ctx->base->IBCR |= (IBCR_MSSL | IBCR_TXRX);
    ctx->base->IBDR = (uint8_t)ctx->slave_addr << 1;

    ctx->state = TWI_NXP_I2C_STATE_DATA;
    return twi_write_as_master_data(ctx, buf, n);
}

static int twi_write_as_master(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_NXP_I2C_STATE_IDLE: return twi_write_as_master_idle(ctx, buf, n);
    case TWI_NXP_I2C_STATE_DATA: return twi_write_as_master_data(ctx, buf, n);
    case TWI_NXP_I2C_STATE_STOP: return twi_send_stop_as_master(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

/* SLAVE TRANSMIT */

static int twi_write_as_slave_data(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    const uint8_t *buf8 = (const uint8_t*)buf;

    if ((ctx->base->IBSR & IBSR_IBIF) == 0)
        return -EAGAIN;

    /* errors */
    if ((ctx->base->IBSR & IBSR_IBB) != 0) {
        ctx->state = TWI_NXP_I2C_STATE_IDLE;
        return -EPIPE;
    }

    /* EOT */
    if ((ctx->base->IBSR & IBSR_RXAK) == 0 ||
        (ctx->base->IBSR & IBSR_TCF) == 0)
        return -EAGAIN;

    ctx->base->IBSR |= IBSR_IBIF; /* ack */
    ctx->base->IBDR = (uint8_t)*buf8;

    if (n == (size_t)1)
        ctx->state = TWI_NXP_I2C_STATE_IDLE;

    return 1;
}

static int twi_write_as_slave_idle(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->IBSR & IBSR_IAAS) == 0)
        return -EAGAIN;

    ctx->base->IBCR |= IBCR_TXRX;

    ctx->state = TWI_NXP_I2C_STATE_DATA;
    return twi_write_as_slave_data(ctx, buf, n);
}

static int twi_write_as_slave(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_NXP_I2C_STATE_IDLE: return twi_write_as_slave_idle(ctx, buf, n);
    case TWI_NXP_I2C_STATE_DATA: return twi_write_as_slave_data(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
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

/* MASTER RECEIVE */

static int twi_read_as_master_data(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    uint8_t *buf8 = (uint8_t*)buf;

    if ((ctx->base->IBSR & IBSR_IBIF) == 0)
        return -EAGAIN;

    /* check for errors */
    if ((ctx->base->IBSR & IBSR_IBB) != 0)
        return twi_stop_as_master(ctx, -EPIPE);

    /* byte trasmitted */
    if ((ctx->base->IBSR & IBSR_TCF) == 0)
        return -EAGAIN;

    *buf8 = ctx->base->IBDR;
    ctx->base->IBSR |= IBSR_IBIF; /* ack */

    if (n == (size_t)1)
        return twi_stop_as_master(ctx, (int)n);

    return 1;
}

static int twi_read_as_master_idle(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->IBSR & IBSR_IBB) != 0)
        return -EAGAIN;

    ctx->base->IBSR |= IBSR_IBIF;
    ctx->base->IBCR |= IBCR_MSSL;
    ctx->base->IBCR &= ~IBCR_TXRX;
    ctx->base->IBDR = (uint8_t)((ctx->slave_addr << 1) | TWI_READ);
    /* perform dummy read */
    /*@i@*/ (void)ctx->base->IBDR;

    ctx->state = TWI_NXP_I2C_STATE_DATA;
    return twi_read_as_master_data(ctx, buf, n);
}

static int twi_read_as_master(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_NXP_I2C_STATE_IDLE: return twi_read_as_master_idle(ctx, buf, n);
    case TWI_NXP_I2C_STATE_DATA: return twi_read_as_master_data(ctx, buf, n);
    case TWI_NXP_I2C_STATE_STOP: return twi_send_stop_as_master(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

/* SLAVE RECEIVE */

static int twi_read_as_slave_data(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    uint8_t *buf8 = (uint8_t*)buf;

    if ((ctx->base->IBSR & IBSR_IBIF) == 0 ||
        (ctx->base->IBSR & IBSR_TCF) == 0)
        return -EAGAIN;

    *buf8 = ctx->base->IBDR;
    ctx->base->IBSR |= IBSR_IBIF; /* ack */

    /* signal EOT */
    if (n == (size_t)2)
        ctx->base->IBCR |= IBCR_NOACK;

    if (n == (size_t)1)
        ctx->state = TWI_NXP_I2C_STATE_IDLE;

    return 1;
}

static int twi_read_as_slave_idle(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->IBSR & IBSR_IAAS) == 0)
        return -EAGAIN;

    ctx->base->IBCR &= ~(IBCR_TXRX | IBCR_NOACK);

    ctx->state = TWI_NXP_I2C_STATE_DATA;
    return twi_read_as_slave_data(ctx, buf, n);
}

static int twi_read_as_slave(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_NXP_I2C_STATE_IDLE: return twi_read_as_slave_idle(ctx, buf, n);
    case TWI_NXP_I2C_STATE_DATA: return twi_read_as_slave_data(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
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
