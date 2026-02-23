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

#define T2CON_TF2    (1 << 7)
#define T2CON_EXF2   (1 << 6)
#define T2CON_RCLK   (1 << 5)
#define T2CON_TCLK   (1 << 4)
#define T2CON_EXEN2  (1 << 3)
#define T2CON_TR2    (1 << 2)
#define T2CON_C_T2   (1 << 1)
#define T2CON_CP_RL2 (1 << 0)

#define T2MOD_bTMR_CLK    (1 << 7)
#define T2MOD_bT2_CLK     (1 << 6)
#define T2MOD_bT1_CLK     (1 << 5)
#define T2MOD_bT0_CLK     (1 << 4)
#define T2MOD_bT2_CAP_M1  (1 << 3)
#define T2MOD_bT2_CAP_M0  (1 << 2)
#define T2MOD_T2OE        (1 << 1)
#define T2MOD_bT2_CAP1_EN (1 << 0)

#define IE_ET2 (1 << IRQ_TMR2)

/* CLOCK */
static unsigned long sysclk_hz = DEVICE_DEFAULT_SYSCLK_HZ;

void arch_timer_init(void)
{
    /* compute period */
    unsigned int value;
    unsigned long period = sysclk_hz / (unsigned long)CONFIG_TICK_HZ;

    /* disable */
    T2CON = (unsigned char)0;

    if (period > 0xfffful) {
        value = 0xffffu - (unsigned int)(period / 12ul) + 1u;
        T2MOD &= ~(unsigned char)T2MOD_bT2_CLK;
    }else{
        value = 0xffffu - (unsigned int)(period / 4ul) + 1u;
        T2MOD &= ~(unsigned char)T2MOD_bTMR_CLK;
        T2MOD |= (unsigned char)T2MOD_bT2_CLK;
    }

    /* init counters */
    TL2 = (unsigned char)value;
    TH2 = (unsigned char)(value >> 8);
    RCAP2L = (unsigned char)value;
    RCAP2H = (unsigned char)(value >> 8);

    /* start, auto-reload */
    T2CON = (unsigned char)T2CON_TR2;

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

    /* 1 cycle per instruction on ch552 */
    unsigned long ncycles = (n * (sysclk_hz / 1000000ul));

    while (ncycles-- != 0)
        ASM(" nop");
}

void arch_idle(void)
{
    for (;;)
        ASM(" nop");
}

void arch_timer_ack(void)
{
    ASM(" anl t2con, #0x7f");
}
