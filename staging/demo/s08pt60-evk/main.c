#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include "picoRTOS_mutex.h"
#include "picoRTOS_cond.h"

#include "s08pt60-evk.h"

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

    struct gpio *LED = (struct gpio*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {

        picoRTOS_mutex_lock(&mutex);

        gpio_write(LED, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60ul));
        gpio_write(LED, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60ul));

        /* ipc */
        picoRTOS_cond_signal(&cond);
        picoRTOS_mutex_unlock(&mutex);

        /* until next second */
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

static void led1_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct gpio *LED = (struct gpio*)priv;

    for (;;) {

        picoRTOS_mutex_lock(&mutex);
        picoRTOS_cond_wait(&cond, &mutex);

        gpio_write(LED, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(120ul));
        gpio_write(LED, true);

        picoRTOS_mutex_unlock(&mutex);
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

        /* just echo */
        if (uart_read(UART, &c, sizeof(c)) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        (void)uart_write(UART, &c, sizeof(c));
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
 * Fade in & out blue led according to potentiometer value
 */
static void pwm_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    int inc = 1;
    int msec = 10;
    long pcent = 0;

    struct pwm *PWM = &((struct s08pt60_evk_ana*)priv)->FTM2CH5;
    struct adc *ADC = &((struct s08pt60_evk_ana*)priv)->ADP1;

    /* init */
    (void)pwm_set_period(PWM, (pwm_period_us_t)100);
    (void)pwm_set_duty_cycle(PWM, 0);

    /* start */
    pwm_start(PWM);

    for (;;) {

        if (pcent == 0l) inc = 1;
        if (pcent == 100l) inc = -1;

        (void)pwm_set_duty_cycle(PWM, PWM_DUTY_CYCLE_PCENT(pcent));
        (void)adc_read(ADC, &msec);

        pcent += (long)inc;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC((long)msec));
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
    static struct s08pt60_evk evk;

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack4[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack5[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack6[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_init();
    (void)s08pt60_evk_init(&evk);

    /* tick */
    picoRTOS_task_init(&task, tick_main, &evk.TICK, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* led */
    picoRTOS_task_init(&task, led0_main, &evk.GREEN, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* led */
    picoRTOS_task_init(&task, led1_main, &evk.GREEN, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* uart */
    picoRTOS_task_init(&task, console_main, &evk.UART2, stack3, PICORTOS_STACK_COUNT(stack3));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* spi */
    picoRTOS_task_init(&task, spi_main, &evk.SPI1, stack4, PICORTOS_STACK_COUNT(stack4));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* analog */
    picoRTOS_task_init(&task, pwm_main, &evk.ANA, stack5, PICORTOS_STACK_COUNT(stack5));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* watchdog */
    picoRTOS_task_init(&task, wd_main, &evk.WD, stack6, PICORTOS_STACK_COUNT(stack6));
    picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
