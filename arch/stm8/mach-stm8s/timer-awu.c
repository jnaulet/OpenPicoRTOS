#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

#include <generated/autoconf.h>

#define CAT(a, b) a ## b
#define UL(x)     CAT(x, ul)

#define LSI_FREQ_HZ 128000ul
#define PERIOD      (LSI_FREQ_HZ / UL(CONFIG_TICK_HZ))

#if CONFIG_TICK_HZ > LSI_FREQ_HZ
# error CONFIG_TICK_HZ is too damn high !
#endif

/*
 * AWU
 */

struct AWU {
    volatile uint8_t CSR;
    volatile uint8_t APR;
    volatile uint8_t TBR;
};

#define CSR_AWUF  (1 << 5)
#define CSR_AWUEN (1 << 4)
#define CSR_MSR   (1 << 0)

#define APR_APR_M  0x3fu
#define APR_APR(x) ((x) & APR_APR_M)

#define TBR_AWUTB_M  0xfu
#define TBR_AWUTB(x) ((x) & TBR_AWUTB_M)

/*
 * OPT4
 */

#define OPT4_EXT_CLK  (1 << 3)
#define OPT4_CKAWUSEL (1 << 2)
#define OPT4_PRSC_M   0x3u
#define OPT4_PRSC(x)  ((x) & OPT4_PRSC_M)

/* CLOCK */

static struct AWU *AWU = (struct AWU*)ADDR_AWU;
/* static volatile uint8_t *OPT4 = (uint8_t*)ADDR_OPT4; */

static unsigned long sysclk_hz = DEVICE_DEFAULT_SYSCLK_HZ;

void arch_timer_init(void)
{
    unsigned int awutb = 1u;
    unsigned int apr = (unsigned int)PERIOD;

    while (apr > 64u) {
        awutb++;
        apr >>= 1;
    }

    /* set timer */
    AWU->APR = (uint8_t)APR_APR(apr - 2u);
    AWU->TBR = (uint8_t)TBR_AWUTB(awutb);
    /* start */
    AWU->CSR |= CSR_AWUEN;
}

void arch_set_clock_frequency(unsigned long freq)
{
    arch_assert_void(freq != 0);
    sysclk_hz = freq;
}

void arch_delay_us(unsigned long n)
{
    arch_assert_void(n != 0);

    /* 4 cycles per instruction on n76e003 */
    unsigned long ncycles = (n * (sysclk_hz / 1000000ul)) >> 2;

    while (ncycles-- != 0)
        ASM(" nop");
}
