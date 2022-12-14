#include "picoRTOS.h"
#include "raspberry-pico.h"

#include "adc.h"
#include "gpio.h"
#include "pwm.h"
#include "spi.h"
#include "twi.h"
#include "uart.h"
#include "wd.h"

#include "ipc/picoRTOS_mutex.h"
#include "ipc/picoRTOS_cond.h"

/* Demo for Raspberry pico + Pico-LCD-1.14 */

/* hardware */
struct blink {
    struct raspberry_pico pico;
    /* IPCs */
    struct picoRTOS_mutex mutex;
    struct picoRTOS_cond cond;
};

static void blink_init(struct blink *ctx)
{
    /* board */
    (void)raspberry_pico_init(&ctx->pico);

    /* IPCs */
    picoRTOS_mutex_init(&ctx->mutex);
    picoRTOS_cond_init(&ctx->cond);

    /* turn off backlight */
    gpio_write(ctx->pico.GPIO13, false);

    /* start WDT */
    (void)wd_start(ctx->pico.WDT);
}

static void tick_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

    struct gpio *TICK = (struct gpio*)priv;

    for (;;) {
        gpio_toggle(TICK);
        picoRTOS_schedule();
    }
}

static void led0_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

    struct blink *ctx = (struct blink*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    (void)pwm_set_period(ctx->pico.PWM4B, (pwm_period_us_t)200);
    pwm_start(ctx->pico.PWM4B);

    for (;;) {

        size_t n = (size_t)100;
        pwm_duty_cycle_t duty_cycle = 0;

        picoRTOS_mutex_lock(&ctx->mutex);

        /* fade in */
        while (n-- != 0) {
            (void)pwm_set_duty_cycle(ctx->pico.PWM4B, duty_cycle);
            duty_cycle += PWM_DUTY_CYCLE_PCENT(1);
            picoRTOS_sleep(PICORTOS_DELAY_MSEC(5));
        }

        /* ipc */
        picoRTOS_cond_signal(&ctx->cond);
        picoRTOS_mutex_unlock(&ctx->mutex);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

static void led1_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

    struct blink *ctx = (struct blink*)priv;

    for (;;) {

        size_t n = (size_t)99;
        pwm_duty_cycle_t duty_cycle = PWM_DUTY_CYCLE_PCENT(100);

        picoRTOS_mutex_lock(&ctx->mutex);
        picoRTOS_cond_wait(&ctx->cond, &ctx->mutex);

        /* fade away */
        while (n-- != 0) {
            (void)pwm_set_duty_cycle(ctx->pico.PWM4B, duty_cycle);
            duty_cycle -= PWM_DUTY_CYCLE_PCENT(1);
            picoRTOS_sleep(PICORTOS_DELAY_MSEC(5));
        }

        picoRTOS_mutex_unlock(&ctx->mutex);
    }
}

static void spi_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

    size_t xfered = 0;
    struct spi *SPI = (struct spi*)priv;
    char rx[6] = { (char)0, (char)0, (char)0, (char)0, (char)0, (char)0 };
    char tx[6] = { (char)0xa5, (char)0x55, (char)0x5a, (char)0x55, (char)0x4d, (char)0xff };

    for (;;) {
        int res;

        if ((res = spi_xfer(SPI, &rx[xfered], &tx[xfered], sizeof(tx) - xfered)) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        picoRTOS_assert_fatal(res > 0);

        /* ack xfer */
        xfered += (size_t)res;

        if (xfered == sizeof(tx)) {
            picoRTOS_assert_fatal(rx[0] == (char)0xa5);
            picoRTOS_assert_fatal(rx[4] == (char)0x4d);
            /* start again */
            xfered = 0;
        }

        /* Some relief */
        picoRTOS_schedule();
    }
}

static void adc_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

    struct adc *ADC = (struct adc*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {

        int value = 0;
        int timeout = CONFIG_DEADLOCK_COUNT;

        while (adc_read(ADC, &value) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_fatal(timeout != -1);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

/*
 * twi_main sends the who_am_i request to the Waveshare Pico-10DOF-IMU
 * and expects 0xea as an answer
 */
static void twi_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

    struct twi *TWI = (struct twi*)priv;

    for (;;) {
        int res;
        char who_am_i = (char)0;

        while ((res = twi_write(TWI, &who_am_i, sizeof(who_am_i))) == -EAGAIN)
            picoRTOS_schedule();

        while ((res = twi_read(TWI, &who_am_i, sizeof(who_am_i))) == -EAGAIN)
            picoRTOS_schedule();

        picoRTOS_assert_fatal(who_am_i == (char)0xea);
        picoRTOS_sleep(PICORTOS_DELAY_SEC(1));
    }
}

static void wd_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

    struct wd *WD = (struct wd*)priv;

    for (;;) {
        wd_refresh(WD);
        picoRTOS_schedule();
    }
}

int main(void)
{
    static struct blink blink;

    blink_init(&blink);
    picoRTOS_init();

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack4[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack5[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack6[CONFIG_DEFAULT_STACK_COUNT];

    /* TICK */
    picoRTOS_task_init(&task, tick_main, blink.pico.GPIO21, stack0, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, TASK_TICK_PRIO);

    /* LED0 on core #0 */
    picoRTOS_task_init(&task, led0_main, &blink, stack1, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* LED1 on core #1 */
    picoRTOS_task_init(&task, led1_main, &blink, stack2, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* SPI */
    picoRTOS_task_init(&task, spi_main, blink.pico.SPI, stack3, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    (void)picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* ADC */
    picoRTOS_task_init(&task, adc_main, blink.pico.ADC0, stack4, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    (void)picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* I2C */
    picoRTOS_task_init(&task, twi_main, blink.pico.I2C, stack5, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    (void)picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* WD, keep last */
    picoRTOS_task_init(&task, wd_main, blink.pico.WDT, stack6, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    (void)picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_fatal(false);
    return 1;
}
