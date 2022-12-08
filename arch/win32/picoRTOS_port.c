#include "picoRTOS.h"
#include "picoRTOS_port.h"

#include <windows.h>
#include <winbase.h>
#include <stdbool.h>

#if CONFIG_TICK_HZ > 50
# pragma message("WARNING: Win32 max tick is typically 50hz")
#endif

/* local data structure */
struct thread {
    void *wthread;
};

static bool suspended = false;
static struct thread *current_thread;

static void arch_TICK(void)
{
    struct thread *t = current_thread;

    current_thread = (struct thread*)picoRTOS_tick((picoRTOS_stack_t*)t);
    ResumeThread(current_thread->wthread);
    SuspendThread(t->wthread);
}

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
}

void arch_suspend(void)
{
    suspended = true;
}

void arch_resume(void)
{
    suspended = false;
}

picoRTOS_stack_t *arch_prepare_stack(struct picoRTOS_task *task)
{
    struct thread *t = (struct thread*)task->stack;

    t->wthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)task->fn, task->priv,
                              CREATE_SUSPENDED | STACK_SIZE_PARAM_IS_A_RESERVATION, NULL);

    return (picoRTOS_stack_t*)t;
}

void arch_start_first_task(picoRTOS_stack_t *sp)
{
    current_thread = (struct thread*)sp;
    ResumeThread(current_thread->wthread);

    /* generate non-RT tick */
    for (;;) {
        Sleep(1000 / CONFIG_TICK_HZ);

        if (!suspended)
            arch_TICK();
    }
}

void arch_syscall(picoRTOS_syscall_t syscall, void *priv)
{
    struct thread *t = current_thread;

    current_thread = (struct thread*)picoRTOS_syscall((picoRTOS_stack_t*)t, syscall, priv);
    ResumeThread(current_thread->wthread);

    /* wait */
    SuspendThread(t->wthread);
}

void arch_idle(void *null)
{
    picoRTOS_assert_fatal(null == NULL);

    for (;;)
        Sleep(1000);
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

picoRTOS_cycles_t arch_counter(void)
{
    return 0;
}

/* INTERRUPTS : unsupported */

void arch_register_interrupt(picoRTOS_irq_t irq __attribute__((unused)),
                             picoRTOS_isr_fn fn __attribute__((unused)),
                             void *priv __attribute__((unused)))
{
}

void arch_enable_interrupt(picoRTOS_irq_t irq __attribute__((unused)))
{
}

void arch_disable_interrupt(picoRTOS_irq_t irq __attribute__((unused)))
{
}
