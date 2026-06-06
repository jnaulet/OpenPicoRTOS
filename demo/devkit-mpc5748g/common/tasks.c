#include "tasks.h"
#include "devkit-mpc5748g.h"

#include "ipc/picoRTOS_mutex.h"
#include "ipc/picoRTOS_cond.h"

#define LED_DELAY_SHORT PICORTOS_DELAY_MSEC(30)
#define LED_DELAY_LONG  PICORTOS_DELAY_MSEC(60)

void tick_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));
    picoRTOS_mpu_add_region(priv, sizeof(struct gpio), MM_URW);

    struct gpio *TICK = gpio_claim((struct gpio*)priv);

    for (;;) {
        gpio_toggle(TICK);
        picoRTOS_schedule();
    }
}

/* IPC test */
static struct picoRTOS_mutex UNPRIVILEGED_DATA mutex = PICORTOS_MUTEX_INITIALIZER;
static struct picoRTOS_cond UNPRIVILEGED_DATA cond = PICORTOS_COND_INITIALIZER;
static struct picoRTOS_cond UNPRIVILEGED_DATA cond_r = PICORTOS_COND_INITIALIZER;

static void gpio_write_sleep(struct gpio *gpio, bool enabled, picoRTOS_tick_t delay)
{
    gpio_write(gpio, enabled);
    picoRTOS_sleep(delay);
}

void led0_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));

    picoRTOS_tick_t ref = picoRTOS_get_tick();
    struct gpio_led *VU = (struct gpio_led*)priv;

    picoRTOS_mpu_add_region(VU, sizeof(*VU), MM_URW);
    (void)gpio_claim(&VU->LED[0]);
    (void)gpio_claim(&VU->LED[1]);
    (void)gpio_claim(&VU->LED[2]);
    (void)gpio_claim(&VU->LED[3]);

    for (;;) {
        picoRTOS_mutex_lock(&mutex);

        /* turn on */
        gpio_write_sleep(&VU->LED[0], false, LED_DELAY_SHORT);
        gpio_write_sleep(&VU->LED[1], false, LED_DELAY_SHORT);
        gpio_write_sleep(&VU->LED[2], false, LED_DELAY_SHORT);
        gpio_write_sleep(&VU->LED[3], false, LED_DELAY_SHORT);

        /* signal to task led1 */
        picoRTOS_cond_signal(&cond);
        picoRTOS_mutex_unlock(&mutex);

        /* force to next task */
        picoRTOS_schedule();

        /* wait for led1 to finish */
        picoRTOS_mutex_lock(&mutex);
        picoRTOS_cond_wait(&cond_r, &mutex);

        /* turn off */
        gpio_write_sleep(&VU->LED[3], true, LED_DELAY_LONG);
        gpio_write_sleep(&VU->LED[2], true, LED_DELAY_LONG);
        gpio_write_sleep(&VU->LED[1], true, LED_DELAY_LONG);
        gpio_write_sleep(&VU->LED[0], true, LED_DELAY_LONG);

        picoRTOS_mutex_unlock(&mutex);
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

void led1_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));

    struct gpio_led *VU = (struct gpio_led*)priv;

    picoRTOS_mpu_add_region(VU, sizeof(*VU), MM_URW);
    (void)gpio_claim(&VU->LED[4]);
    (void)gpio_claim(&VU->LED[5]);
    (void)gpio_claim(&VU->LED[6]);
    (void)gpio_claim(&VU->LED[7]);

    for (;;) {
        picoRTOS_mutex_lock(&mutex);
        picoRTOS_cond_wait(&cond, &mutex);

        /* turn on */
        gpio_write_sleep(&VU->LED[4], false, LED_DELAY_SHORT);
        gpio_write_sleep(&VU->LED[5], false, LED_DELAY_SHORT);
        gpio_write_sleep(&VU->LED[6], false, LED_DELAY_SHORT);
        gpio_write_sleep(&VU->LED[7], false, LED_DELAY_SHORT);

        /* turn off */
        gpio_write_sleep(&VU->LED[7], true, LED_DELAY_LONG);
        gpio_write_sleep(&VU->LED[6], true, LED_DELAY_LONG);
        gpio_write_sleep(&VU->LED[5], true, LED_DELAY_LONG);
        gpio_write_sleep(&VU->LED[4], true, LED_DELAY_LONG);

        /* signal to task led0 */
        picoRTOS_cond_signal(&cond_r);
        picoRTOS_mutex_unlock(&mutex);
    }
}

void adc_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));
    picoRTOS_mpu_add_region(priv, sizeof(struct adc_pwm), MM_URW);

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
void spi_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));
    picoRTOS_mpu_add_region(priv, sizeof(struct spi), MM_URW);

    size_t xfered = 0;
    struct spi *SPI = spi_claim((struct spi*)priv);

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
void can_main(void *priv)
{
#define CAN_TEST_ID 0x6

    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));
    picoRTOS_mpu_add_region(priv, sizeof(struct can), MM_URW);

    struct can *CAN = can_claim((struct can*)priv);

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
void console_main(void *priv)
{
    picoRTOS_assert(priv != NULL, picoRTOS_kill(EINVAL));
    picoRTOS_mpu_add_region(priv, sizeof(struct uart), MM_URW);

    struct uart *UART = uart_claim((struct uart*)priv);

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
