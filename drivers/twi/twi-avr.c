#include "twi-avr.h"
#include "picoRTOS.h"

#include <stdint.h>

struct TWI_AVR {
    volatile uint8_t TWBR;          /* 0xb8 */
    volatile uint8_t TWSR;          /* 0xb9 */
    volatile uint8_t TWAR;          /* 0xba */
    volatile uint8_t TWDR;          /* 0xbb */
    volatile uint8_t TWCR;          /* 0xbc */
    volatile uint8_t TWAMR;         /* 0xbd */
};

#define TWSR_TWS_M       0xf8u
#define TWSR_TWS_READ(x) ((x) & (uint8_t)TWSR_TWS_M)
#define TWSR_TWPS_M      0x3u
#define TWSR_TWPS(x)     ((x) & TWSR_TWPS_M)

#define TWCR_TWINT (1 << 7)
#define TWCR_TWEA  (1 << 6)
#define TWCR_TWSTA (1 << 5)
#define TWCR_TWSTO (1 << 4)
#define TWCR_TWWC  (1 << 3)
#define TWCR_TWEN  (1 << 2)

/* master transmitter mode */
#define TWSR_MT_START_SENT              0x8
#define TWSR_MT_REPEATED_START_SENT     0x10
#define TWSR_MT_SLAW_SENT_ACK_RECV      0x18
#define TWSR_MT_SLAW_SENT_ACK_NOT_RECV  0x20
#define TWSR_MT_BYTE_SENT_ACK_RECV      0x28
#define TWSR_MT_BYTE_SENT_ACK_NOT_RECV  0x30
#define TWSR_MT_ARB_LOST_IN_SLA         0x38
/* master receiver mode */
#define TWSR_MR_SLAR_SENT_ACK_RECV      0x40
#define TWSR_MR_SLAR_SENT_ACK_NOT_RECV  0x48
#define TWSR_MR_BYTE_RECV_ACK_RET       0x50
#define TWSR_MR_BYTE_RECV_NACK_RET      0x58

/* slave receiver mode */
#define TWSR_SR_SLAW_RECV_ACK_RET          0x60
#define TWSR_SR_SLAW_ARB_LOST_ACK_RET      0x68
#define TWSR_SR_GENCALL_RECV_ACK_RET       0x70
#define TWSR_SR_GENCALL_ARB_LOST_ACK_RET   0x78
#define TWSR_SR_BYTE_RECV_ACK_RET          0x80
#define TWSR_SR_BYTE_RECV_NACK_RET         0x88
#define TWSR_SR_GENCALL_BYTE_RECV_ACK_RET  0x90
#define TWSR_SR_GENCALL_BYTE_RECV_NACK_RET 0x98
#define TWSR_SR_STOP_COND_RSTART_RECV      0xa0
/* slave transmitter mode */
#define TWSR_ST_SLAR_RECV_ACK_RET          0xa8
#define TWSR_ST_SLAR_ARB_LOST_ACK_RET      0xb0
#define TWSR_ST_BYTE_SENT_ACK_RECV         0xb8
#define TWSR_ST_BYTE_SENT_NACK_RECV        0xc0
#define TWSR_ST_LAST_BYTE_SEND_ACK_RECV    0xc8

#define TWAR_TWA_M  0x7fu
#define TWAR_TWA(x) (((x) & TWAR_TWA_M) << 1)
#define TWAR_TWCGE  (1 << 0)

typedef enum {
    TWI_AVR_W   = 0,
    TWI_AVR_R   = 1,
    TWI_AVR_RW_COUNT
} twi_avr_rw_t;

/* public */

/* Function: twi_avr_init
 * Initializes a TWI
 *
 * Parameters:
 *  ctx - The TWI to init
 *  base - The TWI base address
 *  clkid - The clock id associated to the TWI
 *
 * Returns:
 * Always 0
 */
int twi_avr_init(struct twi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct TWI_AVR*)base;
    ctx->clkid = clkid;
    ctx->mode = TWI_MODE_IGNORE;
    ctx->state = TWI_AVR_STATE_IDLE;
    ctx->last = 0;

    /* enable */
    ctx->base->TWCR |= TWCR_TWEN;

    return 0;
}

/* hooks */

static int set_bitrate(struct twi *ctx, unsigned long bitrate)
{
    if (!picoRTOS_assert(bitrate > 0)) return -EINVAL;

    unsigned long twbr;
    unsigned long shift;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    if (!picoRTOS_assert(freq > 0))
        return -EIO;

    /* scl = freq / (16 + 2(TWBR) * 4 ^ TWPS) */

    for (twbr = 0; twbr < 256ul; twbr++) {
        for (shift = 0; shift < 7ul; shift += 2) {
            unsigned long divider = 16ul + 2ul * twbr * (1ul << shift);
            unsigned long result = (unsigned long)freq / divider;

            if (result == bitrate) {
                /* write to registers */
                ctx->base->TWBR = (uint8_t)twbr;
                ctx->base->TWSR &= ~TWSR_TWPS_M;
                ctx->base->TWSR |= TWSR_TWPS(shift / 2);
                return 0;
            }
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
    if (settings->slave_addr != (twi_addr_t)0) {
        ctx->base->TWAR = (uint8_t)TWAR_TWA(settings->slave_addr);
        ctx->slave_addr = settings->slave_addr;
    }

    return res;
}

int twi_poll(struct twi *ctx)
{
    int tws = (int)TWSR_TWS_READ(ctx->base->TWSR);

    if (tws == TWSR_SR_SLAW_RECV_ACK_RET) return TWI_WRITE;
    if (tws == TWSR_ST_SLAR_RECV_ACK_RET) return TWI_READ;

    return -EAGAIN;
}

static int twi_rw_as_master_idle(struct twi *ctx)
{
    /* 1. Application writes to TWCR to initiate transmission of START */
    ctx->base->TWCR = (uint8_t)(TWCR_TWSTA | TWCR_TWEA | TWCR_TWEN);

    ctx->state = TWI_AVR_STATE_START;
    return -EAGAIN;
}

static int twi_rw_as_master_start(struct twi *ctx, twi_avr_rw_t rw)
{
    if (!picoRTOS_assert(rw < TWI_AVR_RW_COUNT)) return -EINVAL;

    uint8_t twcr = ctx->base->TWCR;
    int tws = (int)TWSR_TWS_READ(ctx->base->TWSR);

    /* 2. TWINT set. Status code indicates START condition sent */
    if ((twcr & TWCR_TWINT) == 0)
        return -EAGAIN;

    /* 3. Check TWSR to see if START was sent */
    if (tws != TWSR_MT_START_SENT &&
        tws != TWSR_MT_REPEATED_START_SENT) {
        ctx->last = -EFAULT;
        ctx->state = TWI_AVR_STATE_STOP;
        return -EAGAIN;
    }

    /* Application loads SLA+W into TWDR, and loads appropriate control
     * signals into TWCR, makin sure that TWINT is written to one,
     * and TWSTA is written to zero */
    ctx->base->TWDR = (uint8_t)TWAR_TWA(ctx->slave_addr) | rw;
    ctx->base->TWCR = twcr & ~TWCR_TWSTA;

    ctx->state = TWI_AVR_STATE_SLA;
    return -EAGAIN;
}

static int twi_write_as_master_sla(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    uint8_t twcr = ctx->base->TWCR;
    int tws = (int)TWSR_TWS_READ(ctx->base->TWSR);

    /* 4. TWINT set. Status code indicates SLA+W sent, ACK received */
    if ((twcr & TWCR_TWINT) == 0)
        return -EAGAIN;

    /* 5. Check TWSR to see if SLA+W was sent and ACK received */
    if (tws != TWSR_MT_SLAW_SENT_ACK_RECV) {
        ctx->last = -EPIPE;
        ctx->state = TWI_AVR_STATE_STOP;
        return -EAGAIN;
    }

    /* Application loads data into TWDR, and loads appropriate control
     * signals into TWCR, making sure that TWINT is written to one */
    ctx->base->TWDR = *(uint8_t*)buf;
    ctx->base->TWCR = twcr;

    /* if message is 1 byte long, stop immediately */
    if (n == (size_t)1) {
        ctx->last = 1;
        ctx->state = TWI_AVR_STATE_STOP;
        return -EAGAIN;
    }

    ctx->state = TWI_AVR_STATE_DATA;
    return 1;
}

static int twi_write_as_master_data(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    uint8_t twcr = ctx->base->TWCR;
    int tws = (int)TWSR_TWS_READ(ctx->base->TWSR);

    if ((twcr & TWCR_TWINT) == 0)
        return -EAGAIN;

    if (tws == TWSR_MT_BYTE_SENT_ACK_RECV) {
        /* send next byte */
        ctx->base->TWDR = *(uint8_t*)buf;
        ctx->base->TWCR = twcr;
        /* last byte */
        if (n == (size_t)1) {
            ctx->last = 1;
            ctx->state = TWI_AVR_STATE_STOP;
            return -EAGAIN;
        }

        return 1;
    }

    if (tws == TWSR_MT_BYTE_SENT_ACK_NOT_RECV) {
        /* just stop */
        ctx->last = 1;
        ctx->state = TWI_AVR_STATE_STOP;
        return -EAGAIN;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_rw_as_master_stop(struct twi *ctx)
{
    if ((ctx->base->TWCR & TWCR_TWINT) == 0)
        return -EAGAIN;

    /* Application loads appropriate control signals to send STOP into TWCR,
     * making sure that TWINT is written to one */
    ctx->base->TWCR = (uint8_t)(TWCR_TWINT | TWCR_TWSTO | TWCR_TWEN);

    ctx->state = TWI_AVR_STATE_IDLE;
    return ctx->last;
}

static int twi_write_as_master(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->state) {
    case TWI_AVR_STATE_IDLE: return twi_rw_as_master_idle(ctx);
    case TWI_AVR_STATE_START: return twi_rw_as_master_start(ctx, TWI_AVR_W);
    case TWI_AVR_STATE_SLA: return twi_write_as_master_sla(ctx, buf, n);
    case TWI_AVR_STATE_DATA: return twi_write_as_master_data(ctx, buf, n);
    case TWI_AVR_STATE_STOP: return twi_rw_as_master_stop(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_read_as_master_sla(struct twi *ctx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    uint8_t twcr = ctx->base->TWCR;
    int tws = (int)TWSR_TWS_READ(ctx->base->TWSR);

    /* 4. TWINT set. Status code indicates SLA+W sent, ACK received */
    if ((twcr & TWCR_TWINT) == 0)
        return -EAGAIN;

    /* 5. Check TWSR to see if SLA+W was sent and ACK received */
    if (tws != TWSR_MR_SLAR_SENT_ACK_RECV) {
        ctx->last = -EPIPE;
        ctx->state = TWI_AVR_STATE_STOP;
        return -EAGAIN;
    }

    /* 1 byte xfer */
    if (n == (size_t)1) twcr &= ~TWCR_TWEA;

    ctx->base->TWCR = twcr;
    ctx->state = TWI_AVR_STATE_DATA;

    return -EAGAIN;
}

static int twi_read_as_master_data(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    uint8_t twcr = ctx->base->TWCR;
    int tws = (int)TWSR_TWS_READ(ctx->base->TWSR);

    if ((twcr & TWCR_TWINT) == 0)
        return -EAGAIN;

    if (tws != TWSR_MR_BYTE_RECV_ACK_RET &&
        tws != TWSR_MR_BYTE_RECV_NACK_RET) {
        ctx->last = -EIO;
        ctx->state = TWI_AVR_STATE_STOP;
        return -EAGAIN;
    }

    /* data received */
    *(uint8_t*)buf = ctx->base->TWDR;

    if (n == (size_t)1) {
        ctx->last = 1;
        ctx->state = TWI_AVR_STATE_STOP;
        return -EAGAIN;
    }

    /* last byte xfer */
    if (n == (size_t)2) twcr &= ~TWCR_TWEA;

    ctx->base->TWCR = twcr;
    return 1;
}

static int twi_read_as_master(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;
    if (!picoRTOS_assert(ctx->mode == TWI_MODE_MASTER)) return -EIO;

    switch (ctx->state) {
    case TWI_AVR_STATE_IDLE: return twi_rw_as_master_idle(ctx);
    case TWI_AVR_STATE_START: return twi_rw_as_master_start(ctx, TWI_AVR_R);
    case TWI_AVR_STATE_SLA: return twi_read_as_master_sla(ctx, n);
    case TWI_AVR_STATE_DATA: return twi_read_as_master_data(ctx, buf, n);
    case TWI_AVR_STATE_STOP: return twi_rw_as_master_stop(ctx);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_error(struct twi *ctx)
{
    /* disable */
    ctx->base->TWCR &= ~TWCR_TWEN;
    ctx->state = TWI_AVR_STATE_IDLE;
    return -EIO;
}

static int twi_rw_as_slave_idle(struct twi *ctx)
{
    ctx->base->TWCR = (uint8_t)(TWCR_TWEA | TWCR_TWEN);
    ctx->state = TWI_AVR_STATE_SLA;

    return -EAGAIN;
}

static int twi_write_as_slave_sla(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    uint8_t twcr = ctx->base->TWCR;
    int tws = (int)TWSR_TWS_READ(ctx->base->TWSR);

    if ((twcr & TWCR_TWINT) == 0)
        return -EAGAIN;

    if (tws != TWSR_ST_SLAR_RECV_ACK_RET &&
        tws != TWSR_ST_SLAR_ARB_LOST_ACK_RET)
        return twi_error(ctx);

    /* 1 byte frame */
    if (n == (size_t)1)
        twcr &= ~TWCR_TWEA;

    /* send 1st byte */
    ctx->base->TWDR = *(uint8_t*)buf;
    ctx->base->TWCR = twcr;

    ctx->state = TWI_AVR_STATE_DATA;
    return -EAGAIN;
}

static int twi_write_as_slave_data(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    uint8_t twcr = ctx->base->TWCR;
    int tws = (int)TWSR_TWS_READ(ctx->base->TWSR);

    if ((twcr & TWCR_TWINT) == 0)
        return -EAGAIN;

    /* end of xfer */
    if (tws == TWSR_ST_LAST_BYTE_SEND_ACK_RECV ||
        tws == TWSR_ST_BYTE_SENT_NACK_RECV) {
        ctx->base->TWCR = twcr;
        ctx->state = TWI_AVR_STATE_IDLE;
        return 1;
    }

    /* follow-up */
    if (tws != TWSR_ST_BYTE_SENT_ACK_RECV)
        return twi_error(ctx);

    /* prepate last byte */
    if (n == (size_t)1) twcr &= ~TWCR_TWEA;

    ctx->base->TWDR = *(uint8_t*)buf;
    ctx->base->TWCR = twcr;

    return 1;
}

static int twi_write_as_slave(struct twi *ctx, const void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->state) {
    case TWI_AVR_STATE_IDLE: return twi_rw_as_slave_idle(ctx);
    case TWI_AVR_STATE_SLA: return twi_write_as_slave_sla(ctx, buf, n);
    case TWI_AVR_STATE_DATA: return twi_write_as_slave_data(ctx, buf, n);
    default: break;
    }

    picoRTOS_break();
    /*@notreached@*/
    return -EIO;
}

static int twi_read_as_slave_sla(struct twi *ctx, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    uint8_t twcr = ctx->base->TWCR;
    int tws = (int)TWSR_TWS_READ(ctx->base->TWSR);

    if ((twcr & TWCR_TWINT) == 0)
        return -EAGAIN;

    switch (tws) {
    case TWSR_SR_SLAW_RECV_ACK_RET: /*@fallthrough@*/
    case TWSR_SR_SLAW_ARB_LOST_ACK_RET:
        break;

    default:
        picoRTOS_break();
        /*@notreached@*/
        return twi_error(ctx);
    }

    /* 1 byte xfer */
    if (n == (size_t)1) twcr &= ~TWCR_TWEA;

    ctx->base->TWCR = twcr;
    ctx->state = TWI_AVR_STATE_DATA;

    return -EAGAIN;
}

static int twi_read_as_slave_data(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    uint32_t twcr = ctx->base->TWCR;
    int tws = (int)TWSR_TWS_READ(ctx->base->TWSR);

    if ((twcr & TWCR_TWINT) == 0)
        return -EAGAIN;

    switch (tws) {
    case TWSR_SR_BYTE_RECV_ACK_RET:             /*@fallthrough@*/
    case TWSR_SR_BYTE_RECV_NACK_RET:            /*@fallthrough@*/
    case TWSR_SR_GENCALL_BYTE_RECV_ACK_RET:     /*@fallthrough@*/
    case TWSR_SR_GENCALL_BYTE_RECV_NACK_RET:
        break;

    case TWSR_SR_STOP_COND_RSTART_RECV:
        ctx->base->TWCR = twcr;
        ctx->state = TWI_AVR_STATE_IDLE;
        return -EAGAIN;

    default:
        picoRTOS_break();
        /*@notreached@*/
        return twi_error(ctx);
    }

    /* prepare last byte xfer */
    if (n == (size_t)2) twcr &= ~TWCR_TWEA;

    *(uint8_t*)buf = ctx->base->TWDR;
    ctx->base->TWCR = twcr;

    /* last byte xfered */
    if (n == (size_t)1)
        ctx->state = TWI_AVR_STATE_IDLE;

    return 1;
}

static int twi_read_as_slave(struct twi *ctx, void *buf, size_t n)
{
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    switch (ctx->state) {
    case TWI_AVR_STATE_IDLE: return twi_rw_as_slave_idle(ctx);
    case TWI_AVR_STATE_SLA: return twi_read_as_slave_sla(ctx, n);
    case TWI_AVR_STATE_DATA: return twi_read_as_slave_data(ctx, buf, n);
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
