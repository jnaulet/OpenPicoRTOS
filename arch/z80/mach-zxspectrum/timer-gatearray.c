#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

#include <generated/autoconf.h>

/* CLOCK */

void arch_timer_init(void)
{
}

void arch_set_clock_frequency(unsigned long freq)
{
    /* nothing to do */
    /*@i@*/ (void)freq;
}

void arch_delay_us(unsigned long n)
{
    arch_assert_void(n != 0);

    /* ~16 cycles per loop on zxspectrum */
    unsigned long ncycles = (n * (DEVICE_DEFAULT_SYSCLK_HZ / 1000000ul)) >> 4;

    while (ncycles-- != 0)
        ASM(" nop");
}
