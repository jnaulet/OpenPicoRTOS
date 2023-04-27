#include "twi-atmel_sercom.h"
#include "picoRTOS.h"

#include <stdint.h>

struct TWI_ATMEL_SERCOM {
    volatile uint32_t CTRLA;
    volatile uint32_t CTRLB;
    volatile uint32_t CTRLC;
    volatile uint32_t BAUD; /* host */
    uint32_t RESERVED0;
    volatile uint16_t INTENCLR;
    volatile uint16_t INTENSET;
    volatile uint16_t INTFLAG;
    volatile uint16_t STATUS;
    volatile uint32_t SYNCBUSY;
    uint16_t RESERVED1;
    volatile uint16_t LENGTH; /* client */
    volatile uint32_t ADDR;
    volatile uint32_t DATA;
    uint32_t RESERVED2;
    volatile uint8_t DBGCTRL; /* host */
};

#define CTRLA_LOWTOUTEN  (1 << 30)
#define CTRLA_INACTOUT_M  0x3u
#define CTRLA_INACTOUT(x) (((x) & CTRLA_INACTOUT_M) << 28)
#define CTRLA_SCLSM       (1 << 27)
#define CTRLA_SPEED_M     0x3u
#define CTRLA_SPEED(x)    (((x) & CTRLA_SPEED_M) << 24)
#define CTRLA_SEXTTOEN    (1 << 23)
#define CTRLA_MEXTTOEN    (1 << 22)
#define CTRLA_SDAHOLD_M   0x3u
#define CTRLA_SDAHOLD(x)  (((x) & CTRLA_SDAHOLD_M) << 20)
#define CTRLA_PINOUT      (1 << 16)
#define CTRLA_RUNSTDBY    (1 << 7)
#define CTRLA_MODE_M      0x7u
#define CTRLA_MODE(x)     (((x) & CTRLA_MODE_M) << 2)
#define CTRLA_ENABLE      (1 << 1)
#define CTRLA_SWRST       (1 << 0)

#define CTRLB_ACKACT   (1 << 18)
#define CTRLB_CMD_M    0x3u
#define CTRLB_CMD(x)   (((x) & CTRLB_CMD_M) << 16)
#define CTRLB_AMODE_M  0x3u
#define CTRLB_AMODE(x) (((x) & CTRLB_AMODE_M) << 14)
#define CTRLB_AACKEN   (1 << 10)
#define CTRLB_GCMD     (1 << 9)
#define CTRLB_QCEN     (1 << 9)
#define CTRLB_SMEN     (1 << 8)

#define CTRLC_DATA32B     (1 << 24)
#define CTRLC_SDASETUP_M  0xfu
#define CTRLC_SDASETUP(x) ((x) & CTRLC_SDASETUP_M)

#define BAUD_HSBAUDLOW_M  0xffu
#define BAUD_HSBAUDLOW(x) (((x) & BAUD_HSBAUDLOW_M) << 24)
#define BAUD_HSBAUD_M     0xffu
#define BAUD_HSBAUD(x)    (((x) & BAUD_HSBAUD_M) << 16)
#define BAUD_BAUDLOW_M    0xffu
#define BAUD_BAUDLOW(x)   (((x) & BAUD_BAUDLOW_M) << 8)
#define BAUD_BAUD_M       0xffu
#define BAUD_BAUD(x)      ((x) & BAUD_BAUD_M)

#define INTFLAG_ERROR  (1 << 7)
#define INTFLAG_DRDY   (1 << 2) /* client */
#define INTFLAG_AMATCH (1 << 1) /* client */
#define INTFLAG_SB     (1 << 1) /* host */
#define INTFLAG_PREC   (1 << 0) /* client */
#define INTFLAG_MB     (1 << 0) /* host */

#define STATUS_LENERR      (1 << 10)
#define STATUS_SEXTOUT     (1 << 9)
#define STATUS_MEXTTOUT    (1 << 8)
#define STATUS_CLKHOLD     (1 << 7)
#define STATUS_LOWTOUT     (1 << 6)
#define STATUS_SR          (1 << 4) /* client */
#define STATUS_BUSSTATE_M  0x3u
#define STATUS_BUSSTATE(x) (((x) & STATUS_BUSSTATE_M) << 4)
#define STATUS_DIR         (1 << 3)
#define STATUS_RXNACK      (1 << 2)
#define STATUS_COLL        (1 << 1) /* client */
#define STATUS_ARBLOST     (1 << 1) /* host */
#define STATUS_BUSERR      (1 << 0)

#define SYNCBUSY_LENGTH (1 << 4)
#define SYNCBUSY_SYSOP  (1 << 2)
#define SYNCBUSY_ENABLE (1 << 1)
#define SYNCBUSY_SWRST  (1 << 0)

#define LENGTH_LENEN  (1 << 8)
#define LENGTH_LEN_M  0xffu
#define LENGTH_LEN(x) ((x) & LENGTH_LEN_M)

#define ADDR_ADDMASK_M   0x3ffu
#define ADDR_ADDRMASK(x) (((x) & ADDR_ADDMASK_M) << 17)
#define ADDR_TENBITEN    (1 << 15)
#define ADDR_HS          (1 << 14)
#define ADDR_LENEN       (1 << 13)
#define ADDR_ADDR_M      0x3ffu
#define ADDR_ADDR(x)     (((x) & ADDR_ADDR_M) << 1)
#define ADDR_GENCEN      (1 << 0)

#define DBGCTRL_DBGSTOP (1 << 0)

/* Function: twi_atmel_sercom_init
 * Initializes a SERCOM I2C
 *
 * Parameters:
 *  ctx - The TWI peripheral to init
 *  base - The peripheral base address
 *  clkid - The peripheral associated clock id
 *
 * Returns:
 * Always 0
 */
int twi_atmel_sercom_init(struct twi *ctx, struct TWI_ATMEL_SERCOM *base, clock_id_t clkid)
{
    ctx->base = base;
    ctx->clkid = clkid;
    ctx->state = TWI_ATMEL_SERCOM_STATE_IDLE;
    ctx->slave_addr = 0;

    return 0;
}

static int set_bitrate(struct twi *ctx, unsigned long bitrate)
{
    if (!picoRTOS_assert(bitrate > 0)) return -EINVAL;

    clock_freq_t freq = clock_get_freq(ctx->clkid);
    /* compute normal speed tlow & thigh */
    unsigned long factor1 = bitrate * 16ul;
    unsigned long baud = 0x10000ul * (1024ul - (factor1 * 1024ul) / freq) / 1024ul;
    /* compute hi speed tlow & thigh */
    unsigned long hsbaud = ((unsigned long)freq / bitrate) / 2ul;

    if (!picoRTOS_assert(freq > 0)) return -EIO;
    if (!picoRTOS_assert(baud < 0x10000ul))
        return -EINVAL;

    ctx->base->BAUD = (uint32_t)(BAUD_HSBAUD(hsbaud - 1ul) |
                                 BAUD_BAUD(baud - 1ul));

    /* set speed */
    ctx->base->CTRLA &= ~CTRLA_SPEED(CTRLA_SPEED_M);
    if (bitrate > TWI_BITRATE_FAST_PLUS) ctx->base->CTRLA |= CTRLA_SPEED(0x2);
    else if (bitrate > TWI_BITRATE_FAST) ctx->base->CTRLA |= CTRLA_SPEED(0x1);

    return 0;
}

static int set_mode(struct twi *ctx, twi_mode_t mode)
{
    if (!picoRTOS_assert(mode != TWI_MODE_IGNORE)) return -EINVAL;
    if (!picoRTOS_assert(mode < TWI_MODE_COUNT)) return -EINVAL;

    ctx->mode = mode;
    ctx->base->CTRLA &= ~CTRLA_MODE(CTRLA_MODE_M);

    if (mode == TWI_MODE_SLAVE) ctx->base->CTRLA |= CTRLA_MODE(0x4);
    else ctx->base->CTRLA |= CTRLA_MODE(0x5);

    return 0;
}

static int sync_busywait(struct twi *ctx, uint32_t mask)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while (deadlock-- != 0)
        if ((ctx->base->SYNCBUSY & mask) == 0)
            break;

    if (!picoRTOS_assert(deadlock != -1))
        return -EBUSY;

    return 0;
}

int twi_setup(struct twi *ctx, struct twi_settings *settings)
{
    if (!picoRTOS_assert(settings->mode < TWI_MODE_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(settings->slave_addr < (twi_addr_t)TWI_ADDR_COUNT)) return -EINVAL;

    int res;

    /* disable interface */
    ctx->base->CTRLA &= ~CTRLA_ENABLE;
    if ((res = sync_busywait(ctx, (uint32_t)SYNCBUSY_ENABLE)) < 0)
        return res;

    /* bitrate */
    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    /* mode */
    if (settings->mode != TWI_MODE_IGNORE &&
        (res = set_mode(ctx, settings->mode)) < 0)
        return res;

    if (ctx->mode == TWI_MODE_SLAVE)
        ctx->base->ADDR = (uint32_t)ADDR_ADDR(settings->slave_addr);

    ctx->slave_addr = settings->slave_addr;

    /* SCLSM = 1 */
    ctx->base->CTRLA |= CTRLA_SCLSM;

    /* re-enable interface */
    ctx->base->CTRLA |= CTRLA_ENABLE;
    if ((res = sync_busywait(ctx, (uint32_t)SYNCBUSY_ENABLE)) < 0)
        return res;

    /* force bus to idle state */
    ctx->base->STATUS |= STATUS_BUSSTATE(0x1);

    return 0;
}

int twi_poll(struct twi *ctx)
{
    if ((ctx->base->INTFLAG & INTFLAG_AMATCH) != 0) {
        if ((ctx->base->STATUS & STATUS_DIR) == 0) return TWI_WRITE;
        else return TWI_READ;
    }

    return -EAGAIN;
}

/* MASTER STOP */

static int twi_send_stop_as_master(struct twi *ctx)
{
    if ((ctx->base->INTFLAG & INTFLAG_MB) == 0)
        return -EAGAIN;

    ctx->base->CTRLB &= ~CTRLB_CMD(CTRLB_CMD_M);
    ctx->base->CTRLB |= CTRLB_CMD(0x3); /* stop condition */

    ctx->state = TWI_ATMEL_SERCOM_STATE_IDLE;
    return ctx->last;
}

static int twi_stop_as_master(struct twi *ctx, int ret)
{
    if ((ctx->base->INTFLAG & INTFLAG_MB) == 0)
        return -EAGAIN;

    ctx->base->CTRLB &= ~CTRLB_CMD(CTRLB_CMD_M);
    ctx->base->CTRLB |= CTRLB_CMD(0x3); /* stop condition */

    ctx->state = TWI_ATMEL_SERCOM_STATE_STOP;
    ctx->last = ret;

    return twi_send_stop_as_master(ctx);
}

/* SLAVE STOP */

static int twi_wait_stop_as_slave(struct twi *ctx)
{
    if ((ctx->base->INTFLAG & INTFLAG_PREC) == 0)
        return -EAGAIN;

    /* clear intflags */
    ctx->base->INTFLAG = ctx->base->INTFLAG;
    ctx->base->CTRLB &= ~CTRLB_ACKACT;

    ctx->state = TWI_ATMEL_SERCOM_STATE_IDLE;
    return ctx->last;
}

static int twi_stop_as_slave(struct twi *ctx, int ret)
{
    /* nack */
    ctx->base->CTRLB |= CTRLB_ACKACT;
    ctx->state = TWI_ATMEL_SERCOM_STATE_STOP;
    ctx->last = ret;

    return twi_wait_stop_as_slave(ctx);
}

/* MASTER TRANSMIT */

static int twi_write_as_master_idle(struct twi *ctx)
{
    /* check if bus is idle */
    if ((ctx->base->STATUS & STATUS_BUSSTATE(STATUS_BUSSTATE_M)) !=
        STATUS_BUSSTATE(0x1)) return -EAGAIN;

    /* send start & addr */
    ctx->base->ADDR = (uint32_t)ADDR_ADDR(ctx->slave_addr);

    ctx->state = TWI_ATMEL_SERCOM_STATE_DATA;
    return -EAGAIN;
}

static int twi_write_as_master_data(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    const uint8_t *buf8 = (const uint8_t*)buf;

    if ((ctx->base->INTFLAG & INTFLAG_MB) == 0)
        return -EAGAIN;

    ctx->base->DATA = (uint32_t)*buf8;

    /* last byte */
    if (n == (size_t)1)
        return twi_stop_as_master(ctx, (int)n);

    return 1;
}

static int twi_write_as_master(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    /* NACK */
    if ((ctx->base->STATUS & STATUS_RXNACK) != 0)
        return twi_stop_as_master(ctx, 0); /* signal EOT */

    /* ARBLOST / BUSERR */
    if ((ctx->base->STATUS & STATUS_ARBLOST) != 0 ||
        (ctx->base->STATUS & STATUS_BUSERR) != 0)
        return twi_stop_as_master(ctx, -EPIPE);

    switch (ctx->state) {
    case TWI_ATMEL_SERCOM_STATE_IDLE: return twi_write_as_master_idle(ctx);
    case TWI_ATMEL_SERCOM_STATE_DATA: return twi_write_as_master_data(ctx, buf, n);
    case TWI_ATMEL_SERCOM_STATE_STOP: return twi_send_stop_as_master(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

/* SLAVE TRANSMIT */

static int twi_write_as_slave_data(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    const uint8_t *buf8 = (const uint8_t*)buf;

    if ((ctx->base->INTFLAG & INTFLAG_DRDY) == 0)
        return -EAGAIN;

    /* free the line */
    ctx->base->CTRLB |= CTRLB_CMD(0x2);

    /* NACK (EOT) or unexpected stop */
    if ((ctx->base->STATUS & STATUS_RXNACK) != 0 ||
        (ctx->base->INTFLAG & INTFLAG_PREC) != 0)
        return twi_stop_as_slave(ctx, 0);

    ctx->base->DATA = (uint32_t)*buf8;

    if (n == (size_t)1)
        return twi_stop_as_slave(ctx, (int)n);

    return 1;
}

static int twi_write_as_slave_idle(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    if ((ctx->base->INTFLAG & INTFLAG_AMATCH) == 0)
        return -EAGAIN;

    ctx->base->INTFLAG = (uint32_t)INTFLAG_AMATCH;

    /* save 1 schedule */
    ctx->state = TWI_ATMEL_SERCOM_STATE_DATA;
    return twi_write_as_slave_data(ctx, buf, n);
}

static int twi_write_as_slave(struct twi *ctx, const void *buf, size_t n)
{
    /* ARBLOST / BUSERR */
    if ((ctx->base->STATUS & STATUS_COLL) != 0 ||
        (ctx->base->STATUS & STATUS_BUSERR) != 0)
        return twi_stop_as_slave(ctx, -EPIPE);

    switch (ctx->state) {
    case TWI_ATMEL_SERCOM_STATE_IDLE: return twi_write_as_slave_idle(ctx, buf, n);
    case TWI_ATMEL_SERCOM_STATE_DATA: return twi_write_as_slave_data(ctx, buf, n);
    case TWI_ATMEL_SERCOM_STATE_STOP: return twi_wait_stop_as_slave(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
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

/* MASTER RECEIVE */

static int twi_read_as_master_idle(struct twi *ctx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    /* check if bus is idle */
    if ((ctx->base->STATUS & STATUS_BUSSTATE(STATUS_BUSSTATE_M)) !=
        STATUS_BUSSTATE(0x1))
        return -EAGAIN;

    /* nack */
    if (n == (size_t)1) ctx->base->CTRLB |= CTRLB_ACKACT;
    else ctx->base->CTRLB &= ~CTRLB_ACKACT;

    /* send start & addr */
    ctx->base->ADDR = (uint32_t)(ADDR_ADDR(ctx->slave_addr) | 0x1u);

    ctx->state = TWI_ATMEL_SERCOM_STATE_DATA;
    return -EAGAIN;
}

static int twi_read_as_master_data(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    uint8_t *buf8 = (uint8_t*)buf;

    if ((ctx->base->INTFLAG & INTFLAG_SB) == 0)
        return -EAGAIN;

    /* next ot last byte */
    if (n == (size_t)2)
        ctx->base->CTRLB |= CTRLB_ACKACT;

    *buf8 = (uint8_t)ctx->base->DATA;

    if (n == (size_t)1)
        return twi_stop_as_master(ctx, (int)n);

    return 1;
}

static int twi_read_as_master(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    /* NACK */
    if ((ctx->base->STATUS & STATUS_RXNACK) != 0)
        /* this can only happen during address xfer */
        return twi_stop_as_master(ctx, -EAGAIN);

    /* ARBLOST / BUSERR */
    if ((ctx->base->STATUS & STATUS_ARBLOST) != 0 ||
        (ctx->base->STATUS & STATUS_BUSERR) != 0)
        return twi_stop_as_master(ctx, -EPIPE);

    switch (ctx->state) {
    case TWI_ATMEL_SERCOM_STATE_IDLE: return twi_read_as_master_idle(ctx, n);
    case TWI_ATMEL_SERCOM_STATE_DATA: return twi_read_as_master_data(ctx, buf, n);
    case TWI_ATMEL_SERCOM_STATE_STOP: return twi_send_stop_as_master(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

/* SLAVE RECEIVE */

static int twi_read_as_slave_data(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    uint8_t *buf8 = (uint8_t*)buf;

    if ((ctx->base->INTFLAG & INTFLAG_DRDY) == 0)
        return -EAGAIN;

    *buf8 = (uint8_t)ctx->base->DATA;
    ctx->base->INTFLAG = (uint32_t)INTFLAG_DRDY;

    if (n == (size_t)1)
        return twi_stop_as_slave(ctx, (int)n);

    return 1;
}

static int twi_read_as_slave_idle(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    if ((ctx->base->INTFLAG & INTFLAG_AMATCH) == 0)
        return -EAGAIN;

    ctx->base->INTFLAG = (uint32_t)INTFLAG_AMATCH;
    ctx->state = TWI_ATMEL_SERCOM_STATE_DATA;

    /* save 1 schedule */
    return twi_read_as_slave_data(ctx, buf, n);
}

static int twi_read_as_slave(struct twi *ctx, void *buf, size_t n)
{
    /* ARBLOST / BUSERR */
    if ((ctx->base->STATUS & STATUS_COLL) != 0 ||
        (ctx->base->STATUS & STATUS_BUSERR) != 0)
        return twi_stop_as_slave(ctx, -EPIPE);

    switch (ctx->state) {
    case TWI_ATMEL_SERCOM_STATE_IDLE: return twi_read_as_slave_idle(ctx, buf, n);
    case TWI_ATMEL_SERCOM_STATE_DATA: return twi_read_as_slave_data(ctx, buf, n);
    case TWI_ATMEL_SERCOM_STATE_STOP: return twi_wait_stop_as_slave(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/ return -EIO;
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
