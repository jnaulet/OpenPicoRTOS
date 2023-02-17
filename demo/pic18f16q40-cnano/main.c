#include "picoRTOS.h"
// #include "pic18f16q40-cnano.h"

static void dummy_main(void *priv)
{
    picoRTOS_assert_fatal(priv == NULL);

    for (;;)
        picoRTOS_schedule();
}

int main(void)
{
    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_init();

    picoRTOS_task_init(&task, dummy_main, NULL, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* start the scheduler */
    picoRTOS_start();

    /* we're not supposed to end here */
    picoRTOS_assert_fatal(false);
    return -1;
}
