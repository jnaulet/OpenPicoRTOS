#include <generated/autoconf.h>

#ifndef CONFIG_SMP
# include "picoRTOS.h"
#else
# include "picoRTOS-SMP.h"
#endif

#include <stdbool.h>
#include "devkit-mpc5748g.h"

#include "ipc/picoRTOS_mutex.h"
#include "ipc/picoRTOS_cond.h"

#define LED_DELAY_SHORT PICORTOS_DELAY_MSEC(30)
#define LED_DELAY_LONG  PICORTOS_DELAY_MSEC(60)

static void tick_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct gpio *TICK = (struct gpio*)priv;

    for (;;) {
        gpio_toggle(TICK);
        picoRTOS_schedule();
    }
}

/* IPC test */
static struct picoRTOS_mutex mutex = PICORTOS_MUTEX_INITIALIZER;
static struct picoRTOS_cond cond = PICORTOS_COND_INITIALIZER;

static void gpio_write_sleep(struct gpio *gpio, bool enabled, picoRTOS_tick_t delay)
{
    gpio_write(gpio, enabled);
    picoRTOS_sleep(delay);
}

static void led0_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct gpio *LED = (struct gpio*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
        picoRTOS_mutex_lock(&mutex);

        /* turn on */
        gpio_write_sleep(&LED[0], false, LED_DELAY_SHORT);
        gpio_write_sleep(&LED[1], false, LED_DELAY_SHORT);
        gpio_write_sleep(&LED[2], false, LED_DELAY_SHORT);
        gpio_write_sleep(&LED[3], false, LED_DELAY_SHORT);

        /* signal to task led1 */
        picoRTOS_cond_signal(&cond);
        picoRTOS_mutex_unlock(&mutex);

        /* force to next task */
        picoRTOS_schedule();

        /* wait for led1 to finish */
        picoRTOS_mutex_lock(&mutex);
        picoRTOS_cond_wait(&cond, &mutex);

        /* turn off */
        gpio_write_sleep(&LED[3], true, LED_DELAY_LONG);
        gpio_write_sleep(&LED[2], true, LED_DELAY_LONG);
        gpio_write_sleep(&LED[1], true, LED_DELAY_LONG);
        gpio_write_sleep(&LED[0], true, LED_DELAY_LONG);

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

        /* turn on */
        gpio_write_sleep(&LED[4], false, LED_DELAY_SHORT);
        gpio_write_sleep(&LED[5], false, LED_DELAY_SHORT);
        gpio_write_sleep(&LED[6], false, LED_DELAY_SHORT);
        gpio_write_sleep(&LED[7], false, LED_DELAY_SHORT);

        /* turn off */
        gpio_write_sleep(&LED[7], true, LED_DELAY_LONG);
        gpio_write_sleep(&LED[6], true, LED_DELAY_LONG);
        gpio_write_sleep(&LED[5], true, LED_DELAY_LONG);
        gpio_write_sleep(&LED[4], true, LED_DELAY_LONG);

        /* signal to task led0 */
        picoRTOS_cond_signal(&cond);
        picoRTOS_mutex_unlock(&mutex);

        /* force scheduling */
        picoRTOS_schedule();
    }
}

static void adc_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct adc_pwm *ADC_PWM = (struct adc_pwm*)priv;
    struct adc *ADC = &ADC_PWM->ADC1_P0;
    struct pwm *PWM = &ADC_PWM->LED0;

    (void)pwm_set_period(PWM, (pwm_period_us_t)100);
    (void)pwm_set_duty_cycle(PWM, 0);
    (void)pwm_start(PWM);

    for (;;) {
        int val = 0;
        int deadlock = CONFIG_DEADLOCK_COUNT;

        /* get adc value */
        while (adc_read(ADC, &val) == -EAGAIN && deadlock-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(deadlock != -1);
        picoRTOS_assert_void(val <= 100);
        picoRTOS_assert_void(val >= 0);

        /* set LED brightness */
        (void)pwm_set_duty_cycle(PWM, PWM_DUTY_CYCLE_PCENT(val));
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

/*
 * console_main is a thread that simply echoes characters
 */
static void console_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct uart *UART = (struct uart*)priv;

    for (;;) {

        char c = (char)0;
        int deadlock = CONFIG_DEADLOCK_COUNT;

        /* just echo */
        if (uart_read(UART, &c, sizeof(c)) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        while (uart_write(UART, &c, sizeof(c)) == -EAGAIN && deadlock-- != 0)
            picoRTOS_schedule();

        picoRTOS_assert_void(deadlock != -1);
    }
}

int main(void)
{
    static struct devkit_mpc5748g board;

    if (devkit_mpc5748g_init(&board) < 0) {
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

    /* shared task */
    picoRTOS_task_init(&task, tick_main, &board.TICK, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

#ifndef SUPPORT_FOR_SMP
    picoRTOS_task_init(&task, led0_main, board.LED, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    picoRTOS_task_init(&task, led1_main, board.LED, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
#else
    /* per core tasks */
    picoRTOS_task_init(&task, led0_main, board.LED, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_SMP_add_task(&task, picoRTOS_get_next_available_priority(), (picoRTOS_mask_t)0x1);
    picoRTOS_task_init(&task, led1_main, board.LED, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_SMP_add_task(&task, picoRTOS_get_next_available_priority(), (picoRTOS_mask_t)0x2);
#endif

    /* adc + pwm */
    picoRTOS_task_init(&task, adc_main, &board.ADC_PWM, stack3, PICORTOS_STACK_COUNT(stack3));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* spi */
    picoRTOS_task_init(&task, spi_main, &board.SPI, stack4, PICORTOS_STACK_COUNT(stack4));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* can */
    picoRTOS_task_init(&task, can_main, &board.CAN0, stack5, PICORTOS_STACK_COUNT(stack5));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* uart */
    picoRTOS_task_init(&task, console_main, &board.UART, stack6, PICORTOS_STACK_COUNT(stack6));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
