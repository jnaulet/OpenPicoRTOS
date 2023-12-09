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
        int deadlock = CONFIG_DEADLOCK_COUNT;

        while (adc_read(adc, &value) == -EAGAIN && deadlock-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(deadlock != -1);
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

/*
 * This thread uses the loopback mode of the spi to send data and control it has been
 * received correctly
 */
static void spi_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    size_t xfered = 0;
    struct spi *SPI = (struct spi*)priv;

    for (;;) {

        int res;

        char rx[6] = { (char)0, (char)0, (char)0, (char)0, (char)0, (char)0 };
        char tx[6] = { (char)0xa5, (char)0x55, (char)0x5a, (char)0x55, (char)0x4d, (char)0x4f };

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
    }
}

/*
 * This thread uses the CAN loopback mode to send PINGPONG and check the data has been
 * received correctly
 */
static void can_main(void *priv)
{
#define CAN_TEST_ID 0x6

    picoRTOS_assert_fatal(priv != NULL, return );

    struct can *CAN = (struct can*)priv;

    (void)can_accept(CAN, (can_id_t)CAN_TEST_ID, 0);

    for (;;) {

        int res;
        can_id_t id = 0;
        int timeout = (int)PICORTOS_DELAY_MSEC(500l);

        static const char tx[] = { "PINGPONG" };
        char rx[CAN_DATA_COUNT] = { (char)0, (char)0, (char)0, (char)0,
                                    (char)0, (char)0, (char)0, (char)0 };

        /* ping */
        if ((res = can_write(CAN, (can_id_t)CAN_TEST_ID, tx, (size_t)CAN_DATA_COUNT)) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        /* pong */
        while (((res = can_read(CAN, &id, rx, sizeof(rx)))) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(res == (int)sizeof(rx));
        picoRTOS_assert_void(timeout != -1);
        picoRTOS_assert_void(id == (can_id_t)CAN_TEST_ID);
        picoRTOS_assert_void(rx[7] == 'G');
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
    static picoRTOS_stack_t stack5[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack6[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_task_init(&task, tick_main, &evb.TICK, stack0, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

#ifndef CONFIG_SMP
    picoRTOS_task_init(&task, led0_main, &evb.LED0, stack1, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, led1_main, &evb.LED1, stack2, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
#else
    picoRTOS_task_init(&task, led0_main, &evb.LED0, stack1, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_SMP_add_task(&task, picoRTOS_get_next_available_priority(), (picoRTOS_mask_t)0x1);
    picoRTOS_task_init(&task, led1_main, &evb.LED1, stack2, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_SMP_add_task(&task, picoRTOS_get_next_available_priority(), (picoRTOS_mask_t)0x2);
#endif

    /* adc */
    picoRTOS_task_init(&task, adc_main, &evb.ADC, stack3, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* pwm */
    picoRTOS_task_init(&task, pwm_main, &evb.PWM, stack4, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* spi */
    picoRTOS_task_init(&task, spi_main, &evb.SPI, stack5, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* can */
    picoRTOS_task_init(&task, can_main, &evb.CAN, stack6, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
