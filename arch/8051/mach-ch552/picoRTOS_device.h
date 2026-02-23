#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

#define ADDR_ADC_CTRL   0x80
#define ADDR_SP         0x81
#define ADDR_DPL        0x82
#define ADDR_DPH        0x83
#define ADDR_ROM_ADDR_L 0x84
#define ADDR_ROM_ADDR_H 0x85
#define ADDR_ROM_CTRL   0x86
#define ADDR_ROM_STATUS 0x86
#define ADDR_PCON       0x87
#define ADDR_TCON       0x88
#define ADDR_TMOD       0x89
#define ADDR_TL0        0x8a
#define ADDR_TL1        0x8b
#define ADDR_TH0        0x8c
#define ADDR_TH1        0x8d
#define ADDR_ROM_DATA_L 0x8e
#define ADDR_ROM_DATA_H 0x8f
#define ADDR_P1         0x90
#define ADDR_USB_C_CTRL 0x91
#define ADDR_P1_MOD_OC  0x92
#define ADDR_P1_DIR_PU  0x93
#define ADDR_P3_MOD_OC  0x96
#define ADDR_P3_DIR_PU  0x97
#define ADDR_SCON       0x98
#define ADDR_SBUF       0x99
#define ADDR_ADC_CFG    0x9a
#define ADDR_PWM_DATA2  0x9b
#define ADDR_PWM_DATA1  0x9c
#define ADDR_PWM_CTRL   0x9d
#define ADDR_PWM_CK_SE  0x9e
#define ADDR_ADC_DATA   0x9f
#define ADDR_P2         0xa0
#define ADDR_CHIP_ID    0xa1
#define ADDR_SAFE_MOD   0xa1
#define ADDR_XBUS_AUX   0xa2
#define ADDR_IE         0xa8
#define ADDR_WAKE_CTRL  0xa9
#define ADDR_P3         0xb0
#define ADDR_GLOBAL_CFG 0xb1
#define ADDR_IP         0xb8
#define ADDR_CLOCK_CFG  0xb9
#define ADDR_SCON1      0xc0
#define ADDR_SBUF1      0xc1
#define ADDR_SBAUD1     0xc2
#define ADDR_TKEY_CTRL  0xc3
#define ADDR_TKEY_DATL  0xc4
#define ADDR_TKEY_DATH  0xc5
#define ADDR_PIN_FUNC   0xc6
#define ADDR_GPIO_IE    0xc7
#define ADDR_T2CON      0xc8
#define ADDR_T2MOD      0xc9
#define ADDR_RCAP2L     0xca
#define ADDR_RCAP2H     0xcb
#define ADDR_TL2        0xcc
#define ADDR_TH2        0xcd
#define ADDR_T2CAP1L    0xce
#define ADDR_T2CAP1H    0xcf
#define ADDR_PSW        0xd0
#define ADDR_UDEV_CTRL  0xd1
#define ADDR_UEP1_CTRL  0xd2
#define ADDR_UEP1_T_LEN 0xd3
#define ADDR_UEP2_CTRL  0xd4
#define ADDR_UEP2_T_LEN 0xd5
#define ADDR_UEP3_CTRL  0xd6
#define ADDR_UEP3_T_LEN 0xd7
#define ADDR_USB_INT_FG 0xd8
#define ADDR_USB_INT_ST 0xd9
#define ADDR_USB_MIS_ST 0xda
#define ADDR_USB_RX_LEN 0xdb
#define ADDR_UEP0_CTRL  0xdc
#define ADDR_UEP0_T_LEN 0xdd
#define ADDR_UEP4_CTRL  0xde
#define ADDR_UEP4_T_LEN 0xdf
#define ADDR_ACC        0xe0
#define ADDR_USB_INT_EN 0xe1
#define ADDR_USB_CTRL   0xe2
#define ADDR_USB_DEV_AD 0xe3
#define ADDR_UEP2_DMAL  0xe4
#define ADDR_UEP2_DMAH  0xe5
#define ADDR_UEP3_DMAL  0xe6
#define ADDR_UEP3_DMAH  0xe7
#define ADDR_IE_EX      0xe8
#define ADDR_IP_EX      0xe9
#define ADDR_UEP4_1_MOD 0xea
#define ADDR_UEP2_3_MOD 0xeb
#define ADDR_UEP0_DMAL  0xec
#define ADDR_UEP0_DMAH  0xed
#define ADDR_UEP1_DMAL  0xee
#define ADDR_UEP1_DMAH  0xef
#define ADDR_B          0xf0
#define ADDR_SPI0_STAT  0xf8
#define ADDR_SPI0_DATA  0xf9
#define ADDR_SPI0_CTRL  0xfa
#define ADDR_SPI0_CK_SE 0xfb
#define ADDR_SPI0_S_PRE 0xfb
#define ADDR_SPI0_SETUP 0xfc
#define ADDR_RESET_KEEP 0xfe
#define ADDR_WDOG_COUNT 0xff

/* IRQs */

#define IRQ_INT0  0     /* External interrupt 0 */
#define IRQ_TMR0  1     /* Timer 0 interrupt */
#define IRQ_INT1  2     /* External interrupt 1 */
#define IRQ_TMR1  3     /* Timer 1 interrupt */
#define IRQ_UART0 4     /* UART0 interrupt */
#define IRQ_TMR2  5     /* Timer 2 interrupt */
#define IRQ_SPI0  6     /* SPI0 interrupt */
#define IRQ_TKEY  7     /* Touch key timer interrupt */
#define IRQ_USB   8     /* USB interrupt */
#define IRQ_ADC   9     /* ADC interrupt */
#define IRQ_UART1 10    /* UART1 interrupt */
#define IRQ_PWMX  11    /* PWM1/PWM2 interrupt */
#define IRQ_GPIO  12    /* GPIO Interrupt */
#define IRQ_WDOG  13    /* Watchdog timer interrupt */

#define DEVICE_INTERRUPT_VECTOR_COUNT 14
#define DEVICE_DEFAULT_SYSCLK_HZ      6000000ul

#endif
