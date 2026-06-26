#include <picoRTOS.h>
#include <picoRTOS_device.h>

static unsigned char *SCREEN1 = (unsigned char*)ADDR_SCREEN1;
/* static unsigned char *COLOR = (unsigned char*)ADDR_COLOR; */

static void tick_main(void *priv)
{
    picoRTOS_assert(priv == NULL, picoRTOS_kill(EINVAL));

    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        *SCREEN1 = (unsigned char)0xff;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60));
        *SCREEN1 = (unsigned char)0;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60));
        *SCREEN1 = (unsigned char)0xff;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(120));
        *SCREEN1 = (unsigned char)0;

        /* delay */
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

int main(void)
{
    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_init();

    /* dummy */
    picoRTOS_task_init(&task, tick_main, NULL, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
