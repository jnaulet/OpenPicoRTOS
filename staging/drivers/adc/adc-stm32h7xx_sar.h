#ifndef ADC_STM32H7XX_SAR_H
#define ADC_STM32H7XX_SAR_H

#include "adc.h"
#include "dma.h"
#include "clock.h"
#include "picoRTOS_types.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct ADC_STM32H7XX_SAR;
struct ADC_STM32H7XX_SAR_COMMON;

typedef enum {
    ADC_STM32H7XX_SAR_MASTER,
    ADC_STM32H7XX_SAR_SLAVE,
    ADC_STM32H7XX_SAR_COUNT
} adc_stm32h7xx_sar_t;

#define ADC_STM32H7XX_SAR_SEQUENCE_COUNT 16

struct adc_stm32h7xx_sar {
    /* result (clangtidy performance issue) */
    uint32_t DR[ADC_STM32H7XX_SAR_SEQUENCE_COUNT]
    __ATTRIBUTE_ALIGNED__(ARCH_L1_DCACHE_LINESIZE);
    uint32_t DR_mask;
    /* base */
    /*@temp@*/ struct ADC_STM32H7XX_SAR *base;
    /*@temp@*/ struct ADC_STM32H7XX_SAR_COMMON *common;
    clock_id_t clkid;
    /* state machine */
    size_t channel_count;
    /* deps */
    /*@temp@*/ struct dma *drain;
    struct dma_xfer drain_xfer;
};

typedef enum {
    ADC_STM32H7XX_SAR_PRESC_DIV1    = 0,
    ADC_STM32H7XX_SAR_PRESC_DIV2    = 1,
    ADC_STM32H7XX_SAR_PRESC_DIV4    = 2,
    ADC_STM32H7XX_SAR_PRESC_DIV6    = 3,
    ADC_STM32H7XX_SAR_PRESC_DIV8    = 4,
    ADC_STM32H7XX_SAR_PRESC_DIV10   = 5,
    ADC_STM32H7XX_SAR_PRESC_DIV12   = 6,
    ADC_STM32H7XX_SAR_PRESC_DIV16   = 7,
    ADC_STM32H7XX_SAR_PRESC_DIV32   = 8,
    ADC_STM32H7XX_SAR_PRESC_DIV64   = 9,
    ADC_STM32H7XX_SAR_PRESC_DIV128  = 10,
    ADC_STM32H7XX_SAR_PRESC_DIV256  = 11,
    ADC_STM32H7XX_SAR_PRESC_COUNT
} adc_stm32h7xx_sar_presc_t;

typedef enum {
    ADC_STM32H7XX_SAR_CKMODE_CK_ADCX        = 0,
    ADC_STM32H7XX_SAR_CKMODE_ADC_SCLK_DIV1  = 1,
    ADC_STM32H7XX_SAR_CKMODE_ADC_SCLK_DIV2  = 2,
    ADC_STM32H7XX_SAR_CKMODE_ADC_SCLK_DIV4  = 3,
    ADC_STM32H7XX_SAR_CKMODE_COUNT
} adc_stm32h7xx_sar_ckmode_t;

typedef enum {
    ADC_STM32H7XX_SAR_CONT_SINGLE       = 0,
    ADC_STM32H7XX_SAR_CONT_CONTINUOUS   = 1,
    ADC_STM32H7XX_SAR_CONT_COUNT
} adc_stm32h7xx_sar_cont_t;

typedef enum {
    ADC_STM32H7XX_SAR_RES_16BITS        = 0,
    ADC_STM32H7XX_SAR_RES_LEGACY_14BITS = 1,
    ADC_STM32H7XX_SAR_RES_LEGACY_12BITS = 2,
    ADC_STM32H7XX_SAR_RES_14BITS        = 5,
    ADC_STM32H7XX_SAR_RES_12BITS        = 6,
    ADC_STM32H7XX_SAR_RES_10BITS        = 3,
    ADC_STM32H7XX_SAR_RES_8BITS         = 7,
    ADC_STM32H7XX_SAR_RES_COUNT
} adc_stm32h7xx_sar_res_t;

#define ADC_STM32H7XX_SAR_OSVR_MIN 1u
#define ADC_STM32H7XX_SAR_OSVR_MAX 1024u

struct adc_stm32h7xx_sar_settings {
    /* common */
    bool tsen;
    adc_stm32h7xx_sar_presc_t presc;
    adc_stm32h7xx_sar_ckmode_t ckmode;
    /* adcx */
    adc_stm32h7xx_sar_cont_t cont;
    adc_stm32h7xx_sar_res_t res;
    /* oversampling */
    unsigned int osvr; /* 1-1024 */
    /* dma-driven only */
    /*@temp@*/ struct dma *drain;
};

int adc_stm32h7xx_sar_init(/*@out@*/ struct adc_stm32h7xx_sar *ctx, int base, clock_id_t clkid, adc_stm32h7xx_sar_t type);
int adc_stm32h7xx_sar_setup(struct adc_stm32h7xx_sar *ctx, const struct adc_stm32h7xx_sar_settings *settings);

/* channels */

#define ADC_STM32H7XX_SAR_ADC_CHANNEL_COUNT 20

struct adc {
    /*@temp@*/ struct adc_stm32h7xx_sar *parent;
    size_t channel;
    size_t index;
    /* calibration */
    int multiplier;
    int divider;
    int offset;
};

typedef enum {
    ADC_STM32H7XX_SAR_ADC_SMP_1_5_CYCLES,
    ADC_STM32H7XX_SAR_ADC_SMP_2_5_CYCLES,
    ADC_STM32H7XX_SAR_ADC_SMP_8_5_CYCLES,
    ADC_STM32H7XX_SAR_ADC_SMP_16_5_CYCLES,
    ADC_STM32H7XX_SAR_ADC_SMP_32_5_CYCLES,
    ADC_STM32H7XX_SAR_ADC_SMP_64_5_CYCLES,
    ADC_STM32H7XX_SAR_ADC_SMP_387_5_CYCLES,
    ADC_STM32H7XX_SAR_ADC_SMP_810_5_CYCLES,
    ADC_STM32H7XX_SAR_ADC_SMP_COUNT
} adc_stm32h7xx_sar_adc_smp_t;

struct adc_stm32h7xx_sar_adc_settings {
    adc_stm32h7xx_sar_adc_smp_t smp;
};

int adc_stm32h7xx_sar_adc_init(/*@out@*/ struct adc *ctx, struct adc_stm32h7xx_sar *parent, size_t channel);
int adc_stm32h7xx_sar_adc_setup(struct adc *ctx, const struct adc_stm32h7xx_sar_adc_settings *settings);

/* Runtime calls:
 * int adc_setup(struct adc *ctx, const struct adc_settings *settings);
 * int adc_read(struct adc *ctx, int *data);
 * int adc_read_multiple(struct adc *ctx, int *data, size_t n);
 */

#endif
