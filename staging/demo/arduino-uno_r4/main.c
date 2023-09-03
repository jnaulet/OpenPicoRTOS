#include "picoRTOS.h"
#include "arduino-uno_r4.h"

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

/*
 * uart_main is a thread that sends data to the UART and expects to get it back.
 * No loopback is present on the nano, so you must setup a wire between T0 & R0
 */
static void uart_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    char tx = (char)0;
    struct uart *UART = (struct uart*)priv;

    for (;;) {

        char rx = (char)0;
        int timeout = CONFIG_DEADLOCK_COUNT;

        if (uart_write(UART, &tx, sizeof(tx)) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        while (uart_read(UART, &rx, sizeof(rx)) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);
        picoRTOS_assert_void(tx == rx);

        /* increment data */
        tx = (char)((int)tx + 1);
    }
}

static void adc_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct adc *ADC = (struct adc*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {

        int mV = 0;
        int timeout = CONFIG_DEADLOCK_COUNT;

        while (adc_read(ADC, &mV) == -EAGAIN && timeout-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(timeout != -1);
        picoRTOS_assert_void(mV > 3000);
        picoRTOS_assert_void(mV < 4000);

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
        int timeout = (int)PICORTOS_DELAY_SEC(2l);

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

int main( void )
{
    static struct arduino_uno_r4 r4;

    picoRTOS_init();
    (void)arduino_uno_r4_init(&r4);

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack4[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack5[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack6[CONFIG_DEFAULT_STACK_COUNT];

    /* tick */
    picoRTOS_task_init(&task, tick_main, &r4.TICK, stack0, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* led */
    picoRTOS_task_init(&task, led0_main, &r4.L, stack1, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, led1_main, &r4.L, stack2, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* spi */
    picoRTOS_task_init(&task, spi_main, &r4.SPI, stack3, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* uart */
    picoRTOS_task_init(&task, uart_main, &r4.UART, stack4, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* adc */
    picoRTOS_task_init(&task, adc_main, &r4.A0, stack5, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* i2c */
    picoRTOS_task_init(&task, twi_slave_main, &r4.I2C, stack6, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* Start the scheduler. */
    picoRTOS_start();

    /* we're not supposed to end here */
    picoRTOS_assert_void(false);
    return -1;
}
