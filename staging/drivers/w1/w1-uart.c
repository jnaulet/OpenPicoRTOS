#include "w1-uart.h"
#include <picoRTOS.h>

int w1_uart_init(/*@out@*/ struct w1 *ctx, struct uart *uart)
{
    ctx->uart = uart;
    ctx->tx_bit = 0;
    ctx->rx_bit = 0;
    ctx->state = W1_UART_STATE_IDLE;
    ctx->reset_state = W1_UART_RESET_STATE_PULSE;

    return 0;
}

static int w1_reset_pulse(struct w1 *ctx)
{
    int res;

    if ((res = uart_write(ctx->uart, "\x00", (size_t)1)) < 0)
        return res;

    /* flush ? */
    // static unsigned char b = '\0';
    // (void)uart_read(ctx->uart, &b, sizeof(b));

    ctx->reset_state = W1_UART_RESET_STATE_PRESENCE;
    return -EAGAIN;
}

static int w1_reset_presence(struct w1 *ctx)
{
    int res;
    static char b = '\0';

    /* slave will send random pulse that should trigger rx */
    if ((res = uart_read(ctx->uart, &b, sizeof(b))) < 0)
        return res;

    ctx->reset_state = W1_UART_RESET_STATE_PULSE;
    return 0;
}

int w1_reset(struct w1 *ctx)
{
    int res;
    static const struct uart_settings RESET_settings = {
        18000ul, (size_t)8, UART_PAR_NONE, UART_CSTOPB_1BIT
    };

    if (ctx->state != W1_UART_STATE_RESET &&
        (res = uart_setup(ctx->uart, &RESET_settings)) < 0)
        return res;

    ctx->state = W1_UART_STATE_RESET;

    switch (ctx->reset_state) {
    case W1_UART_RESET_STATE_PULSE: return w1_reset_pulse(ctx);
    case W1_UART_RESET_STATE_PRESENCE: return w1_reset_presence(ctx);
    default: break;
    }

    picoRTOS_assert_void(false);
    /*@notreached@*/ return -EINVAL;
}

static int write_zero(struct w1 *ctx)
{
    int res;
    static char b = '\0';

    static const struct uart_settings ZERO_settings = {
        120000ul, (size_t)8, UART_PAR_NONE, UART_CSTOPB_1BIT
    };

    if (ctx->state != W1_UART_STATE_WRITE_0 &&
        (res = uart_setup(ctx->uart, &ZERO_settings)) < 0)
        return res;

    ctx->state = W1_UART_STATE_WRITE_0;
    if ((res = uart_write(ctx->uart, "\xf0", (size_t)1)) < 0)
        return res;

    /* flush */
    (void)uart_read(ctx->uart, &b, sizeof(b));
    return 1;
}

static int write_one(struct w1 *ctx)
{
    int res;
    static char b = '\0';

    static const struct uart_settings ONE_settings = {
        1000000ul, (size_t)8, UART_PAR_NONE, UART_CSTOPB_1BIT
    };

    if (ctx->state != W1_UART_STATE_WRITE_1 &&
        (res = uart_setup(ctx->uart, &ONE_settings)) < 0)
        return res;

    ctx->state = W1_UART_STATE_WRITE_1;
    if ((res = uart_write(ctx->uart, "\xff", (size_t)1)) < 0)
        return res;

    /* flush */
    (void)uart_read(ctx->uart, &b, sizeof(b));
    return 1;
}

int w1_write(struct w1 *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int res;

    if ((*(uint8_t*)buf & (1u << ctx->tx_bit)) != 0) res = write_one(ctx);
    else res = write_zero(ctx);

    if (res == 1 && ++ctx->tx_bit == 8u) {
        ctx->tx_bit = 0;
        return res;
    }

    return -EAGAIN;
}

static int read_pulse(struct w1 *ctx)
{
    int res;
    char b = '\0';

    static const struct uart_settings PULSE_settings = {
        120000ul, (size_t)8, UART_PAR_NONE, UART_CSTOPB_1BIT
    };

    if (ctx->state != W1_UART_STATE_READ &&
        (res = uart_setup(ctx->uart, &PULSE_settings)) < 0)
        return res;

    ctx->state = W1_UART_STATE_READ;
    if ((res = uart_write(ctx->uart, "\x00", (size_t)1)) < 0)
        return res;

    /* rx has something for us */
    (void)uart_read(ctx->uart, &b, sizeof(b));

    if (b != '\0') return 1;
    return 0;
}

int w1_read(struct w1 *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int res;

    if ((res = read_pulse(ctx)) < 0)
        return res;

    /* write buf */
    *(uint8_t*)buf |= ((uint8_t)res << ctx->rx_bit);

    if (++ctx->rx_bit == 8u) {
        ctx->rx_bit = 0;
        return 1;
    }

    return -EAGAIN;
}
