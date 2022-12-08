#include "picoRTOS-SMP.h"
#include "picoRTOS_device.h"
#include "picoRTOS-SMP_port.h"

#ifndef CONFIG_DEADLOCK_COUNT
# error Deadlock count is not defined
#endif

/* SYSTICK */
#define SYSTICK_CSR ((volatile unsigned long*)0xe000e010)

/* PWM */
#define PWM_CHn  7
#define PWM_CSR  ((volatile unsigned long*)(ADDR_PWM + (PWM_CHn * 0x14) + 0x0))
#define PWM_DIV  ((volatile unsigned long*)(ADDR_PWM + (PWM_CHn * 0x14) + 0x4))
#define PWM_CTR  ((volatile unsigned long*)(ADDR_PWM + (PWM_CHn * 0x14) + 0x8))
#define PWM_TOP  ((volatile unsigned long*)(ADDR_PWM + (PWM_CHn * 0x14) + 0x10))
#define PWM_INTR ((volatile unsigned long*)(ADDR_PWM + 0xa4))
#define PWM_INTE ((volatile unsigned long*)(ADDR_PWM + 0xa8))

/* NVIC */
#define NVIC_ISER         ((volatile unsigned long*)0xe000e100)
#define NVIC_ICPR         ((volatile unsigned long*)0xe000e280)
#define NVIC_SHPR3        ((volatile unsigned long*)0xe000ed20)

/* SIO */
#define SIO_CPUID     ((volatile unsigned long*)(ADDR_SIO + 0))
#define SIO_FIFO_ST   ((volatile unsigned long*)(ADDR_SIO + 0x50))
#define SIO_FIFO_WR   ((volatile unsigned long*)(ADDR_SIO + 0x54))
#define SIO_FIFO_RD   ((volatile unsigned long*)(ADDR_SIO + 0x58))
#define SIO_SPINLOCK0 ((volatile unsigned long*)(ADDR_SIO + 0x100))
#define SIO_SPINLOCK1 ((volatile unsigned long*)(ADDR_SIO + 0x104))

/* VTOR */
#define VTOR ((volatile unsigned long*)0xe000ed08)

#define SYSCLK_DIV 16ul

/* ASM */
/*@external@*/ extern void arch_PWM_WRAP(void);
/*@external@*/ extern void arch_PENDSV(void);
/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);

void arch_smp_init(void)
{
    /* basic cm0+ init */
    arch_init();

    unsigned long *VTABLE = (unsigned long*)*VTOR;

    /* stop systick */
    *SYSTICK_CSR = 0;
    /* and replace by pwm wrap */
    VTABLE[20] = (unsigned long)arch_PWM_WRAP;

    /* enable PWM irq */
    *NVIC_ICPR = (1ul << IRQ_PWM_WRAP);
    *NVIC_ISER |= (1ul << IRQ_PWM_WRAP);

    /* PWM as system clock */
    unsigned long clk = CONFIG_SYSCLK_HZ / SYSCLK_DIV;

    *PWM_DIV = (SYSCLK_DIV << 4);
    *PWM_TOP = (clk / CONFIG_TICK_HZ) - 1ul;

    *PWM_INTR = (1ul << PWM_CHn);       /* clear interrupt */
    *PWM_INTE = (1ul << PWM_CHn);       /* enable interrupt */
    *PWM_CSR = 0x1ul;                   /* enable pwm */
}

static void __attribute__((naked)) core1_start_first_task(void)
{
    /* enable PWM irq */
    *NVIC_ICPR = (1ul << IRQ_PWM_WRAP);
    *NVIC_ISER |= (1ul << IRQ_PWM_WRAP);

    /* set PENDSV to min priority */
    *NVIC_SHPR3 |= 0xffff0000ul;

    ASM("pop {r0}");
    ASM("b arch_start_first_task");
}

static void arch_flush_rd_fifo(void)
{
    unsigned long dummy;
    int deadlock = CONFIG_DEADLOCK_COUNT;

    while ((*SIO_FIFO_ST & 0x1) != 0 &&
           deadlock-- != 0)
        dummy = *SIO_FIFO_RD;

    picoRTOS_assert_fatal(deadlock != -1);
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

    picoRTOS_assert_fatal(deadlock_tx != -1);

    /* write to fifo */
    *SIO_FIFO_WR = value;
    ASM("sev");

    /* wait for rx ready */
    while ((*SIO_FIFO_ST & 0x1) == 0 &&
           deadlock_rx-- != 0)
        ASM("wfe");

    picoRTOS_assert_fatal(deadlock_rx != -1);

    /* value is echoed back if ok */
    if (*SIO_FIFO_RD != value)
        return -1;

    return 0;
}

void arch_core_init(picoRTOS_core_t core,
                    picoRTOS_stack_t *stack,
                    size_t stack_count,
                    picoRTOS_stack_t *sp)
{
    /* only 1 auxiliary core */
    picoRTOS_assert_fatal(core == (picoRTOS_core_t)1);

    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* prepare core1 stack */
    stack += stack_count - 1;
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

    picoRTOS_assert_fatal(deadlock != -1);
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
    picoRTOS_assert_fatal(loop != -1);
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

picoRTOS_atomic_t arch_test_and_set(picoRTOS_atomic_t *ptr)
{
    return arch_compare_and_swap(ptr, 0, (picoRTOS_atomic_t)1);
}

/*@-redef@*/
picoRTOS_cycles_t arch_counter(void)
{
    return (picoRTOS_cycles_t)*PWM_CTR * SYSCLK_DIV;
}
/*@=redef@*/
