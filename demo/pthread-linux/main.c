#include "picoRTOS.h"
#include <generated/autoconf.h>

#include <stdio.h>
#include <stdbool.h>

static void tick_main0(void *priv)
{
    picoRTOS_assert_void(priv == NULL);

    for (;;) {
        fprintf(stderr, "|\r");
        picoRTOS_schedule();
    }
}

static void tick_main1(void *priv)
{
    picoRTOS_assert_void(priv == NULL);

    for (;;) {
        fprintf(stderr, "/\r");
        picoRTOS_schedule();
    }
}

static void tick_main2(void *priv)
{
    picoRTOS_assert_void(priv == NULL);

    for (;;) {
        fprintf(stderr, "-\r");
        picoRTOS_schedule();
    }
}

static void tick_main3(void *priv)
{
    picoRTOS_assert_void(priv == NULL);

    for (;;) {
        fprintf(stderr, "\\\r");
        picoRTOS_schedule();
    }
}

static void blink_main(void *priv)
{
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    picoRTOS_assert_void(priv == NULL);

    for (;;) {
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(500));
        fprintf(stderr, "  BLINK !\r");
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(100));
        fprintf(stderr, "         \r");
    }
}

int main(void)
{
#define TASK_TICK_PRIO   (picoRTOS_priority_t)0
#define TASK_BLINK_PRIO  (picoRTOS_priority_t)1

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack4[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_init();

    /* tick */
    picoRTOS_task_init(&task, tick_main0, NULL, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, TASK_TICK_PRIO);
    picoRTOS_task_init(&task, tick_main1, NULL, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, TASK_TICK_PRIO);
    picoRTOS_task_init(&task, tick_main2, NULL, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, TASK_TICK_PRIO);
    picoRTOS_task_init(&task, tick_main3, NULL, stack3, PICORTOS_STACK_COUNT(stack3));
    picoRTOS_add_task(&task, TASK_TICK_PRIO);
    /* blink */
    picoRTOS_task_init(&task, blink_main, NULL, stack4, PICORTOS_STACK_COUNT(stack4));
    picoRTOS_add_task(&task, TASK_BLINK_PRIO);

    picoRTOS_start();
}
