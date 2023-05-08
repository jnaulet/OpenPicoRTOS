#include "twi-pic32mx.h"
#include "picoRTOS.h"

#include <stdint.h>
#include "misc-pic32mx.h"

struct TWI_PIC32MX {
    struct PIC32MX_CLR_SET_INV I2CxCON;
    struct PIC32MX_CLR_SET_INV I2CxSTAT;
    struct PIC32MX_CLR_SET_INV I2CxADD;
    struct PIC32MX_CLR_SET_INV I2CxMSK;
    struct PIC32MX_CLR_SET_INV I2CxBRG;
    struct PIC32MX_CLR_SET_INV I2CxTRN;
    struct PIC32MX_CLR_SET_INV I2CxRCV;
};

#define I2CxCON_PCIE   (1 << 22)
#define I2CxCON_SCIE   (1 << 21)
#define I2CxCON_BOEN   (1 << 20)
#define I2CxCON_SDAHT  (1 << 19)
#define I2CxCON_SDBCE  (1 << 18)
#define I2CxCON_AHEN   (1 << 17)
#define I2CxCON_DHEN   (1 << 16)
#define I2CxCON_ON     (1 << 15)
#define I2CxCON_SIDL   (1 << 13)
#define I2CxCON_SCLREL (1 << 12)
#define I2CxCON_STRICT (1 << 11)
#define I2CxCON_A10M   (1 << 10)
#define I2CxCON_DISSLW (1 << 9)
#define I2CxCON_SMEN   (1 << 8)
#define I2CxCON_GCEN   (1 << 7)
#define I2CxCON_STREN  (1 << 6)
#define I2CxCON_ACKDT  (1 << 5)
#define I2CxCON_ACKEN  (1 << 4)
#define I2CxCON_RCEN   (1 << 3)
#define I2CxCON_PEN    (1 << 2)
#define I2CxCON_RSEN   (1 << 1)
#define I2CxCON_SEN    (1 << 0)

#define I2CxSTAT_ACKSTAT (1 << 15)
#define I2CxSTAT_TRSTAT  (1 << 14)
#define I2CxSTAT_ACKTIM  (1 << 13)
#define I2CxSTAT_BCL     (1 << 10)
#define I2CxSTAT_GCSTAT  (1 << 9)
#define I2CxSTAT_ADD10   (1 << 8)
#define I2CxSTAT_IWCOL   (1 << 7)
#define I2CxSTAT_I2COV   (1 << 6)
#define I2CxSTAT_D_A     (1 << 5)
#define I2CxSTAT_P       (1 << 4)
#define I2CxSTAT_S       (1 << 3)
#define I2CxSTAT_R_W     (1 << 2)
#define I2CxSTAT_RBF     (1 << 1)
#define I2CxSTAT_TBF     (1 << 0)

/* Function: twi_pic32mx_init
 * Initializes a TWI/I2C peripheral
 *
 * Parameters:
 *  ctx - The TWI to init
 *  base - The TWI base adrdress
 *  clkid - The TWI input clock id
 *
 * Returns:
 * Always 0
 */
int twi_pic32mx_init(struct twi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct TWI_PIC32MX*)base;
    ctx->clkid = clkid;
    ctx->mode = TWI_MODE_SLAVE;
    ctx->state = TWI_PIC32MX_STATE_IDLE;
    ctx->slave_addr = (twi_addr_t)0;
    ctx->last = 0;

    /* turn on module */
    ctx->base->I2CxCON.SET = (uint32_t)I2CxCON_ON;

    return 0;
}

static int set_bitrate(struct twi *ctx, unsigned long bitrate)
{
#define PGD_TYPICAL 104ul

    if (!picoRTOS_assert(bitrate > 0)) return -EINVAL;

    clock_freq_t freq = clock_get_freq(ctx->clkid);
    unsigned long t = 1000000000ul / (bitrate * 2ul) - PGD_TYPICAL;
    unsigned long corrected_bitrate = 1000000000ul / t;
    unsigned long brg = (unsigned long)freq / corrected_bitrate - 2ul;

    if (!picoRTOS_assert(freq > 0))
        return -EIO;

    ctx->base->I2CxBRG.REG = (uint32_t)brg;
    return 0;
}

static int set_mode(struct twi *ctx, twi_mode_t mode)
{
    if (!picoRTOS_assert(mode != TWI_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(mode < TWI_MODE_COUNT)) return -EINVAL;

    ctx->mode = mode;

    if (mode == TWI_MODE_SLAVE)
        ctx->base->I2CxCON.SET = (uint32_t)I2CxCON_STREN;
    else
        ctx->base->I2CxCON.CLR = (uint32_t)I2CxCON_STREN;

    return 0;
}

int twi_setup(struct twi *ctx, struct twi_settings *settings)
{
    if (!picoRTOS_assert(settings->mode < TWI_MODE_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->slave_addr < (twi_addr_t)TWI_ADDR_COUNT)) return -EINVAL;

    int res;

    /* bitrate */
    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    /* mode */
    if (settings->mode != TWI_MODE_IGNORE &&
        (res = set_mode(ctx, settings->mode)) < 0)
        return res;

    if (ctx->mode == TWI_MODE_SLAVE)
        ctx->base->I2CxADD.REG = (uint32_t)settings->slave_addr;

    ctx->slave_addr = settings->slave_addr;
    return 0;
}

int twi_poll(struct twi *ctx)
{
    if ((ctx->base->I2CxSTAT.REG & I2CxSTAT_D_A) == 0) {
        if ((ctx->base->I2CxSTAT.REG & I2CxSTAT_R_W) == 0) return TWI_WRITE;
        else return TWI_READ;
    }

    return -EAGAIN;
}

static int twi_rw_as_master_idle(struct twi *ctx)
{
    /* check if bus is idle */
    if ((ctx->base->I2CxCON.REG & I2CxCON_PEN) != 0)
        return -EAGAIN;

    /* send start condition */
    ctx->base->I2CxCON.SET = (uint32_t)I2CxCON_SEN;

    ctx->state = TWI_PIC32MX_STATE_SLA;
    ctx->last = 0;

    return -EAGAIN;
}

static int twi_write_as_master_sla(struct twi *ctx)
{
    if ((ctx->base->I2CxCON.REG & I2CxCON_SEN) != 0)
        return -EAGAIN;

    /* send address + r */
    ctx->base->I2CxTRN.REG = (uint32_t)(ctx->slave_addr << 1);

    ctx->state = TWI_PIC32MX_STATE_DATA;
    return -EAGAIN;
}

static int twi_write_as_master_data(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    /* check ack */
    if ((ctx->base->I2CxSTAT.REG & I2CxSTAT_ACKSTAT) != 0) {
        /* NACK recv */
        ctx->state = TWI_PIC32MX_STATE_STOP;
        return -EAGAIN;
    }

    /* check if tx buf is available */
    if ((ctx->base->I2CxSTAT.REG & I2CxSTAT_TRSTAT) != 0)
        return -EAGAIN;

    /* load data */
    ctx->base->I2CxTRN.REG = (uint32_t)*(uint8_t*)buf;

    /* last byte */
    if (n == (size_t)1) {
        ctx->last = (int)n;
        ctx->state = TWI_PIC32MX_STATE_STOP;
        return -EAGAIN;
    }

    return 1;
}

static int twi_rw_as_master_stop(struct twi *ctx)
{
    /* check status */
    uint32_t mask = (uint32_t)(I2CxCON_ACKEN | I2CxCON_RCEN |
                               I2CxCON_PEN | I2CxCON_RSEN | I2CxCON_SEN);

    /* no pending transmission */
    if ((ctx->base->I2CxSTAT.REG & I2CxSTAT_TRSTAT) != 0 ||
        (ctx->base->I2CxCON.REG & mask) != 0)
        return -EAGAIN;

    /* send stop condition */
    ctx->base->I2CxCON.SET = (uint32_t)I2CxCON_PEN;

    ctx->state = TWI_PIC32MX_STATE_IDLE;
    return ctx->last;
}

static int twi_write_as_master(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->state) {
    case TWI_PIC32MX_STATE_IDLE: return twi_rw_as_master_idle(ctx);
    case TWI_PIC32MX_STATE_SLA: return twi_write_as_master_sla(ctx);
    case TWI_PIC32MX_STATE_DATA: return twi_write_as_master_data(ctx, buf, n);
    case TWI_PIC32MX_STATE_STOP: return twi_rw_as_master_stop(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_write_as_slave_idle(struct twi *ctx)
{
    /* drop addr */
    if ((ctx->base->I2CxSTAT.REG & I2CxSTAT_D_A) == 0 &&
        (ctx->base->I2CxSTAT.REG & I2CxSTAT_R_W) != 0) {
        /*@i@*/ (void)ctx->base->I2CxRCV.REG;
        ctx->state = TWI_PIC32MX_STATE_DATA;
    }

    return -EAGAIN;
}

static int twi_write_as_slave_data(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    /* check if tx buf is available */
    if ((ctx->base->I2CxSTAT.REG & I2CxSTAT_TRSTAT) != 0 ||
        (ctx->base->I2CxSTAT.REG & I2CxSTAT_TBF) != 0)
        return -EAGAIN;

    /* load data */
    ctx->base->I2CxTRN.REG = (uint32_t)*(uint8_t*)buf;
    ctx->base->I2CxCON.SET = (uint32_t)I2CxCON_SCLREL;

    /* end of transmission */
    if (n == (size_t)1)
        ctx->state = TWI_PIC32MX_STATE_IDLE;

    return 1;
}

static int twi_write_as_slave(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->state) {
    case TWI_PIC32MX_STATE_IDLE: return twi_write_as_slave_idle(ctx);
    case TWI_PIC32MX_STATE_DATA: return twi_write_as_slave_data(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

int twi_write(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    if (ctx->mode == TWI_MODE_MASTER)
        return twi_write_as_master(ctx, buf, n);

    if (ctx->mode == TWI_MODE_SLAVE)
        return twi_write_as_slave(ctx, buf, n);

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_read_as_master_sla(struct twi *ctx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    if ((ctx->base->I2CxCON.REG & I2CxCON_SEN) != 0)
        return -EAGAIN;

    /* send address + r */
    ctx->base->I2CxCON.SET = (uint32_t)(I2CxCON_RCEN | I2CxCON_ACKEN);
    ctx->base->I2CxTRN.REG = (uint32_t)((ctx->slave_addr << 1) | 0x1);

    /* if only 1 char */
    if (n == (size_t)1)
        ctx->base->I2CxCON.SET = (uint32_t)I2CxCON_ACKDT;

    ctx->state = TWI_PIC32MX_STATE_DATA;
    return -EAGAIN;
}

static int twi_read_as_master_data(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    /* check if rx buf is available */
    if ((ctx->base->I2CxSTAT.REG & I2CxSTAT_RBF) == 0)
        return -EAGAIN;

    /* one to last byte */
    if (n == (size_t)2)
        ctx->base->I2CxCON.SET = (uint32_t)I2CxCON_ACKDT;

    /* read data */
    *(uint8_t*)buf = (uint8_t)ctx->base->I2CxRCV.REG;

    /* last byte */
    if (n == (size_t)1) {
        ctx->last = (int)n;
        ctx->state = TWI_PIC32MX_STATE_STOP;
        return -EAGAIN;
    }

    /* ack */
    ctx->base->I2CxCON.SET = (uint32_t)(I2CxCON_RCEN | I2CxCON_ACKEN);
    return 1;
}

static int twi_read_as_master(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->state) {
    case TWI_PIC32MX_STATE_IDLE: return twi_rw_as_master_idle(ctx);
    case TWI_PIC32MX_STATE_SLA: return twi_read_as_master_sla(ctx, n);
    case TWI_PIC32MX_STATE_DATA: return twi_read_as_master_data(ctx, buf, n);
    case TWI_PIC32MX_STATE_STOP: return twi_rw_as_master_stop(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_read_as_slave(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    if ((ctx->base->I2CxSTAT.REG & I2CxSTAT_RBF) == 0)
        return -EAGAIN;

    /* drop addr byte */
    if ((ctx->base->I2CxSTAT.REG & I2CxSTAT_D_A) == 0) {
        /*@i@*/ (void)ctx->base->I2CxRCV.REG;
        ctx->base->I2CxCON.SET = (uint32_t)I2CxCON_SCLREL;
        return -EAGAIN;
    }

    *(uint8_t*)buf = (uint8_t)ctx->base->I2CxRCV.REG;
    ctx->base->I2CxCON.SET = (uint32_t)I2CxCON_SCLREL;

    return 1;
}

int twi_read(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    if (ctx->mode == TWI_MODE_MASTER)
        return twi_read_as_master(ctx, buf, n);

    if (ctx->mode == TWI_MODE_SLAVE)
        return twi_read_as_slave(ctx, buf, n);

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}
