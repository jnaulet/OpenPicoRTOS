#include "picoRTOS-SMP.h"
#include "picoRTOS_device.h"
#include "picoRTOS-SMP_port.h"

#include <stdint.h>

/* ASM */
/*@external@*/ extern void arch_core_start(void);
/*@external@*/ extern void arch_SSCIR0(void *priv);

/* STACK */
/*@external@*/ extern picoRTOS_stack_t *arch_core_sp[CONFIG_SMP_CORES - 1];

/* DRIVERS */
/*@external@*/ extern void arch_spinlock_init(void);
/*@external@*/ extern void arch_smp_intc_init(void);
/*@external@*/ extern void arch_core_run(picoRTOS_core_t core);

void arch_smp_init(void)
{
    arch_init();
    arch_spinlock_init();
}

void arch_core_init(picoRTOS_core_t core,
                    picoRTOS_stack_t *stack,
                    size_t stack_count,
                    picoRTOS_stack_t *sp)
{
    if (!picoRTOS_assert_fatal(core > 0)) return;
    if (!picoRTOS_assert_fatal(core < (picoRTOS_core_t)CONFIG_SMP_CORES)) return;
    if (!picoRTOS_assert_fatal(stack != NULL)) return;
    if (!picoRTOS_assert_fatal(stack_count >= (size_t)ARCH_MIN_STACK_COUNT)) return;
    if (!picoRTOS_assert_fatal(sp != NULL)) return;

    /* prepare core main stack */
    stack += (stack_count - (size_t)2);
    *stack = (picoRTOS_stack_t)sp;

    /* store in exchange ram */
    arch_core_sp[core - 1] = stack;

    /* enable propagated tick interrupt */
    arch_register_interrupt((picoRTOS_irq_t)IRQ_SSCIR0, arch_SSCIR0, NULL);
    arch_smp_enable_interrupt((picoRTOS_irq_t)IRQ_SSCIR0,
                              (picoRTOS_mask_t)1 << core);

    /* start */
    arch_core_run(core);
}
