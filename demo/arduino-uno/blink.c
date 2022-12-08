#include "picoRTOS.h"
#include <stdbool.h>

#define DDRB  ((unsigned char*)0x24)
#define PORTB ((unsigned char*)0x25)
#define DDRD  ((unsigned char*)0x2a)
#define PORTD ((unsigned char*)0x2b)

#define BLINK_PERIOD PICORTOS_DELAY_SEC(1)
#define BLINK_DELAY  PICORTOS_DELAY_MSEC(30)

#define LED_BUILTIN (1u << 5)   /* PB7 */
#define TICK        (1u << 0)   /* PD0 */

static void hw_init(void)
{
    /* set IOs as output */
    *DDRB = (unsigned char)LED_BUILTIN;
    *DDRD = (unsigned char)TICK;
}

static void deepcall_schedule(unsigned int n)
{
    if (n != 0)
        deepcall_schedule(n - 1);
    else
        picoRTOS_schedule();
}

static void tick_main(void *priv)
{
    picoRTOS_assert_fatal(priv == NULL);

    bool x = false;

    for (;;) {
        /* toggle pin */
        if (x) *PORTD |= (unsigned char)TICK;
        else *PORTD &= (unsigned char)~TICK;

        /* stack test */
        deepcall_schedule(20u);
        x = !x;
    }
}

static void blink_main(void *priv)
{
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    picoRTOS_assert_fatal(priv == NULL);

    for (;;) {
        picoRTOS_sleep_until(&ref, BLINK_PERIOD);

        /* blink */
        *PORTB |= (unsigned char)LED_BUILTIN;
        picoRTOS_sleep(BLINK_DELAY);
        *PORTB &= (unsigned char)~LED_BUILTIN;
        picoRTOS_sleep(BLINK_DELAY);

        *PORTB |= (unsigned char)LED_BUILTIN;
        picoRTOS_sleep(BLINK_DELAY);
        picoRTOS_sleep(BLINK_DELAY);
        *PORTB &= (unsigned char)~LED_BUILTIN;

        /* stack test */
        deepcall_schedule(10u);
    }
}

int main( void )
{
    hw_init();
    picoRTOS_init();

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_task_init(&task, tick_main, NULL, stack0, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, (picoRTOS_priority_t)TASK_TICK_PRIO);
    picoRTOS_task_init(&task, blink_main, NULL, stack1, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, (picoRTOS_priority_t)TASK_BLINK_PRIO);

    /* Start the scheduler. */
    picoRTOS_start();

    /* we're not supposed to end here */
    picoRTOS_assert_fatal(false);
    return -1;
}
