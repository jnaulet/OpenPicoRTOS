#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

#include <generated/autoconf.h>

/* SFRs */
#ifndef S_SPLINT_S
__sfr __at(ADDR_TCON) TCON;
__sfr __at(ADDR_TMOD) TMOD;
__sfr __at(ADDR_AUXR) AUXR;
__sfr __at(ADDR_TL0)  TL0;
__sfr __at(ADDR_TH0)  TH0;
__sfr __at(ADDR_IE)   IE;
#else
static unsigned char TCON;
static unsigned char TMOD;
static unsigned char AUXR;
static unsigned char TL0;
static unsigned char TH0;
static unsigned char IE;
#endif

#define TCON_TF0 (1 << 5)
#define TCON_TR0 (1 << 4)

#define TMOD_GATE0 (1 << 3)
#define TMOD_CT0   (1 << 2)
#define TMOD_M0_M  0x3u
#define TMOD_M0(x) ((x) & TMOD_M0_M)

#define AUXR_T0X12 (1 << 7)

#define IE_ET0 (1 << 1)

/* CLOCK */
#define T0 ~((DEVICE_DEFAULT_SYSCLK_HZ / CONFIG_TICK_HZ) / 12ul)

void arch_reload_timer(void)
{
    TL0 += (unsigned char)T0;
    TH0 += (unsigned char)(T0 >> 8);
}

void arch_timer_init(void)
{
    /* init counters */
    TL0 = (unsigned char)T0;
    TH0 = (unsigned char)(T0 >> 8);

    AUXR &= ~AUXR_T0X12;    /* count = sysclk */
    IE |= IE_ET0;           /* enable interrupt */

    TMOD &= ~(TMOD_GATE0 | TMOD_CT0 | TMOD_M0(TMOD_M0_M));
    TMOD |= TMOD_M0(1);
    TCON |= TCON_TR0;       /* start */
}

void arch_set_clock_frequency(unsigned long freq)
{
    arch_assert_void(freq != 0);
    /* FIXME */
}

void arch_delay_us(unsigned long n)
{
    arch_assert_void(n != 0);

    /* 1 cycle (1T) per instruction on stc12 */
    unsigned long ncycles = (n * (DEVICE_DEFAULT_SYSCLK_HZ / 1000000ul));

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
