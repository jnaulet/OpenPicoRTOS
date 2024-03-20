#include "picoRTOS.h"
#include "mh-tiny.h"

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

/*
 * spi_main is a thread that sends data to the SPI and expects to get it back.
 * No loopback is present on the arduino, so you must setup a wire between pins 50 & 51.
 * If loopback fails, the thread quits and set the spi status to false
 */
static void spi_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    size_t xfered = 0;
    struct spi *SPI = (struct spi*)priv;

    static char rx[] = { (char)0, (char)0, (char)0, (char)0, (char)0, (char)0 };
    static char tx[] = { (char)0xa5, (char)0x55, (char)0x5a, (char)0x55, (char)0x4d, (char)0x4e };

    for (;;) {
        int res;

        if ((res = spi_xfer(SPI, &rx[xfered], &tx[xfered], sizeof(tx) - xfered)) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        picoRTOS_assert_void(res > 0);

        /* ack xfer */
        xfered += (size_t)res;

        if (xfered == sizeof(tx)) {
            picoRTOS_assert_void(rx[0] == (char)0xa5);
            picoRTOS_assert_void(rx[4] == (char)0x4d);
            /* start again */
            xfered = 0;
        }
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

int main( void )
{
    static struct mh_tiny mh_tiny;

    picoRTOS_init();
    (void)mh_tiny_init(&mh_tiny);

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];

    /* tick */
    picoRTOS_task_init(&task, tick_main, &mh_tiny.TICK, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, (picoRTOS_priority_t)0);
    /* led */
    picoRTOS_task_init(&task, led_main, &mh_tiny.LED, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* spi loopback */
    picoRTOS_task_init(&task, spi_main, &mh_tiny.SPI, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* watchdog refresh */
    picoRTOS_task_init(&task, wd_main, &mh_tiny.WD, stack3, PICORTOS_STACK_COUNT(stack3));
    picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());

    /* start the scheduler */
    picoRTOS_start();

    /* we're not supposed to end here */
    picoRTOS_assert_void(false);
    return -1;
}
