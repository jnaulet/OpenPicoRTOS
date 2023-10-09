#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

/* MEMORY MAPPING */

/* APBA PERIPHERALS */
#define ADDR_PAC        0x40000000
#define ADDR_PM         0x40000400
#define ADDR_MCLK       0x40000800
#define ADDR_RSTC       0x40000c00
#define ADDR_OSCCTRL    0x40001000
#define ADDR_OSC32KCTRL 0x40001400
#define ADDR_SUPC       0x40001800
#define ADDR_GCLK       0x40001c00
#define ADDR_WDT        0x40002000
#define ADDR_RTC        0x40002400
#define ADDR_EIC        0x40002800
#define ADDR_FREQM      0x40002c00
#define ADDR_SERCOM0    0x40003000
#define ADDR_SERCOM1    0x40003400
#define ADDR_TC0        0x40003800
#define ADDR_TC1        0x40003c00

/* APBB PERIPHERALS */
#define ADDR_USB        0x41000000
#define ADDR_DSU        0x41002000
#define ADDR_NVMCTRL    0x41004000
#define ADDR_CMCC       0x41006000
#define ADDR_PORT       0x41008000
#define ADDR_PORTA      0x41008000
#define ADDR_PORTB      0x41008080
#define ADDR_DMAC       0x4100a000
#define ADDR_EVSYS      0x4100e000
#define ADDR_SERCOM2    0x41012000
#define ADDR_SERCOM3    0x41014000
#define ADDR_TCC0       0x41016000
#define ADDR_TCC1       0x41018000
#define ADDR_TC2        0x4101a000
#define ADDR_TC3        0x4101c000
#define ADDR_RAMECC     0x41020000

/* APBC PERIPHERALS */
#define ADDR_GMAC       0x42000800
#define ADDR_TCC2       0x42000c00
#define ADDR_TCC3       0x42001000
#define ADDR_TC4        0x42001400
#define ADDR_TC5        0x42001800
#define ADDR_PDEC       0x42001c00
#define ADDR_AC         0x42002000
#define ADDR_AES        0x42002400
#define ADDR_TRNG       0x42002800
#define ADDR_ICM        0x42002c00
#define ADDR_PUKCC      0x42003000
#define ADDR_QSPI       0x42003400
#define ADDR_CCL        0x42003800

/* APBD PERIPHERALS */
#define ADDR_SERCOM4    0x43000000
#define ADDR_SERCOM5    0x43000400
#define ADDR_SERCOM6    0x43000800
#define ADDR_SERCOM7    0x43000c00
#define ADDR_TCC4       0x43001000
#define ADDR_TC6        0x43001400
#define ADDR_TC7        0x43001800
#define ADDR_ADC0       0x43001c00
#define ADDR_ADC1       0x43002000
#define ADDR_DAC        0x43002400
#define ADDR_I2S        0x43002800
#define ADDR_PCC        0x43002c00

/* AHB-APB */
#define ADDR_SMARTEEPROM 0x44000000
#define ADDR_SDHC0       0x45000000
#define ADDR_SDHC1       0x46000000
#define ADDR_BACKUP_RAM  0x47000000

/* IRQs */

/* DEVICE */

#define DEVICE_INTERRUPT_VECTOR_COUNT 137

/* Clocks after reset
 * - DFLL48M is enabled and configured to run at 48MHz
 * - Generic Generator 0 uses DFLL48M as source and generates GCLK_MAIN
 * - CPU and BUS clocks are undivided
 */
#define DEVICE_DEFAULT_SYSCLK_HZ 48000000

#endif
