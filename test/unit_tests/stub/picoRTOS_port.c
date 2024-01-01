#include "picoRTOS_port.h"

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
}

void arch_suspend(void)
{
}

void arch_resume(void)
{
}

picoRTOS_stack_t *arch_prepare_stack(picoRTOS_stack_t *stack,
                                     size_t stack_count,
                                     arch_entry_point_fn fn,
                                     void *priv)
{
    return stack;
}

void arch_start_first_task(picoRTOS_stack_t *sp)
{
}

void arch_syscall(syscall_t syscall, void *priv)
{
}

void arch_idle(void *null)
{
}

/* ATOMIC OPS emulation */

picoRTOS_atomic_t arch_test_and_set(picoRTOS_atomic_t *ptr)
{
    picoRTOS_atomic_t ret = (picoRTOS_atomic_t)1;

    arch_suspend();

    if (*ptr == 0) {
        *ptr = (picoRTOS_atomic_t)1;
        ret = 0;
    }

    arch_resume();
    return ret;
}

picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                        picoRTOS_atomic_t old,
                                        picoRTOS_atomic_t val)
{
    arch_suspend();

    if (*var == old) {
        *var = val;
        val = old;
    }

    arch_resume();
    return val;
}

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
    return 0;
}

picoRTOS_cycles_t arch_count_since(picoRTOS_cycles_t y)
{
    return 0;
}

/* INTERRUPTS : unsupported */

void arch_register_interrupt(picoRTOS_irq_t irq __attribute__((unused)),
                             arch_isr_fn fn __attribute__((unused)),
                             void *priv __attribute__((unused)))
{
}

void arch_enable_interrupt(picoRTOS_irq_t irq __attribute__((unused)))
{
}

void arch_disable_interrupt(picoRTOS_irq_t irq __attribute__((unused)))
{
}

/* SMP */

void arch_smp_init(void)
{
}

void arch_core_init(picoRTOS_core_t core,
                    picoRTOS_stack_t *stack,
                    size_t stack_count,
                    picoRTOS_stack_t *sp)
{
}

void arch_spin_lock(void)
{
}

void arch_spin_unlock(void)
{
}

void arch_propagate_tick(void)
{
}

void arch_acknowledge_tick(void)
{
}

void arch_smp_enable_interrupt(picoRTOS_irq_t irq,
                               picoRTOS_mask_t core_mask)
{
}

void arch_smp_disable_interrupt(picoRTOS_irq_t irq,
                                picoRTOS_mask_t core_mask)
{
}

/* CACHES */

void arch_invalidate_dcache(void *addr __attribute__ ((unused)),
                            size_t n __attribute__ ((unused)))
{
}

void arch_flush_dcache(void *addr __attribute__ ((unused)),
                       size_t n __attribute__ ((unused)))
{
}
