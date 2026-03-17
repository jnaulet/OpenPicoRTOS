#include "picoRTOS.h"
#include "same70-xplained-ultra.h"

/*
 * led_main double-blinks the user LED
 */
static void led_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    picoRTOS_tick_t ref = picoRTOS_get_tick();
    struct gpio *LED = (struct gpio*)priv;

    for (;;) {

        gpio_write(LED, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(50u));
        gpio_write(LED, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(50u));
        gpio_write(LED, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(100u));
        gpio_write(LED, true);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

static void adc_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct same70_xplained_ultra *ctx = (struct same70_xplained_ultra *)priv;

    (void)pwm_set_period(&ctx->PWM, (pwm_period_us_t)67);
    (void)pwm_set_duty_cycle(&ctx->PWM, 0);

    (void)pwm_start(&ctx->PWM);

    for (;;) {

        int data = 0;
        int deadlock = CONFIG_DEADLOCK_COUNT;

        if (adc_read(&ctx->POT, &data) == -EAGAIN && deadlock-- != 0) {
            picoRTOS_postpone();
            continue;
        }

        /* PWM */
        (void) pwm_set_duty_cycle(&ctx->PWM, PWM_DUTY_CYCLE_PCENT(data));

        picoRTOS_assert_void(deadlock != -1);
        picoRTOS_assert_void(data < 4096);
    }
}

int main(void)
{
    static struct same70_xplained_ultra ultra;

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];

    (void)same70_xplained_ultra_init(&ultra);
    picoRTOS_init();

    /* TICK */
    picoRTOS_task_init(&task, led_main, &ultra.USER_LED0, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* ADC */
    picoRTOS_task_init(&task, adc_main, &ultra, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_start();

    /* stop here */
    picoRTOS_assert_void_fatal(false);
    return 0;
}
