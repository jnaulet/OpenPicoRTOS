#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_SIU      0xc3f90000

#define ADDR_GPIO_PTC 0xc3f90c08
#define ADDR_GPIO_PTD 0xc3f90c0c
#define ADDR_GPIO_PTE 0xc3f90c10
#define ADDR_GPIO_PTF 0xc3f90c14
#define ADDR_GPIO_PTG 0xc3f90c18
#define ADDR_GPIO_PTH 0xc3f90c1c
#define ADDR_GPIO_PTI 0xc3f90c20
#define ADDR_GPIO_PTJ 0xc3f90c24
#define ADDR_GPIO_PTK 0xc3f90c28
#define ADDR_GPIO_PTL 0xc3f90c2c
#define ADDR_GPIO_PTM 0xc3f90c30
#define ADDR_GPIO_PTN 0xc3f90c34
#define ADDR_GPIO_PTO 0xc3f90c38
#define ADDR_GPIO_PTP 0xc3f90c3c

#define ADDR_PIT      0xc3ff0000

#define ADDR_EMIOS0   0xffe64000
#define ADDR_EMIOS1   0xc3fa0000

#define ADDR_PCS_0    0xffe70000

#define ADDR_IGF0     0xffe20000
#define ADDR_IGF1     0xc3e04000

#define ADDR_PLLDIG   0xffe74000

#define ADDR_EDMA0    0xfff44000
#define ADDR_EDMA1    0xfff54000

#define ADDR_EQADC0   0xfff80000
#define ADDR_EQADC1   0xc3e54000

#define ADDR_C55FMC   0xfffe8000

#define ADDR_SEMA4    0xfff24000

#define ADDR_INTC     0xfff48000

#define ADDR_PCU      0xfff50000

/* IRQs */

#define IRQ_SSCIR0 0
/* TBD */

/* DEVICE */

#define DEVICE_INTERRUPT_VECTOR_COUNT 512

/* PERIPHERALS */

/* MUX */
#define MUX_NXP_SIU_PCR_COUNT 512

/* FLEXCAN */
#define CAN_NXP_FLEXCAN_DISABLE_MEM_ERR_DETECTION
#define CAN_NXP_FLEXCAN_MB_COUNT 96

/* C55FMC FLASH */

#define FLASH_NXP_C55FMC_SECTOR_COUNT     18
#define FLASH_NXP_C55FMC_PROGRAMABLE_SIZE 128
#define FLASH_NXP_C55FMC_MAP { \
        { 0x600000ul, 0x603ffful, 1ul, 31ul  }, /* 16KB CSE data block0 */ \
        { 0x604000ul, 0x607ffful, 1ul, 30ul  }, /* 16KB CSE data block1 */ \
        { 0x800000ul, 0x83fffful, 2ul, 0ul  },  /* 256KB boot */ \
        { 0x840000ul, 0x87fffful, 2ul, 1ul  },  /* 256KB application code blockX */ \
        { 0x880000ul, 0x8bfffful, 2ul, 2ul  },  /* 256KB application code blockX */ \
        { 0x8c0000ul, 0x8ffffful, 2ul, 3ul  },  /* 256KB application code blockX */ \
        { 0x900000ul, 0x93fffful, 2ul, 4ul  },  /* 256KB application code blockX */ \
        { 0x940000ul, 0x93fffful, 2ul, 5ul  },  /* 256KB application code */      \
        { 0x980000ul, 0x93fffful, 2ul, 6ul  },  /* 256KB application code */ \
        { 0x9c0000ul, 0x93fffful, 2ul, 7ul  },  /* 256KB application code */ \
        { 0xa00000ul, 0xa3fffful, 2ul, 8ul  },  /* 256KB application code */ \
        { 0xa40000ul, 0xa7fffful, 2ul, 9ul  },  /* 256KB application code */      \
        { 0xa80000ul, 0xabfffful, 2ul, 10ul  }, /* 256KB application code */ \
        { 0xac0000ul, 0xaffffful, 2ul, 11ul  }, /* 256KB application code */ \
        { 0xb00000ul, 0xb3fffful, 2ul, 12ul  }, /* 256KB application code */ \
        { 0xb40000ul, 0xb7fffful, 2ul, 13ul  }, /* 256KB application code */      \
        { 0xb80000ul, 0xbbfffful, 2ul, 14ul  }, /* 256KB application code */ \
        { 0xbc0000ul, 0xbffffful, 2ul, 15ul  }, /* 256KB application code */ }

#endif
