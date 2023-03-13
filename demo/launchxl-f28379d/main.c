#include "picoRTOS.h"
#include "launchxl-f28379d.h"

#include <stdbool.h>
#include <stdint.h>

#include "picoRTOS_mutex.h"
#include "picoRTOS_cond.h"

/*
 * This thread toggles a GPIO on every schedule for debug purposes
 */
static void tick_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct gpio *TICK = (struct gpio*)priv;

    for (;;) {
        /* toggle pin */
        gpio_toggle(TICK);
        picoRTOS_schedule();
    }
}

/*
 * This thread blinks the two onboard LEDs
 */
static struct picoRTOS_mutex mutex = PICORTOS_MUTEX_INITIALIZER;
static struct picoRTOS_cond cond = PICORTOS_COND_INITIALIZER;

#define BLINK_PERIOD PICORTOS_DELAY_SEC(1)
#define BLINK_DELAY  PICORTOS_DELAY_MSEC(30)

static void blink_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct gpio *LED = (struct gpio*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        picoRTOS_mutex_lock(&mutex);

        /* blink */
        gpio_write(LED, false);
        picoRTOS_sleep(BLINK_DELAY);
        picoRTOS_sleep(BLINK_DELAY);
        gpio_write(LED, true);
        picoRTOS_sleep(BLINK_DELAY);
        gpio_write(LED, false);
        picoRTOS_sleep(BLINK_DELAY);
        gpio_write(LED, true);

        /* ipc */
        picoRTOS_cond_signal(&cond);
        picoRTOS_mutex_unlock(&mutex);

        picoRTOS_sleep_until(&ref, BLINK_PERIOD);
    }
}

static void blink_again_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct gpio *LED = (struct gpio*)priv;

    for (;;) {
        picoRTOS_mutex_lock(&mutex);
        picoRTOS_cond_wait(&cond, &mutex);

        /* blink */
        gpio_write(LED, false);
        picoRTOS_sleep(BLINK_DELAY);
        picoRTOS_sleep(BLINK_DELAY);
        gpio_write(LED, true);
        picoRTOS_sleep(BLINK_DELAY);
        gpio_write(LED, false);
        picoRTOS_sleep(BLINK_DELAY);
        gpio_write(LED, true);

        picoRTOS_mutex_unlock(&mutex);
    }
}

/*
 * This thread uses the loopback mode of the spi to send data and control it has been
 * received correctly
 */
static void spi_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    size_t xfered = 0;
    struct spi *SPI = (struct spi*)priv;

    for (;;) {

        int res;

        char rx[5] = { (char)0, (char)0, (char)0, (char)0, (char)0 };
        char tx[5] = { (char)0xa5, (char)0x55, (char)0x5a, (char)0x55, (char)0x4d };

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
 * This thread simply sends a ramp on a PWM
 */
static void pwm_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    pwm_duty_cycle_t dc = 0;
    struct pwm *PWM = (struct pwm*)priv;

    /* start */
    pwm_start(PWM);

    for (;;) {
        (void)pwm_set_duty_cycle(PWM, dc++);
        picoRTOS_schedule();
    }
}

/*
 * This thread reads multiple values from ADCIN13 (temperature sensor)
 */
static void adc_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    int deg_x10[3] = { 0, 0, 0 };
    struct adc *TS = (struct adc*)priv;

    for (;;) {
        int timeout = (int)PICORTOS_DELAY_MSEC(50);

        while (adc_read_multiple(TS, deg_x10, (size_t)3) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);
        picoRTOS_assert_void(deg_x10[0] > 100 && deg_x10[0] < 550);
        picoRTOS_assert_void(deg_x10[1] > 100 && deg_x10[1] < 550);
        picoRTOS_assert_void(deg_x10[2] > 100 && deg_x10[2] < 550);
    }
}

/*
 * This thread uses the CAN loopback mode to send PINGPONG and check the data has been
 * received correctly
 */
static void can_main(void *priv)
{
#define CAN_TEST_ID 0x6

    picoRTOS_assert_void(priv != NULL);

    struct can *CAN = (struct can*)priv;

    (void)can_accept(CAN, (can_id_t)CAN_TEST_ID, 0);

    for (;;) {

        int res;
        can_id_t id = 0;
        int timeout = (int)PICORTOS_DELAY_MSEC(500);

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

/*
 * This thread tests the i2c master by sending a byte (0xa5) to 0x69 and expecting a
 * specific answer from a slave (0x5a).
 *
 * You must wire I2CA to I2CB for this test to work. There's also a clear electrical
 * problem, it only works with an oscilloscope connected to the pins on my desk.
 */
static void twi_master_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct twi *TWI = (struct twi*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        int res;
        char c = (char)0xa5;
        int timeout = (int)PICORTOS_DELAY_MSEC(500);

        while ((res = twi_write(TWI, &c, sizeof(c))) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(res != -EPIPE);
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
    picoRTOS_assert_void(priv != NULL);

    struct twi *TWI = (struct twi*)priv;

    for (;;) {
        char c = (char)0;
        int timeout = (int)PICORTOS_DELAY_SEC(2);

        while (twi_read(TWI, &c, sizeof(c)) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);
        picoRTOS_assert_void(c == (char)0xa5);

        c = (char)0x5a;
        while (twi_write(TWI, &c, sizeof(c)) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);
    }
}

/*
 * This thread refreshes the local windowed watchdog every 7ms
 */
static void wd_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct wd *WDT = (struct wd*)priv;

    for (;;) {
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(7));
        wd_refresh(WDT);
    }
}

struct default_stack {
    picoRTOS_stack_t sp[CONFIG_DEFAULT_STACK_COUNT];
};

int main(void)
{
    static struct launchxl_f28379d launchxl;

    /* this has to be done first cause of the watchdog */
    if (launchxl_f28379d_init(&launchxl) < 0) {
        picoRTOS_assert_void_fatal(false);
        return -1; /* fatal crash */
    }

    picoRTOS_init();

    size_t i = 0;
    picoRTOS_priority_t prio;
    struct picoRTOS_task task;
    static struct default_stack stack[CONFIG_TASK_COUNT];

    /* tick */
    picoRTOS_task_init(&task, tick_main, &launchxl.TICK, stack[i++].sp, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* blink (shared priorities) */
    prio = picoRTOS_get_next_available_priority();
    picoRTOS_task_init(&task, blink_main, &launchxl.LED.R, stack[i++].sp, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, prio);
    picoRTOS_task_init(&task, blink_again_main, &launchxl.LED.B, stack[i++].sp, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, prio);
    /* spi */
    picoRTOS_task_init(&task, spi_main, &launchxl.SPI, stack[i++].sp, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* pwm */
    picoRTOS_task_init(&task, pwm_main, &launchxl.PWM, stack[i++].sp, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* adc */
    picoRTOS_task_init(&task, adc_main, &launchxl.ADCIN13, stack[i++].sp, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* can */
    picoRTOS_task_init(&task, can_main, &launchxl.CAN, stack[i++].sp, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* twi (shared priorities) */
    prio = picoRTOS_get_next_available_priority();
    picoRTOS_task_init(&task, twi_master_main, &launchxl.I2CA, stack[i++].sp, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, prio);
    picoRTOS_task_init(&task, twi_slave_main, &launchxl.I2CB, stack[i++].sp, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, prio);
    /* watchdog */
    picoRTOS_task_init(&task, wd_main, &launchxl.WDT, stack[i++].sp, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());

    /* Start the scheduler. */
    picoRTOS_start();

    /* we're not supposed to end here */
    picoRTOS_assert_void_fatal(false);
    return -1;
}
