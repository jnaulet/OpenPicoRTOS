#include "picoRTOS.h"
#include "longan-nano.h"

#include "picoRTOS_mutex.h"
#include "picoRTOS_cond.h"

static void tick_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    bool x = true;
    struct gpio *TICK = (struct gpio*)priv;

    for (;;) {
        gpio_write(TICK, x);

        x = !x;
        picoRTOS_schedule();
    }
}

/*
 * These threads blink the onboard RGB LEDs
 */
static struct picoRTOS_mutex mutex = PICORTOS_MUTEX_INITIALIZER;
static struct picoRTOS_cond cond = PICORTOS_COND_INITIALIZER;

#define BLINK_PERIOD PICORTOS_DELAY_SEC(1)
#define BLINK_DELAY  PICORTOS_DELAY_MSEC(30)

static void ledr_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct gpio *LED_R = (struct gpio*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        picoRTOS_mutex_lock(&mutex);

        /* blink */
        gpio_write(LED_R, false);
        picoRTOS_sleep(BLINK_DELAY << 1);
        gpio_write(LED_R, true);
        picoRTOS_sleep(BLINK_DELAY);
        gpio_write(LED_R, false);
        picoRTOS_sleep(BLINK_DELAY);
        gpio_write(LED_R, true);

        /* ipc */
        picoRTOS_cond_signal(&cond);
        picoRTOS_mutex_unlock(&mutex);

        picoRTOS_sleep_until(&ref, BLINK_PERIOD);
    }
}

static void ledg_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct gpio *LED_G = (struct gpio*)priv;

    for (;;) {
        picoRTOS_mutex_lock(&mutex);
        picoRTOS_cond_wait(&cond, &mutex);

        /* blink */
        gpio_write(LED_G, false);
        picoRTOS_sleep(BLINK_DELAY << 1);
        gpio_write(LED_G, true);
        picoRTOS_sleep(BLINK_DELAY);
        gpio_write(LED_G, false);
        picoRTOS_sleep(BLINK_DELAY);
        gpio_write(LED_G, true);

        picoRTOS_mutex_unlock(&mutex);
    }
}

static void ledb_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    bool x = true;
    struct gpio *LED_B = (struct gpio*)priv;

    for (;;) {
        gpio_write(LED_B, x);

        x = !x;
        picoRTOS_sleep(PICORTOS_DELAY_SEC(1));
    }
}

int main(void)
{
    static struct longan_nano nano;

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];

    (void)longan_nano_init(&nano);
    picoRTOS_init();

    picoRTOS_task_init(&task, tick_main, &nano.TICK, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, ledr_main, &nano.LED_R, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, ledg_main, &nano.LED_G, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, ledb_main, &nano.LED_B, stack3, PICORTOS_STACK_COUNT(stack3));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
