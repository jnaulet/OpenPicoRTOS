#include "picoRTOS.h"
#include "picoRTOS_port.h"

#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <stddef.h>
#include <sys/time.h>

/* local data structure */
struct thread {
    pthread_t pthread;
    picoRTOS_task_fn_t fn;
    /*@temp@*/ void *priv;
    /* signals */
    int sig;
    sigset_t sigset;
};

/*@exposed@*/ static struct thread *current_thread;

static void wait_for_sigusr1(struct thread *t)
{
    (void)sigaddset(&t->sigset, SIGUSR1);
    sigwait(&t->sigset, &t->sig);
}

/*@null@*/ static void *pthread_start_routine(void *priv)
{
    struct thread *t = priv;

    /* wait for signal */
    wait_for_sigusr1(t);

    /* run task */
    t->fn(t->priv);

    return NULL;
}

static void arch_TICK(int sig)
{
    struct thread *t = current_thread;

    picoRTOS_assert_fatal(sig == SIGALRM);

    current_thread = (struct thread*)picoRTOS_tick((picoRTOS_stack_t*)t);
    pthread_kill(current_thread->pthread, SIGUSR1);

    /* wait for signal */
    wait_for_sigusr1(t);
}

static void itimer_init(void)
{
    int res;
    struct itimerval itimer;
    struct sigaction sig_tick;

    /* register tick function */
    sig_tick.sa_flags = 0;
    sig_tick.sa_handler = arch_TICK;
    (void)sigfillset(&sig_tick.sa_mask);

    res = sigaction(SIGALRM, &sig_tick, NULL);
    picoRTOS_assert_fatal(res == 0);

    /* init itimer struct */
    res = getitimer(ITIMER_REAL, &itimer);
    picoRTOS_assert_fatal(res == 0);

    /* set interval */
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 1000000 / CONFIG_TICK_HZ;

    /* set current value */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 1000000 / CONFIG_TICK_HZ;

    /* set tick */
    res = setitimer(ITIMER_REAL, &itimer, NULL);
    picoRTOS_assert_fatal(res == 0);
}

static void arch_sigusr1(int sig)
{
    picoRTOS_assert_fatal(sig == SIGUSR1);
}

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    int res;
    sigset_t set;
    struct sigaction act;

    /* TICK */
    itimer_init();

    /* SIGNALS */
    (void)sigemptyset(&set);
    (void)sigaddset(&set, SIGUSR1);
    res = pthread_sigmask(SIG_BLOCK, &set, NULL);
    picoRTOS_assert_fatal(res == 0);

    /* pthread_sigmask should be enough to avoid
     * the main thread to receive the signal but for some
     * reason my chromebook refuses to understand it, so
     * here's the fix... */
    (void)sigfillset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = arch_sigusr1;
    (void)sigaction(SIGUSR1, &act, NULL);
}

void arch_suspend(void)
{
    sigset_t set;

    (void)sigemptyset(&set);
    (void)sigaddset(&set, SIGALRM);
    (void)pthread_sigmask(SIG_BLOCK, &set, NULL);
}

void arch_resume(void)
{
    sigset_t set;

    (void)sigemptyset(&set);
    (void)sigaddset(&set, SIGALRM);
    (void)pthread_sigmask(SIG_UNBLOCK, &set, NULL);
}

picoRTOS_stack_t *arch_prepare_stack(struct picoRTOS_task *task)
{
    int res;
    struct thread *t = (struct thread*)task->stack;

    t->fn = task->fn;
    t->priv = task->priv;
    (void)sigemptyset(&t->sigset);

    res = pthread_create(&t->pthread, NULL, pthread_start_routine, t);
    picoRTOS_assert_fatal(res == 0);

    return (picoRTOS_stack_t*)t;
}

void arch_start_first_task(picoRTOS_stack_t *sp)
{
    /* unlock */
    current_thread = (struct thread*)sp;
    pthread_kill(current_thread->pthread, SIGUSR1);

    /* wait forever */
    (void)pause();
}

void arch_syscall(picoRTOS_syscall_t syscall, void *priv)
{
    struct thread *t = current_thread;

    current_thread = (struct thread*)picoRTOS_syscall((picoRTOS_stack_t*)t, syscall, priv);
    pthread_kill(current_thread->pthread, SIGUSR1);

    /* wait */
    wait_for_sigusr1(t);
}

void __attribute__((weak)) arch_idle(void *null)
{
    picoRTOS_assert_fatal(null == NULL);

    for (;;)
        (void)pause();
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
