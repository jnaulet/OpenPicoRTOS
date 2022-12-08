/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "picoRTOS.h"
#include "pico/stdlib.h"

#ifndef PICO_DEFAULT_LED_PIN
# error blink example requires a board with a regular LED
#endif

#define LED_PIN  PICO_DEFAULT_LED_PIN
#define TICK_PIN 15

static void hw_init(void)
{
    /* LED */
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    /* TICK */
    gpio_init(TICK_PIN);
    gpio_set_dir(TICK_PIN, GPIO_OUT);
}

static void tick_main(void *priv)
{
    picoRTOS_assert_fatal(priv == NULL);

    int x = 0;

    for (;;) {
        gpio_put(TICK_PIN, x);

        x = !x;
        picoRTOS_schedule();
    }
}

static void led_main(void *priv)
{
    picoRTOS_assert_fatal(priv == NULL);

    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));

        /* turn on */
        gpio_put(LED_PIN, 1);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60));

        /* turn off */
        gpio_put(LED_PIN, 0);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(30));
    }
}

int main(void)
{
    hw_init();
    picoRTOS_init();

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];

    /* tasks */
    picoRTOS_task_init(&task, tick_main, NULL, stack0, CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, TASK_TICK_PRIO);

    picoRTOS_task_init(&task, led_main, NULL, stack1, CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, TASK_LED_PRIO);

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_fatal(false);
    return 1;
}
