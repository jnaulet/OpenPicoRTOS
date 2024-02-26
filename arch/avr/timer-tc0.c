#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <generated/autoconf.h>

/* systick registers (8bit TC0) */
#define TIMSK0 ((volatile unsigned char*)ADDR_TIMSK0)
#define TCCR0A ((volatile unsigned char*)(ADDR_TC0 + 0))
#define TCCR0B ((volatile unsigned char*)(ADDR_TC0 + 1))
#define TCNT0  ((volatile unsigned char*)(ADDR_TC0 + 2))
#define OCR0A  ((volatile unsigned char*)(ADDR_TC0 + 3))

#ifdef CONFIG_SYSCLK_HZ
# undef DEVICE_DEFAULT_SYSCLK_HZ
# define DEVICE_DEFAULT_SYSCLK_HZ CONFIG_SYSCLK_HZ
#endif

static unsigned long sysclk_hz = (unsigned long)DEVICE_DEFAULT_SYSCLK_HZ;

static void __ATTRIBUTE_NAKED__ arch_TIMER0_COMPA(/*@unused@*/ void *priv)
{
    /*@i@*/ (void)priv;

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
    unsigned long prescaler = 0;
    unsigned long ocr = sysclk_hz / (unsigned long)CONFIG_TICK_HZ - 1ul;

    *TCCR0A = (unsigned char)(1 << 1); /* CTC mode */
    *TCCR0B = (unsigned char)0;
    *TCNT0 = (unsigned char)0;
    /* output compare A match interrupt enable */
    *TIMSK0 |= (unsigned char)(1u << 1);

    /* prescaler computation */
    arch_assert(ocr < 0x40000ul, return );

    if (ocr < 0x100ul) {
        prescaler = 0;
        *TCCR0B |= (unsigned char)1; /* no prescaler */
    }else if (ocr < 0x800ul) {
        prescaler = 8ul;
        *TCCR0B |= (unsigned char)2; /* div8 prescaler */
    }else if (ocr < 0x4000ul) {
        prescaler = 64ul;
        *TCCR0B |= (unsigned char)3; /* div64 prescaler */
    }else if (ocr < 0x10000ul) {
        prescaler = 256ul;
        *TCCR0B |= (unsigned char)4; /* div256 prescaler */
    }else{
        prescaler = 1024ul;
        *TCCR0B |= (unsigned char)5; /* div1024 prescaler */
    }

    arch_assert(prescaler != 0, return );
    *OCR0A = (unsigned char)(ocr / prescaler);

    /* register interrupt */
    arch_register_interrupt((picoRTOS_irq_t)IRQ_TIMER0_COMPA,
                            arch_TIMER0_COMPA, NULL);
}

/* STATS */

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)*TCNT0;

    if (counter == ARCH_COUNTER_SINCE) {
        picoRTOS_cycles_t ocr = (picoRTOS_cycles_t)*OCR0A;
        picoRTOS_cycles_t tcnt = (picoRTOS_cycles_t)*TCNT0;

        if (t >= ocr) return ocr;           /* only used on first tick */
        if (tcnt < t) return ocr - t;       /* rollover */
        /* normal */
        return tcnt - t;
    }

    arch_assert_void(false);
    return 0;
}

/* CLOCK */

void arch_set_clock_frequency(unsigned long freq)
{
    arch_assert_void(freq != 0);
    sysclk_hz = freq;
}

void arch_delay_us(unsigned long n)
{
    arch_assert_void(n != 0);

    unsigned long ncycles = n * (sysclk_hz / 1000000ul);

    while (ncycles-- != 0)
        ASM("nop");
}
