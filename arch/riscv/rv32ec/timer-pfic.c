#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <generated/autoconf.h>

/* PFIC STK */
#define STK_CTLR  ((volatile unsigned long*)0xe000f000)
#define STK_SR    ((volatile unsigned long*)0xe000f004)
#define STK_CNTL  ((volatile unsigned long*)0xe000f008)
#define STK_CMPLR ((volatile unsigned long*)0xe000f010)

#define STK_CTLR_SWIE  (1u << 31)
#define STK_CTLR_STRE  (1 << 3)
#define STK_CTLR_STCLK (1 << 2)
#define STK_CTLR_STIE  (1 << 1)
#define STK_CTLR_STE   (1 << 0)

#define STK_SR_CNTIF (1 << 0)

static int sysclk_hz = DEVICE_DEFAULT_SYSCLK_HZ;

int arch_timer_init(void)
{
  /* SYSTICK */
  *STK_CNTL = 0;
  *STK_CMPLR = (unsigned long)((sysclk_hz / CONFIG_TICK_HZ) - 1);
  /* start */
  *STK_CTLR = (unsigned long)(STK_CTLR_STRE | STK_CTLR_STCLK |
                              STK_CTLR_STIE | STK_CTLR_STE);
}

/* STATS */

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)*SYSTICK_CVR;

    if (counter == ARCH_COUNTER_SINCE) {
        picoRTOS_cycles_t rvr = (picoRTOS_cycles_t)*SYSTICK_RVR;
        picoRTOS_cycles_t cvr = (picoRTOS_cycles_t)*SYSTICK_CVR;

        /* several cases here */
        if (t > rvr) return rvr + 1;            /* only used on first tick */
        if (cvr > t) return (rvr - cvr) + t;    /* rollover */
        /* normal */
        return t - cvr;
    }

    arch_assert_void(false);
    return 0;
}

/* CLOCKS */

void arch_set_clock_frequency(unsigned long freq)
{
    arch_assert_void(freq != 0);
    sysclk_hz = (int)freq;
}

void arch_delay_us(unsigned long n)
{
    arch_assert_void(n != 0);

    unsigned long ncycles = n * ((unsigned long)sysclk_hz / 1000000ul);

    while (ncycles-- != 0)
        ASM("nop");
}
