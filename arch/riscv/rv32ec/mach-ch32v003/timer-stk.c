#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stddef.h>
#include <stdint.h>

struct R32_STK {
    volatile uint32_t CTLR;
    volatile uint32_t SR;
    volatile uint32_t CNTL;
    uint32_t RESERVED0;
    volatile uint32_t CMPLR;
};

#define CTLR_SWIE  (1 << 31u)
#define CTLR_STRE  (1 << 3)
#define CTLR_STCLK (1 << 2)
#define CTLR_STIE  (1 << 1)
#define CTLR_STE   (1 << 0)

#define PFIC_IENR1 ((volatile uint32_t*)0xe000e100)

static struct R32_STK *STK = (struct R32_STK*)0xe000f000;

/* CLOCK */
static int sysclk_hz = DEVICE_DEFAULT_SYSCLK_HZ;
static int ncycles_per_us = (DEVICE_DEFAULT_SYSCLK_HZ / 1000000);

/* FUNCTIONS TO IMPLEMENT */

void arch_timer_init(void)
{
    STK->CNTL = 0;
    STK->CMPLR = (uint32_t)((sysclk_hz / CONFIG_TICK_HZ) - 1);  /* set period */

    *PFIC_IENR1 |= (1 << 12);                                   /* enable systick interrupt */
    STK->CTLR = (uint32_t)(CTLR_STRE | CTLR_STCLK |
                           CTLR_STIE | CTLR_STE);
}

/* STATS */

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)STK->CNTL;

    if (counter == ARCH_COUNTER_SINCE) {
        picoRTOS_cycles_t cntl = (picoRTOS_cycles_t)STK->CNTL;
        picoRTOS_cycles_t cmplr = (picoRTOS_cycles_t)STK->CMPLR;

        /* several cases here */
        if (t > cmplr) return cmplr + 1;    /* only used on first tick */
        if (cntl < t) return cntl + t;      /* rollover */
        /* normal */
        return cntl;
    }

    arch_assert_void(false);
    return 0;
}

/* CLOCKS */

void arch_set_clock_frequency(unsigned long freq)
{
    arch_assert_void(freq != 0);

    sysclk_hz = (int)freq;
    ncycles_per_us = (int)freq / 1000000;
}

void arch_delay_us(unsigned long n)
{
    arch_assert_void(n != 0);

    unsigned long ncycles = n * (unsigned long)ncycles_per_us;

    while (ncycles-- != 0)
        ASM("nop");
}
