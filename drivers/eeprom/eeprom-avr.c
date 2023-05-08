#include "eeprom-avr.h"
#include "picoRTOS.h"

struct C99_EEPROM_AVR {
    volatile uint8_t EECR;
    volatile uint8_t EEDR;
    volatile uint16_t EEAR;
};

#define C99_EECR_EEPM_M  0x3u
#define C99_EECR_EEPM(x) (((x) & C99_EECR_EEPM_M) << 4)
#define C99_EECR_EERIE   (1 << 3)
#define C99_EECR_EEMPE   (1 << 2)
#define C99_EECR_EEPE    (1 << 1)
#define C99_EECR_EERE    (1 << 0)

#define SPMCSR_SPMEN (1 << 0)

/* Function: eeprom_avr_init
 * Initializes an EEPROM block
 *
 * Parameters:
 *  ctx - The EEPROM to init
 *  base - The EEPROM base address
 *  SPMCSR - The SPMCSR register base address
 *
 * Returns:
 * Always 0
 */
int eeprom_avr_init(struct eeprom *ctx, int base, int SPMCSR_base)
{
    ctx->base = (struct C99_EEPROM_AVR*)base;
    ctx->SPMCSR = (uint8_t*)SPMCSR_base;

    return 0;
}

int eeprom_read(struct eeprom *ctx, size_t addr, void *buf, size_t n)
{
    if (!picoRTOS_assert(addr < (size_t)C99_EEPROM_AVR_ADDR_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    if ((ctx->base->EECR & C99_EECR_EEPE) != 0)
        return -EAGAIN;

    /* addr */
    ctx->base->EEAR = (uint16_t)addr;
    /* trigger read */
    ctx->base->EECR = (uint8_t)C99_EECR_EERE;

    /* data is available immediately */
    *(uint8_t*)buf = ctx->base->EEDR;
    return 1;
}

int eeprom_erase(struct eeprom *ctx, size_t addr, size_t n)
{
    if (!picoRTOS_assert(addr < (size_t)C99_EEPROM_AVR_ADDR_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    volatile register uint8_t *C99_EECR = &ctx->base->EECR;
    register uint8_t step1 = (uint8_t)(C99_EECR_EEPM(1) | C99_EECR_EEMPE);
    register uint8_t step2 = step1 | C99_EECR_EEPE;

    if ((*C99_EECR & C99_EECR_EEPE) != 0 &&
        (*ctx->SPMCSR & SPMCSR_SPMEN) != 0)
        return -EAGAIN;

    /* addr */
    ctx->base->EEAR = (uint16_t)addr;

    *C99_EECR = step1;  /* master programming enable */
    *C99_EECR = step2;  /* trigger write within 4 cycles */

    return 1;
}

int eeprom_write(struct eeprom *ctx, size_t addr, const void *buf, size_t n)
{
    if (!picoRTOS_assert(addr < (size_t)C99_EEPROM_AVR_ADDR_COUNT)) return -EINVAL;
    if (!picoRTOS_assert(n > 0)) return -EINVAL;

    volatile register uint8_t *C99_EECR = &ctx->base->EECR;
    register uint8_t step2 = (uint8_t)(C99_EECR_EEMPE | C99_EECR_EEPE);

    if ((*C99_EECR & C99_EECR_EEPE) != 0 &&
        (*ctx->SPMCSR & SPMCSR_SPMEN) != 0)
        return -EAGAIN;

    /* addr */
    ctx->base->EEAR = (uint16_t)addr;

    /* data */
    ctx->base->EEDR = *(uint8_t*)buf;
    if (ctx->base->EEDR != *(uint8_t*)buf)
        return -EAGAIN;

    *C99_EECR = (uint8_t)C99_EECR_EEMPE;    /* master programming enable */
    *C99_EECR = step2;                      /* trigger write within 4 cycles */

    return 1;
}
