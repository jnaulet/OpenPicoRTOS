#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "picoRTOS_mutex.h"
#include "picoRTOS_cond.h"

#include "s08pt60-evk.h"

static struct picoRTOS_mutex mutex = PICORTOS_MUTEX_INITIALIZER;
static struct picoRTOS_cond cond = PICORTOS_COND_INITIALIZER;

static void tick_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct gpio *TICK = (struct gpio*)priv;

    for (;;) {
        gpio_toggle(TICK);
        picoRTOS_schedule();
    }
}

static void led0_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct gpio *LED = (struct gpio*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {

        picoRTOS_mutex_lock(&mutex);

        gpio_write(LED, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60ul));
        gpio_write(LED, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60ul));

        /* ipc */
        picoRTOS_cond_signal(&cond);
        picoRTOS_mutex_unlock(&mutex);

        /* until next second */
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

static void led1_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct gpio *LED = (struct gpio*)priv;

    for (;;) {

        picoRTOS_mutex_lock(&mutex);
        picoRTOS_cond_wait(&cond, &mutex);

        gpio_write(LED, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(120ul));
        gpio_write(LED, true);

        picoRTOS_mutex_unlock(&mutex);
    }
}

int main(void)
{
    static struct s08pt60_evk evk;

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_init();
    (void)s08pt60_evk_init(&evk);

    /* tick */
    picoRTOS_task_init(&task, tick_main, &evk.TICK, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* led */
    picoRTOS_task_init(&task, led0_main, &evk.GREEN, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* led */
    picoRTOS_task_init(&task, led1_main, &evk.RED, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
