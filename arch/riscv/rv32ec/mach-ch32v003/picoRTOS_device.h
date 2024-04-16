#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_EXTEND          0x40023800
#define ADDR_FLASH_INTERFACE 0x40022400
#define ADDR_RCC             0x40021000
#define ADDR_DMA             0x40020000
#define ADDR_USART           0x40013800
#define ADDR_SPI             0x40013000
#define ADDR_TIM1            0x40012c00
#define ADDR_ADC             0x40012400
#define ADDR_PORTD           0x40011400
#define ADDR_PORTC           0x40011000
#define ADDR_PORTA           0x40010800
#define ADDr_EXTI            0x40010400
#define ADDR_AFIO            0x40010000
#define ADDR_PWR             0x40007000
#define ADDR_I2C             0x40005400
#define ADDR_IWDG            0x40003000
#define ADDR_WWDG            0x40002c00
#define ADDR_TIM2            0x40000000

/* IRQs */

#define IRQ_WWDG     0
#define IRQ_PVD      1
#define IRQ_FLASH    2
#define IRQ_RCC      3
#define IRQ_EXTI7_0  4
#define IRQ_AWU      5
#define IRQ_DMA1_CH1 6
#define IRQ_DMA1_CH2 7
#define IRQ_DMA1_CH3 8
#define IRQ_DMA1_CH4 9
#define IRQ_DMA1_CH5 10
#define IRQ_DMA1_CH6 11
#define IRQ_DMA1_CH7 12
#define IRQ_ADC      13
#define IRQ_I2C1_EV  14
#define IRQ_I2C1_ER  15
#define IRQ_USART1   16
#define IRQ_SPI1     17
#define IRQ_TIM1BRK  18
#define IRQ_TIM1UP   19
#define IRQ_TIM1TRG  20
#define IRQ_TIM1CC   21
#define IRQ_TIM2     22

#define DEVICE_INTERRUPT_VECTOR_COUNT 23
#define DEVICE_DEFAULT_SYSCLK_HZ      8000000

/* *INDENT-OFF* */
#ifdef __ASSEMBLER__
    .macro ARCH_ACK_SYSTICK
    la a0, 0xe000f004
    sw zero, 0 (a0)
   .endm
#endif
/* *INDENT-ON* */

/* PERIPHERALS */
/* ARCH */

#endif
