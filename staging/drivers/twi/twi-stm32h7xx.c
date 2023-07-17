#include "twi-stm32h7xx.h"
#include "picoRTOS.h"

#include <stdint.h>

struct TWI_STM32H7XX {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t TIMINGR;
    volatile uint32_t TIMEOUTR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t PECR;
    volatile uint32_t RXDR;
    volatile uint32_t TXDR;
};

#define CR1_PECEN     (1 << 23)
#define CR1_ALERTEN   (1 << 22)
#define CR1_SMBDEN    (1 << 21)
#define CR1_SMBHEN    (1 << 20)
#define CR1_GCEN      (1 << 19)
#define CR1_WUPEN     (1 << 18)
#define CR1_NOSTRETCH (1 << 17)
#define CR1_SBC       (1 << 16)
#define CR1_RXDMAEN   (1 << 16)
#define CR1_TXDMAEN   (1 << 14)
#define CR1_ANFOFF    (1 << 12)
#define CR1_DNF_M     0xfu
#define CR1_DNF(x)    (((x) & CR1_DNF_M) << 8)
#define CR1_ERRIE     (1 << 7)
#define CR1_TCIE      (1 << 6)
#define CR1_STOPIE    (1 << 5)
#define CR1_NACKIE    (1 << 4)
#define CR1_ADDRIE    (1 << 3)
#define CR1_RXIE      (1 << 2)
#define CR1_TXIE      (1 << 1)
#define CR1_PE        (1 << 0)

#define DR2_PECBYE    (1 << 26)
#define DR2_AUTOEND   (1 << 25)
#define DR2_RELOAD    (1 << 24)
#define DR2_NBYTES_M  0xffu
#define DR2_NBYTES(x) (((x) & DR2_NBYTES_M) << 16)
#define DR2_NACK      (1 << 15)
#define DR2_STOP      (1 << 14)
#define DR2_START     (1 << 13)
#define DR2_HEAD10R   (1 << 12)
#define DR2_ADD10     (1 << 11)
#define DR2_RD_WRN    (1 << 10)
#define DR2_SADD_M    0x3ffu
#define DR2_SADD(x)   ((x) & DR2_SADD_M)

#define OAR1_OA1EN   (1 << 15)
#define OAR1_OA1MODE (1 << 10)
#define OAR1_OA1_M   0x3ffu
#define OAR1_OA1(x)  ((x) & OAR1_OA1_M)

#define OAR2_OA2EN     (1 << 15)
#define OAR2_OA2MSK_M  0x7u
#define OAR2_OA2MSK(x) (((x) & OAR2_OA2MSK_M) << 8)
#define OAR2_OA2_M     0x7fu
#define OAR2_OA2(x)    ((x) & OAR1_OA1_M)

#define TIMINGR_PRESC_M 0xfu
#define TIMINGR_PRESC(x) (((x) & TIMINGR_PRESC_M) << 28)
#define TIMINGR_SCLDEL_M 0xfu
#define TIMINGR_SCLDEL(x) (((x) & TIMINGR_SCLDEL_M) << 20)
#define TIMINGR_SDADEL_M 0xfu
#define TIMINGR_SDADEL(x) (((x) & TIMINGR_SDADEL_M) << 16)
#define TIMINGR_SCLH_M 0xffu
#define TIMINGR_SCLH(x) (((x) & TIMINGR_SCLH_M) << 8)
#define TIMINGR_SCLL_M 0xffu
#define TIMINGR_SCLL(x) ((x) & TIMINGR_SCLL_M)

#define TIMEOUTR_TEXTEN (1u << < 31)
#define TIMEOUTR_TIMEOUTB_M 0xfffu
#define TIMEOUTR_TIMEOUTB(x) (((x) & TIMEOUTR_TIMEOUTB_M) << 16)
#define TIMEOUTR_TIMOUTEN (1 << 15)
#define TIMEOUTR_TIDLE (1 << 12)
#define TIMEOUTR_TIMEOUTA_M 0xfffu
#define TIMEOUTR_TIMEOUTA(x) ((x) & TIMEOUTR_TIMEOUTA_M)

#define ISR_ADDCODE_M  0x7fu
#define ISR_ADDCODE(x) (((x) & ISR_ADDCODE_M) << 17)
#define ISR_DIR        (1 << 16)
#define ISR_BUSY       (1 << 15)
#define ISR_ALERT      (1 << 13)
#define ISR_TIMEOUT    (1 << 12)
#define ISR_PECERR     (1 << 11)
#define ISR_OVR        (1 << 10)
#define ISR_ARLO       (1 << 9)
#define ISR_BERR       (1 << 8)
#define ISR_TCR        (1 << 7)
#define ISR_TC         (1 << 6)
#define ISR_STOPF      (1 << 5)
#define ISR_NACKF      (1 << 4)
#define ISR_ADDR       (1 << 3)
#define ISR_RXNE       (1 << 2)
#define ISR_TXIS       (1 << 1)
#define ISR_TXE        (1 << 0)

int twi_stm32h7xx_init(struct twi *ctx, int base, clock_id_t clkid)
{
    ctx->base = (struct TWI_STM32H7XX*)base;
    ctx->clkid = clkid;

    return 0;
}

static int set_bitrate(struct twi *ctx, unsigned long bitrate)
{
    if (!picoRTOS_assert(bitrate > 0)) return -EINVAL;

    picoRTOS_break();
    /*@notreached@*/ return -EINVAL;
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

    /* TODO: slave_addr */

    return res;
}

int twi_poll(struct twi *ctx)
{
    return -EAGAIN;
}

int twi_write(struct twi *ctx, const void *buf, size_t n)
{
}

int twi_read(struct twi *ctx, void *buf, size_t n)
{
}
