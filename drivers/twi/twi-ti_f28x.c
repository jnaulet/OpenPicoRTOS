#include "twi-ti_f28x.h"
#include "picoRTOS.h"

#include <stdint.h>

#define TWI_TI_F28X_TXFFIL 16
#define TWI_TI_F28X_RXFFIL 0

struct I2C_REGS {
    volatile uint16_t I2COAR;
    volatile uint16_t I2CIER;
    volatile uint16_t I2CSTR;
    volatile uint16_t I2CCLKL;
    volatile uint16_t I2CCLKH;
    volatile uint16_t I2CCNT;
    volatile uint16_t I2CDRR;
    volatile uint16_t I2CSAR;
    volatile uint16_t I2CDXR;
    volatile uint16_t I2CMDR;
    volatile uint16_t I2CISRC;
    volatile uint16_t I2CEMDR;
    volatile uint16_t I2CPSC;
    uint16_t RESERVED0[19];
    volatile uint16_t I2CFFTX;
    volatile uint16_t I2CFFRX;
};

#define I2COAR_OAR_M  0x3ffu
#define I2COAR_OAR(x) ((x) & I2COAR_OAR_M)

#define I2CSTR_SDIR    (1 << 14)
#define I2CSTR_NACKSNT (1 << 13)
#define I2CSTR_BB      (1 << 12)
#define I2CSTR_RSFULL  (1 << 11)
#define I2CSTR_XSMT    (1 << 10)
#define I2CSTR_AAS     (1 << 9)
#define I2CSTR_AD0     (1 << 8)
#define I2CSTR_SCD     (1 << 5)
#define I2CSTR_XRDY    (1 << 4)
#define I2CSTR_RRDY    (1 << 3)
#define I2CSTR_ARDY    (1 << 2)
#define I2CSTR_NACK    (1 << 1)
#define I2CSTR_ARBL    (1 << 0)

#define I2CSAR_SAR_M  0x3ffu
#define I2CSAR_SAR(x) ((x) & I2CSAR_SAR_M)

#define I2CMDR_NACKMOD (1 << 15)
#define I2CMDR_FREE    (1 << 14)
#define I2CMDR_STT     (1 << 13)
#define I2CMDR_STP     (1 << 11)
#define I2CMDR_MST     (1 << 10)
#define I2CMDR_TRX     (1 << 9)
#define I2CMDR_XA      (1 << 8)
#define I2CMDR_RM      (1 << 7)
#define I2CMDR_DLB     (1 << 6)
#define I2CMDR_IRS     (1 << 5)
#define I2CMDR_STB     (1 << 4)
#define I2CMDR_FDF     (1 << 3)
#define I2CMDR_BC_M    0x7u
#define I2CMDR_BC(x)   ((x) & I2CMDR_BC_M)

#define I2CISRC_WRITE_ZEROS_M  0xfu
#define I2CISRC_WRITE_ZEROS(x) (((x) & I2CISRC_WRITE_ZEROS_M) << 8)
#define I2CISRC_INTCODE_M      0x7u
#define I2CISRC_INTCODE(x)     ((x) & I2CISRC_INTCODE_M)

#define I2CEMDR_BC (1 << 0)

#define I2CPSC_IPSC_M  0xffu
#define I2CPSC_IPSC(x) ((x) & I2CPSC_IPSC_M)

#define I2CFFTX_I2CFFEN    (1 << 14)
#define I2CFFTX_TXFFRST    (1 << 13)
#define I2CFFTX_TXFFST_M   0x1fu
#define I2CFFTX_TXFFST(x)  (((x) & I2CFFTX_TXFFST_M) << 8)
#define I2CFFTX_TXFFINT    (1 << 7)
#define I2CFFTX_TXFFINTCLR (1 << 6)
#define I2CFFTX_TXFFIENA   (1 << 5)
#define I2CFFTX_TXFFIL_M   0x1fu
#define I2CFFTX_TXFFIL(x)  ((x) & I2CFFTX_TXFFIL_M)

#define I2CFFRX_RXFFRST    (1 << 13)
#define I2CFFRX_RXFFST_M   0x1fu
#define I2CFFRX_RXFFST(x)  (((x) & I2CFFRX_RXFFST_M) << 8)
#define I2CFFRX_RXFFINT    (1 << 7)
#define I2CFFRX_RXFFINTCLR (1 << 6)
#define I2CFFRX_RXFFIENA   (1 << 5)
#define I2CFFRX_RXFFIL_M   0x1fu
#define I2CFFRX_RXFFIL(x)  ((x) & I2CFFRX_RXFFIL_M)

static int reset(struct twi *ctx)
{
    /* reset fifos & mdr */
    ctx->base->I2CMDR = (uint16_t)0;
    ctx->base->I2CFFTX = (uint16_t)0;
    ctx->base->I2CFFRX = (uint16_t)0;

    /* start in fifo mode */
    ctx->base->I2CFFTX = (uint16_t)(I2CFFTX_I2CFFEN | I2CFFTX_TXFFRST | I2CFFTX_TXFFINTCLR);
    ctx->base->I2CFFTX |= I2CFFTX_TXFFIL(TWI_TI_F28X_TXFFIL);
    ctx->base->I2CFFRX = (uint16_t)(I2CFFRX_RXFFRST | I2CFFRX_RXFFINTCLR);
    ctx->base->I2CFFTX |= I2CFFRX_RXFFIL(TWI_TI_F28X_RXFFIL);
    ctx->base->I2CMDR = (uint16_t)I2CMDR_IRS;
    return 0;
}

/* Function: twi_ti_f28x_init
 * Init an I2C interface
 *
 * Parameters:
 *  ctx - The I2C interface to init
 *  base - The I2C base address
 *  clkid - The I2C input clock id
 *
 * Returns:
 * 0 if success, -errno otherwise
 */
int twi_ti_f28x_init(struct twi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct I2C_REGS*)base;
    ctx->clkid = clkid;
    ctx->mode = TWI_MODE_SLAVE;
    ctx->state = TWI_TI_F28X_STATE_IDLE;

    /* start i2c */
    return reset(ctx);
}

static int set_bitrate(struct twi *ctx, unsigned long bitrate)
{
#define PERIOD_MAX 131072ul

    if (!picoRTOS_assert(bitrate > 0)) return -EINVAL;

    unsigned long ipsc;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    if (!picoRTOS_assert(freq > 0))
        return -EIO;

    for (ipsc = 1ul; ipsc <= (unsigned long)(I2CPSC_IPSC_M + 1); ipsc++) {

        unsigned long d = 5ul;
        unsigned long period;

        if (ipsc == 1ul) d = 7ul;
        if (ipsc == 2ul) d = 6ul;

        period = (((unsigned long)freq / ipsc) / bitrate) - d;
        if (period < PERIOD_MAX) {
            ctx->base->I2CPSC = (uint16_t)(ipsc - 1ul);
            ctx->base->I2CCLKL = (uint16_t)(period / 2ul);
            ctx->base->I2CCLKH = (uint16_t)(period / 2ul);
            return 0;
        }
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EINVAL;
}

static int set_mode(struct twi *ctx, twi_mode_t mode)
{
    if (!picoRTOS_assert(mode != TWI_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(mode < TWI_MODE_COUNT)) return -EINVAL;

    ctx->mode = mode;
    return 0;
}

/* hooks */

int twi_setup(struct twi *ctx, struct twi_settings *settings)
{
    int res = 0;

    /* bitrate */
    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    /* mode */
    if (settings->mode != TWI_MODE_IGNORE &&
        (res = set_mode(ctx, settings->mode)) < 0)
        return res;

    /* slave_addr */
    if (ctx->mode == TWI_MODE_MASTER) {
        ctx->base->I2COAR = (uint16_t)0;
        ctx->base->I2CSAR = (uint16_t)I2CSAR_SAR(settings->slave_addr);
    }else{
        ctx->base->I2CSAR = (uint16_t)0;
        ctx->base->I2COAR = (uint16_t)I2COAR_OAR(settings->slave_addr);
    }

    return res;
}

int twi_poll(struct twi *ctx)
{
    if ((ctx->base->I2CSTR & I2CSTR_AAS) != 0) {
        if ((ctx->base->I2CSTR & I2CSTR_SDIR) == 0) return TWI_WRITE;
        else return TWI_READ;
    }

    return -EAGAIN;
}

/* SLAVE TRANSMIT */

static int twi_write_as_slave_xfer(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    int sent = 0;
    uint16_t *buf16 = (uint16_t*)buf;

    while ((size_t)sent != n) {
        if ((ctx->base->I2CFFTX & I2CFFTX_TXFFINT) == 0)
            break;

        ctx->base->I2CDXR = buf16[sent++];
    }

    if (sent == 0)
        return -EAGAIN;

    if (sent == (int)n)
        ctx->state = TWI_TI_F28X_STATE_IDLE;

    return sent;
}

static int twi_write_as_slave_idle(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    /* prepare xfer */
    ctx->base->I2CCNT = (uint16_t)n;
    ctx->base->I2CMDR = (uint16_t)(I2CMDR_TRX | I2CMDR_IRS);

    ctx->state = TWI_TI_F28X_STATE_XFER;
    return twi_write_as_slave_xfer(ctx, buf, n);
}

static int twi_write_as_slave(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->state) {
    case TWI_TI_F28X_STATE_IDLE: return twi_write_as_slave_idle(ctx, buf, n);
    case TWI_TI_F28X_STATE_XFER: return twi_write_as_slave_xfer(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

/* MASTER TRANSMIT */

static int check_for_errors(struct twi *ctx)
{
    /* check for nack */
    if ((ctx->base->I2CSTR & I2CSTR_NACK) != 0) {
        ctx->base->I2CSTR = (uint32_t)I2CSTR_NACK;
        (void)reset(ctx);
        return -EPIPE;
    }

    return 0;
}

static int twi_write_as_master_xfer(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    int sent = 0;
    uint16_t *buf16 = (uint16_t*)buf;

    while ((size_t)sent != n) {

        int res;

        if ((res = check_for_errors(ctx)) != 0) {
            ctx->state = TWI_TI_F28X_STATE_IDLE;
            return res;
        }

        if ((ctx->base->I2CFFTX & I2CFFTX_TXFFINT) == 0)
            break;

        ctx->base->I2CDXR = buf16[sent++];
    }

    if (sent == 0)
        return -EAGAIN;

    if (sent == (int)n)
        ctx->state = TWI_TI_F28X_STATE_IDLE;

    return sent;
}

static int twi_write_as_master_idle(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    /* check if bus is available */
    if ((ctx->base->I2CSTR & I2CSTR_BB) != 0)
        return -EAGAIN;

    /* prepare xfer */
    ctx->base->I2CCNT = (uint16_t)n;
    ctx->base->I2CMDR = (uint16_t)(I2CMDR_STT | I2CMDR_MST | I2CMDR_STP | I2CMDR_TRX | I2CMDR_IRS);

    ctx->state = TWI_TI_F28X_STATE_XFER;
    return twi_write_as_master_xfer(ctx, buf, n);
}

static int twi_write_as_master(struct twi *ctx, const void *buf, size_t n)
{
    switch (ctx->state) {
    case TWI_TI_F28X_STATE_IDLE: return twi_write_as_master_idle(ctx, buf, n);
    case TWI_TI_F28X_STATE_XFER: return twi_write_as_master_xfer(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

int twi_write(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->mode) {
    case TWI_MODE_SLAVE: return twi_write_as_slave(ctx, buf, n);
    case TWI_MODE_MASTER: return twi_write_as_master(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

/* SLAVE RECEIVE */

static int twi_read_as_slave_xfer(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    int recv = 0;
    uint16_t *buf16 = (uint16_t*)buf;

    while ((size_t)recv != n) {
        if ((ctx->base->I2CFFRX & I2CFFRX_RXFFINT) == 0)
            break;

        buf16[recv++] = ctx->base->I2CDRR;
    }

    if (recv == 0)
        return -EAGAIN;

    if (recv == (int)n || (ctx->base->I2CSTR & I2CSTR_SCD) != 0)
        ctx->state = TWI_TI_F28X_STATE_IDLE;

    return recv;
}

static int twi_read_as_slave_idle(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    /* prepare xfer */
    ctx->base->I2CCNT = (uint16_t)n;
    ctx->base->I2CMDR = (uint16_t)I2CMDR_IRS;

    ctx->state = TWI_TI_F28X_STATE_XFER;
    return twi_read_as_slave_xfer(ctx, buf, n);
}

static int twi_read_as_slave(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->state) {
    case TWI_TI_F28X_STATE_IDLE: return twi_read_as_slave_idle(ctx, buf, n);
    case TWI_TI_F28X_STATE_XFER: return twi_read_as_slave_xfer(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

/* MASTER RECEIVE */

static int twi_read_as_master_xfer(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    int recv = 0;
    uint16_t *buf16 = (uint16_t*)buf;

    while ((size_t)recv != n) {
        if ((ctx->base->I2CFFRX & I2CFFRX_RXFFINT) == 0)
            break;

        buf16[recv++] = ctx->base->I2CDRR;
    }

    if (recv == 0)
        return -EAGAIN;

    if (recv == (int)n)
        ctx->state = TWI_TI_F28X_STATE_IDLE;

    return recv;
}

static int twi_read_as_master_idle(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    /* check if bus is available */
    if ((ctx->base->I2CSTR & I2CSTR_BB) != 0)
        return -EAGAIN;

    /* prepare xfer */
    ctx->base->I2CCNT = (uint16_t)n;
    ctx->base->I2CMDR = (uint16_t)(I2CMDR_STT | I2CMDR_MST | I2CMDR_STP | I2CMDR_IRS);

    ctx->state = TWI_TI_F28X_STATE_XFER;
    return twi_read_as_master_xfer(ctx, buf, n);
}

static int twi_read_as_master(struct twi *ctx, void *buf, size_t n)
{
    switch (ctx->state) {
    case TWI_TI_F28X_STATE_IDLE: return twi_read_as_master_idle(ctx, buf, n);
    case TWI_TI_F28X_STATE_XFER: return twi_read_as_master_xfer(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}

int twi_read(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->mode) {
    case TWI_MODE_SLAVE: return twi_read_as_slave(ctx, buf, n);
    case TWI_MODE_MASTER: return twi_read_as_master(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
}
