#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

#define ADDR_ROM       0x0000
#define ADDR_BANK5     0x4000 /* shared */
#define ADDR_SCREEN1   ADDR_BANK5
#define ADDR_COLOR     0x5800
#define ADDR_PRINT_BUF 0x5b00
#define ADDR_SYS_VARS  0x5c00
#define ADDR_MEM_ULA   0x5ccb                   /* shared */
#define ADDR_BANK2     0x8000                   /* non-shared */
#define ADDR_BANKn     0xc000                   /* non-shared, except 5 & 7 */
#define ADDR_MEM_PAGE  0x7ffd                   /* 128k */

#define DEVICE_DEFAULT_SYSCLK_HZ      3500000ul /* 3.5Mhz */
#define DEVICE_INTERRUPT_VECTOR_COUNT 2         /* Hack to avoid warnings */

#endif
