/*
 * ATTINY88 has a specific variant of TC0
 */
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <generated/autoconf.h>

/* systick registers (8bit TC0) */
#define TIMSK0 ((volatile unsigned char*)ADDR_TIMSK0)
#define TCCR0A ((volatile unsigned char*)(ADDR_TC0 + 1))
#define TCNT0  ((volatile unsigned char*)(ADDR_TC0 + 2))
#define OCR0A  ((volatile unsigned char*)(ADDR_TC0 + 3))

static void __ATTRIBUTE_NAKED__ arch_TIMER0_COMPA(/*@unused@*/ void *priv)
{
    /*@i@*/ (void)priv;

    ASM("in r25, 0x2b");    /* GPIOR2 */
    ASM("in r24, 0x2a");    /* GPIOR1 */
    /* execute tick */
    ASM("rcall picoRTOS_tick");
    /* store back task sp */
    ASM("out 0x2b, r25");   /* GPIOR2 */
    ASM("out 0x2a, r24");   /* GPIOR1 */
    ASM("ret");
}

void arch_timer_init(void)
{
#define OCR ((CONFIG_SYSCLK_HZ / CONFIG_TICK_HZ) - 1)

    *TCCR0A = (unsigned char)(1 << 3); /* CTC mode */
    *TCNT0 = (unsigned char)0;
    /* output compare A match interrupt enable */
    *TIMSK0 |= (unsigned char)(1u << 1);

    /* prescaler computation */
#if OCR < 0x100
# define PRESCALER 1
    *TCCR0A |= (unsigned char)1; /* no prescaler */
#elif OCR < 0x800
# define PRESCALER 8
    *TCCR0A |= (unsigned char)2; /* div8 prescaler */
#elif OCR < 0x4000
# define PRESCALER 64
    *TCCR0A |= (unsigned char)3; /* div64 prescaler */
#elif OCR < 0x10000
# define PRESCALER 256
    *TCCR0A |= (unsigned char)4; /* div256 prescaler */
#elif OCR < 0x40000
# define PRESCALER 1024
    *TCCR0A |= (unsigned char)5; /* div1024 prescaler */
#else
# error Cannot compute systick
#endif

    *OCR0A = (unsigned char)(OCR / PRESCALER);

    /* register interrupt */
    arch_register_interrupt((picoRTOS_irq_t)IRQ_TIMER0_COMPA,
                            arch_TIMER0_COMPA, NULL);
}

/* STATS */

picoRTOS_cycles_t arch_counter(/*@unused@*/ arch_counter_t counter,
                               /*@unused@*/ picoRTOS_cycles_t t)
{
    /*@i@*/ (void)counter;
    /*@i@*/ (void)t;
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

    unsigned long ncycles = n * ((unsigned long)CONFIG_SYSCLK_HZ / 1000000ul);

    while (ncycles-- != 0)
        ASM("nop");
}
