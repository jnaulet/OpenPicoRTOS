#include "picoRTOS-SMP.h"
#include <stdbool.h>

#include "ipc/picoRTOS_mutex.h"
#include "ipc/picoRTOS_cond.h"

#include "devkit-mpc5748g.h"

#define LED_DELAY_SHORT PICORTOS_DELAY_MSEC(30)
#define LED_DELAY_LONG  PICORTOS_DELAY_MSEC(60)

static void tick_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    bool x = false;
    struct gpio *TICK = (struct gpio*)priv;

    for (;;) {
        gpio_write(TICK, x);

        x = !x; /* no toggle on this arch yet */
        picoRTOS_schedule();
    }
}

/* IPC test */
static struct picoRTOS_mutex mutex = PICORTOS_MUTEX_INITIALIZER;
static struct picoRTOS_cond cond = PICORTOS_COND_INITIALIZER;

static void gpio_write_sleep(struct gpio *gpio, bool enabled, picoRTOS_tick_t delay)
{
    gpio_write(gpio, enabled);
    picoRTOS_sleep(delay);
}

static void led0_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct gpio *LED = (struct gpio*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
        picoRTOS_mutex_lock(&mutex);

        /* turn on */
        gpio_write_sleep(&LED[0], false, LED_DELAY_SHORT);
        gpio_write_sleep(&LED[1], false, LED_DELAY_SHORT);
        gpio_write_sleep(&LED[2], false, LED_DELAY_SHORT);
        gpio_write_sleep(&LED[3], false, LED_DELAY_SHORT);

        /* signal to task led1 */
        picoRTOS_cond_signal(&cond);
        picoRTOS_mutex_unlock(&mutex);

        /* wait for led1 to finish */
        picoRTOS_mutex_lock(&mutex);
        picoRTOS_cond_wait(&cond, &mutex);

        /* turn off */
        gpio_write_sleep(&LED[3], true, LED_DELAY_LONG);
        gpio_write_sleep(&LED[2], true, LED_DELAY_LONG);
        gpio_write_sleep(&LED[1], true, LED_DELAY_LONG);
        gpio_write_sleep(&LED[0], true, LED_DELAY_LONG);

        picoRTOS_mutex_unlock(&mutex);
    }
}

static void led1_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct gpio *LED = (struct gpio*)priv;

    for (;;) {
        picoRTOS_mutex_lock(&mutex);
        picoRTOS_cond_wait(&cond, &mutex);

        /* turn on */
        gpio_write_sleep(&LED[4], false, LED_DELAY_SHORT);
        gpio_write_sleep(&LED[5], false, LED_DELAY_SHORT);
        gpio_write_sleep(&LED[6], false, LED_DELAY_SHORT);
        gpio_write_sleep(&LED[7], false, LED_DELAY_SHORT);

        /* turn off */
        gpio_write_sleep(&LED[7], true, LED_DELAY_LONG);
        gpio_write_sleep(&LED[6], true, LED_DELAY_LONG);
        gpio_write_sleep(&LED[5], true, LED_DELAY_LONG);
        gpio_write_sleep(&LED[4], true, LED_DELAY_LONG);

        /* signal to task led0 */
        picoRTOS_cond_signal(&cond);
        picoRTOS_mutex_unlock(&mutex);
    }
}

int main(void)
{
    static struct devkit_mpc5748g board;

    if (devkit_mpc5748g_init(&board) < 0) {
        picoRTOS_assert_void_fatal(false);
        /*@notreached@*/ return -1;
    }

    picoRTOS_init();

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];

    /* shared task */
    picoRTOS_task_init(&task, tick_main, &board.TICK, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* per core tasks */
    picoRTOS_task_init(&task, led0_main, board.LED, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_SMP_add_task(&task, picoRTOS_get_next_available_priority(), (picoRTOS_mask_t)0x1);
    picoRTOS_task_init(&task, led1_main, board.LED, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_SMP_add_task(&task, picoRTOS_get_next_available_priority(), (picoRTOS_mask_t)0x2);

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
