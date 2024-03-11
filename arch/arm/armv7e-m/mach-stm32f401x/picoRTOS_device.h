#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

/* AHB2 */
#define ADDR_USBOTGFS 0x50000000
/* AHB1 */
#define ADDR_DMA2  0x40026400
#define ADDR_DMA1  0x40026000
#define ADDR_FIR   0x40023c00
#define ADDR_RCC   0x40023800
#define ADDR_CRC   0x40023000
#define ADDR_GPIOH 0x40021c00
#define ADDR_GPIOE 0x40021000
#define ADDR_GPIOD 0x40020c00
#define ADDR_GPIOC 0x40020800
#define ADDR_GPIOB 0x40020400
#define ADDR_GPIOA 0x40020000
/* APB2 */
#define ADDR_TIM11  0x40014800
#define ADDR_TIM10  0x40014400
#define ADDR_TIM9   0x40014000
#define ADDR_EXTI   0x40013c00
#define ADDR_SYSCFG 0x40013800
#define ADDR_SPI4   0x40013400
#define ADDR_SPI1   0x40013000
#define ADDR_SDIO   0x40012c00
#define ADDR_ADC1   0x40012000
#define ADDR_USART6 0x40011400
#define ADDR_USART1 0x40011000
#define ADDR_TIM1   0x40010000
/* APB1 */
#define ADDR_PWR       0x40007000
#define ADDR_I2C3      0x40005c00
#define ADDR_I2C2      0x40005800
#define ADDR_I2C1      0x40005400
#define ADDR_USART2    0x40004400
#define ADDR_I2S3EXT   0x40004000
#define ADDR_SPI3_I2S3 0x40003c00
#define ADDR_SPI2_I2S2 0x40003800
#define ADDR_I2S2EXT   0x40003400
#define ADDR_IWDG      0x40003000
#define ADDR_WWDG      0x40002c00
#define ADDR_RTC_BKP   0x40002800
#define ADDR_TIM5      0x40000c00
#define ADDR_TIM4      0x40000800
#define ADDR_TIM3      0x40000400
#define ADDR_TIM2      0x40000000

/* IRQs */

#define IRQ_WWDG                0
#define IRQ_EXTI16_PVD          1
#define IRQ_EXTI21_TAMP_STAMP   2
#define IRQ_EXTI22_RTC_WKUP     3
#define IRQ_FLASH               4
#define IRQ_RCC                 5
#define IRQ_EXTI0               6
#define IRQ_EXTI1               7
#define IRQ_EXTI2               8
#define IRQ_EXTI3               9
#define IRQ_EXTI4               10
#define IRQ_DMA1_STREAM0        11
#define IRQ_DMA1_STREAM1        12
#define IRQ_DMA1_STREAM2        13
#define IRQ_DMA1_STREAM3        14
#define IRQ_DMA1_STREAM4        15
#define IRQ_DMA1_STREAM5        16
#define IRQ_DMA1_STREAM6        17
#define IRQ_ADC                 18
#define IRQ_EXTI9_5             23
#define IRQ_TIM1_BRK_TIM9       24
#define IRQ_TIM1_UP_TIM10       25
#define IRQ_TIM1_TRG_COM_TIM11  26
#define IRQ_TIM1_CC             27
#define IRQ_TIM2                28
#define IRQ_TIM3                29
#define IRQ_TIM4                30
#define IRQ_I2C1_EV             31
#define IRQ_I2C1_ER             32
#define IRQ_I2C2_EV             33
#define IRQ_I2C2_ER             34
#define IRQ_SPI1                35
#define IRQ_SPI2                36
#define IRQ_USART1              37
#define IRQ_USART2              38
#define IRQ_EXTI15_10           40
#define IRQ_EXTI17_RTC_ALARM    41
#define IRQ_EXTI18_OTG_FS_WKUP  42
#define IRQ_DMA1_STREAM7        47
#define IRQ_SDIO                49
#define IRQ_TIM5                50
#define IRQ_SPI3                51
#define IRQ_DMA2_STREAM0        56
#define IRQ_DMA2_STREAM1        57
#define IRQ_DMA2_STREAM2        58
#define IRQ_DMA2_STREAM3        59
#define IRQ_DMA2_STREAM4        60
#define IRQ_OTG_FS              67
#define IRQ_DMA2_STREAM5        68
#define IRQ_DMA2_STREAM6        69
#define IRQ_DMA2_STREAM7        70
#define IRQ_USART6              71
#define IRQ_I2C3_EV             72
#define IRQ_I2C3_ER             73
#define IRQ_FPU                 81
#define IRQ_SPI4                84

/* DEVICE */

#define DEVICE_INTERRUPT_VECTOR_COUNT 85
#define DEVICE_DEFAULT_SYSCLK_HZ      16000000 /* HSI, 16Mhz */

#endif
