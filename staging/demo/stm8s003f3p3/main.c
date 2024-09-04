#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "stm8s003f3p3.h"

static void tick_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct gpio *TICK = (struct gpio*)priv;

    for (;;) {
        gpio_toggle(TICK);
        picoRTOS_schedule();
    }
}

static void led_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct gpio *LED = (struct gpio*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        gpio_write(LED, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60l));
        gpio_write(LED, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60l));
        gpio_write(LED, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(120l));
        gpio_write(LED, true);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

int main(void)
{
    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];

    static struct stm8s003f3p3 stm8s;

    picoRTOS_init();
    (void)stm8s003f3p3_init(&stm8s);

    picoRTOS_task_init(&task, tick_main, &stm8s.TICK, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, led_main, &stm8s.TEST, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
