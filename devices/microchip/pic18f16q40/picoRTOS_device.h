#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_NVM      0x0040
#define ADDR_VIC      0x0367
#define ADDR_OSC      0x00ac
#define ADDR_CRC      0x034e
#define ADDR_BOR      0x0049
#define ADDR_WWDT     0x0078
#define ADDR_DMA      0x00e8
#define ADDR_PMD      0x0063
#define ADDR_IOPORTS  0x0286
#define ADDR_IOC      0x0405
#define ADDR_PPS      0x0200
#define ADDR_CLC      0x00d4
#define ADDR_CLKREF   0x0039
#define ADDR_TMR0     0x0318
#define ADDR_TMR1     0x0312
#define ADDR_TMR3     0x0323
#define ADDR_TMR2     0x031c
#define ADDR_TMR4     0x0329
#define ADDR_SMT      0x0300
#define ADDR_CCP      0x0340
#define ADDR_CCPTMRS0 0x034c
#define ADDR_PWM      0x0460
#define ADDR_CWG      0x03bc
#define ADDR_NCO      0x0440
#define ADDR_DSM      0x006a
#define ADDR_UART     0x02a1
#define ADDR_SPI      0x0080
#define ADDR_I2C      0x028a
#define ADDR_HVLD     0x004a
#define ADDR_FVR      0x03d7
#define ADDR_ADC      0x03d8
#define ADDR_DAC      0x007d
#define ADDR_CMP      0x006f
#define ADDR_ZCD      0x004c

/* DEVICE INFO */

#define DEVICE_INTERRUPT_VECTOR_COUNT 2

#endif
