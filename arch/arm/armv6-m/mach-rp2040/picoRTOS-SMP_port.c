#include "picoRTOS_device.h"
#include "picoRTOS-SMP_port.h"

#include <generated/autoconf.h>

#ifndef CONFIG_DEADLOCK_COUNT
# error Deadlock count is not defined
#endif

#if CONFIG_RP2040_PWM_CHANNEL > 7
# error Wrong PWM channel number
#endif

/* PWM */
#define PWM_CHn  CONFIG_RP2040_PWM_CHANNEL
#define PWM_CSR  ((volatile unsigned long*)(ADDR_PWM + (PWM_CHn * 0x14) + 0x0))
#define PWM_DIV  ((volatile unsigned long*)(ADDR_PWM + (PWM_CHn * 0x14) + 0x4))
#define PWM_CTR  ((volatile unsigned long*)(ADDR_PWM + (PWM_CHn * 0x14) + 0x8))
#define PWM_TOP  ((volatile unsigned long*)(ADDR_PWM + (PWM_CHn * 0x14) + 0x10))
#define PWM_INTR ((volatile unsigned long*)(ADDR_PWM + 0xa4))
#define PWM_INTE ((volatile unsigned long*)(ADDR_PWM + 0xa8))

/* NVIC */
#define NVIC_ISER  ((volatile unsigned long*)0xe000e100)
#define NVIC_ICPR  ((volatile unsigned long*)0xe000e280)
#define NVIC_SHPR3 ((volatile unsigned long*)0xe000ed20)

/* SIO */
#define SIO_CPUID     ((volatile unsigned long*)(ADDR_SIO + 0))
#define SIO_FIFO_ST   ((volatile unsigned long*)(ADDR_SIO + 0x50))
#define SIO_FIFO_WR   ((volatile unsigned long*)(ADDR_SIO + 0x54))
#define SIO_FIFO_RD   ((volatile unsigned long*)(ADDR_SIO + 0x58))
#define SIO_SPINLOCK0 ((volatile unsigned long*)(ADDR_SIO + 0x100))
#define SIO_SPINLOCK1 ((volatile unsigned long*)(ADDR_SIO + 0x104))

/* VTOR */
#define VTOR ((volatile unsigned long*)0xe000ed08)

/* RESET */
#define RESET ((volatile unsigned long*)(ADDR_RESET + 0))

/* ASM */
/*@external@*/ extern void PwmWrap_Handler(void*);
/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);

/*@external@*/ extern picoRTOS_stack_t __Stack1Top[];
/*@external@*/ extern picoRTOS_stack_t __Stack1Bottom[];

/* CLOCK */
static unsigned long sysclk_hz = (unsigned long)DEVICE_DEFAULT_SYSCLK_HZ;

void arch_smp_init(void)
{
    /* PWM as system clock */
    unsigned long pwm_div = 1ul;
    unsigned long pwm_top = (sysclk_hz / (unsigned long)CONFIG_TICK_HZ);

    /* find divider */
    while (pwm_top > 0xfffful)
        pwm_top = (sysclk_hz / (unsigned long)CONFIG_TICK_HZ) / (++pwm_div);

    /* basic cm0+ init */
    arch_init();

    /* un-reset */
    *RESET &= ~(1ul << 14); /* PWM */

    *PWM_DIV = (pwm_div << 4);
    *PWM_TOP = pwm_top - 1ul;

    *PWM_INTR = (1ul << PWM_CHn);       /* clear interrupt */
    *PWM_INTE |= (1ul << PWM_CHn);      /* enable interrupt */

    /* NVIC */
    *NVIC_ICPR = (1ul << IRQ_PWM_WRAP);
    *NVIC_ISER |= (1ul << IRQ_PWM_WRAP);
}

/* STATS */

picoRTOS_cycles_t arch_counter_opt(arch_counter_t counter, picoRTOS_cycles_t t)
{
    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)*PWM_CTR;

    if (counter == ARCH_COUNTER_SINCE) {
        picoRTOS_cycles_t top = (picoRTOS_cycles_t)*PWM_TOP;
        picoRTOS_cycles_t ctr = (picoRTOS_cycles_t)*PWM_CTR;

        /* several cases here */
        if (t > top) return top + 1;            /* only used on first tick */
        if (ctr < t) return (top - t) + ctr;    /* rollover */
        /* normal */
        return ctr - t;
    }

    arch_assert_void(false);
    return 0;
}

/* CLOCKS */

void arch_set_clock_frequency(unsigned long freq)
{
    arch_assert_void(freq != 0);
    sysclk_hz = freq;
}

void arch_delay_us(unsigned long n)
{
    arch_assert_void(n != 0);

    unsigned long ncycles = n * ((unsigned long)sysclk_hz / 1000000ul);

    while (ncycles-- != 0)
        ASM("nop");
}


static void __attribute__((naked)) core1_start_first_task(void)
{
    ASM("cpsid i");

    /* clear FIFO flags */
    *SIO_FIFO_ST = 0xfful;

    /* set PENDSV to min priority */
    *NVIC_SHPR3 |= 0xffff0000ul;

    /* enable PwmWrap irq */
    *NVIC_ICPR = (1ul << IRQ_PWM_WRAP);
    *NVIC_ISER |= (1ul << IRQ_PWM_WRAP);

    ASM("pop {r0}");
    ASM("ldr r1, =arch_start_first_task");
    ASM("bx r1");
}

static void arch_flush_rd_fifo(void)
{
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while ((*SIO_FIFO_ST & 0x1) != 0 &&
           deadlock-- != 0)
        /*@i@*/ (void)*SIO_FIFO_RD;

    arch_assert_void(deadlock != -1);
    ASM("sev");
}

static int arch_xfer_to_core1(unsigned long value)
{
    int deadlock_tx = CONFIG_DEADLOCK_COUNT;
    int deadlock_rx = CONFIG_DEADLOCK_COUNT;

    /* wait for tx fifo */
    while ((*SIO_FIFO_ST & 0x2) == 0 &&
           deadlock_tx-- != 0) {
    }

    arch_assert(deadlock_tx != -1, return -1);

    /* write to fifo */
    *SIO_FIFO_WR = value;
    ASM("sev");

    /* wait for rx ready */
    while ((*SIO_FIFO_ST & 0x1) == 0 &&
           deadlock_rx-- != 0)
        ASM("wfe");

    arch_assert(deadlock_rx != -1, return -1);

    /* value is echoed back if ok */
    if (*SIO_FIFO_RD != value)
        return -1;

    return 0;
}

void arch_core_init(picoRTOS_core_t core,
                    picoRTOS_stack_t *stack,
                    /*@unused@*/
                    size_t stack_count __attribute__((unused)),
                    picoRTOS_stack_t *sp)
{
    /* only 1 auxiliary core */
    arch_assert_void(core == (picoRTOS_core_t)1);

    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* setup core1 init protection */
    arch_spin_lock();

    /* prepare core1 stack.
     * this complex computation is to avoid compiler warnings
     */
    picoRTOS_uintptr_t Stack1Top = (picoRTOS_uintptr_t)__Stack1Top;
    picoRTOS_uintptr_t Stack1Bottom = (picoRTOS_uintptr_t)__Stack1Bottom;
    picoRTOS_uintptr_t bias = ((Stack1Top - Stack1Bottom) >> 2);

    stack = (picoRTOS_stack_t*)__Stack1Bottom + bias - 1;
    *stack = (picoRTOS_stack_t)sp;

    /* send sequence */
    while (deadlock-- != 0) {
        /* always flush before sending a 0 */
        arch_flush_rd_fifo();
        if (arch_xfer_to_core1((unsigned long)0) < 0) continue;
        /* always flush before sending a 0 */
        arch_flush_rd_fifo();
        if (arch_xfer_to_core1((unsigned long)0) < 0) continue;
        /* normal sequence */
        if (arch_xfer_to_core1((unsigned long)1) < 0) continue;
        if (arch_xfer_to_core1((unsigned long)*VTOR) < 0) continue;
        if (arch_xfer_to_core1((unsigned long)stack) < 0) continue;
        if (arch_xfer_to_core1((unsigned long)core1_start_first_task) < 0) continue;
        /* sequence passed */
        break;
    }

    arch_assert_void(deadlock != -1);

    /* wait until core1 is idling */
    arch_spin_lock();
    arch_spin_unlock();
}

void arch_idle(void)
{
    /* unlock core0 init */
    if (arch_core() != 0)
        arch_spin_unlock();

    for (;;)
        ASM("wfe");
}

picoRTOS_core_t arch_core(void)
{
    return (picoRTOS_core_t)*SIO_CPUID;
}

void arch_spin_lock(void)
{
    int loop = CONFIG_DEADLOCK_COUNT;

    /* spins full throttle */
    while (*SIO_SPINLOCK0 == 0 && loop-- != 0) {
    }

    /* potential deadlock */
    arch_assert_void(loop != -1);
}

void arch_spin_unlock(void)
{
    *SIO_SPINLOCK0 = 1ul;
}

/* ATOMIC ops */

picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                        picoRTOS_atomic_t old,
                                        picoRTOS_atomic_t val)
{
    picoRTOS_atomic_t res = val;

    if (*SIO_SPINLOCK1 != 0) {
        res = *var;
        if (res == old)
            *var = val;

        *SIO_SPINLOCK1 = 1ul;
    }

    return res;
}

/* INTERRUPT MANAGEMENT */

void arch_smp_enable_interrupt(/*@unused@*/ picoRTOS_irq_t irq __attribute__((unused)),
                               /*@unused@*/ picoRTOS_mask_t core_mask __attribute__((unused)))
{
    /* unsupported on RP2040 */
    arch_break();
}

void arch_smp_disable_interrupt(/*@unused@*/ picoRTOS_irq_t irq __attribute__((unused)),
                                /*@unused@*/ picoRTOS_mask_t core_mask __attribute__((unused)))
{
    /* unsupported on RP2040 */
    arch_break();
}
