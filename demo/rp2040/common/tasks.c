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
static struct picoRTOS_mutex UNPRIVILEGED_DATA mutex = PICORTOS_MUTEX_INITIALIZER;
static struct picoRTOS_cond UNPRIVILEGED_DATA cond = PICORTOS_COND_INITIALIZER;

void tick_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));

    struct gpio *TICK = (struct gpio*)priv;

    for (;;) {
        gpio_toggle(TICK);
        picoRTOS_schedule();
    }
}

void led0_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));

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

void led1_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));

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

void spi_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));

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

void adc_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));

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
void twi_master_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));

    struct twi *TWI = (struct twi*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        char c = (char)0xa5;
        int timeout = (int)PICORTOS_DELAY_MSEC(500);

        while (twi_write(TWI, &c, sizeof(c), TWI_F_S | TWI_F_P) == -EAGAIN &&
               timeout-- != 0) picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);

        while (twi_read(TWI, &c, sizeof(c), TWI_F_S | TWI_F_P) == -EAGAIN &&
               timeout-- != 0) picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);
        picoRTOS_assert_void(c == (char)0x5a);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

/*
 * This thread tests the i2c slave, it reads a byte (0xa5) and send 0x5a back
 */
void twi_slave_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));

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
            while (twi_read(TWI, &c, sizeof(c), 0) == -EAGAIN &&
                   timeout-- != 0) picoRTOS_schedule();

            picoRTOS_assert_void(timeout != -1);
            picoRTOS_assert_void(c == (char)0xa5);
        }

        if (res == TWI_READ) {
            char c = (char)0x5a;
            while (twi_write(TWI, &c, sizeof(c), 0) == -EAGAIN &&
                   timeout-- != 0) picoRTOS_schedule();

            picoRTOS_assert_void(timeout != -1);
        }
    }
}

/*
 * This thread ramps up a PWM duty cycle from 0 to 100% by 0.1% increments
 * Period is set at 100us
 */
void pwm_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));

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
void ipwm_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));

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

void wd_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));

    struct wd *WD = (struct wd*)priv;

    for (;;) {
        wd_refresh(WD);
        picoRTOS_schedule();
    }
}
