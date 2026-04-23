#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

#include <generated/autoconf.h>

struct MTIMx {
    volatile uint8_t SC;
    volatile uint8_t CLK;
    volatile uint8_t CNT;
    volatile uint8_t MOD;
};

#define SC_TOF  (1 << 7)
#define SC_TOIE (1 << 6)
#define SC_TRST (1 << 5)
#define SC_TSTP (1 << 4)

#define CLK_CLKS_M  0x3u
#define CLK_CLKS(x) (((x) & CLK_CLKS_M) << 4)
#define CLK_PS_M    0xfu
#define CLK_PS(x)   ((x) & CLK_PS_M)

static struct MTIMx *MTIMn = (struct MTIMx*)ADDR_MTIM1;
static unsigned long sysclk_hz = (unsigned long)DEVICE_DEFAULT_SYSCLK_HZ;

static void arch_timer_isr(/*@unused@*/ void *priv)
{
    /*@i@*/ (void)priv;
    MTIMn->SC &= ~SC_TOF;

    ASM("ldhx _bhx");
    ASM("txa");
    ASM("pshh");
    ASM("pulx");
    ASM("jsr _picoRTOS_tick");
    ASM("pshx");
    ASM("pulh");
    ASM("tax");
    ASM("sthx _bhx");
}

void arch_timer_init(void)
{
    uint8_t ps;
    unsigned long mod = (sysclk_hz / (unsigned long)CONFIG_TICK_HZ) + 1ul;

    for (ps = 0; ps <= (uint8_t)8; ps++) {
        if (mod <= 255ul) {
            MTIMn->CLK = (uint8_t)CLK_PS(ps);
            MTIMn->MOD = (uint8_t)mod;
            /* interrupt register, enable & start */
            arch_register_interrupt((picoRTOS_irq_t)IRQ_MTIM1, arch_timer_isr, NULL);
            MTIMn->SC = (uint8_t)(SC_TOIE | SC_TRST);
            return;
        }
        /* next */
        mod >>= 1;
    }

    /* break */
    arch_break();
}

void arch_set_clock_frequency(unsigned long freq)
{
    arch_assert_void(freq != 0);
    sysclk_hz = freq;
}

void arch_delay_us(unsigned long n)
{
    arch_assert_void(n != 0);

    /*
     * 32-bit computation is problematic here,
     * we take the worst case scenario (20Mhz bus)
     */
    while (n-- != 0) {
        ASM(".rept 20");
        ASM("nop");
        ASM(".endm");
    }
}

/* STATS */

picoRTOS_cycles_t arch_counter(arch_counter_t counter,
                               picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)MTIMn->CNT;

    if (counter == ARCH_COUNTER_SINCE) {
        picoRTOS_cycles_t mod = (picoRTOS_cycles_t)MTIMn->MOD;
        picoRTOS_cycles_t cnt = (picoRTOS_cycles_t)MTIMn->CNT;

        if (t >= mod) return mod;           /* only used on first tick */
        if (cnt < t) return mod - t;        /* rollover */
        /* normal */
        return cnt - t;
    }

    arch_assert_void(false);
    return 0;
}
