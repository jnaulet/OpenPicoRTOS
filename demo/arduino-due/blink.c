#include "picoRTOS.h"
#include <stdbool.h>

#include "picoRTOS_mutex.h"
#include "picoRTOS_cond.h"

#define BLINK_PERIOD PICORTOS_DELAY_SEC(1)
#define BLINK_DELAY  PICORTOS_DELAY_MSEC(60)

#define BUILTIN_LED (1u << 27)  /* PB27 */
#define TICK        (1u << 18)  /* PA18 */

#define PIOA_PER ((volatile unsigned long*)0x400e0e00)
#define PIOA_OER ((volatile unsigned long*)0x400e0e10)
#define PIOA_SODR ((volatile unsigned long*)0x400e0e30)
#define PIOA_CODR ((volatile unsigned long*)0x400e0e34)

#define PIOB_PER ((volatile unsigned long*)0x400e1000)
#define PIOB_OER ((volatile unsigned long*)0x400e1010)
#define PIOB_SODR ((volatile unsigned long*)0x400e1030)
#define PIOB_CODR ((volatile unsigned long*)0x400e1034)

#define CKGR_MOR ((volatile unsigned long*)0x400e0620)
#define PMC_SR ((volatile unsigned long*)0x400e0668)

static void hw_init(void)
{
    /* PIOA */
    *PIOA_PER |= TICK;
    *PIOA_OER |= TICK;

    /* PIOB */
    *PIOB_PER |= BUILTIN_LED;
    *PIOB_OER |= BUILTIN_LED;

    /* main clock */
    *CKGR_MOR = 0x370028ul; /* 12Mhz internal */
    while ((*PMC_SR & 0x1) != 0)
        ASM("nop");         /* Warning: can loop indefinitely */
}

static void deepcall_schedule(unsigned long n)
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
        if (x) *PIOA_SODR |= TICK;
        else *PIOA_CODR |= TICK;

        /* stack test */
        deepcall_schedule(20ul);
        x = !x;
    }
}

/* IPC test */
static struct picoRTOS_mutex mutex = PICORTOS_MUTEX_INITIALIZER;
static struct picoRTOS_cond cond = PICORTOS_COND_INITIALIZER;

static void blink_main(void *priv)
{
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    picoRTOS_assert_fatal(priv == NULL);

    for (;;) {
        picoRTOS_sleep_until(&ref, BLINK_PERIOD);

        picoRTOS_mutex_lock(&mutex);

        /* blink */
        *PIOB_SODR |= BUILTIN_LED;
        picoRTOS_sleep(BLINK_DELAY);
        picoRTOS_sleep(BLINK_DELAY);
        *PIOB_CODR |= BUILTIN_LED;

        /* ipc */
        picoRTOS_cond_signal(&cond);
        picoRTOS_mutex_unlock(&mutex);
    }
}

static void blink_again_main(void *priv)
{
    picoRTOS_assert_fatal(priv == NULL);

    for (;;) {
        picoRTOS_mutex_lock(&mutex);
        picoRTOS_cond_wait(&cond, &mutex);

        /* blink */
        picoRTOS_sleep(BLINK_DELAY);

        *PIOB_SODR |= BUILTIN_LED;
        picoRTOS_sleep(BLINK_DELAY);
        *PIOB_CODR |= BUILTIN_LED;

        picoRTOS_mutex_unlock(&mutex);
    }
}

static void atomic_test(void)
{
    picoRTOS_atomic_t res;
    picoRTOS_atomic_t tset = 0;
    picoRTOS_atomic_t one = (picoRTOS_atomic_t)1;
    picoRTOS_atomic_t two = (picoRTOS_atomic_t)2;

    res = arch_test_and_set(&tset);
    picoRTOS_assert_fatal(res == 0);
    res = arch_test_and_set(&tset);
    picoRTOS_assert_fatal(res != 0);

    res = arch_compare_and_swap(&tset, one, two);
    picoRTOS_assert_fatal(res == one);
    res = arch_compare_and_swap(&tset, one, two);
    picoRTOS_assert_fatal(res != one);
}

int main( void )
{
    hw_init();
    picoRTOS_init();

    atomic_test();

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_task_init(&task, tick_main, NULL, stack0, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, (picoRTOS_priority_t)TASK_TICK_PRIO);
    picoRTOS_task_init(&task, blink_main, NULL, stack1, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, (picoRTOS_priority_t)TASK_BLINK_PRIO);
    picoRTOS_task_init(&task, blink_again_main, NULL, stack2, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, (picoRTOS_priority_t)TASK_BLINK_AGAIN_PRIO);

    /* Start the scheduler. */
    picoRTOS_start();

    /* we're not supposed to end here */
    picoRTOS_assert_fatal(false);
    return -1;
}
