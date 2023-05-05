#include "picoRTOS_mutex.h"

/* check */
#ifndef CONFIG_DEADLOCK_COUNT
# error no deadlock defined for locks
#endif

/* Function: picoRTOS_mutex_init
 * Initialises a mutex
 *
 * Mutexes are re-entrant on picoRTOS, if you don't need this feature,
 * prefer futexes
 *
 * Parameters:
 *  mutex - A pointer to the mutex to initialize
 *
 * Remarks:
 * Another way to initialise a mutex at startup is this one:
 * (start code)
 * struct picoRTOS_mutex mutex = PICORTOS_MUTEX_INITIALIZER;
 * (end)
 */
void picoRTOS_mutex_init(struct picoRTOS_mutex *mutex)
{
    mutex->owner = (picoRTOS_atomic_t)-1;
    mutex->count = 0;
    arch_flush_dcache(mutex, sizeof(*mutex));
}

/* Function: picoRTOS_mutex_trylock
 * Tries to acquire a mutex
 *
 * Returns:
 * 0 in case of success, -EAGAIN otherwise
 *
 * Parameters:
 *  mutex - The mutex you wish you can acquire
 */
int picoRTOS_mutex_trylock(struct picoRTOS_mutex *mutex)
{
    picoRTOS_atomic_t unused = (picoRTOS_atomic_t)-1;
    picoRTOS_atomic_t pid = (picoRTOS_atomic_t)picoRTOS_self();

    /* mutex is re-entrant */
    if (arch_compare_and_swap(&mutex->owner, unused, pid) != unused &&
        mutex->owner != pid)
        return -EAGAIN;

    mutex->count++;

    arch_flush_dcache(mutex, sizeof(*mutex));
    return 0;
}

/* Function: picoRTOS_mutex_lock
 * Acquires a mutex
 *
 * Parameters:
 *  mutex - The mutex you want to acquire
 *
 * Remarks:
 * The fuction will make CONFIG_DEADLOCK_COUNT attempts to acquire the mutex,
 * if it fails, picoRTOS will throw a debug exception and declare deadlock
 */
void picoRTOS_mutex_lock(struct picoRTOS_mutex *mutex)
{
    int loop = CONFIG_DEADLOCK_COUNT;

    while (picoRTOS_mutex_trylock(mutex) != 0 && loop-- != 0)
        picoRTOS_schedule();

    /* deadlock ? */
    if (!picoRTOS_assert_fatal(loop != -1))
        return;
}

/* Function: picoRTOS_mutex_unlock
 * Releases a mutex
 *
 * Parameters:
 *  mutex - The mutex you want to release
 *
 * Remarks:
 * picoRTOS will throw a debug exception if :
 * - the mutex has not been acquired
 * - the mutex has already been released
 * - you are not the owner
 */
void picoRTOS_mutex_unlock(struct picoRTOS_mutex *mutex)
{
    picoRTOS_atomic_t owner = (picoRTOS_atomic_t)picoRTOS_self();

    arch_invalidate_dcache(mutex, sizeof(*mutex));
    if (!picoRTOS_assert_fatal(mutex->owner == owner)) return;
    if (!picoRTOS_assert_fatal(mutex->count > 0)) return;

    if (--mutex->count == 0)
        mutex->owner = (picoRTOS_atomic_t)-1;

    arch_flush_dcache(mutex, sizeof(*mutex));
}
