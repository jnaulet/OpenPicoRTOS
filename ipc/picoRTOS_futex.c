#include "picoRTOS_futex.h"
#include "picoRTOS_port.h"

/* check */
#ifndef CONFIG_DEADLOCK_COUNT
# error no deadlock defined for locks
#endif

/**
 * void **picoRTOS_futex_init**(**picoRTOS_futex_t** *<ins>futex</ins>);
 * > Initializes a <ins>futex</ins> at runtime
 * ### NOTES
 * > Futexes are fast non-reentrant mutexes on picoRTOS.
 * >
 * > A way to statically initialise a futex at startup is this one:
 * ```c
 *     struct picoRTOS_futex futex = PICORTOS_FUTEX_INITIALIZER;
 * ```
 * > On memory-protected systems you almost always want to put your futexes
 * > in the `UNPRIVILEGED_DATA` area, so all your tasks can directly access them:
 * ```c
 *     struct picoRTOS_futex UNPRIVILEGED_DATA futex = PICORTOS_FUTEX_INITIALIZER;
 * ```
 */
void picoRTOS_futex_init(picoRTOS_futex_t *futex)
{
    *futex = (picoRTOS_futex_t)0;
}

/**
 * int **picoRTOS_futex_trylock**(**struct picoRTOS_futex** *<ins>futex</ins>);
 * > Tries to acquire a <ins>futex</ins>
 * ### RETURN
 * > Returns 0 if the futex has been acquired, -`EAGAIN` otherwise
 */
int picoRTOS_futex_trylock(picoRTOS_futex_t *futex)
{
    if (arch_test_and_set((picoRTOS_atomic_t*)futex) != (picoRTOS_atomic_t)0)
        return -EAGAIN;

    return 0;
}

/**
 * void **picoRTOS_futex_lock**(**struct picoRTOS_futex** *<ins>futex</ins>)
 * > Acquires a <ins>futex</ins> or dies
 * ### NOTES
 * > The function will make `CONFIG_DEADLOCK_COUNT` attempts to acquire the futex.<br>
 * > If it fails to acquire it, the calling task will be killed with a `EDEADLK`
 * > failure code
 */
void picoRTOS_futex_lock(picoRTOS_futex_t *futex)
{
    int loop = CONFIG_DEADLOCK_COUNT;

    while (picoRTOS_futex_trylock(futex) != 0 && loop-- != 0)
        picoRTOS_schedule();

    /* check for deadlock */
    picoRTOS_assert(loop != -1, picoRTOS_kill(EDEADLK));
}

/**
 * void **picoRTOS_futex_unlock**(**struct picoRTOS_futex** *<ins>futex</ins>)
 * > Releases a <ins>futex</ins>
 * ### NOTES
 * > Ths function will kill the calling task with `EINVAL` if the futex
 * > has already been released.
 */
void picoRTOS_futex_unlock(picoRTOS_futex_t *futex)
{
    picoRTOS_assert(*futex != (picoRTOS_atomic_t)0, picoRTOS_kill(EINVAL));
    *futex = (picoRTOS_futex_t)0;
}
