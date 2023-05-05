#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_SEMA42     0xfc03c000

#define ADDR_INTC       0xfc040000

#define ADDR_EDMA       0xfc0a3000

#define ADDR_EMIOS0     0xffc38000
#define ADDR_EMIOS1     0xfbc38000
#define ADDR_EMIOS2     0xffc3c000

#define ADDR_ADC0       0xffe00000
#define ADDR_ADC1       0xfbe00000

#define ADDR_I2C0       0xffe68000
#define ADDR_I2C1       0xfbe68000
#define ADDR_I2C2       0xffe6c000
#define ADDR_I2C3       0xfbe6c000

#define ADDR_DSPI0      0xffe70000
#define ADDR_DSPI1      0xfbe70000
#define ADDR_DSPI2      0xffe74000
#define ADDR_DSPI3      0xfbe74000

#define ADDR_LINFLEXD0  0xffe8c000
#define ADDR_LINFLEXD1  0xfbe8c000
#define ADDR_LINFLEXD2  0xffe90000
#define ADDR_LINFLEXD3  0xfbe90000
#define ADDR_LINFLEXD4  0xffe94000
#define ADDR_LINFLEXD5  0xfbe94000
#define ADDR_LINFLEXD6  0xffe98000
#define ADDR_LINFLEXD7  0xfbe98000
#define ADDR_LINFLEXD8  0xffe9c000
#define ADDR_LINFLEXD9  0xfbe9c000
#define ADDR_LINFLEXD10 0xffea0000
#define ADDR_LINFLEXD11 0xfbea0000
#define ADDR_LINFLEXD12 0xffea4000
#define ADDR_LINFLEXD13 0xfbea4000
#define ADDR_LINFLEXD14 0xffea8000
#define ADDR_LINFLEXD15 0xfbea8000
#define ADDR_LINFLEXD16 0xffeac000
#define ADDR_LINFLEXD17 0xfbeac000

#define ADDR_FLEXCAN0   0xffec0000
#define ADDR_FLEXCAN1   0xfbec0000
#define ADDR_FLEXCAN2   0xffec4000
#define ADDR_FLEXCAN3   0xfbec4000
#define ADDR_FLEXCAN4   0xffec8000
#define ADDR_FLEXCAN5   0xfbec8000
#define ADDR_FLEXCAN6   0xffecc000
#define ADDR_FLEXCAN7   0xfbecc000

#define ADDR_PIT        0xfff84000

#define ADDR_MC_CGM     0xfffb0000
#define ADDR_PLLDIG     0xfffb0080
#define ADDR_MC_ME      0xfffb8000

#define ADDR_SIUL2      0xfffc0000

#define ADDR_GPIO_PTA   0xfffc1700
#define ADDR_GPIO_PTB   0xfffc1702
#define ADDR_GPIO_PTC   0xfffc1704
#define ADDR_GPIO_PTD   0xfffc1706
#define ADDR_GPIO_PTE   0xfffc1708
#define ADDR_GPIO_PTF   0xfffc170a
#define ADDR_GPIO_PTG   0xfffc170c
#define ADDR_GPIO_PTH   0xfffc170e
#define ADDR_GPIO_PTI   0xfffc1710
#define ADDR_GPIO_PTJ   0xfffc1712
#define ADDR_GPIO_PTK   0xfffc1714
#define ADDR_GPIO_PTL   0xfffc1716
#define ADDR_GPIO_PTM   0xfffc1718
#define ADDR_GPIO_PTN   0xfffc171a
#define ADDR_GPIO_PTO   0xfffc171c
#define ADDR_GPIO_PTP   0xfffc171e
#define ADDR_GPIO_PTQ   0xfffc1720

#define ADDR_SPI0       0xfffcc000
#define ADDR_SPI1       0xfbf9c000
#define ADDR_SPI2       0xfffd0000
#define ADDR_SPI3       0xfbfa0000
#define ADDR_SPI4       0xfffd4000
#define ADDR_SPI5       0xfbfa4000

#define ADDR_C55FMC     0xfffe0000

/* IRQs */

#define IRQ_SSCIR0 0
/* TBD */

/* DEVICE */

#define DEVICE_INTERRUPT_VECTOR_COUNT 768
#define DEVICE_HAS_SOFTWARE_CACHE_COHERENCY

/* PERIPHERALS */

/* ADC */
#define ADC_NXP_SAR_CHANNEL_COUNT 96

/* FLEXCAN */
#define CAN_NXP_FLEXCAN_MB_COUNT 96

/* C55FMC FLASH */

#define FLASH_NXP_C55FMC_SECTOR_COUNT     39
#define FLASH_NXP_C55FMC_MAP { \
        { 0x0f8c000ul, 0x0f8fffful, 0ul, 15ul },    /* 16KB block */ \
        { 0x0f90000ul, 0x0f93ffful, 0ul, 31ul },    /* 16KB block */ \
        { 0x0f94000ul, 0x0f97ffful, 0ul, 30ul },    /* 16KB block */ \
        { 0x0f98000ul, 0x0f9bffful, 0ul, 29ul },    /* 16KB block */ \
        { 0x0f9c000ul, 0x0f9fffful, 0ul, 28ul },    /* 16KB block */ \
        { 0x0fa0000ul, 0x0fa3ffful, 0ul, 27ul },    /* 16KB block */ \
        { 0x0fa4000ul, 0x0fa7ffful, 0ul, 26ul },    /* 16KB block */ \
        { 0x0fa8000ul, 0x0fabffful, 0ul, 25ul },    /* 16KB block */ \
        { 0x0fac000ul, 0x0fafffful, 0ul, 24ul },    /* 16KB block */ \
        { 0x0fb0000ul, 0x0fb7ffful, 0ul, 23ul },    /* 32KB block */ \
        { 0x0fb8000ul, 0x0fbfffful, 0ul, 22ul },    /* 32KB block */ \
        { 0x0fc0000ul, 0x0fc7ffful, 0ul, 13ul },    /* 32KB block */ \
        { 0x0fc8000ul, 0x0fcfffful, 0ul, 12ul },    /* 32KB block */ \
        { 0x0fd0000ul, 0x0fd7ffful, 0ul, 11ul },    /* 32KB block */ \
        { 0x0fd8000ul, 0x0fdfffful, 0ul, 10ul },    /* 32KB block */ \
        { 0x0fe0000ul, 0x0fefffful, 0ul, 9ul  },    /* 64KB block */ \
        { 0x0ff0000ul, 0x0fffffful, 0ul, 7ul  },    /* 64KB block */ \
        { 0x1000000ul, 0x103fffful, 2ul, 31ul },    /* 256KB block */ \
        { 0x1040000ul, 0x107fffful, 2ul, 30ul },    /* 256KB block */ \
        { 0x1080000ul, 0x10bfffful, 2ul, 29ul },    /* 256KB block */ \
        { 0x10c0000ul, 0x10ffffful, 2ul, 28ul },    /* 256KB block */ \
        { 0x1100000ul, 0x113fffful, 2ul, 27ul },    /* 256KB block */ \
        { 0x1140000ul, 0x117fffful, 2ul, 26ul },    /* 256KB block */ \
        { 0x1180000ul, 0x11bfffful, 2ul, 25ul },    /* 256KB block */ \
        { 0x11c0000ul, 0x11ffffful, 2ul, 24ul },    /* 256KB block */ \
        { 0x1200000ul, 0x123fffful, 2ul, 23ul },    /* 256KB block */ \
        { 0x1240000ul, 0x127fffful, 2ul, 22ul },    /* 256KB block */ \
        { 0x1280000ul, 0x12bfffful, 2ul, 21ul },    /* 256KB block */ \
        { 0x12c0000ul, 0x12ffffful, 2ul, 20ul },    /* 256KB block */ \
        { 0x1300000ul, 0x133fffful, 2ul, 19ul },    /* 256KB block */ \
        { 0x1340000ul, 0x137fffful, 2ul, 18ul },    /* 256KB block */ \
        { 0x1380000ul, 0x13bfffful, 2ul, 17ul },    /* 256KB block */ \
        { 0x13c0000ul, 0x13ffffful, 2ul, 16ul },    /* 256KB block */ \
        { 0x1400000ul, 0x143fffful, 2ul, 15ul },    /* 256KB block */ \
        { 0x1440000ul, 0x147fffful, 2ul, 14ul },    /* 256KB block */ \
        { 0x1480000ul, 0x14bfffful, 2ul, 13ul },    /* 256KB block */ \
        { 0x14c0000ul, 0x14ffffful, 2ul, 12ul },    /* 256KB block */ \
        { 0x1500000ul, 0x153fffful, 2ul, 11ul },    /* 256KB block */ \
        { 0x1540000ul, 0x157fffful, 2ul, 10ul },    /* 256KB block */ }

#endif
