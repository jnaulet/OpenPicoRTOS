#include "picoRTOS_mutex.h"

/* check */
#ifndef CONFIG_DEADLOCK_COUNT
# error no deadlock defined for locks
#endif

#define nobody (picoRTOS_atomic_t)PICORTOS_MUTEX_NOOWNER

/**
 * void **picoRTOS_mutex_init**(**picoRTOS_mutex_t** *<ins>mutex</ins>);
 * > Initializes a <ins>mutex</ins> at runtime
 * ### NOTES
 * > Mutexes are re-entrant on picoRTOS, if you don't need this feature, prefer futexes.<br>
 * > Another way to statically initialise a mutex at startup is this one:
 * ```c
 *     struct picoRTOS_mutex mutex = PICORTOS_MUTEX_INITIALIZER;
 * ```
 * > On memory-protected systems you almost always want to put your mutexes
 * > in the `UNPRIVILEGED_DATA` area, so all your tasks can directly access them:
 * ```c
 *     struct picoRTOS_mutex UNPRIVILEGED_DATA mutex = PICORTOS_MUTEX_INITIALIZER;
 * ```
 */
void picoRTOS_mutex_init(struct picoRTOS_mutex *mutex)
{
    mutex->owner = nobody;
    mutex->count = 0;
}

/**
 * int **picoRTOS_mutex_trylock**(**struct picoRTOS_mutex** *<ins>mutex</ins>);
 * > Tries to acquire a <ins>mutex</ins>
 * ### RETURN
 * > Returns 0 if the mutex has been acquired, -`EAGAIN` otherwise
 * ### NOTES
 * > On SMP configurations, if a task is the previous owner of the mutex it
 * > tries to acquire, it is `picoRTOS_postpone()`d to allow other cores
 * > some garanteed access to the shared resource.
 * >
 * > <ins>Beware</ins>: this is potentially a violation of the tasks priorities order
 */
int picoRTOS_mutex_trylock(struct picoRTOS_mutex *mutex)
{
    picoRTOS_atomic_t pid = (picoRTOS_atomic_t)picoRTOS_self();

#ifdef CONFIG_SMP
    /*
     * HOTFIX: in SMP, if the cores are perfectly in sync,
     * one of them can never acquire the lock, this is a crude
     * attempt at solving this very real problem
     */
    if (mutex->prev_owner == pid)
        picoRTOS_postpone();
#endif

    /* mutex is re-entrant */
    if (arch_compare_and_swap(&mutex->owner, nobody, pid) != nobody &&
        mutex->owner != pid)
        return -EAGAIN;

    mutex->count++;
    mutex->prev_owner = pid;
    return 0;
}

/**
 * void **picoRTOS_mutex_lock**(**struct picoRTOS_mutex** *<ins>mutex</ins>)
 * > Acquires a <ins>mutex</ins> or dies
 * ### NOTES
 * > The function will make `CONFIG_DEADLOCK_COUNT` attempts to acquire the mutex.<br>
 * > If it fails to acquire it, the calling task will be killed with a `EDEADLK`
 * > failure code
 */
void picoRTOS_mutex_lock(struct picoRTOS_mutex *mutex)
{
    int loop = CONFIG_DEADLOCK_COUNT;

    while (picoRTOS_mutex_trylock(mutex) != 0 && loop-- != 0)
        picoRTOS_schedule();

    /* deadlock ? */
    picoRTOS_assert(loop != -1, picoRTOS_kill(EDEADLK));
}

/**
 * void **picoRTOS_mutex_unlock**(**struct picoRTOS_mutex** *<ins>mutex</ins>)
 * > Releases a <ins>mutex</ins>
 * ### NOTES
 * > Ths function will kill the calling task if :
 * >  - the calling task is not the current owner (EPERM)
 * >  - the mutex has already been released (EINVAL)
 */
void picoRTOS_mutex_unlock(struct picoRTOS_mutex *mutex)
{
    picoRTOS_atomic_t owner = (picoRTOS_atomic_t)picoRTOS_self();

    picoRTOS_assert(mutex->owner == owner, picoRTOS_kill(EPERM));
    picoRTOS_assert(mutex->count > 0, picoRTOS_kill(EINVAL));

    if (--mutex->count == 0)
        mutex->owner = nobody;
}
