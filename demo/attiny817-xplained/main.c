#include "picoRTOS.h"
#include "attiny817-xplained.h"

#ifdef DEMO_LED
/*
 * led_main double-blinks the user LED
 */
static void led_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

    picoRTOS_tick_t ref = picoRTOS_get_tick();
    struct gpio *USER_LED = (struct gpio*)priv;

    for (;;) {

        gpio_write(USER_LED, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(50u));
        gpio_write(USER_LED, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(50u));
        gpio_write(USER_LED, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(100u));
        gpio_write(USER_LED, false);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}
#endif

#ifdef DEMO_CONSOLE
/*
 * console_main is a thread that simply echoes characters
 */
static void console_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

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
#endif

#ifdef DEMO_SPI
/*
 * spi_main is a thread that sends data to the SPI and expects to get it back.
 * No loopback is present on the attiny817, so you must setup a wire between pins PA1 & PA2
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
#endif

#ifdef DEMO_ADC
/*
 * adc_main samples AIN04 every 40ms and stores the result in sample,
 * AIN04 must be wired to the 3.3v line (solder PA4 to 3V3)
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
        picoRTOS_assert_fatal(sample_mV > 3200);
        picoRTOS_assert_fatal(sample_mV < 3400);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(40l));
    }
}
#endif

#ifdef DEMO_PWM
/*
 * pwm_main put a ramp on a PWM. No blinking possible on this board as the LED
 * is shared with the SPI, output on XXX
 */
static void pwm_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

    picoRTOS_tick_t ref = picoRTOS_get_tick();
    struct attiny817_xplained_PWM *PWM = (struct attiny817_xplained_PWM*)priv;

    pwm_duty_cycle_t duty_cycle0 = 0;
    pwm_duty_cycle_t duty_cycle1 = PWM_DUTY_CYCLE_PCENT(15);

    (void)pwm_set_period(&PWM->WO0, (pwm_period_us_t)100);
    (void)pwm_set_period(&PWM->WO1, (pwm_period_us_t)100); /* useless */

    pwm_start(&PWM->WO0);
    pwm_start(&PWM->WO1);

    for (;;) {
        (void)pwm_set_duty_cycle(&PWM->WO0, duty_cycle0);
        (void)pwm_set_duty_cycle(&PWM->WO1, duty_cycle1);

        duty_cycle0 += PWM_DUTY_CYCLE_PCENT(1);
        duty_cycle1 += PWM_DUTY_CYCLE_PCENT(1);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(10)); /* 100Hz */
    }
}
#endif

#ifdef DEMO_WDT
/*
 * wd_main is a thread that starts & refreshes the internal watchdog.
 * This thread should be the last one by order of priority. If it fails, the system resets
 */
static void wd_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL);

    struct wd *WDT = (struct wd*)priv;

    (void)wd_start(WDT);

    for (;;) {
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(12));
        wd_refresh(WDT);
    }
}
#endif

int main(void)
{
    struct picoRTOS_task task;
    static struct attiny817_xplained attiny817;

    picoRTOS_init();
    (void)attiny817_xplained_init(&attiny817);

#ifdef DEMO_LED
    /* led */
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    picoRTOS_task_init(&task, led_main, &attiny817.USER_LED, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
#endif
#ifdef DEMO_CONSOLE
    /* console */
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    picoRTOS_task_init(&task, console_main, &attiny817.UART, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
#endif
#ifdef DEMO_SPI
    /* spi */
    static picoRTOS_stack_t stack2[CONFIG_EXT_STACK_COUNT];
    picoRTOS_task_init(&task, spi_main, &attiny817.SPI, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
#endif
#ifdef DEMO_ADC
    /* adc */
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];
    picoRTOS_task_init(&task, adc_main, &attiny817.AIN04, stack3, PICORTOS_STACK_COUNT(stack3));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
#endif
#ifdef DEMO_PWM
    /* pwm */
    static picoRTOS_stack_t stack4[CONFIG_DEFAULT_STACK_COUNT];
    picoRTOS_task_init(&task, pwm_main, &attiny817.PWM, stack4, PICORTOS_STACK_COUNT(stack4));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
#endif
#ifdef DEMO_WDT
    /* wdt */
    static picoRTOS_stack_t stack5[CONFIG_DEFAULT_STACK_COUNT];
    picoRTOS_task_init(&task, wd_main, &attiny817.WDT, stack5, PICORTOS_STACK_COUNT(stack5));
    picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());
#endif

    /* start the scheduler */
    picoRTOS_start();

    /* we're not supposed to end here */
    picoRTOS_assert_fatal(false);
    return -1;
}
