#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "picoRTOS_mutex.h"
#include "picoRTOS_cond.h"

#include "clock-n76e003.h"

/* IPCs
 * Due to a bug in SDCC v4.2.6 by Nuvoton (XISEG section starts at 0)
 * this cannot be statically initialized
 */
static struct picoRTOS_mutex mutex;
static struct picoRTOS_cond cond;

#ifndef S_SPLINT_S
__sfr __at(ADDR_P1)   P0;
__sfr __at(ADDR_P1M1) P0M1;
__sfr __at(ADDR_P1M2) P0M2;
__sfr __at(ADDR_P1)   P1;
__sfr __at(ADDR_P1M1) P1M1;
__sfr __at(ADDR_P1M2) P1M2;
#else
static unsigned char P0;
static unsigned char P0M1;
static unsigned char P0M2;
static unsigned char P1;
static unsigned char P1M1;
static unsigned char P1M2;
#endif

/* cppcheck-suppress constParameter */
static void tick_main(void *priv)
{
    picoRTOS_assert_fatal(priv == NULL, return );

    for (;;) {
        P0 ^= 0x1;
        picoRTOS_schedule();
    }
}

/* cppcheck-suppress constParameter */
static void led0_main(void *priv)
{
    picoRTOS_assert_fatal(priv == (void*)0xcafe, return );

    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {

        picoRTOS_mutex_lock(&mutex);

        P1 &= ~0x4;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60ul));
        P1 |= 0x4;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60ul));

        /* ipc */
        picoRTOS_cond_signal(&cond);
        picoRTOS_mutex_unlock(&mutex);

        /* until next second */
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

/* cppcheck-suppress constParameter */
static void led1_main(void *priv)
{
    picoRTOS_assert_fatal(priv == (void*)0xf00d, return );

    for (;;) {

        picoRTOS_mutex_lock(&mutex);
        picoRTOS_cond_wait(&cond, &mutex);

        P1 &= ~0x4;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(120ul));
        P1 |= 0x4;

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
        CLOCK_N76E003_FOSC_FHIRC,
        0ul,    /* feclk */
        1u,     /* clkdiv */
    };

    (void)clock_n76e003_init(&CLOCK_settings);

    /* push-pull */
    P0M1 = (unsigned char)0x0;
    P0M2 = (unsigned char)0x1;
    P1M1 = (unsigned char)0x0;
    P1M2 = (unsigned char)0x4;

    /* SYSTEM */

    picoRTOS_init();
    picoRTOS_mutex_init(&mutex);
    picoRTOS_cond_init(&cond);

    /* tick */
    picoRTOS_task_init(&task, tick_main, NULL, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* led */
    picoRTOS_task_init(&task, led0_main, (void*)0xcafe, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* led */
    picoRTOS_task_init(&task, led1_main, (void*)0xf00d, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
