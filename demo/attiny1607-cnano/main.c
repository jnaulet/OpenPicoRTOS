#include "picoRTOS.h"
#include "attiny1607-cnano.h"

#define CONFIG_EXT_STACK_COUNT 52

/*
 * led_main drives the PWM LED.
 * Warning: in order to work, PB5 must be wired to PB7 on th cnano board
 */
static void led_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct pwm *PWM = (struct pwm*)priv;

    int duty_cycle = 0;
    bool fade_in = true;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    (void)pwm_set_period(PWM, (pwm_period_us_t)50);
    pwm_start(PWM);

    for (;;) {

        if (fade_in) {
            duty_cycle += 2;
            if (duty_cycle == 100) fade_in = false;
        }else{
            duty_cycle -= 2;
            if (duty_cycle == 0) fade_in = true;
        }

        (void)pwm_set_duty_cycle(PWM, PWM_DUTY_CYCLE_PCENT(duty_cycle));
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(10)); /* 100Hz */
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

/*
 * spi_main is a thread that sends data to the SPI and expects to get it back.
 * No loopback is present on the attiny1607-cnano, wire PC2 & PC1 together
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
 * adc_main samples AIN04 every 40ms and stores the result in sample,
 * AIN04 must be wired to the 3.3v line (solder PA4 to 3V3)
 */
static void adc_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct adc *ADC = (struct adc*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {

        int sample_mV = 0;

        if (adc_read(ADC, &sample_mV) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        /* Analog measurement from 3.3v */
        picoRTOS_assert_void(sample_mV > 3200);
        picoRTOS_assert_void(sample_mV < 5000);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(40l));
    }
}

/*
 * wd_main is a thread that starts & refreshes the internal watchdog.
 * This thread should be the last one by order of priority. If it fails, the system resets
 */
static void wd_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    struct wd *WDT = (struct wd*)priv;

    (void)wd_start(WDT);

    for (;;) {
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(12));
        wd_refresh(WDT);
    }
}

int main(void)
{
    struct picoRTOS_task task;
    static struct attiny1607_cnano cnano;

    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_EXT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack4[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_init();
    (void)attiny1607_cnano_init(&cnano);

    /* led */

    picoRTOS_task_init(&task, led_main, &cnano.LED0, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* console */

    picoRTOS_task_init(&task, console_main, &cnano.UART, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* spi */
    picoRTOS_task_init(&task, spi_main, &cnano.SPI, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* adc */
    picoRTOS_task_init(&task, adc_main, &cnano.AIN04, stack3, PICORTOS_STACK_COUNT(stack3));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* wdt */
    picoRTOS_task_init(&task, wd_main, &cnano.WDT, stack4, PICORTOS_STACK_COUNT(stack4));
    picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());

    /* start the scheduler */
    picoRTOS_start();

    /* we're not supposed to end here */
    picoRTOS_assert_void(false);
    return -1;
}
