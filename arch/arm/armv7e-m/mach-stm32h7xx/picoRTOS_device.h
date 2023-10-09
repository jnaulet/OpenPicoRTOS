#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_RAMECC3      0x58027000
#define ADDR_HSEM         0x58026400
#define ADDR_ADC3         0x58026000
#define ADDR_DMAMUX2      0x58025800
#define ADDR_BDMA         0x58025400
#define ADDR_CRC          0x58024c00
#define ADDR_PWR          0x58024800
#define ADDR_RCC          0x58024400
#define ADDR_GPIOK        0x58022800
#define ADDR_GPIOJ        0x58022400
#define ADDR_GPIOI        0x58022000
#define ADDR_GPIOH        0x58021c00
#define ADDR_GPIOG        0x58021800
#define ADDR_GPIOF        0x58021400
#define ADDR_GPIOE        0x58021000
#define ADDR_GPIOD        0x58020c00
#define ADDR_GPIOC        0x58020800
#define ADDR_GPIOB        0x58020400
#define ADDR_GPIOA        0x58020000
#define ADDR_SAI4         0x58005400
#define ADDR_IWDG1        0x58004800
#define ADDR_RTC_BKPT     0x58004000
#define ADDR_VREF         0x58003c00
#define ADDR_COMP1_COMP2  0x58003800
#define ADDR_LPTIM5       0x58003000
#define ADDR_LPTIM4       0x58002c00
#define ADDR_LPTIM3       0x58002800
#define ADDR_LPTIM2       0x58002400
#define ADDR_I2C4         0x58001c00
#define ADDR_SPI6         0x58001400
#define ADDR_LPUART1      0x58000c00
#define ADDR_SYSCFG       0x58000400
#define ADDR_EXTI         0x58000000
#define ADDR_RAMECC1      0x52009000
#define ADDR_DB_SDMMC1    0x52008000
#define ADDR_SDMMC1       0x52007000
#define ADDR_DB_QUADSPI   0x52006000
#define ADDR_QUADSPI_CR   0x52005000
#define ADDR_FMC_CR       0x52004000
#define ADDR_JPEG         0x52003000
#define ADDR_FLASH_IR     0x25002000
#define ADDR_DMA2D        0x52001000
#define ADDR_MDMA         0x52000000
#define ADDR_GPV          0x51000000
#define ADDR_WWDG1        0x50003000
#define ADDR_LTDC         0x50001000
#define ADDR_RAMECC2      0x48023000
#define ADDR_DB_SDMMC2    0x48022800
#define ADDR_SDMMC2       0x48022400
#define ADDR_RNG          0x48021800
#define ADDR_HASH         0x48021400
#define ADDR_CRYPTO       0x48021000
#define ADDR_DCMI         0x48020000
#define ADDR_USB2_OTG_FS  0x40080000
#define ADDR_USB1_OTG_HS  0x40040000
#define ADDR_ETHERNET_MAC 0x40028000
#define ADDR_ADC1_ADC2    0x40022000
#define ADDR_DMAMUX1      0x40020800
#define ADDR_DMA2         0x40020400
#define ADDR_DMA1         0x40020000
#define ADDR_HRTIM        0x40017400
#define ADDR_DFSDM1       0x40017000
#define ADDR_SAI3         0x40016000
#define ADDR_SAI2         0x40015c00
#define ADDR_SAI1         0x40015800
#define ADDR_SPI5         0x40015000
#define ADDR_TIM17        0x40014800
#define ADDR_TIM16        0x40014400
#define ADDR_TIM15        0x40014000
#define ADDR_SPI4         0x40013400
#define ADDR_SPI1_I2S1    0x40013000
#define ADDR_USART6       0x40011400
#define ADDR_USART1       0x40011000
#define ADDR_TIM8         0x40010400
#define ADDR_TIM1         0x40010000
#define ADDR_CAN_MSG_RAM  0x4000ac00
#define ADDR_CAN_CCU      0x4000a800
#define ADDR_FDCAN2       0x4000a400
#define ADDR_FDCAN1       0x4000a000
#define ADDR_MDIOS        0x40009400
#define ADDR_OPAMP        0x40009000
#define ADDR_SWPMI        0x40008800
#define ADDR_CRS          0x40008400
#define ADDR_UART8        0x40007c00
#define ADDR_UART7        0x40007800
#define ADDR_DAC1         0x40007400
#define ADDR_HDMI_CEC     0x40006c00
#define ADDR_I2C3         0x40005c00
#define ADDR_I2C2         0x40005800
#define ADDR_I2C1         0x40005400
#define ADDR_UART5        0x40005000
#define ADDR_UART4        0x40004c00
#define ADDR_USART3       0x40004800
#define ADDR_USART2       0x40004400
#define ADDR_SPDIFRX1     0x40004000
#define ADDR_SPI3_I2S3    0x40003c00
#define ADDR_SPI2_I2S2    0x40003800
#define ADDR_LPTIM1       0x40002400
#define ADDR_TIM14        0x40002000
#define ADDR_TIM13        0x40001c00
#define ADDR_TIM12        0x40001800
#define ADDR_TIM7         0x40001400
#define ADDR_TIM6         0x40001000
#define ADDR_TIM5         0x40000c00
#define ADDR_TIM4         0x40000800
#define ADDR_TIM3         0x40000400
#define ADDR_TIM2         0x40000000

/* DEVICE */

#define DEVICE_INTERRUPT_VECTOR_COUNT 150

/* The HSI block provides the default clock to the product */
#define DEVICE_DEFAULT_SYSCLK_HZ 64000000

/* DMA */
#define DMA_PADDR_BASE ADDR_TIM2

#endif
