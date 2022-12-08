#include "picoRTOS_spinlock.h"
#include "picoRTOS_port.h"

/* check */
#ifndef CONFIG_DEADLOCK_COUNT
# error no deadlock defined for locks
#endif

void picoRTOS_spinlock_init(picoRTOS_spinlock_t *lock)
{
    *lock = (picoRTOS_spinlock_t)0;
}

int picoRTOS_spin_trylock(picoRTOS_spinlock_t *lock)
{
    if (arch_test_and_set((picoRTOS_atomic_t*)lock) != (picoRTOS_atomic_t)0)
        return -1;

    return 0;
}

void picoRTOS_spin_lock(picoRTOS_spinlock_t *lock)
{
    int loop = CONFIG_DEADLOCK_COUNT;

    /* spins full throttle */
    while (picoRTOS_spin_trylock(lock) != 0 &&
           loop-- != 0) {
        /* NOP */
    }

    /* check for deadlock */
    picoRTOS_assert_fatal(loop != -1);
}

void picoRTOS_spin_unlock(picoRTOS_spinlock_t *lock)
{
    *lock = (picoRTOS_spinlock_t)0;
}
