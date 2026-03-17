#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

#define ADDR_ITCM           0x00000000
#define ADDR_INTERNAL_FLASH 0x00400000
#define ADDR_ROM            0x00800000

#define ADDR_HSMCI   0x40000000
#define ADDR_SSC     0x40004000
#define ADDR_SPI0    0x40008000
#define ADDR_TC0     0x4000c000
#define ADDR_TC0_CH0 ADDR_TC0
#define ADDR_TC0_CH1 0x4000c040
#define ADDR_TC0_CH2 0x4000c080
#define ADDR_TC1     0x40010000
#define ADDR_TC1_CH0 ADDR_TC1
#define ADDR_TC1_CH1 0x40010040
#define ADDR_TC1_CH2 0x40010080
#define ADDR_TC2     0x40014000
#define ADDR_TC2_CH0 ADDR_TC2
#define ADDR_TC2_CH1 0x40014040
#define ADDR_TC2_CH2 0x40014080
#define ADDR_TWIHS0  0x40018000
#define ADDR_TWIHS1  0x4001c000
#define ADDR_PWM0    0x40020000
#define ADDR_USART0  0x40024000
#define ADDR_USART1  0x40028000
#define ADDR_USART2  0x4002c000
#define ADDR_MCAN0   0x40030000
#define ADDR_MCAN1   0x40034000
#define ADDR_USBHS   0x40038000
#define ADDR_AFEC0   0x4003c000
#define ADDR_DACC    0x40040000
#define ADDR_ACC     0x40044000
#define ADDR_ICM     0x40048000
#define ADDR_ISI     0x4004c000
#define ADDR_GMAC    0x40050000
#define ADDR_TC3     0x40054000
#define ADDR_TC3_CH0 ADDR_TC3
#define ADDR_TC3_CH1 0x40054040
#define ADDR_TC3_CH2 0x40054080
#define ADDR_SPI1    0x40058000
#define ADDR_PWM1    0x4005c000
#define ADDR_TWIHS2  0x40060000
#define ADDR_AFEC1   0x40064000
#define ADDR_MLB     0x40068000
#define ADDR_AES     0x4006c000
#define ADDR_TRNG    0x40070000
#define ADDR_BRAM    0x40074000
#define ADDR_XDMAC   0x40078000
#define ADDR_QSPI    0x4007c000
#define ADDR_SMC     0x40080000
#define ADDR_SDRAMC  0x40084000
#define ADDR_MATRIX  0x40088000
#define ADDR_I2S0    0x4008c000
#define ADDR_I2S1    0x40090000
#define ADDR_UTMI    0x400e0400
#define ADDR_PMC     0x400e0600
#define ADDR_UART0   0x400e0800
#define ADDR_CHIPID  0x400e0940
#define ADDR_UART1   0x400e0a00
#define ADDR_EFC     0x400e0c00
#define ADDR_PIOA    0x400e0e00
#define ADDR_PIOB    0x400e1000
#define ADDR_PIOC    0x400e1200
#define ADDR_PIOD    0x400e1400
#define ADDR_PIOE    0x400e1600
#define ADDR_RSTC    0x400e1800
#define ADDR_SUPC    0x400e1810
#define ADDR_RTT     0x400e1820
#define ADDR_WDT0    0x400e1850
#define ADDR_RTC     0x400e1860
#define ADDR_GPBR    0x400e1890
#define ADDR_WDT1    0x400e1900
#define ADDR_UART2   0x400e1a00
#define ADDR_UART3   0x400e1c00
#define ADDR_UART4   0x400e1e00

/* IRQs */

#define IRQ(pid) (pid)

#define DEVICE_PID_COUNT  74

#define PID_SUPC         0u
#define PID_RSTC         1u
#define PID_RTC          2u
#define PID_RTT          3u
#define PID_WDT          4u
#define PID_PMC          5u
#define PID_EFC          6u
#define PID_UART0        7u
#define PID_UART1        8u
#define PID_SMC          9u
#define PID_PIOA         10u
#define PID_PIOB         11u
#define PID_PIOC         12u
#define PID_USART0       13u
#define PID_USART1       14u
#define PID_USART2       15u
#define PID_PIOD         16u
#define PID_PIOE         17u
#define PID_HSMCI        18u
#define PID_TWIHS0       19u
#define PID_TWIHS1       20u
#define PID_SPI0         21u
#define PID_SSC          22u
#define PID_TC0_CHANNEL0 23u
#define PID_TC0_CHANNEL1 24u
#define PID_TC0_CHANNEL2 25u
#define PID_TC1_CHANNEL0 26u
#define PID_TC1_CHANNEL1 27u
#define PID_TC1_CHANNEL2 28u
#define PID_AFEC0        29u
#define PID_DACC         30u
#define PID_PWM0         31u
#define PID_ICM          32u
#define PID_ACC          33u
#define PID_USBHS        34u
#define PID_MCAN0        35u
#define PID_MCAN0_INT1   36u
#define PID_MCAN1        37u
#define PID_MCAN1_INT1   38u
#define PID_GMAC         39u
#define PID_AFEC1        40u
#define PID_TWIHS2       41u
#define PID_SPI1         42u
#define PID_QSPI         43u
#define PID_UART2        44u
#define PID_UART3        45u
#define PID_UART4        46u
#define PID_TC2_CHANNEL0 47u
#define PID_TC2_CHANNEL1 48u
#define PID_TC2_CHANNEL2 49u
#define PID_TC3_CHANNEL0 50u
#define PID_TC3_CHANNEL1 51u
#define PID_TC3_CHANNEL2 52u
#define PID_MLB_IRQ0     53u
#define PID_MLB_IRQ1     54u
#define PID_AES          56u
#define PID_TRNG         57u
#define PID_XDMAC        58u
#define PID_ISI          59u
#define PID_PWM1         60u
#define PID_ARM_FPU      61u
#define PID_SDRAMC       62u
#define PID_RSWDT        63u
#define PID_ARM_CCW      64u
#define PID_ARM_CCF      65u
#define PID_GMAC_Q1      66u
#define PID_GMAC_Q2      67u
#define PID_ARM_IXC      68u
#define PID_I2SC0        69u
#define PID_I2SC1        70u
#define PID_GMAC_Q3      71u
#define PID_GMAC_Q4      72u
#define PID_GMAC_Q5      73u

/* DEVICE */

#define DEVICE_INTERRUPT_VECTOR_COUNT DEVICE_PID_COUNT
#define DEVICE_MCAN_RAM_WORD_COUNT    4352

/* Clocks after reset
 * A Main RC oscillator (4/8/12 MHz) with a fast startup time and
 * that is selected by default to start the system
 * After reset, the Main RC oscillator is enabled with the 12 MHz frequency selected
 */
#define DEVICE_DEFAULT_SYSCLK_HZ 12000000

#endif
