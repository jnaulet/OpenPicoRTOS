#include "rng-stm32h7xx_trng.h"

#include "picoRTOS.h"
#include "picoRTOS_port.h"

#include <stdint.h>

struct RNG_STM32H7XX_TRNG {
    volatile uint32_t CR;
    volatile uint32_t SR;
    volatile uint32_t DR;
    uint32_t RESERVED;
    volatile uint32_t HTCR;
};

#define CR_CONFIGLOCK     (1u << 31)
#define CR_CONDRST        (1 << 30)
#define CR_RNG_CONFIG1_M  0x3fu
#define CR_RNG_CONFIG1(x) (((x) & CR_RNG_CONFIG1_M) << 20)
#define CR_CLKDIV_M       0xfu
#define CR_CLKDIV(x)      (((x) & CR_CLKDIV_M) << 16)
#define CR_RNG_CONFIG2_M  0x7u
#define CR_RNG_CONFIG2(x) (((x) & CR_RNG_CONFIG2_M) << 13)
#define CR_NISTC          (1 << 12)
#define CR_RNG_CONFIG3_M  0xfu
#define CR_RNG_CONFIG3(x) (((x) & CR_RNG_CONFIG3_M) << 8)
#define CR_CED            (1 << 5)
#define CR_IE             (1 << 3)
#define CR_RNGEN          (1 << 2)

#define SR_SEIS (1 << 6)
#define SR_CEIS (1 << 5)
#define SR_SECS (1 << 2)
#define SR_CECS (1 << 1)
#define SR_DRDY (1 << 0)

int rng_stm32h7xx_trng_init(struct rng *ctx, int base, clock_id_t clkid)
{
    clock_freq_t freq;
    unsigned int div_p2;
    int deadlock = CONFIG_DEADLOCK_COUNT;

    ctx->base = (struct RNG_STM32H7XX_TRNG*)base; // NOLINT
    if ((freq = clock_get_freq(clkid)) <= 0)
        return -EINVAL;

    /* st recommended configuration */
    /* 48mhz clock */
    for (div_p2 = 0; div_p2 <= 15u; div_p2++)
        if (((unsigned)freq >> div_p2) <= 48000000u)
            break;

    picoRTOS_assert(div_p2 < 16u, return -EIO);

    /* Reset to configuration A */
    ctx->base->CR = (uint32_t)(CR_CONDRST |
                               CR_RNG_CONFIG1(0xf) |
                               CR_RNG_CONFIG3(0xd) |
                               CR_CLKDIV(div_p2));

    /* When writing this register, the magic number 0x17590ABC
     * must be written immediately before the indicated value. */
    ctx->base->HTCR = (uint32_t)0x17590abc;
    ctx->base->HTCR = (uint32_t)0xaa74; /* FIXME: this is chip-specific */

    /* un-reset */
    ctx->base->CR &= ~(uint32_t)CR_CONDRST;
    while ((ctx->base->CR & CR_CONDRST) != 0 &&
           deadlock-- != 0) arch_delay_us(1ul);

    picoRTOS_assert(deadlock != -1, return -EBUSY);

    /* turn on */
    ctx->base->CR |= (CR_CONFIGLOCK | CR_RNGEN);
    return 0;
}

/*
 * hal
 */

int rng_read(struct rng *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    size_t count = n;
    uint8_t *buf8 = (uint8_t*)buf;

    do {
        if ((ctx->base->SR & SR_DRDY) == 0)
            break;

        /* read random number */
        uint32_t DR = ctx->base->DR;

        switch (count) {
        default: *buf8++ = (uint8_t)(DR >> 24); count--;    /* >= 4 */ /*@fallthrough@*/
        case 3: *buf8++ = (uint8_t)(DR >> 16); count--;     /*@fallthrough@*/
        case 2: *buf8++ = (uint8_t)(DR >> 8); count--;      /*@fallthrough@*/
        case 1: *buf8++ = (uint8_t)(DR >> 0); count--;      /*@fallthrough@*/
        case 0: break;
        }
    } while (count > 0);

    /* how many read */
    if ((n - count) == 0)
        return -EAGAIN;

    return (int)(n - count);
}
