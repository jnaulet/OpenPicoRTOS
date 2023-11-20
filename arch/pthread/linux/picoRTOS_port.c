#include "picoRTOS_port.h"
#include <generated/autoconf.h>

#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <stddef.h>
#include <sys/time.h>

/* local data structure */
struct thread {
    pthread_t pthread;
    arch_entry_point_fn fn;
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

    arch_assert_void(sig == SIGALRM);

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
    arch_assert_void(res == 0);

    /* init itimer struct */
    res = getitimer(ITIMER_REAL, &itimer);
    arch_assert_void(res == 0);

    /* set interval */
    itimer.it_interval.tv_sec = 1l / (long)CONFIG_TICK_HZ;
    itimer.it_interval.tv_usec = (1000000l / (long)CONFIG_TICK_HZ) % 1000000l;

    /* set current value */
    itimer.it_value.tv_sec = 1l / (long)CONFIG_TICK_HZ;
    itimer.it_value.tv_usec = (1000000l / (long)CONFIG_TICK_HZ) % 1000000l;

    /* set tick */
    res = setitimer(ITIMER_REAL, &itimer, NULL);
    arch_assert_void(res == 0);
}

static void arch_sigusr1(int sig)
{
    arch_assert_void(sig == SIGUSR1);
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
    arch_assert_void(res == 0);

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

picoRTOS_stack_t *arch_prepare_stack(picoRTOS_stack_t *stack,
                                     size_t stack_count,
                                     arch_entry_point_fn fn,
                                     void *priv)
{
    int res;
    struct thread *t = (struct thread*)stack;

    t->fn = fn;
    t->priv = priv;
    (void)sigemptyset(&t->sigset);

    res = pthread_create(&t->pthread, NULL, pthread_start_routine, t);
    arch_assert_void(res == 0);

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

/* cppcheck-suppress constParameter */
void __attribute__((weak)) arch_idle(void *null)
{
    arch_assert_void(null == NULL);

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

picoRTOS_cycles_t arch_counter(arch_counter_t counter, picoRTOS_cycles_t t)
{
#define NS_PER_S 1000000000

    arch_assert_void(counter < ARCH_COUNTER_COUNT);

    struct timespec ts;
    (void)clock_gettime(CLOCK_REALTIME, &ts);

    if (counter == ARCH_COUNTER_CURRENT)
        return (picoRTOS_cycles_t)ts.tv_nsec;

    if (counter == ARCH_COUNTER_SINCE) {
        /* fist tick */
        if (t >= (picoRTOS_cycles_t)NS_PER_S) return (picoRTOS_cycles_t)NS_PER_S;
        /* rollover */
        if ((picoRTOS_cycles_t)ts.tv_nsec < t)
            return ((picoRTOS_cycles_t)NS_PER_S - t) + (picoRTOS_cycles_t)ts.tv_nsec;
        /* normal */
        return (picoRTOS_cycles_t)ts.tv_nsec - t;
    }

    arch_assert_void(false);
    return 0;
}

/* INTERRUPTS : unsupported */

void arch_register_interrupt(/*@unused@*/ picoRTOS_irq_t irq __attribute__((unused)),
                             /*@unused@*/ arch_isr_fn fn __attribute__((unused)),
                             /*@unused@*/ void *priv __attribute__((unused)))
{
}

void arch_enable_interrupt(/*@unused@*/ picoRTOS_irq_t irq __attribute__((unused)))
{
}

void arch_disable_interrupt(/*@unused@*/ picoRTOS_irq_t irq __attribute__((unused)))
{
}
