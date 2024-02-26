#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <generated/autoconf.h>

/* RTC registers */
#define RTC_CTRLA   ((volatile unsigned char*)(ADDR_RTC + 0x0))
#define RTC_INTCTRL ((volatile unsigned char*)(ADDR_RTC + 0x2))
#define RTC_CLKSEL  ((volatile unsigned char*)(ADDR_RTC + 0x7))
#define RTC_CNT     ((volatile unsigned int*)(ADDR_RTC + 0x8))
#define RTC_PER     ((volatile unsigned int*)(ADDR_RTC + 0xa))

/* CLOCK */
#define RTC_CLK_HZ 32768

static void __ATTRIBUTE_NAKED__ arch_RTC_CNT(/*@unused@*/ void *priv)
{
    /*@i@*/ (void)priv;

    /* ack RTC OVF */
    ASM("ldi r24, 0x1");
    ASM("sts ADDR_RTC + 0x3, r24");
    /* switch context */
    ASM("in r25, 0x2b");    /* GPIOR2 */
    ASM("in r24, 0x2a");    /* GPIOR1 */
    /* execute tick */
    ASM("call picoRTOS_tick");
    /* store back task sp */
    ASM("out 0x2b, r25");   /* GPIOR2 */
    ASM("out 0x2a, r24");   /* GPIOR1 */
    ASM("ret");
}

void arch_timer_init(void)
{
    *RTC_INTCTRL = (unsigned char)0x1;  /* OVF */
    *RTC_CLKSEL = (unsigned char)0;     /* INT32K */
    *RTC_CNT = (unsigned char)0;

    /* set period & start */
    *RTC_PER = (unsigned int)((RTC_CLK_HZ / CONFIG_TICK_HZ) - 1);
    *RTC_CTRLA = (unsigned char)0x1;

    /* register interrupt */
    arch_register_interrupt((picoRTOS_irq_t)IRQ_RTC_CNT,
                            arch_RTC_CNT, NULL);
}

/* STATS */

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)*RTC_CNT;

    if (counter == ARCH_COUNTER_SINCE) {
        picoRTOS_cycles_t per = (picoRTOS_cycles_t)*RTC_PER;
        picoRTOS_cycles_t cnt = (picoRTOS_cycles_t)*RTC_CNT;

        if (t >= per) return per;       /* 1st tick */
        if (cnt < t) return per - t;    /* rollover */
        /* normal */
        return cnt - t;
    }

    arch_assert_void(false);
    return 0;
}

/* CLOCK */

void arch_set_clock_frequency(unsigned long freq)
{
    arch_assert_void(freq != 0);
}

void arch_delay_us(unsigned long n)
{
    arch_assert_void(n != 0);

    unsigned long ncycles = n * (unsigned long)RTC_CLK_HZ / 1000000ul;

    while (ncycles-- != 0)
        ASM("nop");
}
