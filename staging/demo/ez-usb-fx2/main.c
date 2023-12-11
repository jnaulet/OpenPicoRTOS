#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "ipc/picoRTOS_mutex.h"
#include "ipc/picoRTOS_cond.h"

#include "clock-cy7c6801xa.h"

/* IPCs */
static struct picoRTOS_mutex mutex;
static struct picoRTOS_cond cond;

#ifndef S_SPLINT_S
__sfr __at(ADDR_IOA) IOA;
__sfr __at(ADDR_OEA) OEA;
__sfr __at(ADDR_IOB) IOB;
__sfr __at(ADDR_OEB) OEB;
#else
static unsigned char IOA;
static unsigned char OEA;
static unsigned char IOB;
static unsigned char OEB;
#endif

static void tick_main(void *priv)
{
    picoRTOS_assert_fatal(priv == NULL, return );

    for (;;) {
        /* PB0 */
        IOB ^= 0x1;
        picoRTOS_schedule();
    }
}

static void d1_main(void *priv)
{
    picoRTOS_assert_fatal(priv == NULL, return );

    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {

        picoRTOS_mutex_lock(&mutex);

        /* PA0 */
        IOA &= ~0x1;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(30ul));
        IOA |= 0x1;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(30ul));
        IOA &= ~0x1;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60ul));
        IOA |= 0x1;

        /* ipc */
        picoRTOS_cond_signal(&cond);
        picoRTOS_mutex_unlock(&mutex);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

static void d2_main(void *priv)
{
    picoRTOS_assert_fatal(priv == NULL, return );

    for (;;) {

        picoRTOS_mutex_lock(&mutex);
        picoRTOS_cond_wait(&cond, &mutex);

        /* PA1 */
        IOA &= ~0x2;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(30ul));
        IOA |= 0x2;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(30ul));
        IOA &= ~0x2;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60ul));
        IOA |= 0x2;

        picoRTOS_mutex_unlock(&mutex);
    }
}

int main(void)
{
    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];

    /* PERIPHERALS */

    struct clock_settings CLOCK_settings = {
        CLOCK_CY7C6801XA_CLKSPD_48MHZ,
    };

    (void)clock_cy7c6801xa_init(&CLOCK_settings);
    OEA = (unsigned char)0x3;
    OEB = (unsigned char)0x1;

    /* SYSTEM */

    picoRTOS_init();
    picoRTOS_mutex_init(&mutex);
    picoRTOS_cond_init(&cond);

    /* tick */
    picoRTOS_task_init(&task, tick_main, NULL, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* d1 */
    picoRTOS_task_init(&task, d1_main, NULL, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* d2 */
    picoRTOS_task_init(&task, d2_main, NULL, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
