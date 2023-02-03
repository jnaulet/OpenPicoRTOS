#include "picoRTOS.h"
#include "adafruit-itsybitsy-m4.h"

/*
 * led_main double-blinks the user LED
 */
static void led_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

    picoRTOS_tick_t ref = picoRTOS_get_tick();
    struct gpio *RED = (struct gpio*)priv;

    for (;;) {

        gpio_write(RED, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(50u));
        gpio_write(RED, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(50u));
        gpio_write(RED, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(100u));
        gpio_write(RED, false);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

/*
 * spi_main is a thread that sends data to the SPI and expects to get it back.
 * No loopback is present on the adafruit, so you must setup a wire between pins PA0(MO) & PA1 (MI)
 */
static void spi_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

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

        picoRTOS_assert_fatal(res > 0);

        /* ack xfer */
        xfered += (size_t)res;

        if (xfered == sizeof(tx)) {
            picoRTOS_assert_fatal(rx[0] == (char)0xa5);
            picoRTOS_assert_fatal(rx[4] == (char)0x4d);
            /* start again */
            xfered = 0;
        }
    }
}

/*
 * uart_main is a thread that sends data to the UART and expects to get it back.
 * No loopback is present on the adafruit, so you must setup a wire between pins PA16(RX) & PA17 (TX)
 */
static void uart_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

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

        picoRTOS_assert_fatal(timeout != -1);
        picoRTOS_assert_fatal(tx == rx);

        /* increment data */
        tx = (char)((int)tx + 1);
    }
}

/*
 * pwm_main put a ramp on a PWM
 */
static void pwm_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

    struct pwm *PWM = (struct pwm*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    pwm_duty_cycle_t duty_cycle = 0;

    (void)pwm_set_period(PWM, (pwm_period_us_t)100);
    pwm_start(PWM);

    for (;;) {
        (void)pwm_set_duty_cycle(PWM, duty_cycle);
        duty_cycle += PWM_DUTY_CYCLE_PCENT(1);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(10)); /* 100Hz */
    }
}

/*
 * adc_main samples SCALEDCOREVCC every 40ms and stores the result in sample
 */
static void adc_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

    struct adc *ADC = (struct adc*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {

        int sample_mV = 0;

        if (adc_read(ADC, &sample_mV) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        /* Analog measurement from 3.3v */
        picoRTOS_assert_fatal(sample_mV > 900);
        picoRTOS_assert_fatal(sample_mV < 1200);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(40l));
    }
}

int main(void)
{
    static struct adafruit_itsybitsy_m4 itsybitsy;

    (void)adafruit_itsybitsy_m4_init(&itsybitsy);
    picoRTOS_init();

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack4[CONFIG_DEFAULT_STACK_COUNT];

    /* TICK */
    picoRTOS_task_init(&task, led_main, &itsybitsy.RED, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* SPI */
    picoRTOS_task_init(&task, spi_main, &itsybitsy.SPI, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* UART */
    picoRTOS_task_init(&task, uart_main, &itsybitsy.UART, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* PWM */
    picoRTOS_task_init(&task, pwm_main, &itsybitsy.D5, stack3, PICORTOS_STACK_COUNT(stack3));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* ADC */
    picoRTOS_task_init(&task, adc_main, &itsybitsy.ADC, stack4, PICORTOS_STACK_COUNT(stack4));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_fatal(false);
    return 1;
}
