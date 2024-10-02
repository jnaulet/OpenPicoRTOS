#ifndef CLOCK_STM8_H
#define CLOCK_STM8_H

#include "clock.h"

#define CLOCK_STM8_CPU    CLOCK_SYSCLK
#define CLOCK_STM8_MASTER (clock_id_t)1
#define CLOCK_STM8_LSI    (clock_id_t)2

typedef enum {
    CLOCK_STM8_CKM_HSI = 0,
    CLOCK_STM8_CKM_HSE = 1,
    CLOCK_STM8_CKM_LSI = 2,
    CLOCK_STM8_CKM_COUNT
} clock_stm8_ckm_t;

typedef enum {
  CLOCK_STM8_HSIDIV_1 = 0,
  CLOCK_STM8_HSIDIV_2 = 1,
  CLOCK_STM8_HSIDIV_4 = 2,
  CLOCK_STM8_HSIDIV_8 = 3,
  CLOCK_STM8_HSIDIV_COUNT
} clock_stm8_hsidiv_t;

typedef enum {
  CLOCK_STM8_CPUDIV_1 = 0,
  CLOCK_STM8_CPUDIV_2 = 1,
  CLOCK_STM8_CPUDIV_4 = 2,
  CLOCK_STM8_CPUDIV_8 = 3,
  CLOCK_STM8_CPUDIV_16 = 4,
  CLOCK_STM8_CPUDIV_32 = 5,
  CLOCK_STM8_CPUDIV_64 = 6,
  CLOCK_STM8_CPUDIV_128 = 7,
  CLOCK_STM8_CPUDIV_COUNT
} clock_stm8_cpudiv_t;

struct clock_settings {
  /* master clock switch */
  clock_stm8_ckm_t ckm;
  unsigned long hse; /* freq in hz */
  clock_stm8_hsidiv_t hsidiv;
  clock_stm8_cpudiv_t cpudiv;
};

int clock_stm8_init(struct clock_settings *settings);

typedef enum {
  CLOCK_PCKEN10 = 0, /* I2C */
  CLOCK_PCKEN11 = 1, /* SPI */
  CLOCK_PCKEN12 = 2, /* UART1/2/3/4 */
  CLOCK_PCKEN13 = 3, /* UART1/2/3/4 */
  CLOCK_PCKEN14 = 4, /* TIM4/TIM6 */
  CLOCK_PCKEN15 = 5, /* TIM2/TIM5 */
  CLOCK_PCKEN16 = 6, /* TIM3 */
  CLOCK_PCKEN17 = 7, /* TIM1 */
  CLOCK_PCKEN22 = 10, /* AWU */
  CLOCK_PCKEN23 = 11, /* ADC */
  CLOCK_PCKEN27 = 15, /* CAN */
  CLOCK_PCKEN_COUNT
} clock_pcken_t;

int clock_stm8_pck_enable(clock_pcken_t pck);
int clock_stm8_pck_disable(clock_pcken_t pck);

/* Implements:
 * clock_freq_t clock_get_freq(clock_id_t clkid);
 */

#endif
