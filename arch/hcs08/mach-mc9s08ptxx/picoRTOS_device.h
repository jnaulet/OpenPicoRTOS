#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/*
 * NXP MC9S08PT60/32
 */

/* MEMORY MAPPING */

#define ADDR_PORT  0x0000
#define ADDR_TSI   0x0008
#define ADDR_ADC   0x0010
#define ADDR_MTIM0 0x0018
#define ADDR_MTIM1 0x001c
#define ADDR_FTM0  0x0020
#define ADDR_ACMP  0x002c
#define ADDR_FTM1  0x0030
#define ADDR_IRQ   0x003b
#define ADDR_KBI0  0x003c
#define ADDR_KBI1  0x003d
#define ADDR_IPC   0x003e
#define ADDR_SIM   0x3000
#define ADDR_SCG   0x300c
#define ADDR_DBG   0x3010
#define ADDR_NVM   0x3020
#define ADDR_WDOG  0x3030
#define ADDR_ICS   0x3038
#define ADDR_PMC   0x3040
#define ADDR_SYS   0x304a
#define ADDR_IPCH  0x3050
#define ADDR_CRC   0x3060
#define ADDR_RTC   0x306a
#define ADDR_IIC   0x3070
#define ADDR_KBI0H 0x307c
#define ADDR_KBI1H 0x307e
#define ADDR_SCI0  0x3080
#define ADDR_SCI1  0x3088
#define ADDR_SCI2  0x3090
#define ADDR_SPI0  0x3098
#define ADDR_SPI1  0x30a0
#define ADDR_ADCH  0x30ac
#define ADDR_PHDE  0x30af
#define ADDR_POE   0x30b0
#define ADDR_PIE   0x30b8
#define ADDR_FTM2  0x30c0
#define ADDR_PF    0x30ec
#define ADDR_PPU   0x30f0
#define ADDR_SYSH  0x30f8

/* IRQs */

/* #define IRQ_VECT(x) (39 - (x)) */

#define IRQ_NVM       0     /* IRQ_VECT(39) */
#define IRQ_KBI1      1     /* IRQ_VECT(38) */
#define IRQ_KBI0      2     /* IRQ_VECT(37) */
#define IRQ_TSI       3     /* IRQ_VECT(36) */
#define IRQ_RTC       4     /* IRQ_VECT(35) */
#define IRQ_IIC       5     /* IRQ_VECT(34) */
#define IRQ_SPI1      6     /* IRQ_VECT(33) */
#define IRQ_SPI0      7     /* IRQ_VECT(32) */
#define IRQ_SCI2TX    8     /* IRQ_VECT(31) */
#define IRQ_SCI2RX    9     /* IRQ_VECT(30) */
#define IRQ_SCI2ERR   10    /* IRQ_VECT(29) */
#define IRQ_SCI1TX    11    /* IRQ_VECT(28) */
#define IRQ_SCI1RX    12    /* IRQ_VECT(27) */
#define IRQ_SCI1ERR   13    /* IRQ_VECT(26) */
#define IRQ_SCI0TX    14    /* IRQ_VECT(25) */
#define IRQ_SCI0RX    15    /* IRQ_VECT(24) */
#define IRQ_SCI0ERR   16    /* IRQ_VECT(23) */
#define IRQ_ADC       17    /* IRQ_VECT(22) */
#define IRQ_ACMP      18    /* IRQ_VECT(21) */
#define IRQ_MTIM1     19    /* IRQ_VECT(20) */
#define IRQ_MTIM0     20    /* IRQ_VECT(19) */
#define IRQ_FTM0OVF   21    /* IRQ_VECT(18) */
#define IRQ_FTM0CH1   22    /* IRQ_VECT(17) */
#define IRQ_FTM0CH0   23    /* IRQ_VECT(16) */
#define IRQ_FTM1OVF   24    /* IRQ_VECT(15) */
#define IRQ_FTM1CH1   25    /* IRQ_VECT(14) */
#define IRQ_FTM1CH0   26    /* IRQ_VECT(13) */
#define IRQ_FTM2OVF   27    /* IRQ_VECT(12) */
#define IRQ_FTM2CH5   28    /* IRQ_VECT(11) */
#define IRQ_FTM2CH4   29    /* IRQ_VECT(10) */
#define IRQ_FTM2CH3   30    /* IRQ_VECT(9) */
#define IRQ_FTM2CH2   31    /* IRQ_VECT(8) */
#define IRQ_FTM2CH1   32    /* IRQ_VECT(7) */
#define IRQ_FTM2CH0   33    /* IRQ_VECT(6) */
#define IRQ_FTM2FAULT 34    /* IRQ_VECT(5) */
#define IRQ_ICS       35    /* IRQ_VECT(4) */
#define IRQ_LVW       36    /* IRQ_VECT(3) */
#define IRQ_WDOG      37    /* IRQ_VECT(2) */
/* #define IRQ_SWI    Reserved */

/* DEVICE INFO */

#define DEVICE_INTERRUPT_VECTOR_COUNT 37
#define DEVICE_DEFAULT_SYSCLK_HZ      8000000ul

#endif
