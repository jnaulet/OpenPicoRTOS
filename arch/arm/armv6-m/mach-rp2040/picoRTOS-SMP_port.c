#include "picoRTOS_device.h"
#include "picoRTOS-SMP_port.h"

#include <generated/autoconf.h>

#ifndef CONFIG_DEADLOCK_COUNT
# error Deadlock count is not defined
#endif

/* SYSTICK */
#define SYSTICK_CSR ((volatile unsigned long*)0xe000e010)
#define SYSTICK_RVR ((volatile unsigned long*)0xe000e014)
#define SYSTICK_CVR ((volatile unsigned long*)0xe000e018)

#define SYSTICK_RVR_VALUE ((CONFIG_SYSCLK_HZ / CONFIG_TICK_HZ) - 1)

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

/* ASM */
/*@external@*/ extern void arch_SIO_PROC1(void*);
/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);

/*@external@*/ extern picoRTOS_stack_t __Stack1Top[];

void arch_smp_init(void)
{
    /* basic cm0+ init */
    arch_init();

    /* register FIFO interrupt beforehand, only used by core#1 */
    arch_register_interrupt((picoRTOS_irq_t)IRQ_SIO_PROC1, arch_SIO_PROC1, NULL);
}

static void __attribute__((naked)) core1_start_first_task(void)
{
    ASM("cpsid i");

    /* SYSTICK */
    *SYSTICK_CSR = 0x4ul;                                   /* stop systick */
    *SYSTICK_CVR = 0;                                       /* reset */
    *SYSTICK_RVR = (unsigned long)SYSTICK_RVR_VALUE;        /* set period */

    /* clear FIFO flags */
    *SIO_FIFO_ST = 0xfful;

    /* set PENDSV to min priority */
    *NVIC_SHPR3 |= 0xffff0000ul;

    /* enable SIO_PROC1 irq */
    *NVIC_ICPR |= (1 << 16);
    *NVIC_ISER |= (1 << 16);

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

    /* prepare core1 stack */
    stack = (picoRTOS_stack_t*)__Stack1Top - 1;
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

void arch_propagate_tick(void)
{
    *SIO_FIFO_WR = 0;
}

void arch_acknowledge_tick(void)
{
    arch_flush_rd_fifo();
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
