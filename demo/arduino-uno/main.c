#include "picoRTOS.h"
#include "arduino-uno.h"

#include <generated/autoconf.h>

#include "adc.h"
#include "eeprom.h"
#include "gpio.h"
#include "uart.h"
#include "pwm.h"
#include "spi.h"
#include "twi.h"
#include "wd.h"

struct blink {
    struct arduino_uno uno;
};

/*
 * This routine will increment byte 0 of the EEPROM on every startup / reset
 */
static void eeprom_update_info(struct blink *ctx)
{
    uint8_t nreset = 0;
    int deadlock = CONFIG_DEADLOCK_COUNT;

    /* eeprom */
    while (eeprom_read(&ctx->uno.C99_EEPROM, (size_t)0,
                       &nreset, sizeof(nreset)) == -EAGAIN &&
           deadlock-- != 0) {
    }

    picoRTOS_assert_void(deadlock != 1);

    /* increment */
    nreset++;

    /* store */
    while (eeprom_write(&ctx->uno.C99_EEPROM, (size_t)0,
                        &nreset, sizeof(nreset)) == -EAGAIN &&
           deadlock-- != 0) {
    }

    picoRTOS_assert_void(deadlock != 1);
}

static void blink_init(struct blink *ctx)
{
    /* board */
    (void)arduino_uno_init(&ctx->uno);

    /* watchdog */
    (void)wd_start(&ctx->uno.WDT);

    /* eeprom */
    eeprom_update_info(ctx);
}

/*
 * Threads
 */

/*
 * tick_main is a thread that toggles the TICK gpio so you can
 * make measurements on a logical anaylser or oscilloscope and check
 * the scheduling frequency is right
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
 * blink_main put a ramp on a PWM. No blinking possible on this board as the LED
 * is shared with the SPI
 */
static void blink_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

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
 * console_main is a thread that simply echoes characters
 */
static void console_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

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
 * No loopback is present on the arduino, so you must setup a wire between pins 11 & 12
 * If loopback fails, the thread quits and set the spi status to false
 */
static void spi_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    size_t xfered = 0;
    struct spi *SPI = (struct spi*)priv;
    char rx[5] = { (char)0, (char)0, (char)0, (char)0, (char)0 };
    char tx[5] = { (char)0xa5, (char)0x55, (char)0x5a, (char)0x55, (char)0x4d };

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
 * adc_main samples A0 every 40ms and stores the result in sample,
 * A0 must be wired to the 3.3v line and the result should be roughly 0x300
 */
static void adc_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct adc *ADC = (struct adc*)priv;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {

        int sample_mv = 0;

        if (adc_read(ADC, &sample_mv) == -EAGAIN) {
            picoRTOS_schedule();
            continue;
        }

        /* Analog measurement from 3.3v */
        picoRTOS_assert_void(sample_mv > 3200);
        picoRTOS_assert_void(sample_mv < 3400);

        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(40l));
    }
}

/*
 * This thread tests the i2c slave, it reads a byte (0xa5) and send 0x5a back
 */
static void twi_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

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

/*
 * wd_main is a thread that refreshes the internal watchdog.
 * This thread should be the last one by order of priority. If it fails, the system resets
 */
static void wd_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct wd *WDT = (struct wd*)priv;

    for (;;) {
        wd_refresh(WDT);
        picoRTOS_schedule();
    }
}

int main( void )
{
    static struct blink blink;

    picoRTOS_init();
    blink_init(&blink);

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack3[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack4[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack5[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack6[CONFIG_DEFAULT_STACK_COUNT];

    /* tick */
    picoRTOS_task_init(&task, tick_main, &blink.uno.DIGITAL4,
                       stack0, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, (picoRTOS_priority_t)0);
    /* led */
    picoRTOS_task_init(&task, blink_main, &blink.uno.PWM9,
                       stack1, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* serial console */
    picoRTOS_task_init(&task, console_main, &blink.uno.SERIAL,
                       stack2, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* spi loopback */
    picoRTOS_task_init(&task, spi_main, &blink.uno.SPI,
                       stack3, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* adc */
    picoRTOS_task_init(&task, adc_main, &blink.uno.A0,
                       stack4, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* i2c */
    picoRTOS_task_init(&task, twi_main, &blink.uno.I2C,
                       stack5, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* watchdog refresh */
    picoRTOS_task_init(&task, wd_main, &blink.uno.WDT,
                       stack6, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());

    /* Start the scheduler. */
    picoRTOS_start();

    /* we're not supposed to end here */
    picoRTOS_assert_void(false);
    return -1;
}
