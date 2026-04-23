#include "picoRTOS_device.h"
#include "picoRTOS_port.h"

#include <generated/autoconf.h>

struct RTC {
    volatile uint8_t SC1;
    volatile uint8_t SC2;
    volatile uint8_t MODH;
    volatile uint8_t MODL;
    volatile uint8_t CNTH;
    volatile uint8_t CNTL;
};

#define SC1_RTIF (1 << 7)
#define SC1_RTIE (1 << 6)
#define SC1_RTCO (1 << 4)

#define SC2_RTCLKS_M  0x3u
#define SC2_RTCLKS(x) (((x) & SC2_RTCLKS_M) << 6)
#define SC2_RTCPS_M   0x7u
#define SC2_RTCPS(x)  ((x) & SC2_RTCPS_M)

static struct RTC *RTC = (struct RTC*)ADDR_RTC;
static unsigned long sysclk_hz = (unsigned long)DEVICE_DEFAULT_SYSCLK_HZ;

static void arch_timer_isr(/*@unused@*/ void *priv)
{
    /*@i@*/ (void)priv;
    RTC->SC1 |= SC1_RTIF;

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
    unsigned long mod = (sysclk_hz / (unsigned long)CONFIG_TICK_HZ) + 1ul;

    /* clock setup */
    RTC->MODH = (uint8_t)(mod >> 8);
    RTC->MODL = (uint8_t)mod;
    RTC->SC2 = (uint8_t)(SC2_RTCLKS(2) |    /* bus clock */
                         SC2_RTCPS(1));     /* prescaler */

    /* reset & enable */
    arch_register_interrupt((picoRTOS_irq_t)IRQ_RTC, arch_timer_isr, NULL);
    RTC->CNTL = 0;
    RTC->CNTH = 0;
    RTC->SC1 = (uint8_t)(SC1_RTIF | SC1_RTIE);
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

picoRTOS_cycles_t arch_counter(/*@unused@*/ arch_counter_t counter,
                               /*@unused@*/ picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    /*@i@*/ (void)counter;
    /*@i@*/ (void)t;
    return 0;
}
