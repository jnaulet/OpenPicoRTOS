#include "picoRTOS_cond.h"
#include "picoRTOS_port.h"

/**
 * void **picoRTOS_cond_init**(**picoRTOS_cond_t** *<ins>cond</ins>);
 * > Initializes a condition <ins>cond</ins> at runtime
 * ### NOTES
 * > Conditions are used to synchronize threads. They work in pair with mutexes.<br>
 * > See **picoRTOS_cond_wait()** documentation for more info.
 * >
 * > Another way to statically init a condition at startup is the following:
 * ```c
 *     struct picoRTOS_cond cond = PICORTOS_COND_INITIALIZER;
 * ```
 * > On memory-protected systems you almost always want to put your conditions
 * > in the `UNPRIVILEGED_DATA` area, so all your tasks can directly access them:
 * ```c
 *     struct picoRTOS_cond UNPRIVILEGED_DATA cond = PICORTOS_COND_INITIALIZER;
 * ```
 */
void picoRTOS_cond_init(struct picoRTOS_cond *cond)
{
    cond->act = PICORTOS_COND_NONE;
    cond->count = (size_t)0;
}

/**
 * void **picoRTOS_cond_signal**(**struct picoRTOS_cond** *<ins>cond</ins>);
 * > Signals a condition to a single waiting tasks
 * ### NOTES
 * > <ins>Beware</ins>: the task that will wake up is not necessarily the highest
 * > priority one, as a lower to-be-executed task might be waiting for
 * > the same condition
 */
void picoRTOS_cond_signal(struct picoRTOS_cond *cond)
{
    cond->act = PICORTOS_COND_SIGNAL;
}

/**
 * void **picoRTOS_cond_broadcast**(**struct picoRTOS_cond** *<ins>cond</ins>);
 * > Signals a condition to a all waiting tasks
 * ### NOTES
 * > All the waiting tasks will wake up by order of execution (not priority)
 */
void picoRTOS_cond_broadcast(struct picoRTOS_cond *cond)
{
    cond->act = PICORTOS_COND_BROADCAST;
}

/**
 * void **picoRTOS_cond_wait**(**struct picoRTOS_cond** \*<ins>cond</ins>,
 * **struct picoRTOS_mutex** *<ins>mutex</ins>);
 * > Waits for a condition to be signaled
 * ### NOTES
 * > After this call the mutex is released, allowing the signaling task to take it back.<br>
 * > Right out of this call, you own the mutex again and need to release it.
 * >
 * > This is how conditions are typically used:<br>
 * >> **Thread A:**
 * ```c
 *     picoRTOS_mutex_lock(&mutex);
 *     picoRTOS_cond_wait(&cond, &mutex); // <- here we're waiting for ThreadB
 *
 *     do_something();
 *
 *     picoRTOS_mutex_unlock(&mutex);
 * ```
 * >> **Thread B:**
 * ```c
 *     picoRTOS_mutex_lock(&mutex);
 *
 *     use_shared_resource();
 *
 *     picoRTOS_cond_signal(&cond, mutex);
 *     picoRTOS_mutex_unlock(&mutex); // <- ThreadA can now do_something()
 * ```
 */
void picoRTOS_cond_wait(struct picoRTOS_cond *cond, struct picoRTOS_mutex *mutex)
{
    picoRTOS_assert(cond->count < (size_t)CONFIG_TASK_COUNT, picoRTOS_kill(EINVAL));

    /* we already own the mutex */
    cond->count++;

    /*
     * BUG: this loop should have a better upper limit
     */
    for (;;) {
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
}
