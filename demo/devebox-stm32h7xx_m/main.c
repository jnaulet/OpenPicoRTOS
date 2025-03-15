#include "picoRTOS.h"
#include "stm32h7xx_m.h"

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

/*
 * spi_main is a thread that sends data to the SPI and expects to get it back.
 * No loopback is present on the stm32h7, so you must setup a wire between pins PA6 & PA7
 */
static void spi_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    size_t xfered = 0;
    struct spi *SPI = (struct spi*)priv;
    char rx[6] = { (char)0, (char)0, (char)0, (char)0, (char)0, (char)0 };
    char tx[6] = { (char)0xa5, (char)0x55, (char)0x5a, (char)0x55, (char)0x4d, (char)0x4e };

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
    }
}

/*
 * uart_main is a thread that sends data to the UART and expects to get it back.
 * No loopback is present on the chip, so you must setup a wire between PA0 & PA1
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
            picoRTOS_postpone();
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

/*
 * This thread uses the CAN loopback mode to send PINGPONG and check the data has been
 * received correctly
 */
static void can_main(void *priv)
{
#define CAN_TEST_ID 0x6
#define CAN_TIMEOUT PICORTOS_DELAY_MSEC(500l)

    picoRTOS_assert_fatal(priv != NULL, return );

    int count = 0;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    struct can *CAN = &((struct can_test*)priv)->CAN;
    struct rng *TRNG = &((struct can_test*)priv)->TRNG;

    (void)can_accept(CAN, (can_id_t)CAN_TEST_ID, 0);

    for (;;) {

        int res;
        can_id_t id = 0;
        int deadlock = CONFIG_DEADLOCK_COUNT;

        static char tx[CAN_DATA_COUNT];
        char rx[CAN_DATA_COUNT] = { (char)0, (char)0, (char)0, (char)0,
                                    (char)0, (char)0, (char)0, (char)0 };

        /* random */
        while (rng_read(TRNG, tx, sizeof(tx)) < 0 &&
               !PICORTOS_DELAY_ELAPSED(ref, CAN_TIMEOUT) &&
               deadlock-- != 0) {
            picoRTOS_postpone();
            continue;
        }

        picoRTOS_assert_void(deadlock != -1);
        picoRTOS_assert_void(!PICORTOS_DELAY_ELAPSED(ref, CAN_TIMEOUT));

        /* ping */
        if ((res = can_write(CAN, (can_id_t)CAN_TEST_ID, tx, (size_t)CAN_DATA_COUNT)) == -EAGAIN &&
            !PICORTOS_DELAY_ELAPSED(ref, CAN_TIMEOUT)) {
            picoRTOS_schedule();
            continue;
        }

        picoRTOS_assert_void(res > 0);
        picoRTOS_assert_void(!PICORTOS_DELAY_ELAPSED(ref, CAN_TIMEOUT));

        /* pong */
        while (((res = can_read(CAN, &id, rx, sizeof(rx)))) == -EAGAIN &&
               !PICORTOS_DELAY_ELAPSED(ref, CAN_TIMEOUT))
            picoRTOS_schedule();

        picoRTOS_assert_void(res == (int)sizeof(rx));
        picoRTOS_assert_void(!PICORTOS_DELAY_ELAPSED(ref, CAN_TIMEOUT));
        picoRTOS_assert_void(id == (can_id_t)CAN_TEST_ID);
        picoRTOS_assert_void(rx[0] == tx[0]);
        picoRTOS_assert_void(rx[7] == tx[7]);

        /* reset timeout */
        ref = picoRTOS_get_tick();
        count++;
    }
}

static void adc_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct adc *TEMP = (struct adc*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {

        int temp = 0;
        int deadlock = CONFIG_DEADLOCK_COUNT;

        while (adc_read(TEMP, &temp) == -EAGAIN && deadlock-- != 0)
            picoRTOS_postpone();

        picoRTOS_assert_void(deadlock != -1);
        picoRTOS_assert_void(temp != 0);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

int main(void)
{
    static struct stm32h7xx_m stm32;

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack4[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack5[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack6[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_init();
    (void)stm32h7xx_m_init(&stm32);

    /* dummy */
    picoRTOS_task_init(&task, tick_main, &stm32.TICK, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, led0_main, &stm32.LED, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, led1_main, &stm32.LED, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, spi_main, &stm32.SPI1, stack3, PICORTOS_STACK_COUNT(stack3));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, uart_main, &stm32.UART4, stack4, PICORTOS_STACK_COUNT(stack4));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, can_main, &stm32.can, stack5, PICORTOS_STACK_COUNT(stack5));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, adc_main, &stm32.TEMP, stack6, PICORTOS_STACK_COUNT(stack6));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* start scheduling */
    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
