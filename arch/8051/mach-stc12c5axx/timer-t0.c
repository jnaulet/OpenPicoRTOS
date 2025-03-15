#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

#include <generated/autoconf.h>

/* SFRs */
#ifndef S_SPLINT_S
__sfr __at(ADDR_CLK_DIV) CLK_DIV;
__sfr __at(ADDR_TCON)    TCON;
__sfr __at(ADDR_TMOD)    TMOD;
__sfr __at(ADDR_AUXR)    AUXR;
__sfr __at(ADDR_TL0)     TL0;
__sfr __at(ADDR_TH0)     TH0;
__sfr __at(ADDR_IE)      IE;
#else
static unsigned char CLK_DIV;
static unsigned char TCON;
static unsigned char TMOD;
static unsigned char AUXR;
static unsigned char TL0;
static unsigned char TH0;
static unsigned char IE;
#endif

#define CLK_DIV_CLKS_M  0x7u
#define CLK_DIV_CLKS(x) ((x) & CLK_DIV_CLKS_M)

#define TCON_TF0 (1 << 5)
#define TCON_TR0 (1 << 4)

#define TMOD_GATE0 (1 << 3)
#define TMOD_CT0   (1 << 2)
#define TMOD_M0_M  0x3u
#define TMOD_M0(x) ((x) & TMOD_M0_M)

#define AUXR_T0X12 (1 << 7)

#define IE_ET0 (1 << 1)

/* CLOCK */

#if (CONFIG_SYSCLK_DIV == 1)
# define CLKS 0
#elif (CONFIG_SYSCLK_DIV == 2)
# define CLKS 1
#elif (CONFIG_SYSCLK_DIV == 4)
# define CLKS 2
#elif (CONFIG_SYSCLK_DIV == 8)
# define CLKS 3
#elif (CONFIG_SYSCLK_DIV == 16)
# define CLKS 4
#elif (CONFIG_SYSCLK_DIV == 32)
# define CLKS 5
#elif (CONFIG_SYSCLK_DIV == 64)
# define CLKS 6
#elif (CONFIG_SYSCLK_DIV == 128)
# define CLKS 7
#else
# error SYSCLK_DIV has erroneous value, check config
#endif

#define INTERNAL_SYSCLK_HZ (CONFIG_SYSCLK_HZ / CONFIG_SYSCLK_DIV)
#define LATENCY            8ul
#define T0                 ~(((INTERNAL_SYSCLK_HZ / CONFIG_TICK_HZ) / 12ul) - LATENCY)

void arch_timer_init(void)
{
    /* init divider */
    CLK_DIV = (unsigned char)CLK_DIV_CLKS(CLKS);

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
    unsigned long ncycles = (n * (CONFIG_SYSCLK_HZ / 1000000ul));

    while (ncycles-- != 0)
        ASM(" nop");
}

void arch_idle(const void *null)
{
    arch_assert_void(null == NULL);

    for (;;)
        ASM(" orl pcon, #1");
}

void arch_timer_ack(void)
{
    TL0 = (unsigned char)T0;
    TH0 = (unsigned char)(T0 >> 8);
}
