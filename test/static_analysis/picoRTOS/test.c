#include "picoRTOS.h"
#include "ipc/picoRTOS_futex.h"
#include "ipc/picoRTOS_mutex.h"
#include "ipc/picoRTOS_cond.h"
#include "ipc/picoRTOS_queue.h"

static void task_main(/*@unused@*/ void *priv)
{
}

static void task_isr(/*@unused@*/ void *priv)
{
}

int main(void)
{
    int dummy = 0;
    struct picoRTOS_task task;
    picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];
    picoRTOS_futex_t futex = PICORTOS_FUTEX_INITIALIZER;
    struct picoRTOS_mutex mutex = PICORTOS_MUTEX_INITIALIZER;
    struct picoRTOS_cond cond = PICORTOS_COND_INITIALIZER;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    PICORTOS_QUEUE(int, 32) queue;

    /* basics */
    picoRTOS_task_init(&task, task_main, NULL, stack, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_init();
    picoRTOS_add_task(&task, (picoRTOS_priority_t)0);
    picoRTOS_start();
    picoRTOS_suspend();
    picoRTOS_resume();
    picoRTOS_schedule();
    picoRTOS_sleep(PICORTOS_DELAY_MSEC(1));
    picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(1));
    (void)picoRTOS_self();
    /* irqs */
    picoRTOS_register_interrupt((picoRTOS_irq_t)0, task_isr, NULL);
    picoRTOS_enable_interrupt((picoRTOS_irq_t)0);
    picoRTOS_disable_interrupt((picoRTOS_irq_t)0);
    /* ipcs */
    picoRTOS_futex_lock(&futex);
    picoRTOS_futex_unlock(&futex);
    picoRTOS_mutex_lock(&mutex);
    picoRTOS_mutex_unlock(&mutex);
    picoRTOS_cond_signal(&cond);
    picoRTOS_cond_broadcast(&cond);
    picoRTOS_cond_wait(&cond, &mutex);

    PICORTOS_QUEUE_INIT(&queue);
    (void)PICORTOS_QUEUE_WRITE(&queue, 0);
    (void)PICORTOS_QUEUE_READ(&queue, &dummy);

    picoRTOS_kill();
    /*@notreached@*/
    return 0;
}
