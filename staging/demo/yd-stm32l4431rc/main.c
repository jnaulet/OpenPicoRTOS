#include "picoRTOS.h"
#include "yd-stm32l431rc.h"

static void tick_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct gpio *TICK = (struct gpio*)priv;

    for (;;) {
        /* toggle pin */
        gpio_toggle(TICK);
        picoRTOS_schedule();
    }
}

static void led_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct gpio *LED = (struct gpio*)priv;

    for (;;) {
        gpio_write(LED, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60l));
        gpio_write(LED, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60l));
        gpio_write(LED, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(120l));
        gpio_write(LED, false);

        picoRTOS_sleep(PICORTOS_DELAY_SEC(1));
    }
}

int main(void)
{
    static struct yd_stm32l431rc stm32;

    picoRTOS_init();
    (void)yd_stm32l431rc_init(&stm32);

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];

    /* tick */
    picoRTOS_task_init(&task, tick_main, &stm32.TICK, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* led */
    picoRTOS_task_init(&task, led_main, &stm32.LED, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* start the scheduler */
    picoRTOS_start();

    /* we're not supposed to end here */
    picoRTOS_assert_void(false);
    return -1;
}
