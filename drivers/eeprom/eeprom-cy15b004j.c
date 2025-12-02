#include "eeprom-cy15b004j.h"
#include <picoRTOS.h>

/* Function: eeprom_cy15b004j_init
 * Initializes an i2c EEPROM module
 *
 * Parameters:
 *  ctx - The EEPROM to init
 *  twi - The twi/i2c port to use
 *  device - The Device Select id to use [0-3]
 *
 * Returns:
 * Always 0
 */
int eeprom_cy15b004j_init(struct eeprom *ctx, struct twi *twi, int device)
{
    picoRTOS_assert(device < C99_EEPROM_CY15B004J_DEVICE_COUNT, return -EINVAL);

    ctx->twi = twi;
    ctx->settings.bitrate = 0;
    ctx->settings.mode = TWI_MODE_MASTER;
    ctx->settings.slave_addr = (twi_addr_t)(0x50 | ((uint16_t)device << 1));
    ctx->state = CY15B004J_STATE_SETUP;

    return 0;
}

/*
 * runtime
 */

static int setup(struct eeprom *ctx, size_t addr)
{
    picoRTOS_assert(addr < (size_t)C99_EEPROM_CY15B004J_ADDR_COUNT, return -EINVAL);

    if (addr >= (size_t)256) ctx->settings.slave_addr |= 0x1;
    else ctx->settings.slave_addr &= ~0x1;
    /* quick setup */
    (void)twi_setup(ctx->twi, &ctx->settings);

    ctx->state = CY15B004J_STATE_ADDRESS;
    return -EAGAIN;
}

static int req_address_read(struct eeprom *ctx, size_t addr)
{
    picoRTOS_assert(addr < (size_t)C99_EEPROM_CY15B004J_ADDR_COUNT, return -EINVAL);

    uint8_t addr8 = (uint8_t)addr;

    if (twi_write(ctx->twi, &addr8, sizeof(addr8), TWI_F_S) == 1)
        ctx->state = CY15B004J_STATE_DATA;

    return -EAGAIN;
}

static int data_read(struct eeprom *ctx, void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int res;

    /* send repeated start */
    if ((res = twi_read(ctx->twi, buf, n, TWI_F_S | TWI_F_P)) == (int)n)
        ctx->state = CY15B004J_STATE_SETUP;

    return res;
}

int eeprom_read(struct eeprom *ctx, size_t addr, void *buf, size_t n)
{
    picoRTOS_assert(addr < (size_t)C99_EEPROM_CY15B004J_ADDR_COUNT, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case CY15B004J_STATE_SETUP: return setup(ctx, addr);
    case CY15B004J_STATE_ADDRESS: return req_address_read(ctx, addr);
    case CY15B004J_STATE_DATA: return data_read(ctx, buf, n);
    default: break;
    }

    picoRTOS_assert_void(false);
    /*@notreached@*/ return -EIO;
}

int eeprom_erase(struct eeprom *ctx, size_t addr, size_t n)
{
    picoRTOS_assert(addr < (size_t)C99_EEPROM_CY15B004J_ADDR_COUNT, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);

    /*@i@*/ (void)ctx;
    return -ENOSYS;
}

static int req_address_write(struct eeprom *ctx, size_t addr, size_t n)
{
    picoRTOS_assert(addr < (size_t)C99_EEPROM_CY15B004J_ADDR_COUNT, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);

    uint8_t addr8 = (uint8_t)addr;

    /* avoid repeated start */
    if (twi_write(ctx->twi, &addr8, sizeof(addr8) + n, TWI_F_S | TWI_F_P | TWI_F_N(1)) == 1)
        ctx->state = CY15B004J_STATE_DATA;

    return -EAGAIN;
}

static int data_write(struct eeprom *ctx, const void *buf, size_t n)
{
    picoRTOS_assert(n > 0, return -EINVAL);

    int res;

    if ((res = twi_write(ctx->twi, buf, n, 0)) == (int)n)
        ctx->state = CY15B004J_STATE_SETUP;

    return res;
}

int eeprom_write(struct eeprom *ctx, size_t addr, const void *buf, size_t n)
{
    picoRTOS_assert(addr < (size_t)C99_EEPROM_CY15B004J_ADDR_COUNT, return -EINVAL);
    picoRTOS_assert(n > 0, return -EINVAL);

    switch (ctx->state) {
    case CY15B004J_STATE_SETUP: return setup(ctx, addr);
    case CY15B004J_STATE_ADDRESS: return req_address_write(ctx, addr, n);
    case CY15B004J_STATE_DATA: return data_write(ctx, buf, n);
    default: break;
    }

    picoRTOS_assert_void(false);
    /*@notreached@*/ return -EIO;
}
