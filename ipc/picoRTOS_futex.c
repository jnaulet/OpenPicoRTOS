#include "picoRTOS_futex.h"
#include "picoRTOS_port.h"

/* check */
#ifndef CONFIG_DEADLOCK_COUNT
# error no deadlock defined for locks
#endif

/* Function: picoRTOS_futex_init
 * Initialises a futex
 *
 * Futexes are fast non re-entrant mutexes
 *
 * Parameters:
 *  futex - A pointer to the futex to initialize
 *
 * Remarks:
 * Another way to initialise a futex at startup is this one:
 * (start code)
 * picoRTOS_futex_t futex = PICORTOS_FUTEX_INITIALIZER;
 * (end)
 */
void picoRTOS_futex_init(picoRTOS_futex_t *futex)
{
    *futex = (picoRTOS_futex_t)0;
}

/* Function: picoRTOS_futex_trylock
 * Tries to acquire a futex
 *
 * Returns:
 * 0 in case of success, -EAGAIN otherwise
 *
 * Parameters:
 *  futex - The futex you wish you can acquire
 */
int picoRTOS_futex_trylock(picoRTOS_futex_t *futex)
{
    if (arch_test_and_set((picoRTOS_atomic_t*)futex) != (picoRTOS_atomic_t)0)
        return -EAGAIN;

    return 0;
}

/* Function: picoRTOS_futex_lock
 * Acquires a futex
 *
 * Parameters:
 *  futex - The futex you want to acquire
 *
 * Remarks:
 * The fuction will make CONFIG_DEADLOCK_COUNT attempts to acquire the futex,
 * if it fails, picoRTOS will throw a debug exception and declare deadlock
 */
void picoRTOS_futex_lock(picoRTOS_futex_t *futex)
{
    int loop = CONFIG_DEADLOCK_COUNT;

    while (picoRTOS_futex_trylock(futex) != 0 && loop-- != 0)
        picoRTOS_schedule();

    /* check for deadlock */
    picoRTOS_assert_fatal(loop != -1);
}

/* Function: picoRTOS_futex_unlock
 * Releases a futex
 *
 * Parameters:
 *  futex - The futex you want to release
 *
 * Remarks:
 * picoRTOS will throw a debug exception if :
 * - the futex has not been acquired
 * - the futex has already been released
 */
void picoRTOS_futex_unlock(picoRTOS_futex_t *futex)
{
    picoRTOS_assert_fatal(*futex != (picoRTOS_atomic_t)0);

    *futex = (picoRTOS_futex_t)0;
}
