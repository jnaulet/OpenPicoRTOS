#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_PREFETCH 0xbf8e0000
#define ADDR_EBI      0xbf8e1000
#define ADDR_SQI1     0xbf8e2000
#define ADDR_USB      0xbf8e3000
#define ADDR_CRYPTO   0xbf8e5000
#define ADDR_RNG      0xbf8e6000

#define ADDR_CAN1_AND_CAN2 0xbf880000
#define ADDR_CAN1          0xbf880000
#define ADDR_CAN2          0xbf881000

#define ADDR_ETHERNET      0xbf882000
#define ADDR_USBCR         0xbf884000

#define ADDR_PORTA_PORTK 0xbf860000
#define ADDR_PORTA       0xbf860000
#define ADDR_PORTB       0xbf860100
#define ADDR_PORTC       0xbf860200
#define ADDR_PORTD       0xbf860300
#define ADDR_PORTE       0xbf860400
#define ADDR_PORTF       0xbf860500
#define ADDR_PORTG       0xbf860600
#define ADDR_PORTH       0xbf860700
#define ADDR_PORTJ       0xbf860800
#define ADDR_PORTK       0xbf860900

#define ADDR_TIMER1_TIMER9  0xbf840000
#define ADDR_TIMER1         0xbf840000
#define ADDR_TIMER2         0xbf840200
#define ADDR_TIMER3         0xbf840400
#define ADDR_TIMER4         0xbf840600
#define ADDR_TIMER5         0xbf840800
#define ADDR_TIMER6         0xbf840a00
#define ADDR_TIMER7         0xbf840c00
#define ADDR_TIMER8         0xbf840e00
#define ADDR_TIMER9         0xbf841000

#define ADDR_IC1_IC9        0xbf842000
#define ADDR_IC1            0xbf842000
#define ADDR_IC2            0xbf842200
#define ADDR_IC3            0xbf842400
#define ADDR_IC4            0xbf842600
#define ADDR_IC5            0xbf842800
#define ADDR_IC6            0xbf842a00
#define ADDR_IC7            0xbf842c00
#define ADDR_IC8            0xbf842e00
#define ADDR_IC9            0xbf843000

#define ADDR_OC1_OC9        0xbf844000
#define ADDR_OC1            0xbf844000
#define ADDR_OC2            0xbf844200
#define ADDR_OC3            0xbf844400
#define ADDR_OC4            0xbf844600
#define ADDR_OC5            0xbf844800
#define ADDR_OC6            0xbf844a00
#define ADDR_OC7            0xbf844c00
#define ADDR_OC8            0xbf844e00
#define ADDR_OC9            0xbf845000

#define ADDR_ADC            0xbf84b000

#define ADDR_COMPARATOR_1_2 0xbf84c000
#define ADDR_COMPARATOR_1   0xbf84c000
#define ADDR_COMPARATOR_2   0xbf84c010

#define ADDR_I2C1_I2C5   0xbf820000
#define ADDR_I2C1        0xbf820000
#define ADDR_I2C2        0xbf820200
#define ADDR_I2C3        0xbf820400
#define ADDR_I2C4        0xbf820600
#define ADDR_I2C5        0xbf820800

#define ADDR_SPI1_SPI6   0xbf821000
#define ADDR_SPI1        0xbf821000
#define ADDR_SPI2        0xbf821200
#define ADDR_SPI3        0xbf821400
#define ADDR_SPI4        0xbf821600
#define ADDR_SPI5        0xbf821800
#define ADDR_SPI6        0xbf821a00

#define ADDR_UART1_UART6 0xbf822000     /* ! */
#define ADDR_UART1       0xbf822000
#define ADDR_UART2       0xbf822200
#define ADDR_UART3       0xbf822400
#define ADDR_UART4       0xbf822600
#define ADDR_UART5       0xbf822800
#define ADDR_UART6       0xbf822a00

#define ADDR_PMP         0xbf82e000

#define ADDR_INTC 0xbf810000
#define ADDR_DMA  0xbf811000

#define ADDR_CONFIGURATION    0xbf800000
#define ADDR_FLASH_CONTROLLER 0xbf800600
#define ADDR_WATCHDOG_TIMER   0xbf800800
#define ADDR_DEADMAN_TIMER    0xbf800a00
#define ADDR_RTCC             0xbf800c00
#define ADDR_CVREF            0xbf800e00
#define ADDR_OSCILLATOR       0xbf801200
#define ADDR_PPS              0xbf801400

/* CONFIGURATION */

/* initial values */
#define DEVCP0  0xefffffff
#define DEVCFG0 0xbfc00080
#define DEVCFG1 0x00007000
#define DEVCFG2 0xbff88008
#define DEVCFG3 0x84ff0000

/* flags */

/* CP */
#define CP (1 << 28)
/* DEVCFG0 */
#define C99_EJTAGBEN (1 << 30)
#define POSCBOOST    (1 << 21)
#define POSCGAIN_M   0x3u
#define POSCGAIN(x)  (((x) & POSCGAIN_M) << 19)
#define SOSCBOOST    (1 << 18)
#define SOSCGAIN_M   0x3u
#define SOSCGAIN(x)  (((x) & SOSCGAIN_M) << 16)
#define SMCLR        (1 << 15)
#define FSLEEP       (1 << 10)
#define FECCCON_M    0x3u
#define FECCCON(x)   (((x) & FECCCON_M) << 8)
#define BOOTISA      (1 << 6)
#define TRCEN        (1 << 5)
#define ICESEL_M     0x3u
#define ICESEL(x)    (((x) & ICESEL_M) << 3)
#define JTAGEN       (1 << 2)
#define DEBUG_M      0x3u
#define DEBUG(x)     ((x) & DEBUG_M)
/* DEVCFG1 */
#define FDMTEN      (1 << 31)
#define DMTCNT_M    0x1fu
#define DMTCNT(x)   (((x) & DMTCNT_M) << 26)
#define FWDTWINZ_M  0x3u
#define FWDTWINZ(x) (((x) & FWDTWINZ_M) << 24)
#define FWDTEN      (1 << 23)
#define WINDIS      (1 << 22)
#define WDTSPGM     (1 << 21)
#define WDTPS_M     0x1fu
#define WDTPS(x)    (((x) & WDTPS_M) << 16)
#define FCKSM_M     0x3u
#define FCKSM(x)    (((x) & FCKSM_M) << 14)
#define OSCIOFNC    (1 << 10)
#define POSCMOD_M   0x3u
#define POSCMOD(x)  (((x) & POSCMOD_M) << 8)
#define IESO        (1 << 7)
#define FSOSCEN     (1 << 6)
#define DMTINTV_M   0x7u
#define DMTINTV(x)  (((x) & DMTINTV_M) << 3)
#define FNOSC_M     0x7u
#define FNOSC(x)    ((x) & FNOSC_M)
/* DEVCFG2 */
#define UPLLFSEL    (1 << 30)
#define FPLLODIV_M  0x7u
#define FPLLODIV(x) (((x) & FPLLODIV_M) << 16)
#define FPLLMULT_M  0x3fu
#define FPLLMULT(x) (((x) & FPLLMULT_M) << 8)
#define FPLLICLK    (1 << 7)
#define FPLLRNG_M   0x7u
#define FPLLRNG(x)  (((x) & FPLLRNG_M) << 4)
#define FPLLIDIV_M  0x7u
#define FPLLIDIV(x) ((x) & FPLLIDIV_M)
/* DEVCFG3 */
#define FUSBIDIO  (1 << 30)
#define IOL1WAY   (1 << 29)
#define PMDL1WAY  (1 << 28)
#define PGL1WAY   (1 << 27)
#define FETHIO    (1 << 25)
#define FMIIEN    (1 << 24)
#define USERID_M  0xffffu
#define USERID(x) ((x) & USERID_M)

/* Factory programmed flash */

#define ADDR_DEVADCx 0xbfc54000
#define ADDR_DEVSNx  0xbfc54020

/* DEVICE */

#define DEVICE_INTERRUPT_VECTOR_COUNT   213

#define DEVICE_ICACHE_SIZE 16384
#define DEVICE_DCACHE_SIZE 4096

#define DEVICE_ADC_DEDICATED_SAR_COUNT  5
#define DEVICE_ADC_SHARED_CL2_SAR_COUNT 7
#define DEVICE_ADC_SHARED_CL3_SAR_COUNT 33

/* COPROCESSORS */

#define MIPS32_M51XX_HAS_FPU64
#define MIPS32_M51XX_HAS_DSP

/* PERIPHERALS */

#define FLASH_PIC32MX_ADDR_START 0x1d000000
#define FLASH_PIC32MX_PAGE_SIZE  16384
#define FLASH_PIC32MX_ROW_SIZE   2048

#endif
