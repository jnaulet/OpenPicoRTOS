#include "picoRTOS_cond.h"
#include "picoRTOS_port.h"

/* Function: picoRTOS_cond_init
 * Initialises a condition
 *
 * Conditions are used to synchronize threads. They work in pair with mutexes
 *
 * Parameters:
 *  cond - A pointer to the condition to initialize
 *
 * Remarks:
 * Another way to init a condition at startup is the following:
 * (start code)
 * struct picoRTOS_cond cond = PICORTOS_COND_ININTIALIZER;
 * (end)
 */
void picoRTOS_cond_init(struct picoRTOS_cond *cond)
{
    cond->act = PICORTOS_COND_NONE;
    cond->count = (size_t)0;
    arch_flush_dcache(cond, sizeof(*cond));
}

/* Function: picoRTOS_cond_signal
 * Signals a condition to a single waiting thread
 *
 * Parameters:
 *  cond - A pointer to the condition you want to signal
 */
void picoRTOS_cond_signal(struct picoRTOS_cond *cond)
{
    arch_invalidate_dcache(cond, sizeof(*cond));
    cond->act = PICORTOS_COND_SIGNAL;
    arch_flush_dcache(cond, sizeof(*cond));
}

/* Function: picoRTOS_cond_broadcast
 * Broadcasts a condition to a all waiting threads
 *
 * Parameters:
 *  cond - A pointer to the condition you want to broadcast
 */
void picoRTOS_cond_broadcast(struct picoRTOS_cond *cond)
{
    arch_invalidate_dcache(cond, sizeof(*cond));
    cond->act = PICORTOS_COND_BROADCAST;
    arch_flush_dcache(cond, sizeof(*cond));
}

/* Function: picoRTOS_cond_wait
 * Waits for a condition to be signaled
 *
 * Parameters:
 *  cond - A pointer to the condition you want to wait for
 *  mutex - A previously acquired mutex that is shared with the signaling thread
 *
 * After this call the mutex is released, allowing the signaling task to take it back.
 *
 * Right out of this call, you own the mutex again and need to release it.
 *
 * This is how conditions are typically used:
 * Thread A:
 * (start code)
 * picoRTOS_mutex_lock(&mutex);
 * picoRTOS_cond_wait(&cond, &mutex);
 *
 * do_something();
 *
 * picoRTOS_mutex_unlock(&mutex);
 * (end)
 * Thread B:
 * (start code)
 * picoRTOS_mutex_lock(&mutex);
 *
 * use_shared_resource();
 *
 * picoRTOS_cond_signal(&cond, mutex);
 * picoRTOS_mutex_unlock(&mutex);
 * (end)
 */
void picoRTOS_cond_wait(struct picoRTOS_cond *cond, struct picoRTOS_mutex *mutex)
{
    arch_invalidate_dcache(cond, sizeof(*cond));
    if (!picoRTOS_assert_fatal(cond->count < (size_t)CONFIG_TASK_COUNT)) return;

    /* we already own the mutex */
    cond->count++;

    for (;;) {
        arch_invalidate_dcache(cond, sizeof(*cond));
        if (cond->act != PICORTOS_COND_NONE)
            break;

        picoRTOS_mutex_unlock(mutex);
        picoRTOS_schedule();
        picoRTOS_mutex_lock(mutex);
    }

    /* reset */
    if (--cond->count == 0 ||
        cond->act == PICORTOS_COND_SIGNAL)
        cond->act = PICORTOS_COND_NONE;

    arch_flush_dcache(cond, sizeof(*cond));
}
