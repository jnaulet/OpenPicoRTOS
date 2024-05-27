#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

#include <generated/autoconf.h>

#ifndef S_SPLINT_S
__sfr __at(ADDR_T2CON)  T2CON;
__sfr __at(ADDR_T2MOD)  T2MOD;
__sfr __at(ADDR_RCMP2L) RCMP2L;
__sfr __at(ADDR_RCMP2H) RCMP2H;
__sfr __at(ADDR_TL2)    TL2;
__sfr __at(ADDR_TH2)    TH2;
__sfr __at(ADDR_EIE)    C99_EIE;
__sfr __at(ADDR_EIP)    C99_EIP;
__sfr __at(ADDR_EIP)    C99_EIPH;
#else
static unsigned char T2CON;
static unsigned char T2MOD;
static unsigned char RCMP2L;
static unsigned char RCMP2H;
static unsigned char TL2;
static unsigned char TH2;
static unsigned char C99_EIE;
static unsigned char C99_EIP;
static unsigned char C99_EIPH;
#endif

#define T2CON_TF2   (1 << 7)
#define T2CON_EXF2  (1 << 6)
#define T2CON_RCLK  (1 << 5)
#define T2CON_TCLK  (1 << 4)
#define T2CON_EXEN2 (1 << 3)
#define T2CON_TR2   (1 << 2)
#define T2CON_CT2   (1 << 1)
#define T2CON_CMRL2 (1 << 0)

#define T2MOD_LDEN     (1 << 7)
#define T2MOD_T2DIV_M  0x7u
#define T2MOD_T2DIV(x) (((x) & T2MOD_T2DIV_M) << 4)
#define T2MOD_CAPCR    (1 << 3)
#define T2MOD_CMPCR    (1 << 2)
#define T2MOD_LDTS_M   0x3u
#define T2MOD_LDTS(x)  ((x) & T2MOD_LDTS_M)

#define C99_EIE_ET2 (1 << 7)

#define C99_EIP_PT2 (1 << 7)

#define C99_EIPH_PT2H (1 << 7)

/* CLOCK
 * Can't init statically due to a bug in SDCC v4.2.6 by Nuvoton
 */
static unsigned long sysclk_hz;

void arch_timer_init(void)
{
    unsigned long period;
    unsigned int value;

    if (sysclk_hz == 0)
        sysclk_hz = DEVICE_DEFAULT_SYSCLK_HZ;

    period = sysclk_hz / (unsigned long)CONFIG_TICK_HZ;
    value = (unsigned int)(period / 16ul);

    /* init counters */
    TL2 = (unsigned char)0;
    TH2 = (unsigned char)0;

    RCMP2L = (unsigned char)value;
    RCMP2H = (unsigned char)(value >> 8);

    T2MOD = (unsigned char)(T2MOD_T2DIV(2) | T2MOD_CMPCR);  /* div by 16 & auto-clear */
    T2CON = (unsigned char)(T2CON_TR2 | T2CON_CMRL2);       /* start, compare mode */

    /* high priority */
    C99_EIP = (unsigned char)C99_EIP_PT2;
    C99_EIPH = (unsigned char)C99_EIPH_PT2H;

    /* enable timer2 interrupt */
    C99_EIE |= C99_EIE_ET2;
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

/* cppcheck-suppress constParameter */
void arch_idle(void *null)
{
    arch_assert_void(null == NULL);

    for (;;)
        ASM(" nop");
}

void arch_timer_ack(void)
{
    ASM(" anl t2con, #0x7f");
}
