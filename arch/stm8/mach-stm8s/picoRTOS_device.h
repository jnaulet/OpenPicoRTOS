#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/*
 * STMicro STM8S/AF series
 */

/* MEMORY MAPPING */

#define ADDR_OPT0  0x4800
#define ADDR_OPT1  0x4801
#define ADDR_NOPT1 0x4802
#define ADDR_OPT2  0x4803
#define ADDR_NOPT2 0x4804
#define ADDR_OPT3  0x4805
#define ADDR_NOPT3 0x4806
#define ADDR_OPT4  0x4807
#define ADDR_NOPT4 0x4808
#define ADDR_OPT5  0x4809
#define ADDR_NOPT5 0x480a

#define ADDR_PA    0x5000
#define ADDR_PB    0x5005
#define ADDR_PC    0x500a
#define ADDR_PD    0x500f
#define ADDR_PE    0x5014
#define ADDR_PF    0x5019
#define ADDR_FLASH 0x505a
#define ADDR_EXTI  0x50a0
#define ADDR_RST   0x50b3
#define ADDR_CLK   0x50c0
#define ADDR_WWDG  0x50d1
#define ADDR_IWDG  0x50e0
#define ADDR_AWU   0x50f0
#define ADDR_BEEP  0x50f3
#define ADDR_SPI   0x5200
#define ADDR_I2C   0x5210
#define ADDR_UART1 0x5230
#define ADDR_TIM1  0x5250
#define ADDR_TIM2  0x5300
#define ADDR_TIM4  0x5340
#define ADDR_ADC1  0x53e0

/* IRQs */

#define IRQ_TLI       0
#define IRQ_AWU       1
#define IRQ_CLK       2
#define IRQ_EXTI0     3
#define IRQ_EXTI1     4
#define IRQ_EXTI2     5
#define IRQ_EXTI3     6
#define IRQ_EXTI4     7
#define IRQ_SPI      10
#define IRQ_TIM1_CC  11
#define IRQ_TIM1_OVF 12
#define IRQ_TIM2_CC  13
#define IRQ_TIM2_OVF 14
#define IRQ_UART1_TX 17
#define IRQ_UART1_RX 18
#define IRQ_I2C      19
#define IRQ_ADC1     22
#define IRQ_TIM4     23
#define IRQ_FLASH    24

/* DEVICE INFO */

#define DEVICE_INTERRUPT_VECTOR_COUNT 32
#define DEVICE_DEFAULT_SYSCLK_HZ      2000000ul

#endif
