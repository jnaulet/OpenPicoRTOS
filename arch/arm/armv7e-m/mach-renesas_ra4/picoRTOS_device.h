#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* Renesas RA4 series memory mapping */

#define ADDR_MMPU    0x40000000
#define ADDR_SMPU    0x40000c00
#define ADDR_SPMON   0x40000d00
#define ADDR_SRAM    0x40002000
#define ADDR_BUS     0x40003000
#define ADDR_DMAC0   0x40005000
#define ADDR_DMAC1   0x40005040
#define ADDR_DMAC2   0x40005080
#define ADDR_DMAC3   0x400050c0
#define ADDR_DMA     0x40005200
#define ADDR_DTC     0x40005400
#define ADDR_ICU     0x40005400
#define ADDR_DBG     0x4001b000
#define ADDR_FCACHE  0x4001c000
#define ADDR_SYSTEM  0x4001e000
#define ADDR_PORT0   0x40040000
#define ADDR_PORT1   0x40040020
#define ADDR_PORT2   0x40040040
#define ADDR_PORT3   0x40040060
#define ADDR_PORT4   0x40040080
#define ADDR_PORT5   0x400400a0
#define ADDR_PORT6   0x400400c0
#define ADDR_PORT7   0x400400e0
#define ADDR_PORT8   0x40040100
#define ADDR_PORT9   0x40040120
#define ADDR_PFS     0x40040800
#define ADDR_PMISC   0x40040d00
#define ADDR_ELC     0x40041000
#define ADDR_POEG    0x40042000
#define ADDR_RTC     0x40044000
#define ADDR_WDT     0x40044200
#define ADDR_IWDT    0x40044400
#define ADDR_CAC     0x40044600
#define ADDR_MSTP    0x40047000
#define ADDR_SSIE0   0x4004e000
#define ADDR_CAN0    0x40050000
#define ADDR_IIC0    0x40053000
#define ADDR_IIC1    0x40053100
#define ADDR_DOC     0x40054100
#define ADDR_ADC140  0x4005c000
#define ADDR_DAC12   0x4005e000
#define ADDR_SCI0    0x40070000
#define ADDR_SCI1    0x40070020
#define ADDR_SCI2    0x40070040
#define ADDR_SCI9    0x40070120
#define ADDR_SPI0    0x40072000
#define ADDR_SPI1    0x40072100
#define ADDR_CRC     0x40074000
#define ADDR_GPT320  0x40078000
#define ADDR_GPT321  0x40078100
#define ADDR_GPT162  0x40078200
#define ADDR_GPT163  0x40078300
#define ADDR_GPT164  0x40078400
#define ADDR_GPT165  0x40078500
#define ADDR_GPT166  0x40078600
#define ADDR_GPT167  0x40078700
#define ADDR_GPT_OPS 0x40078ff0
#define ADDR_KINT    0x40080000
#define ADDR_CTSU    0x40081000
#define ADDR_SLCDC   0x40082000
#define ADDR_AGT0    0x40084000
#define ADDR_AGT1    0x40084100
#define ADDR_ACMPLP  0x40085e00
#define ADDR_OPAMP   0x40086000
#define ADDR_USBFS   0x40090000
#define ADDR_DAC8    0x4009e000
#define ADDR_TSN     0x407ec000

/* IRQs */

/* DEVICE */

#define DEVICE_INTERRUPT_VECTOR_COUNT 174

#endif
