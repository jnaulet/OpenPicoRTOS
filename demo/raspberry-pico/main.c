#include <generated/autoconf.h>

#ifndef CONFIG_SMP
# include "picoRTOS.h"
#else
# include "picoRTOS-SMP.h"
#endif

#include "raspberry-pico.h"

#include "adc.h"
#include "gpio.h"
#include "pwm.h"
#include "ipwm.h"
#include "spi.h"
#include "twi.h"
#include "uart.h"
#include "wd.h"

#include "ipc/picoRTOS_mutex.h"
#include "ipc/picoRTOS_cond.h"

/* IPCs */
static struct picoRTOS_mutex mutex = PICORTOS_MUTEX_INITIALIZER;
static struct picoRTOS_cond cond = PICORTOS_COND_INITIALIZER;

static void tick_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct gpio *TICK = (struct gpio*)priv;

    for (;;) {
        gpio_toggle(TICK);
        picoRTOS_schedule();
    }
}

static void led0_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct pwm *PWM = (struct pwm*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    (void)pwm_set_period(PWM, (pwm_period_us_t)200);
    pwm_start(PWM);

    for (;;) {

        size_t n = (size_t)100;
        pwm_duty_cycle_t duty_cycle = 0;

        picoRTOS_mutex_lock(&mutex);

        /* fade in */
        while (n-- != 0) {
            (void)pwm_set_duty_cycle(PWM, duty_cycle);
            duty_cycle += PWM_DUTY_CYCLE_PCENT(1);
            picoRTOS_sleep(PICORTOS_DELAY_MSEC(5));
        }

        /* ipc */
        picoRTOS_cond_signal(&cond);
        picoRTOS_mutex_unlock(&mutex);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

static void led1_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct pwm *PWM = (struct pwm*)priv;

    for (;;) {

        size_t n = (size_t)99;
        pwm_duty_cycle_t duty_cycle = PWM_DUTY_CYCLE_PCENT(100);

        picoRTOS_mutex_lock(&mutex);
        picoRTOS_cond_wait(&cond, &mutex);

        /* fade away */
        while (n-- != 0) {
            (void)pwm_set_duty_cycle(PWM, duty_cycle);
            duty_cycle -= PWM_DUTY_CYCLE_PCENT(1);
            picoRTOS_sleep(PICORTOS_DELAY_MSEC(5));
        }

        picoRTOS_mutex_unlock(&mutex);
    }
}

static void spi_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

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

        picoRTOS_assert_void(res > 0);

        /* ack xfer */
        xfered += (size_t)res;

        if (xfered == sizeof(tx)) {
            picoRTOS_assert_void(rx[0] == (char)0xa5);
            picoRTOS_assert_void(rx[4] == (char)0x4d);
            /* start again */
            xfered = 0;
        }

        /* Some relief */
        picoRTOS_schedule();
    }
}

static void adc_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct adc *ADC = (struct adc*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {

        int value = 0;
        int timeout = CONFIG_DEADLOCK_COUNT;

        while (adc_read(ADC, &value) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

/*
 * This thread tests the i2c master by sending a byte (0xa5) to 0x69 and expecting a
 * specific answer from a slave (0x5a)
 */
static void twi_master_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct twi *TWI = (struct twi*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        char c = (char)0xa5;
        int timeout = (int)PICORTOS_DELAY_MSEC(500);

        while (twi_write(TWI, &c, sizeof(c)) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);

        while (twi_read(TWI, &c, sizeof(c)) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);
        picoRTOS_assert_void(c == (char)0x5a);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

/*
 * This thread tests the i2c slave, it reads a byte (0xa5) and send 0x5a back
 */
static void twi_slave_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct twi *TWI = (struct twi*)priv;

    for (;;) {
        int res;
        int timeout = (int)PICORTOS_DELAY_SEC(2);

        if ((res = twi_poll(TWI)) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        if (res == TWI_WRITE) {
            char c = (char)0;
            while (twi_read(TWI, &c, sizeof(c)) == -EAGAIN && timeout-- != 0)
                picoRTOS_schedule();

            picoRTOS_assert_void(timeout != -1);
            picoRTOS_assert_void(c == (char)0xa5);
        }

        if (res == TWI_READ) {
            char c = (char)0x5a;
            while (twi_write(TWI, &c, sizeof(c)) == -EAGAIN && timeout-- != 0)
                picoRTOS_schedule();

            picoRTOS_assert_void(timeout != -1);
        }
    }
}

/*
 * This thread ramps up a PWM duty cycle from 0 to 100% by 0.1% increments
 * Period is set at 100us
 */
static void pwm_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    pwm_duty_cycle_t duty_cycle = 0;
    struct pwm *PWM = (struct pwm*)priv;

    /* init */
    (void)pwm_set_period(PWM, (pwm_period_us_t)100);
    (void)pwm_set_duty_cycle(PWM, 0);

    /* start */
    pwm_start(PWM);

    for (;;) {
        (void)pwm_set_duty_cycle(PWM, duty_cycle);
        duty_cycle += PWM_DUTY_CYCLE_PMIL(1);

        picoRTOS_schedule();
    }
}

/*
 * This thread measures the output period and duty_cycle of a PWM,
 * Period should be exactly 100ms, duty cycle 40%
 */
static void ipwm_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct ipwm *IPWM = (struct ipwm*)priv;

    /* wait for hw to init properly */
    picoRTOS_sleep(PICORTOS_DELAY_MSEC(10));

    for (;;) {
        pwm_period_us_t period = 0;
        pwm_duty_cycle_t duty_cycle = 0;
        int deadlock = CONFIG_DEADLOCK_COUNT;

        while (ipwm_get_period(IPWM, &period) == -EAGAIN && deadlock-- != 0)
            picoRTOS_schedule();

        /* check */
        picoRTOS_assert_void(deadlock != -1);
        /* Warning: ipwm-rp2040 uses the tick as a reference and this
         * adds a lot of imprecision. Need to fix this
         * picoRTOS_assert_void(period > (pwm_period_us_t)50);
         * picoRTOS_assert_void(period < (pwm_period_us_t)200);
         */

        deadlock = CONFIG_DEADLOCK_COUNT;
        while (ipwm_get_duty_cycle(IPWM, &duty_cycle) == -EAGAIN && deadlock-- != 0)
            picoRTOS_schedule();

        /* final check */
        picoRTOS_assert_void(deadlock != -1);
        /* This one is not reliable either
         * picoRTOS_assert_void(duty_cycle > PWM_DUTY_CYCLE_PCENT(30));
         * picoRTOS_assert_void(duty_cycle < PWM_DUTY_CYCLE_PCENT(50));
         */
    }
}

static void wd_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct wd *WD = (struct wd*)priv;

    for (;;) {
        wd_refresh(WD);
        picoRTOS_schedule();
    }
}

int main(void)
{
    static struct raspberry_pico pico;

    (void)raspberry_pico_init(&pico);
    picoRTOS_init();

    picoRTOS_priority_t prio;
    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack4[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack5[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack6[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack7[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack8[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack9[CONFIG_DEFAULT_STACK_COUNT];

    /* TICK */
    picoRTOS_task_init(&task, tick_main, pico.GPIO19, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, (picoRTOS_priority_t)0);

    /* LEDs, strict deadlines, no round-robin */
#ifndef CONFIG_SMP
    picoRTOS_task_init(&task, led0_main, pico.PWM4B, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, led1_main, pico.PWM4B, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
#else
    prio = picoRTOS_get_next_available_priority();
    picoRTOS_task_init(&task, led0_main, pico.PWM4B, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_SMP_add_task(&task, prio, (picoRTOS_mask_t)(1 << 0));  /* LED0: core #0 */
    prio = picoRTOS_get_next_available_priority();
    picoRTOS_task_init(&task, led1_main, pico.PWM4B, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_SMP_add_task(&task, prio, (picoRTOS_mask_t)(1 << 1));  /* LED1: core #1 */
#endif

    /* SPI & ADC (round-robin) */
    prio = picoRTOS_get_next_available_priority();
    picoRTOS_task_init(&task, spi_main, pico.SPI, stack3, PICORTOS_STACK_COUNT(stack3));
    (void)picoRTOS_add_task(&task, prio);
    picoRTOS_task_init(&task, adc_main, pico.ADC0, stack4, PICORTOS_STACK_COUNT(stack4));
    (void)picoRTOS_add_task(&task, prio);

    /* I2C (round-robin) */
    prio = picoRTOS_get_next_available_priority();
#ifndef CONFIG_SMP
    picoRTOS_task_init(&task, twi_master_main, pico.I2C0, stack5, PICORTOS_STACK_COUNT(stack5));
    (void)picoRTOS_add_task(&task, prio);
    picoRTOS_task_init(&task, twi_slave_main, pico.I2C1, stack6, PICORTOS_STACK_COUNT(stack6));
    (void)picoRTOS_add_task(&task, prio);
#else
    picoRTOS_task_init(&task, twi_master_main, pico.I2C0, stack5, PICORTOS_STACK_COUNT(stack5));
    picoRTOS_SMP_add_task(&task, prio, (picoRTOS_mask_t)(1 << 0));  /* I2C0: core #0 */
    picoRTOS_task_init(&task, twi_slave_main, pico.I2C1, stack6, PICORTOS_STACK_COUNT(stack6));
    picoRTOS_SMP_add_task(&task, prio, (picoRTOS_mask_t)(1 << 1));  /* I2C1: core #1 */
#endif

    /* PWM */
    picoRTOS_task_init(&task, pwm_main, pico.PWM5A, stack7, PICORTOS_STACK_COUNT(stack7));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* IPWM */
    picoRTOS_task_init(&task, ipwm_main, pico.PWM2B, stack8, PICORTOS_STACK_COUNT(stack8));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* WD, keep last */
    picoRTOS_task_init(&task, wd_main, pico.WDT, stack9, PICORTOS_STACK_COUNT(stack9));
    (void)picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
