#include <generated/autoconf.h>

#ifndef CONFIG_SMP
# include "picoRTOS.h"
#else
# include "picoRTOS-SMP.h"
#endif

#include "mpc5775e-evb.h"

#include "ipc/picoRTOS_mutex.h"
#include "ipc/picoRTOS_cond.h"

#include <stdbool.h>

static void gpio_write_sleep(struct gpio *led, bool enabled, picoRTOS_tick_t delay)
{
    gpio_write(led, enabled);
    picoRTOS_sleep(delay);
}

static void tick_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct gpio *TICK = (struct gpio*)priv;

    for (;;) {
        gpio_toggle(TICK);
        picoRTOS_schedule();
    }
}

#define LED_DELAY_SHORT PICORTOS_DELAY_MSEC(30)
#define LED_DELAY_LONG  PICORTOS_DELAY_MSEC(60)

/* IPC test */
static struct picoRTOS_mutex mutex = PICORTOS_MUTEX_INITIALIZER;
static struct picoRTOS_cond cond = PICORTOS_COND_INITIALIZER;

static void led0_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct gpio *LED = (struct gpio*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
        picoRTOS_mutex_lock(&mutex);

        gpio_write_sleep(LED, true, LED_DELAY_SHORT);   /* on */
        gpio_write_sleep(LED, false, LED_DELAY_SHORT);  /* off */

        /* ipc */
        picoRTOS_cond_signal(&cond);
        picoRTOS_mutex_unlock(&mutex);
    }
}

static void led1_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct gpio *LED = (struct gpio*)priv;

    for (;;) {
        picoRTOS_mutex_lock(&mutex);
        picoRTOS_cond_wait(&cond, &mutex);

        gpio_write_sleep(LED, true, LED_DELAY_LONG);    /* on */
        gpio_write_sleep(LED, false, 0);                /* off */

        picoRTOS_mutex_unlock(&mutex);
    }
}

static void adc_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct adc *adc = (struct adc*)priv;

    for (;;) {
        int value = 0;

        (void)adc_read(adc, &value);
        picoRTOS_schedule();
    }
}

static void pwm_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    pwm_duty_cycle_t duty = 0;
    struct pwm *pwm = (struct pwm*)priv;

    (void)pwm_set_period(pwm, (pwm_period_us_t)50);
    (void)pwm_set_duty_cycle(pwm, 0);

    pwm_start(pwm);

    for (;;) {
        (void)pwm_set_duty_cycle(pwm, duty);
        duty += PWM_DUTY_CYCLE_PMIL(1);
        picoRTOS_schedule();
    }
}

int main(void)
{
    static struct mpc5775e_evb evb;

    if (mpc5775e_evb_init(&evb) < 0) {
        picoRTOS_assert_void_fatal(false);
        /*@notreached@*/ return -1;
    }

    picoRTOS_init();

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack4[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_task_init(&task, tick_main, &evb.TICK, stack0, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

#ifndef CONFIG_SMP
    picoRTOS_task_init(&task, led0_main, &evb.LED[0], stack1, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, led1_main, &evb.LED[0], stack2, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
#else
    picoRTOS_task_init(&task, led0_main, &evb.LED[0], stack1, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_SMP_add_task(&task, picoRTOS_get_next_available_priority(), (picoRTOS_mask_t)0x1);
    picoRTOS_task_init(&task, led1_main, &evb.LED[0], stack2, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_SMP_add_task(&task, picoRTOS_get_next_available_priority(), (picoRTOS_mask_t)0x2);
#endif

    /*
       picoRTOS_task_init(&task, adc_main, &evb.ADC, stack3, (size_t)CONFIG_DEFAULT_STACK_COUNT);
       picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

       picoRTOS_task_init(&task, pwm_main, &evb.PWM, stack4, (size_t)CONFIG_DEFAULT_STACK_COUNT);
       picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
     */

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
