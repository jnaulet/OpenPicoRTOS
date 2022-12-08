#include "picoRTOS.h"

#include <stdio.h>
#include <stdbool.h>

static void tick_main(void *priv)
{
    bool x = false;

    picoRTOS_assert_fatal(priv == NULL);

    for (;;) {
        fprintf(stderr, "%c", x ? '.' : ' ');

        x = !x;
        picoRTOS_schedule();
    }
}

static void blink_main(void *priv)
{
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    picoRTOS_assert_fatal(priv == NULL);

    for (;;) {
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(500));
        fprintf(stderr, "O");
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(30));
        fprintf(stderr, "o");
    }
}

int main(void)
{
    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_init();

    picoRTOS_task_init(&task, tick_main, NULL, stack0, CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, TASK_TICK_PRIO);

    picoRTOS_task_init(&task, blink_main, NULL, stack1, CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, TASK_BLINK_PRIO);

    picoRTOS_start();
}
