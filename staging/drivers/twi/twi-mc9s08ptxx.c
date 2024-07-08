#include "twi-mc9s08ptxx.h"
#include "picoRTOS.h"

struct TWI_MC9S08PTXX {
    volatile uint8_t A1;
    volatile uint8_t F;
    volatile uint8_t C1;
    volatile uint8_t S;
    volatile uint8_t D;
    volatile uint8_t C2;
    volatile uint8_t FLT;
    volatile uint8_t RA;
    volatile uint8_t SMB;
    volatile uint8_t A2;
    volatile uint8_t SLTH;
    volatile uint8_t SLTL;
};

#define A1_AD_M  0x7f
#define A1_AD(x) (((x) & A1_AD_M) << 1)

#define F_MULT_M  0x3u
#define F_MULT(x) (((x) & F_MULT_M) << 6)
#define F_ICR_M   0x3fu
#define F_ICR(x)  ((x) & F_ICR_M)

#define C1_IICEN (1 << 7)
#define C1_IICE  (1 << 6)
#define C1_MST   (1 << 5)
#define C1_TX    (1 << 4)
#define C1_TXAK  (1 << 3)
#define C1_RSTA  (1 << 2)
#define C1_WUEN  (1 << 1)

#define S_TCF   (1 << 7)
#define S_IAAS  (1 << 6)
#define S_BUSY  (1 << 5)
#define S_ARBL  (1 << 4)
#define S_RAM   (1 << 3)
#define S_SRW   (1 << 2)
#define S_IICIF (1 << 1)
#define S_RXAK  (1 << 0)

#define C2_GCAEN (1 << 7)
#define C2_ADEXT (1 << 6)
#define C2_RMEN  (1 << 5)
#define C2_AD_M  0x7u
#define C2_AD(x) ((x) & C2_AD_M)

#define FLT_FLT_M  0x1fu
#define FLT_FLT(x) ((x) & FLT_FLT_M)

#define RA_RAD_M  0x7fu
#define RA_RAD(x) (((x) & RA_RAD_M) << 1)

#define SMB_FACK    (1 << 7)
#define SMB_ALERTEN (1 << 6)
#define SMB_SIICAEN (1 << 5)
#define SMB_TCKSEL  (1 << 4)
#define SMB_SLTF    (1 << 3)
#define SMB_SHTF1   (1 << 2)
#define SMB_SHTF2   (1 << 1)
#define SMB_SHTF2IE (1 << 0)

#define A2_SAD_M  0x7fu
#define A2_SAD(x) (((x) & A2_SAD_M) << 1)

int twi_mc9s08ptxx_init(struct twi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct TWI_MC9S08PTXX*)base;
    ctx->clkid = clkid;
    ctx->state = TWI_MC9S08PTXX_STATE_IDLE;
    ctx->last = 0;

    return 0;
}

static int set_bitrate(struct twi *ctx, unsigned long bitrate)
{
#define SCL_DIVIDER_COUNT (F_ICR_M + 1)

    picoRTOS_assert(bitrate > 0, return -EINVAL);

    int icr;
    unsigned int mult;
    clock_freq_t freq = clock_get_freq(ctx->clkid);

    /* *INDENT-OFF* */
    static const int scl_divider[SCL_DIVIDER_COUNT] = {
        20,  22,  24,  26,   28,   30,   34,   40,   28,   32,   36,   40,   44,   48,   56,   68,
        48,  56,  64,  72,   80,   88,   104,  128,  80,   96,   112,  128,  144,  160,  192,  240,
        160, 192, 224, 256,  288,  320,  284,  480,  320,  384,  448,  512,  576,  640,  768,  960,
        640, 768, 896, 1024, 1152, 1280, 1536, 1920, 1280, 1536, 1792, 2048, 2304, 2560, 3072, 3840
    };
    /* *INDENT-ON* */

    picoRTOS_assert(freq > 0, return -EIO);

    for (mult = 0; mult < 3u; mult++) {
        for (icr = 0; icr < SCL_DIVIDER_COUNT; icr++) {
            unsigned long x = (1ul << mult) * (unsigned long)scl_divider[icr];
            if (x == bitrate) {
                ctx->base->F = (uint8_t)(F_MULT(mult) | F_ICR(icr));
                return 0;
            }
        }
    }

    picoRTOS_break();
    /*@notreached@*/ return -EINVAL;
}

static int set_mode(struct twi *ctx, twi_mode_t mode)
{
    picoRTOS_assert(mode != TWI_MODE_IGNORE, return -EINVAL);
    picoRTOS_assert(mode < TWI_MODE_COUNT, return -EINVAL);

    if (mode == TWI_MODE_MASTER) ctx->base->C1 |= (C1_MST | C1_TX);
    else ctx->base->C1 &= ~(C1_MST | C1_TX);

    ctx->mode = mode;
    return 0;
}

int twi_setup(struct twi *ctx, struct twi_settings *settings)
{
    int res;

    /* turn off */
    ctx->base->C1 &= ~C1_IICEN;

    /* bitrate */
    if (settings->bitrate != 0 &&
        (res = set_bitrate(ctx, settings->bitrate)) < 0)
        return res;

    /* mode */
    if (settings->mode != TWI_MODE_IGNORE &&
        (res = set_mode(ctx, settings->mode)) < 0)
        return res;

    if (ctx->mode == TWI_MODE_SLAVE)
        ctx->base->A1 = (uint8_t)A1_AD(settings->slave_addr);

    /* turn on */
    ctx->base->C1 |= C1_IICEN;

    ctx->slave_addr = settings->slave_addr;
    return 0;
}

int twi_poll(struct twi *ctx)
{
    if ((ctx->base->S & S_IAAS) != 0) {
        if ((ctx->base->S & S_SRW) == 0) return TWI_WRITE;
        else return TWI_READ;
    }

    return -EAGAIN;
}

/* MASTER STOP */

static int twi_send_stop_as_master(struct twi *ctx)
{
    if ((ctx->base->S & S_IICIF) == 0)
        return -EAGAIN;

    ctx->base->S |= S_IICIF; /* ack */
    ctx->base->C1 &= ~C1_MST;

    return ctx->last;
}

static int twi_stop_as_master(struct twi *ctx, int ret)
{
    ctx->state = TWI_MC9S08PTXX_STATE_STOP;
    ctx->last = ret;

    return twi_send_stop_as_master(ctx);
}

/* MASTER TRANSMIT */

static int twi_write_as_master_data(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    const uint8_t *buf8 = (const uint8_t*)buf;

    if ((ctx->base->S & S_IICIF) == 0)
        return -EAGAIN;

    /* check for errors */
    if ((ctx->base->S & S_RXAK) != 0)
        return twi_stop_as_master(ctx, 0);

    if ((ctx->base->S & S_BUSY) != 0)
        return twi_stop_as_master(ctx, -EPIPE);

    /* byte trasmitted */
    if ((ctx->base->S & S_TCF) == 0)
        return -EAGAIN;

    ctx->base->S |= S_IICIF; /* ack */
    ctx->base->D = (uint8_t)*buf8;

    if (n == (size_t)1)
        return twi_stop_as_master(ctx, (int)n);

    return 1;
}

static int twi_write_as_master_idle(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->S & S_BUSY) != 0)
        return -EAGAIN;

    ctx->base->S |= S_IICIF;
    ctx->base->C1 |= (C1_MST | C1_TX);
    ctx->base->D = (uint8_t)A1_AD(ctx->slave_addr);

    ctx->state = TWI_MC9S08PTXX_STATE_DATA;
    return twi_write_as_master_data(ctx, buf, n);
}

static int twi_write_as_master(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_MC9S08PTXX_STATE_IDLE: return twi_write_as_master_idle(ctx, buf, n);
    case TWI_MC9S08PTXX_STATE_DATA: return twi_write_as_master_data(ctx, buf, n);
    case TWI_MC9S08PTXX_STATE_STOP: return twi_send_stop_as_master(ctx);
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

    if ((ctx->base->S & S_IICIF) == 0)
        return -EAGAIN;

    /* errors */
    if ((ctx->base->S & S_BUSY) != 0) {
        ctx->state = TWI_MC9S08PTXX_STATE_IDLE;
        return -EPIPE;
    }

    /* EOT */
    if ((ctx->base->S & S_RXAK) == 0 ||
        (ctx->base->S & S_TCF) == 0)
        return -EAGAIN;

    ctx->base->S |= S_IICIF; /* ack */
    ctx->base->D = (uint8_t)*buf8;

    if (n == (size_t)1)
        ctx->state = TWI_MC9S08PTXX_STATE_IDLE;

    return 1;
}

static int twi_write_as_slave_idle(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->S & S_IAAS) == 0)
        return -EAGAIN;

    ctx->base->C1 |= C1_TX;

    ctx->state = TWI_MC9S08PTXX_STATE_DATA;
    return twi_write_as_slave_data(ctx, buf, n);
}

static int twi_write_as_slave(struct twi *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_MC9S08PTXX_STATE_IDLE: return twi_write_as_slave_idle(ctx, buf, n);
    case TWI_MC9S08PTXX_STATE_DATA: return twi_write_as_slave_data(ctx, buf, n);
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

    if ((ctx->base->S & S_IICIF) == 0)
        return -EAGAIN;

    /* no TX */
    ctx->base->C1 &= ~C1_TX;

    /* check for errors */
    if ((ctx->base->S & S_BUSY) != 0)
        return twi_stop_as_master(ctx, -EPIPE);

    /* byte trasmitted */
    if ((ctx->base->S & S_TCF) == 0)
        return -EAGAIN;

    *buf8 = ctx->base->D;
    ctx->base->S |= S_IICIF; /* ack */

    if (n == (size_t)1)
        return twi_stop_as_master(ctx, (int)n);

    return 1;
}

static int twi_read_as_master_idle(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->S & S_BUSY) != 0)
        return -EAGAIN;

    ctx->base->S |= S_IICIF;
    ctx->base->C1 |= (C1_MST | C1_TX);
    ctx->base->D = (uint8_t)(A1_AD(ctx->slave_addr) | TWI_READ);
    /* perform dummy read */
    /*@i@*/ (void)ctx->base->D;

    ctx->state = TWI_MC9S08PTXX_STATE_DATA;
    return twi_read_as_master_data(ctx, buf, n);
}

static int twi_read_as_master(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_MC9S08PTXX_STATE_IDLE: return twi_read_as_master_idle(ctx, buf, n);
    case TWI_MC9S08PTXX_STATE_DATA: return twi_read_as_master_data(ctx, buf, n);
    case TWI_MC9S08PTXX_STATE_STOP: return twi_send_stop_as_master(ctx);
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

    if ((ctx->base->S & S_IICIF) == 0 ||
        (ctx->base->S & S_TCF) == 0)
        return -EAGAIN;

    *buf8 = ctx->base->D;
    ctx->base->S |= S_IICIF; /* ack */

    /* signal EOT */
    if (n == (size_t)2)
        ctx->base->C1 |= C1_TXAK;

    if (n == (size_t)1)
        ctx->state = TWI_MC9S08PTXX_STATE_IDLE;

    return 1;
}

static int twi_read_as_slave_idle(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    if ((ctx->base->S & S_IAAS) == 0)
        return -EAGAIN;

    ctx->base->C1 &= ~(C1_TX | C1_TXAK);

    ctx->state = TWI_MC9S08PTXX_STATE_DATA;
    return twi_read_as_slave_data(ctx, buf, n);
}

static int twi_read_as_slave(struct twi *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case TWI_MC9S08PTXX_STATE_IDLE: return twi_read_as_slave_idle(ctx, buf, n);
    case TWI_MC9S08PTXX_STATE_DATA: return twi_read_as_slave_data(ctx, buf, n);
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
