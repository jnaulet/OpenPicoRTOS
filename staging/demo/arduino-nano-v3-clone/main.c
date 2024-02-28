#include "picoRTOS.h"
#include "arduino-nano-v3-clone.h"

#include "gpio.h"
#include "uart.h"
#include "wd.h"

/*
 * Threads
 */

/*
 * tick_main is a thread that toggles the TICK gpio so you can
 * make measurements on a logical anaylser or oscilloscope and check
 * the scheduling frequency is right
 */
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

static void blink_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct gpio *L = (struct gpio*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        gpio_write(L, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(30l));
        gpio_write(L, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(30l));
        gpio_write(L, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60l));
        gpio_write(L, false);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

/*
 * console_main is a thread that simply echoes characters
 */
static void console_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct uart *UART = (struct uart*)priv;

    for (;;) {

        char c = (char)0;

        /* just echo */
        if (uart_read(UART, &c, sizeof(c)) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        (void)uart_write(UART, &c, sizeof(c));
    }
}

/*
 * wd_main is a thread that refreshes the internal watchdog.
 * This thread should be the last one by order of priority. If it fails, the system resets
 */
static void wd_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct wd *WDT = (struct wd*)priv;

    for (;;) {
        wd_refresh(WDT);
        picoRTOS_schedule();
    }
}

int main(void)
{
    static struct arduino_nano_v3_clone nano;

    picoRTOS_init();
    (void)arduino_nano_v3_clone_init(&nano);

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];

    /* tick */
    picoRTOS_task_init(&task, tick_main, &nano.TICK, stack0, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, (picoRTOS_priority_t)0);
    /* led */
    picoRTOS_task_init(&task, blink_main, &nano.L, stack1, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* serial console */
    picoRTOS_task_init(&task, console_main, &nano.SERIAL, stack2, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* watchdog refresh */
    picoRTOS_task_init(&task, wd_main, &nano.WDT, stack3, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());

    /* Start the scheduler. */
    picoRTOS_start();

    /* we're not supposed to end here */
    picoRTOS_assert_void(false);
    return -1;
}
