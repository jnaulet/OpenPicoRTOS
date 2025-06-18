#include "twi-n76e003.h"

#include "picoRTOS.h"
#include "picoRTOS_device.h"

#ifndef S_SPLINT_S
__sfr __at(ADDR_I2CON)  I2CON;
__sfr __at(ADDR_I2STAT) I2STAT;
__sfr __at(ADDR_I2DAT)  I2DAT;
__sfr __at(ADDR_I2ADDR) I2ADDR;
__sfr __at(ADDR_I2CLK)  I2CLK;
#else
static unsigned char I2CON;
static unsigned char I2STAT;
static unsigned char I2DAT;
/*@unused@*/ static unsigned char I2ADDR;
static unsigned char I2CLK;
#endif

#define I2CON_I2CEN (1 << 6)
#define I2CON_STA   (1 << 5)
#define I2CON_STO   (1 << 4)
#define I2CON_SI    (1 << 3)
#define I2CON_AA    (1 << 2)
#define I2CON_I2CPX (1 << 0)

#define I2STAT_I2STAT_M 0x1fu
#define I2STAT_I2STAT(x) (((x) & I2STAT_I2STAT_M) << 3)

#define I2ADDR_I2ADDR_M  0x7fu
#define I2ADDR_I2ADDR(x) (((x) & I2ADDR_I2ADDR_M) << 1)
#define I2ADDR_GC        (1 << 0)

int twi_n76e003_init(struct twi *ctx, clock_id_t clkid, bool i2cpx)
{
    ctx->clkid = clkid;
    ctx->pending = false;
    ctx->state = TWI_N76E003_STATE_IDLE;

    if (i2cpx) I2CON |= I2CON_I2CPX;
    I2CON |= I2CON_I2CEN;

    return 0;
}

static int set_bitrate(struct twi *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq > 0, return -EIO);
    I2CLK = (unsigned char)((((unsigned long)freq / bitrate) / 4ul) - 1ul);

    return 0;
}

int twi_setup(struct twi *ctx, const struct twi_settings *settings)
{
    int res;

    /* bitrate */
    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    /* ignore mode */
    /* slave addr */
    ctx->addr = (unsigned char)settings->slave_addr;

    return 0;
}

static int master_rw_idle(struct twi *ctx, int flags)
{
    int res = -EAGAIN;

    if ((flags & TWI_F_START) == 0) {
        ctx->state = TWI_N76E003_STATE_DATA;
        return -EAGAIN;
    }

    if (!ctx->pending) {
        I2CON &= ~I2CON_SI;
        I2CON = (unsigned char)(I2CON_STA | I2CON_I2CEN);
        ctx->pending = true;

    }else{
        if ((I2CON & I2CON_SI) != (unsigned char)0) {
            if (I2STAT == (unsigned char)0x08 ||    /* start */
                I2STAT == (unsigned char)0x10) {    /* repeated start */
                I2CON &= ~I2CON_STA;
                ctx->state = TWI_N76E003_STATE_START;

            }else{
                /* STOP */
                I2CON |= I2CON_STO;
                I2CON &= ~(I2CON_STA | I2CON_SI);
                /* stop debugging */
                picoRTOS_break();
                /*@notreached@*/ res = -EIO;
            }

            /* reset */
            ctx->pending = false;
        }
    }

    return res;
}

static int master_rw_start(struct twi *ctx, unsigned char rw, /*@unused@*/ int flags)
{
    picoRTOS_assert(rw <= (unsigned char)TWI_READ, return -EINVAL);

    int res = -EAGAIN;

    if (!ctx->pending) {
        I2DAT = (unsigned char)((ctx->addr << 1) | rw);
        I2CON &= ~I2CON_SI;
        ctx->pending = true;

    }else{
        if ((I2CON & I2CON_SI) != (unsigned char)0) {
            if (I2STAT == (unsigned char)0x18 ||
                I2STAT == (unsigned char)0x40)
                /* SLA+RW , ACK */
                ctx->state = TWI_N76E003_STATE_DATA;

            else if (I2STAT == (unsigned char)0x20 ||
                     I2STAT == (unsigned char)0x48) {
                /* SLA+RW , NACK */
                ctx->state = TWI_N76E003_STATE_IDLE;
                res = -ENOENT;
                /* stop */
                I2CON |= (I2CON_STO | I2CON_AA);
                I2CON &= ~I2CON_SI;

            }else{
                /* ??? */
                picoRTOS_break();
                /*@notreached@*/ res = -EIO;
            }

            /* reset */
            ctx->pending = false;
        }
    }

    return res;
}

static int master_write_data(struct twi *ctx, const void *buf, size_t n, int flags)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int res = -EAGAIN;

    if (!ctx->pending) {
        I2DAT = *(unsigned char*)buf;
        ctx->pending = true;
        I2CON &= ~I2CON_SI;

    }else{
        if ((I2CON & I2CON_SI) != (unsigned char)0) {
            if (I2STAT == (unsigned char)0x28) {
                /* ACK */
                if (n == (size_t)1) {
                    if ((flags & TWI_F_STOP) != 0) {
                        I2CON |= I2CON_STO;
                        I2CON &= ~I2CON_SI;
                    }

                    ctx->state = TWI_N76E003_STATE_IDLE;
                }
                /* next */
                res = 1;

            }else if (I2STAT == (unsigned char)0x30) {
                /* NACK */
                I2CON |= I2CON_STO;
                ctx->state = TWI_N76E003_STATE_IDLE;
                res = 1;

            }else{
                /* ??? */
                picoRTOS_break();
                /*@notreached@*/ res = -EIO;
            }

            /* reset */
            ctx->pending = false;
        }
    }

    return res;
}

int twi_write(struct twi *ctx, const void *buf, size_t n, int flags)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_N76E003_STATE_IDLE: return master_rw_idle(ctx, flags);
    case TWI_N76E003_STATE_START: return master_rw_start(ctx, (unsigned char)TWI_WRITE, flags);
    case TWI_N76E003_STATE_DATA: return master_write_data(ctx, buf, n, flags);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

static int master_read_data(struct twi *ctx, void *buf, size_t n, int flags)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int res = -EAGAIN;

    if (!ctx->pending) {
        if (n == (size_t)1) I2CON = (unsigned char)I2CON_I2CEN;
        else I2CON = (unsigned char)(I2CON_AA | I2CON_I2CEN);
        ctx->pending = true;

    }else{
        if ((I2CON & I2CON_SI) != (unsigned char)0) {
            if (I2STAT == (unsigned char)0x50) {
                /* RX, ACK */
                *(unsigned char*)buf = I2DAT;
                res = 1;

            }else if (I2STAT == (unsigned char)0x58) {
                /* RX, NACK */
                *(unsigned char*)buf = I2DAT;
                if ((flags & TWI_F_STOP) != 0)
                    I2CON |= I2CON_STO;

                ctx->state = TWI_N76E003_STATE_IDLE;
                res = 1;

            }else {
                /* ??? */
                picoRTOS_break();
                /*@notreached@*/ res = -EIO;
            }

            /* reset */
            ctx->pending = false;
        }
    }

    return res;
}

int twi_read(struct twi *ctx, void *buf, size_t n, int flags)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_N76E003_STATE_IDLE: return master_rw_idle(ctx, flags);
    case TWI_N76E003_STATE_START: return master_rw_start(ctx, (unsigned char)TWI_READ, flags);
    case TWI_N76E003_STATE_DATA: return master_read_data(ctx, buf, n, flags);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

/*
 * No slave support yet
 */

int twi_poll(struct twi *ctx)
{
    /*@i@*/ (void)ctx;
    return -EIO;
}
