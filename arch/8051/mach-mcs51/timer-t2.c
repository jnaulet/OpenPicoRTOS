#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

#include <generated/autoconf.h>

/* SFRs */
#ifndef S_SPLINT_S
__sfr __at(ADDR_T2CON)  T2CON;
__sfr __at(ADDR_T2MOD)  T2MOD;
__sfr __at(ADDR_RCAP2L) RCAP2L;
__sfr __at(ADDR_RCAP2H) RCAP2H;
__sfr __at(ADDR_TL2)    TL2;
__sfr __at(ADDR_TH2)    TH2;
__sfr __at(ADDR_IE)     IE;
#else
static unsigned char T2CON;
static unsigned char T2MOD;
static unsigned char RCAP2L;
static unsigned char RCAP2H;
static unsigned char TL2;
static unsigned char TH2;
static unsigned char IE;
#endif

#define T2CON_TF2   (1 << 7)
#define T2CON_EXF2  (1 << 6)
#define T2CON_RCLK  (1 << 5)
#define T2CON_TCLK  (1 << 4)
#define T2CON_EXEN2 (1 << 3)
#define T2CON_TR2   (1 << 2)
#define T2CON_CT2   (1 << 1)
#define T2CON_CPRL2 (1 << 0)

#define T2MOD_T20E (1 << 1)
#define T2MOD_DCEN (1 << 0)

#define IE_ET2 (1 << 5)

/* CLOCK */
static unsigned long sysclk_hz = DEVICE_DEFAULT_SYSCLK_HZ;

void arch_timer_init(void)
{
    /* compute period */
    unsigned long period = sysclk_hz / (unsigned long)CONFIG_TICK_HZ;
    unsigned int value = ~(unsigned int)(period / 12ul);

    /* init counters */
    TL2 = (unsigned char)0;
    TH2 = (unsigned char)0;
    RCAP2L = (unsigned char)value;
    RCAP2H = (unsigned char)(value >> 8);

    T2MOD = (unsigned char)T2MOD_DCEN;      /* decrementing counter */
    T2CON = (unsigned char)T2CON_TR2;       /* start, auto-reload */

    /* enable timer2 interrupt */
    IE |= IE_ET2;
}

void arch_set_clock_frequency(unsigned long freq)
{
    arch_assert_void(freq != 0);
    sysclk_hz = freq;
}

void arch_delay_us(unsigned long n)
{
    arch_assert_void(n != 0);

    /* 12 cycles per instruction on mcs51 */
    unsigned long ncycles = (n * (sysclk_hz / 1000000ul)) / 12ul;

    while (ncycles-- != 0)
        ASM(" nop");
}

/* cppcheck-suppress constParameter */
void arch_idle(void *null)
{
    arch_assert_void(null == NULL);

    for (;;)
        ASM(" nop");
}
