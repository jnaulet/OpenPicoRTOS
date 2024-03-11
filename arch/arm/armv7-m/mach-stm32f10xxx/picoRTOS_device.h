#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

/* AHB */
#define ADDR_FSMC       0xa0000000
#define ADDR_USB_OTG_FS 0x50000000
#define ADDR_ETHERNET   0x40028000
#define ADDR_CRC        0x40023000
#define ADDR_FMI        0x40022000
#define ADDR_RCC        0x40021000
#define ADDR_DMA2       0x40020400
#define ADDR_DMA1       0x40020000
#define ADDR_SDIO       0x40018000
/* APB2 */
#define ADDR_TIM11  0x40015400
#define ADDR_TIM10  0x40015000
#define ADDR_TIM9   0x40014c00
#define ADDR_ADC3   0x40013c00
#define ADDR_USART1 0x40013800
#define ADDR_TIM8   0x40013400
#define ADDR_SPI1   0x40013000
#define ADDR_TIM1   0x40012c00
#define ADDR_ADC2   0x40012800
#define ADDR_ADC1   0x40012400
#define ADDR_GPIO_G 0x40012000
#define ADDR_GPIO_F 0x40011c00
#define ADDR_GPIO_E 0x40011800
#define ADDR_GPIO_D 0x40011400
#define ADDR_GPIO_C 0x40011000
#define ADDR_GPIO_B 0x40010c00
#define ADDR_GPIO_A 0x40010800
#define ADDR_EXTI   0x40010400
#define ADDR_AFIO   0x40010000
/* APB1 */
#define ADDR_DAC           0x40007400
#define ADDR_PWR           0x40007000
#define ADDR_BKP           0x40006c00
#define ADDR_BXCAN1        0x40006400
#define ADDR_BXCAN2        0x40006800
#define ADDR_SHMEM_USB_CAN 0x40006000
#define ADDR_USB_DEVFS     0x40005c00
#define ADDR_I2C2          0x40005800
#define ADDR_I2C1          0x40005400
#define ADDR_UART5         0x40005000
#define ADDR_UART4         0x40004c00
#define ADDR_USART3        0x40004800
#define ADDR_USART2        0x40004400
#define ADDR_SPI3_I2S      0x40003c00
#define ADDR_SPI2_I2S      0x40003800
#define ADDR_IWDG          0x40003000
#define ADDR_WWDG          0x40002c00
#define ADDR_RTC           0x40002800
#define ADDR_TIM14         0x40002000
#define ADDR_TIM13         0x40001c00
#define ADDR_TIM12         0x40001800
#define ADDR_TIM7          0x40001400
#define ADDR_TIM6          0x40001000
#define ADDR_TIM5          0x40000c00
#define ADDR_TIM4          0x40000800
#define ADDR_TIM3          0x40000400
#define ADDR_TIM2          0x40000000

/* IRQs */

#define IRQ_WWDG         0
#define IRQ_PVD          1
#define IRQ_TAMPER       2
#define IRQ_RTC          3
#define IRQ_FLASH        4
#define IRQ_RCC          5
#define IRQ_EXTI0        6
#define IRQ_EXTI1        7
#define IRQ_EXTI2        8
#define IRQ_EXTI3        9
#define IRQ_EXTI4        10
#define IRQ_DMA1_CH1     11
#define IRQ_DMA1_CH2     12
#define IRQ_DMA1_CH3     13
#define IRQ_DMA1_CH4     14
#define IRQ_DMA1_CH5     15
#define IRQ_DMA1_CH6     16
#define IRQ_DMA1_CH7     17
#define IRQ_ADC1_2       18
#define IRQ_CAN1_TX      19
#define IRQ_CAN1_RX0     20
#define IRQ_CAN1_RX1     21
#define IRQ_CAN1_SCE     22
#define IRQ_EXTI9_5      23
#define IRQ_TIM1_BRK     24
#define IRQ_TIM1_UP      25
#define IRQ_TIM1_TRG_COM 26
#define IRQ_TIM1_CC      27
#define IRQ_TIM2         28
#define IRQ_TIM3         29
#define IRQ_TIM4         30
#define IRQ_I2C1_EV      31
#define IRQ_I2C1_ER      32
#define IRQ_I2C2_EV      33
#define IRQ_I2C2_ER      34
#define IRQ_SPI1         35
#define IRQ_SPI2         36
#define IRQ_USART1       37
#define IRQ_USART2       38
#define IRQ_USART3       39
#define IRQ_EXTI15_10    40
#define IRQ_RTC_ALARM    41
#define IRQ_OTG_FS_WKUP  42
#define IRQ_TIM5         50
#define IRQ_SPI3         51
#define IRQ_UART4        52
#define IRQ_UART5        53
#define IRQ_TIM6         54
#define IRQ_TIM7         55
#define IRQ_DMA2_CH1     56
#define IRQ_DMA2_CH2     57
#define IRQ_DMA2_CH3     58
#define IRQ_DMA2_CH4     59
#define IRQ_DMA2_CH5     60
#define IRQ_ETH          61
#define IRQ_ETH_WKUP     62
#define IRQ_CAN2_TX      63
#define IRQ_CAN2_RX0     64
#define IRQ_CAN2_RX1     65
#define IRQ_CAN2_SCE     66
#define IRQ_OTG_FS       67

/* DEVICE */

#define DEVICE_INTERRUPT_VECTOR_COUNT 68
#define DEVICE_DEFAULT_SYSCLK_HZ      8000000 /* HSI, 8Mhz */

#endif