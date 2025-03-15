#include "twi-dw_apb_i2c.h"
#include "picoRTOS.h"

#include <stdint.h>

struct TWI_DW_APB_I2C {
    volatile uint32_t IC_CON;
    volatile uint32_t IC_TAR;
    volatile uint32_t IC_SAR;
    uint32_t RESERVED0;
    volatile uint32_t IC_DATA_CMD;
    volatile uint32_t IC_SS_SCL_HCNT;
    volatile uint32_t IC_SS_SCL_LCNT;
    volatile uint32_t IC_FS_SCL_HCNT;
    volatile uint32_t IC_FS_SCL_LCNT;
    uint32_t RESERVED1[2];
    volatile uint32_t IC_INTR_STAT;
    volatile uint32_t IC_INTR_MASK;
    volatile uint32_t IC_RAW_INTR_STAT;
    volatile uint32_t IC_RX_TL;
    volatile uint32_t IC_TX_TL;
    volatile uint32_t IC_CLR_INTR;
    volatile uint32_t IC_CLR_RX_UNDER;
    volatile uint32_t IC_CLR_RX_OVER;
    volatile uint32_t IC_CLR_TX_OVER;
    volatile uint32_t IC_CLR_RD_REQ;
    volatile uint32_t IC_CLR_TX_ABRT;
    volatile uint32_t IC_CLR_RX_DONE;
    volatile uint32_t IC_CLR_ACTIVITY;
    volatile uint32_t IC_CLR_STOP_DET;
    volatile uint32_t IC_CLR_START_DET;
    volatile uint32_t IC_CLR_GEN_CALL;
    volatile uint32_t IC_ENABLE;
    volatile uint32_t IC_STATUS;
    volatile uint32_t IC_TXFLR;
    volatile uint32_t IC_RXFLR;
    volatile uint32_t IC_SDA_HOLD;
    volatile uint32_t IC_TX_ABRT_SOURCE;
    volatile uint32_t IC_SLV_DATA_NACK_ONLY;
    volatile uint32_t IC_DMA_CR;
    volatile uint32_t IC_DMA_TDLR;
    volatile uint32_t IC_DMA_RDLR;
    volatile uint32_t IC_SDA_SETUP;
    volatile uint32_t IC_ACK_GENERAL_CALL;
    volatile uint32_t IC_ENABLE_STATUS;
    volatile uint32_t IC_FS_SPKLEN;
    uint32_t RESERVED2;
    volatile uint32_t IC_CLR_RESTART_DET;
    uint32_t RESERVED3[18];
    volatile uint32_t IC_COMP_PARAM_1;
    volatile uint32_t IC_COMP_VERSION;
    volatile uint32_t IC_COMP_TYPE;
};

#define IC_CON_IC_SLAVE_DISABLE    (1 << 6)
#define IC_CON_IC_RESTART_EN       (1 << 5)
#define IC_CON_IC_10BITADDR_MASTER (1 << 4)
#define IC_CON_IC_10BITADDR_SLAVE  (1 << 3)
#define IC_CON_SPEED_M             0x3u
#define IC_CON_SPEED(x)            (((x) & IC_CON_SPEED_M) << 1)
#define IC_CON_MASTER_MODE         (1 << 0)

#define IC_DATA_CMD_FIRST_DATA_BYTE (1 << 11)
#define IC_DATA_CMD_RESTART         (1 << 10)
#define IC_DATA_CMD_STOP            (1 << 9)
#define IC_DATA_CMD_CMD             (1 << 8)
#define IC_DATA_CMD_DAT_M           0xffu
#define IC_DATA_CMD_DAT(x)          ((x) & IC_DATA_CMD_DAT_M)

#define IC_INTR_STAT_R_RESTART_DET (1 << 12)
#define IC_INTR_STAT_R_GEN_CALL    (1 << 11)
#define IC_INTR_STAT_R_START_DET   (1 << 10)
#define IC_INTR_STAT_R_STOP_DET    (1 << 9)
#define IC_INTR_STAT_R_ACTIVITY    (1 << 8)
#define IC_INTR_STAT_R_RX_DONE     (1 << 7)
#define IC_INTR_STAT_R_TX_ABRT     (1 << 6)
#define IC_INTR_STAT_R_RD_REQ      (1 << 5)
#define IC_INTR_STAT_R_TX_EMPTY    (1 << 4)
#define IC_INTR_STAT_R_TX_OVER     (1 << 3)
#define IC_INTR_STAT_R_RX_FULL     (1 << 2)
#define IC_INTR_STAT_R_RX_OVER     (1 << 1)
#define IC_INTR_STAT_R_RX_UNDER    (1 << 0)

#define IC_ENABLE_TX_CMD_BLOCK (1 << 2)
#define IC_ENABLE_ABORT        (1 << 1)
#define IC_ENABLE_ENABLE       (1 << 0)

#define IC_STATUS_SLV_ACTIVITY (1 << 6)
#define IC_STATUS_MST_ACTIVITY (1 << 5)
#define IC_STATUS_RFF          (1 << 4)
#define IC_STATUS_RFNE         (1 << 3)
#define IC_STATUS_TFE          (1 << 2)
#define IC_STATUS_TFNF         (1 << 1)
#define IC_STATUS_ACTIVITY     (1 << 0)

#define IC_TX_ABRT_SOURCE_TX_FLFUSH_CNT_M      0x1fu
#define IC_TX_ABRT_SOURCE_TX_FLFUSH_CNT_GET(x) (((x) >> 23) & IC_TX_ABRT_SOURCE_TX_FLFUSH_CNT_M)
#define IC_TX_ABRT_SOURCE_ABRT_USER_ABRT       (1 << 16)
#define IC_TX_ABRT_SOURCE_ABRT_SLVRD_INTX      (1 << 15)
#define IC_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST     (1 << 14)
#define IC_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO (1 << 13)
#define IC_TX_ABRT_SOURCE_ARB_LOST             (1 << 12)
#define IC_TX_ABRT_SOURCE_ABRT_MASTER_DIS      (1 << 11)
#define IC_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT  (1 << 10)
#define IC_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT   (1 << 9)
#define IC_TX_ABRT_SOURCE_ABRT_HS_NORSTRT      (1 << 8)
#define IC_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET    (1 << 7)
#define IC_TX_ABRT_SOURCE_ABRT_HS_ACKDET       (1 << 6)
#define IC_TX_ABRT_SOURCE_ABRT_GCALL_READ      (1 << 5)
#define IC_TX_ABRT_SOURCE_ABRT_GCALL_NOACK     (1 << 4)
#define IC_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK    (1 << 3)
#define IC_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK   (1 << 2)
#define IC_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK   (1 << 1)
#define IC_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK   (1 << 0)

#define IC_SDA_HOLD_IC_SDA_RX_HOLD_M  0x7fu
#define IC_SDA_HOLD_IC_SDA_RX_HOLD(x) (((x) & IC_SDA_HOLD_IC_SDA_RX_HOLD_M) << 16)
#define IC_SDA_HOLD_IC_SDA_TX_HOLD_M  0xffffu
#define IC_SDA_HOLD_IC_SDA_TX_HOLD(x) ((x) & IC_SDA_HOLD_IC_SDA_TX_HOLD_M)

/* Function: twi_dw_apb_i2c_init
 * Initializes a TWI/I2C
 *
 * Parameters:
 *  ctx - The TWI to init
 *  base - The TWI base address
 *  clkid - The TWI clock ID
 *
 * Returns:
 * Always 0
 */
int twi_dw_apb_i2c_init(struct twi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct TWI_DW_APB_I2C*)base;
    ctx->clkid = clkid;
    ctx->mode = TWI_MODE_IGNORE;
    ctx->state = TWI_DW_APB_I2C_STATE_IDLE;
    ctx->slave_addr = 0;

    /* ensure is disabled */
    ctx->base->IC_ENABLE &= ~IC_ENABLE_ENABLE;

    /* set watermarks */
    ctx->base->IC_TX_TL = 0;
    ctx->base->IC_RX_TL = 0;

    return 0;
}

static int set_bitrate(struct twi *ctx, unsigned long bitrate)
{
    picoRTOS_assert(bitrate > 0, return -EINVAL);

    uint32_t ic_con = ctx->base->IC_CON;
    uint32_t speed = (uint32_t)0x1; /* standard */
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    picoRTOS_assert(freq > 0, return -EIO);

    /* SCL */
    unsigned long period = (unsigned long)freq / bitrate;

    if (bitrate > TWI_BITRATE_STANDARD) {
        ctx->base->IC_FS_SCL_HCNT = (uint32_t)(period / 2ul);
        ctx->base->IC_FS_SCL_LCNT = (uint32_t)(period / 2ul);
    }else{
        ctx->base->IC_SS_SCL_HCNT = (uint32_t)(period / 2ul);
        ctx->base->IC_SS_SCL_LCNT = (uint32_t)(period / 2ul);
    }

    /* speed */
    ic_con &= ~IC_CON_SPEED(IC_CON_SPEED_M);
    if (bitrate > TWI_BITRATE_STANDARD) speed = (uint32_t)0x2;      /* fast */
    if (bitrate >= TWI_BITRATE_FAST_PLUS) speed = (uint32_t)0x3;    /* hi speed */
    ctx->base->IC_CON = ic_con | (uint32_t)IC_CON_SPEED(speed);

    return 0;
}

static int set_mode(struct twi *ctx, twi_mode_t mode)
{
    picoRTOS_assert(mode != TWI_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < TWI_MODE_COUNT, return -EINVAL);

    uint32_t ic_con = ctx->base->IC_CON;

    if (mode == TWI_MODE_MASTER)
        ic_con |= (IC_CON_IC_SLAVE_DISABLE | IC_CON_MASTER_MODE);
    else
        ic_con &= ~(IC_CON_IC_SLAVE_DISABLE | IC_CON_MASTER_MODE);

    ctx->base->IC_CON = ic_con;
    ctx->mode = mode;

    return 0;
}

static void set_addr(struct twi *ctx, twi_addr_t addr)
{
    if (ctx->mode == TWI_MODE_SLAVE)
        ctx->base->IC_SAR = (uint32_t)addr;

    ctx->slave_addr = addr;
}

int twi_setup(struct twi *ctx, const struct twi_settings *settings)
{
    int res;

    ctx->base->IC_ENABLE &= ~IC_ENABLE_ENABLE;

    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    if (settings->mode != TWI_MODE_IGNORE &&
        (res = set_mode(ctx, settings->mode)) < 0)
        return res;

    set_addr(ctx, settings->slave_addr);
    ctx->base->IC_ENABLE |= IC_ENABLE_ENABLE;
    return 0;
}

int twi_poll(struct twi *ctx)
{
    if ((ctx->base->IC_RAW_INTR_STAT & IC_INTR_STAT_R_RX_FULL) != 0) return TWI_WRITE;
    if ((ctx->base->IC_RAW_INTR_STAT & IC_INTR_STAT_R_RD_REQ) != 0) return TWI_READ;

    return -EAGAIN;
}

static int twi_rw_as_master_check_abort(struct twi *ctx)
{
    int ret = 0;

    uint32_t source = ctx->base->IC_TX_ABRT_SOURCE;
    uint32_t intr_stat = ctx->base->IC_INTR_STAT;

    /* check for abort */
    if ((intr_stat & IC_INTR_STAT_R_TX_ABRT) != 0) {
        /* NACK on tx data */
        if ((source & IC_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK) != 0)
            ret = -ENOMSG;

        /* NACK on 7bit address */
        if ((source & IC_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK) != 0)
            ret = -ENXIO;

        /* To be continued */

        /* ack */
        ctx->base->IC_CLR_TX_ABRT |= 1;
    }

    return ret;
}

static int twi_write_as_master_idle(struct twi *ctx)
{
    /* check for activity first */
    if ((ctx->base->IC_STATUS & IC_STATUS_MST_ACTIVITY) != 0)
        return -EAGAIN;

    ctx->base->IC_ENABLE &= ~IC_ENABLE_ENABLE;
    ctx->base->IC_TAR = (uint32_t)ctx->slave_addr;
    ctx->base->IC_ENABLE |= IC_ENABLE_ENABLE;

    if ((ctx->base->IC_STATUS & IC_STATUS_TFNF) != 0)
        ctx->state = TWI_DW_APB_I2C_STATE_XFER;

    return -EAGAIN;
}

static int twi_write_as_master_xfer(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    size_t sent = 0;
    const uint8_t *buf8 = (uint8_t*)buf;

    while (n-- != 0) {

        int res;
        uint32_t stop = 0;

        if ((res = twi_rw_as_master_check_abort(ctx)) < 0) {
            ctx->state = TWI_DW_APB_I2C_STATE_IDLE;
            return res;
        }

        if ((ctx->base->IC_STATUS & IC_STATUS_TFNF) == 0)
            break;

        /* last byte */
        if (n == 0) {
            stop = (uint32_t)IC_DATA_CMD_STOP;
            ctx->state = TWI_DW_APB_I2C_STATE_IDLE;
        }

        ctx->base->IC_DATA_CMD = (uint32_t)buf8[sent++] | stop;
    }

    if (sent == 0)
        return -EAGAIN;

    return (int)sent;
}

static int twi_write_as_master(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_DW_APB_I2C_STATE_IDLE: return twi_write_as_master_idle(ctx);
    case TWI_DW_APB_I2C_STATE_XFER: return twi_write_as_master_xfer(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_write_as_slave_idle(struct twi *ctx)
{
    if ((ctx->base->IC_RAW_INTR_STAT & IC_INTR_STAT_R_TX_ABRT) != 0) {
        ctx->base->IC_CLR_TX_ABRT |= 1;
        return -EPIPE;
    }

    if ((ctx->base->IC_RAW_INTR_STAT & IC_INTR_STAT_R_RD_REQ) != 0) {
        ctx->base->IC_CLR_RD_REQ |= 1;
        ctx->state = TWI_DW_APB_I2C_STATE_XFER;
    }

    return -EAGAIN;
}

static int twi_write_as_slave_check_abort(struct twi *ctx)
{
    int ret = 0;
    uint32_t source = ctx->base->IC_TX_ABRT_SOURCE;
    uint32_t intr_stat = ctx->base->IC_INTR_STAT;

    /* check for abort */
    if ((intr_stat & IC_INTR_STAT_R_TX_ABRT) != 0) {
        /* NACK */
        if ((source & IC_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK) != 0)
            ret = -ENOMSG;

        /* ack bit */
        ctx->base->IC_CLR_TX_ABRT |= 1;
    }

    return ret;
}

static int twi_write_as_slave_xfer(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int sent = 0;
    const uint8_t *buf8 = (uint8_t*)buf;

    while (n-- != 0) {

        if ((ctx->base->IC_STATUS & IC_STATUS_TFNF) == 0)
            break;

        if (twi_write_as_slave_check_abort(ctx) < 0 ||
            (ctx->base->IC_RAW_INTR_STAT & IC_INTR_STAT_R_STOP_DET) != 0) {
            ctx->state = TWI_DW_APB_I2C_STATE_STOP;
            ctx->last = sent;
            return -EAGAIN;
        }

        ctx->base->IC_DATA_CMD = (uint32_t)buf8[sent++];

        if (n == 0) {
            ctx->state = TWI_DW_APB_I2C_STATE_STOP;
            ctx->last = sent;
            return -EAGAIN;
        }
    }

    if (sent == 0)
        return -EAGAIN;

    return sent;
}

static int twi_rw_as_slave_stop(struct twi *ctx)
{
    if ((ctx->base->IC_RAW_INTR_STAT & IC_INTR_STAT_R_STOP_DET) == 0)
        return -EAGAIN;

    ctx->base->IC_CLR_STOP_DET |= 1;
    (void)twi_write_as_slave_check_abort(ctx);

    ctx->state = TWI_DW_APB_I2C_STATE_IDLE;
    return ctx->last;
}

static int twi_write_as_slave(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_DW_APB_I2C_STATE_IDLE: return twi_write_as_slave_idle(ctx);
    case TWI_DW_APB_I2C_STATE_XFER: return twi_write_as_slave_xfer(ctx, buf, n);
    case TWI_DW_APB_I2C_STATE_STOP: return twi_rw_as_slave_stop(ctx);
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

    /* not implemented yet */
    if (ctx->mode == TWI_MODE_SLAVE)
        return twi_write_as_slave(ctx, buf, n);

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_read_as_master_idle(struct twi *ctx)
{
    /* check for activity first */
    if ((ctx->base->IC_STATUS & IC_STATUS_MST_ACTIVITY) != 0)
        return -EAGAIN;

    ctx->base->IC_ENABLE &= ~IC_ENABLE_ENABLE;
    ctx->base->IC_TAR = (uint32_t)ctx->slave_addr;
    ctx->base->IC_ENABLE |= IC_ENABLE_ENABLE;

    ctx->state = TWI_DW_APB_I2C_STATE_REQ;
    return -EAGAIN;
}

static int twi_read_as_master_req(struct twi *ctx, size_t n)
{
    if ((ctx->base->IC_STATUS & IC_STATUS_TFNF) == 0)
        return -EAGAIN;

    int res;
    uint32_t stop = 0;

    if ((res = twi_rw_as_master_check_abort(ctx)) < 0) {
        ctx->state = TWI_DW_APB_I2C_STATE_IDLE;
        return res;
    }

    /* last byte */
    if (n == (size_t)1)
        stop = (uint32_t)IC_DATA_CMD_STOP;

    ctx->base->IC_DATA_CMD = (uint32_t)IC_DATA_CMD_CMD | stop;

    ctx->state = TWI_DW_APB_I2C_STATE_XFER;
    return -EAGAIN;
}

static int twi_read_as_master_xfer(struct twi *ctx, void *buf, size_t n)
{
    int res;
    uint8_t *buf8 = (uint8_t*)buf;

    if ((res = twi_rw_as_master_check_abort(ctx)) < 0) {
        ctx->state = TWI_DW_APB_I2C_STATE_IDLE;
        return res;
    }

    if ((ctx->base->IC_STATUS & IC_STATUS_RFNE) == 0)
        return -EAGAIN;

    *buf8 = (uint8_t)ctx->base->IC_DATA_CMD;

    if (n > (size_t)1) ctx->state = TWI_DW_APB_I2C_STATE_REQ;
    else ctx->state = TWI_DW_APB_I2C_STATE_IDLE;

    return 1;
}

static int twi_read_as_master(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_DW_APB_I2C_STATE_IDLE: return twi_read_as_master_idle(ctx);
    case TWI_DW_APB_I2C_STATE_REQ: return twi_read_as_master_req(ctx, n);
    case TWI_DW_APB_I2C_STATE_XFER: return twi_read_as_master_xfer(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_read_as_slave_idle(struct twi *ctx)
{
    if ((ctx->base->IC_RAW_INTR_STAT & IC_INTR_STAT_R_RX_FULL) != 0)
        ctx->state = TWI_DW_APB_I2C_STATE_XFER;

    return -EAGAIN;
}

static int twi_read_as_slave_xfer(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int recv = 0;
    uint8_t *buf8 = (uint8_t*)buf;

    while (n-- != 0) {
        if ((ctx->base->IC_STATUS & IC_STATUS_RFNE) == 0)
            break;

        buf8[recv++] = (uint8_t)ctx->base->IC_DATA_CMD;

        /* end of xfer */
        if (n == 0 ||
            (ctx->base->IC_RAW_INTR_STAT & IC_INTR_STAT_R_STOP_DET) != 0) {
            ctx->state = TWI_DW_APB_I2C_STATE_STOP;
            ctx->last = recv;
            return -EAGAIN;
        }
    }

    if (recv == 0)
        return -EAGAIN;

    return recv;
}

static int twi_read_as_slave(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_DW_APB_I2C_STATE_IDLE: return twi_read_as_slave_idle(ctx);
    case TWI_DW_APB_I2C_STATE_XFER: return twi_read_as_slave_xfer(ctx, buf, n);
    case TWI_DW_APB_I2C_STATE_STOP: return twi_rw_as_slave_stop(ctx);
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
