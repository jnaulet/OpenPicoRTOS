#include "picoRTOS_device.h"
#include "picoRTOS-SMP_port.h"

#include <stdint.h>
#include <generated/autoconf.h>

/* ASM */
/*@external@*/ extern void arch_core_start(void);

/* STACK */
/*@external@*/ extern picoRTOS_stack_t *__StackTop[];

/* DRIVERS */
/*@external@*/ extern void arch_spinlock_init(void);
/*@external@*/ extern void arch_smp_intc_init(void);
/*@external@*/ extern void arch_aux_core_reset_status(void);
/*@external@*/ extern int arch_aux_core_is_idling(void);
/*@external@*/ extern void arch_core_run(picoRTOS_core_t core);

/* DRIVERS */
/*@external@*/ extern void arch_timer_start(void);
/*@external@*/ extern void arch_timer_stop(void);

void arch_smp_init(void)
{
    arch_init();
    arch_spinlock_init();
}

void arch_core_init(picoRTOS_core_t core, picoRTOS_stack_t *sp)
{
    arch_assert_void(core > 0);
    arch_assert_void(core < (picoRTOS_core_t)CONFIG_CORE_COUNT);
    arch_assert_void(sp != NULL);

    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* prepare core1 main stack */
    picoRTOS_stack_t *stack = __StackTop[core] - 2; /* lr + backchain */

    *--stack = (picoRTOS_stack_t)sp;

    arch_aux_core_reset_status();   /* reset state machine */
    arch_core_run(core);            /* start */

    /* wait until aux core is idling */
    while (arch_aux_core_is_idling() != 0 && deadlock-- != 0) {
        arch_delay_us(10ul);
#if defined(CONFIG_DEBUG_AUX_CORE_STARTUP) && !defined(S_SPLINT_S)
# warning CONFIG_DEBUG_AUX_CORE_STARTUP is defined ! Debug only !
        deadlock = CONFIG_DEADLOCK_COUNT;
#endif
    }

    arch_assert_void(deadlock != -1);
}
