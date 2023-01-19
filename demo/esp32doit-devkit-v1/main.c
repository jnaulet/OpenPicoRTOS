#include "picoRTOS.h"

static void nop_main(void *priv)
{
    picoRTOS_assert_fatal(priv == NULL);


    for (;;) {
        ASM("nop");
        (void)picoRTOS_get_tick();
        picoRTOS_schedule();
    }
}

int main(void)
{
    picoRTOS_init();

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_task_init(&task, nop_main, NULL, stack0, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, (picoRTOS_priority_t)0);

    picoRTOS_assert_fatal(false);
    return -1;
}
