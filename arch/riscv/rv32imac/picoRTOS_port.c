#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

/*@external@*/ extern /*@temp@*/
picoRTOS_stack_t *arch_save_first_context(picoRTOS_stack_t *sp,
                                          picoRTOS_task_fn_t fn,
                                          void *priv);

/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_syscall(picoRTOS_syscall_t syscall, void *priv);
/*@external@*/ extern picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                                              picoRTOS_atomic_t old,
                                                              picoRTOS_atomic_t val);

/* stats */
/*@external@*/ extern picoRTOS_cycles_t arch_counter(void);

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    arch_enable_interrupt((picoRTOS_irq_t)IRQ_TMR);
}

void arch_suspend(void)
{
    /* disable interrupts */
    ASM("csrci mstatus, 0x8");     /* ~MIE */
}

void arch_resume(void)
{
    /* enable interrupts */
    ASM("csrsi mstatus, 0x8");     /* MIE */
}

picoRTOS_stack_t *arch_prepare_stack(struct picoRTOS_task *task)
{
    /* RISC-Vs have a decrementing stack */
    return arch_save_first_context(task->stack + task->stack_count,
                                   task->fn, task->priv);
}

/* cppcheck-suppress constParameter */
void __attribute__((weak)) arch_idle(void *null)
{
    if (!picoRTOS_assert_fatal(null == NULL)) return;

    for (;;)
        ASM("wfi");
}

/* ATOMIC OPS */

picoRTOS_atomic_t arch_test_and_set(picoRTOS_atomic_t *ptr)
{
    return arch_compare_and_swap(ptr, 0, (picoRTOS_atomic_t)1);
}
