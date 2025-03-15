#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

#include <generated/autoconf.h>

#define CAT(a, b) a ## b
#define U(x)      CAT(x, u)
#define UL(x)     CAT(x, ul)

#define LIRC_FREQ_HZ 10000u
#define PERIOD       (LIRC_FREQ_HZ / U(CONFIG_TICK_HZ))

#if CONFIG_TICK_HZ > LIRC_FREQ_HZ
# error CONFIG_TICK_HZ is too high !
#endif

#ifndef S_SPLINT_S
__sfr __at(ADDR_WKCON) WKCON;
__sfr __at(ADDR_RWK)   RWK;
__sfr __at(ADDR_EIE1)  C99_EIE1;
#else
static unsigned char WKCON;
static unsigned char RWK;
static unsigned char C99_EIE1;
#endif

#define WKCON_WKTF    (1 << 4)
#define WKCON_WKTR    (1 << 3)
#define WKCON_WKPS_M  0x7u
#define WKCON_WKPS(x) ((x) & WKCON_WKPS_M)

#define C99_EIE1_EWKT (1 << 2)

/* CLOCK */
static unsigned long sysclk_hz = DEVICE_DEFAULT_SYSCLK_HZ;

void arch_timer_init(void)
{
    /* set pre-scaler */
    WKCON &= ~WKCON_WKPS(WKCON_WKPS_M);
#if PERIOD > 4095
    /* ~4-10Khz */
    WKCON |= WKCON_WKPS(3);
    RWK = (unsigned char)(256u - (PERIOD / 64u));
#else
#if PERIOD > 1023
    /* ~1-4Khz */
    WKCON |= WKCON_WKPS(2);
    RWK = (unsigned char)(256u - (PERIOD / 16u));
#else
#if PERIOD > 255
    /* 256-1023Hz */
    WKCON |= WKCON_WKPS(1);
    RWK = (unsigned char)(256u - (PERIOD / 4u));
#else
    /* 1-255Hz */
    RWK = (unsigned char)(256u - PERIOD);
#endif
#endif
#endif

    /* start timer */
    WKCON |= WKCON_WKTR;
    C99_EIE1 |= C99_EIE1_EWKT;
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

void arch_idle(const void *null)
{
    arch_assert_void(null == NULL);

    for (;;)
        ASM(" orl 0x87, #1");
}

void arch_timer_ack(void)
{
    ASM(" anl 0x8f, #0xef");
}
