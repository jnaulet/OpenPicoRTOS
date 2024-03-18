#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "devebox-stc-c51.h"

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

    struct gpio *D2 = (struct gpio*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        gpio_write(D2, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60ul));
        gpio_write(D2, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60ul));
        gpio_write(D2, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(120ul));
        gpio_write(D2, true);


        /* until next second */
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

static void uart_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    char c = (char)0x30;
    struct uart *UART = (struct uart*)priv;

    for (;;) {

        if (uart_read(UART, &c, sizeof(c)) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        (void)uart_write(UART, &c, sizeof(c));
    }
}

int main(void)
{
    static struct devebox_stc_c51 stc;

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_init();
    (void)devebox_stc_c51_init(&stc);

    /* tick */
    picoRTOS_task_init(&task, tick_main, &stc.TICK, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* led */
    picoRTOS_task_init(&task, led_main, &stc.D2, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* console */
    picoRTOS_task_init(&task, uart_main, &stc.UART, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
