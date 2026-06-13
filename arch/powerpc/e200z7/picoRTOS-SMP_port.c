#include "picoRTOS_device.h"
#include "picoRTOS-SMP_port.h"

#include <stdint.h>
#include <generated/autoconf.h>

/* ASM */
/*@external@*/ extern void arch_core_start(void);
/*@external@*/ extern void arch_start_idle_task(picoRTOS_stack_t *sp);

/* STACK */
/*@external@*/ extern picoRTOS_stack_t *__StackTop[];

/* DRIVERS */
/*@external@*/ extern void arch_spinlock_init(void);
/*@external@*/ extern void arch_core_run(picoRTOS_core_t core);
/*@external@*/ extern void arch_timer_start(void);
/*@external@*/ extern void arch_timer_stop(void);

/* SW LOCK */
static bool aux_core_is_idling
__attribute__((aligned(ARCH_L1_DCACHE_LINESIZE)));

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

    /* reset state machine  & start */
    aux_core_is_idling = false;
    arch_core_run(core);

    /* wait until aux core is idling */
    while (!aux_core_is_idling && deadlock-- != 0) {
        arch_delay_us(10ul);
#if defined(CONFIG_DEBUG_AUX_CORE_STARTUP) && !defined(S_SPLINT_S)
# warning CONFIG_DEBUG_AUX_CORE_STARTUP is defined ! Debug only !
        deadlock = CONFIG_DEADLOCK_COUNT;
#endif
    }

    arch_assert_void(deadlock != -1);
}

void arch_start_first_task(picoRTOS_stack_t *sp)
{
    arch_timer_start();
    arch_start_idle_task(sp);
}

void arch_idle(void)
{
    /* signal we're idling */
    aux_core_is_idling = true;

    for (;;)
        ASM("wait");
}
