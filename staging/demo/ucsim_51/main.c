#include "picoRTOS.h"
#include "picoRTOS_device.h"

#ifndef S_SPLINT_S
__sfr __at(ADDR_P1)   P1;
__sfr __at(ADDR_P1M1) P1M1;
__sfr __at(ADDR_P1M2) P1M2;
#else
static unsigned char P1;
static unsigned char P1M1;
static unsigned char P1M2;
#endif

/* cppcheck-suppress [constParameter] */
static void tick_main(void *priv)
{
    picoRTOS_assert_fatal(priv == (void*)0xf00d, return );

    for (;;) {
        P1 ^= 0x1;
        picoRTOS_schedule();
    }
}

/* cppcheck-suppress [constParameter] */
static void led_main(void *priv)
{
    picoRTOS_assert_fatal(priv == (void*)0xcafe, return );

    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        P1 &= ~0x2;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60ul));
        P1 |= 0x2;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60ul));
        P1 &= ~0x2;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(120ul));
        P1 |= 0x2;

        /* until next second */
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

int main(void)
{
    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];

    /* push-pull */
    P1M1 = (unsigned char)0x0;
    P1M2 = (unsigned char)0x3;

    picoRTOS_init();

    /* tick */
    picoRTOS_task_init(&task, tick_main, (void*)0xf00d, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* led */
    picoRTOS_task_init(&task, led_main, (void*)0xcafe, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
