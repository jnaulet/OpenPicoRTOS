#include "picoRTOS_device.h"
#include "picoRTOS-SMP_port.h"

#include <stdint.h>
#include <generated/autoconf.h>

/* ASM */
/*@external@*/ extern void arch_core_start(void);
/*@external@*/ extern void arch_SSCIR0(void *priv);

/* STACK */
/*@external@*/ extern picoRTOS_stack_t *arch_core_sp[CONFIG_SMP_CORES - 1];

/* DRIVERS */
/*@external@*/ extern void arch_spinlock_init(void);
/*@external@*/ extern void arch_smp_intc_init(void);
/*@external@*/ extern void arch_core_run(picoRTOS_core_t core);

/* SW LOCK */
static bool aux_core_is_idling
__attribute__((aligned(ARCH_L1_DCACHE_LINESIZE)));

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
    arch_assert_void(core > 0);
    arch_assert_void(core < (picoRTOS_core_t)CONFIG_SMP_CORES);
    arch_assert_void(stack != NULL);
    arch_assert_void(stack_count >= (size_t)ARCH_MIN_STACK_COUNT);
    arch_assert_void(sp != NULL);

    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* prepare core main stack */
    stack += (stack_count - (size_t)2);
    *stack = (picoRTOS_stack_t)sp;

    /* store in exchange ram */
    arch_core_sp[core - 1] = stack;

    /* enable propagated tick interrupt */
    arch_register_interrupt((picoRTOS_irq_t)IRQ_SSCIR0, arch_SSCIR0, NULL);
    arch_smp_enable_interrupt((picoRTOS_irq_t)IRQ_SSCIR0,
                              (picoRTOS_mask_t)1 << core);

    /* reset state machine */
    aux_core_is_idling = false;
    arch_flush_dcache(&aux_core_is_idling, sizeof(aux_core_is_idling));

    /* start */
    arch_core_run(core);

    /* wait until aux core is idling */
    while (!aux_core_is_idling && deadlock-- != 0) {
        arch_invalidate_dcache(&aux_core_is_idling, sizeof(aux_core_is_idling));
        arch_delay_us(1ul);
#if defined(CONFIG_DEBUG_AUX_CORE_STARTUP) && !defined(S_SPLINT_S)
# warning CONFIG_DEBUG_AUX_CORE_STARTUP is defined ! Debug only !
        deadlock = CONFIG_DEADLOCK_COUNT;
#endif
    }

    arch_assert_void(deadlock != -1);
}

void arch_idle(const void *null)
{
    arch_assert_void(null == NULL);

    /* signal we're idling */
    aux_core_is_idling = true;
    arch_flush_dcache(&aux_core_is_idling, sizeof(aux_core_is_idling));

    for (;;)
        ASM("wait");
}
