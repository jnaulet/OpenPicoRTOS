#ifndef PICORTOS_DEVICE_H
#define PICORTOS_DEVICE_H

#define ADDR_P0      0x80
#define ADDR_SP      0x81
#define ADDR_DPL     0x82
#define ADDR_DPH     0x83
#define ADDR_RCTRIM0 0x84
#define ADDR_RCTRIM1 0x85
#define ADDR_RWK     0x86
#define ADDR_PCON    0x87
#define ADDR_TCON    0x88
#define ADDR_TMOD    0x89
#define ADDR_TL0     0x8a
#define ADDR_TL1     0x8b
#define ADDR_TH0     0x8c
#define ADDR_TH1     0x8d
#define ADDR_CKCON   0x8e
#define ADDR_WKCON   0x8f
#define ADDR_P1      0x90
#define ADDR_SFRS    0x91
#define ADDR_CAPCON0 0x92
#define ADDR_CAPCON1 0x93
#define ADDR_CAPCON2 0x94
#define ADDR_CKDIV   0x95
#define ADDR_CKSWT   0x96
#define ADDR_CKEN    0x97
#define ADDR_SCON    0x98
#define ADDR_SBUF    0x99
#define ADDR_SBUF_1  0x9a
#define ADDR_EIE     0x9b
#define ADDR_EIE1    0x9c
#define ADDR_CHPCON  0x9f
#define ADDR_P2      0xa0
#define ADDR_AUXR1   0xa2
#define ADDR_BODCON0 0xa3
#define ADDR_IAPTRG  0xa4
#define ADDR_IAPUEN  0xa5
#define ADDR_IAPAL   0xa6
#define ADDR_IAPAH   0xa7
#define ADDR_IE      0xa8
#define ADDR_SADDR   0xa9
#define ADDR_WDCON   0xaa
#define ADDR_BODCON1 0xab
#define ADDR_P3M1    0xac
#define ADDR_P3S     0xac   /* page1 */
#define ADDR_P3M2    0xad
#define ADDR_P3SR    0xad   /* page1 */
#define ADDR_IAPFD   0xae
#define ADDR_IAPCN   0xaf
#define ADDR_P3      0xb0
#define ADDR_P0M1    0xb1
#define ADDR_P0S     0xb1   /* page1 */
#define ADDR_P0M2    0xb2
#define ADDR_P0SR    0xb2   /* page1 */
#define ADDR_P1M1    0xb3
#define ADDR_P1S     0xb3   /* page1 */
#define ADDR_P1M2    0xb4
#define ADDR_P1SR    0xb4   /* page1 */
#define ADDR_P2S     0xb5
#define ADDR_IPH     0xb7
#define ADDR_PWMINTC 0xb7 /* page1 */
#define ADDR_IP      0xb8
#define ADDR_SADEN   0xb9
#define ADDR_SADEN_1 0xba
#define ADDR_SADDR_1 0xbb
#define ADDR_I2DAT   0xbc
#define ADDR_I2STAT  0xbd
#define ADDR_I2CLK   0xbe
#define ADDR_I2TOC   0xbf
#define ADDR_I2CON   0xc0
#define ADDR_I2ADDR  0xc1
#define ADDR_ADCRL   0xc2
#define ADDR_ADCRH   0xc3
#define ADDR_T3CON   0xc4
#define ADDR_PWM4H   0xc4   /* page1 */
#define ADDR_RL3     0xc5
#define ADDR_PWM5H   0xc5   /* page1 */
#define ADDR_RH3     0xc6
#define ADDR_PIOCON1 0xc6   /* page1 */
#define ADDR_TA      0xc7
#define ADDR_T2CON   0xc8
#define ADDR_T2MOD   0xc9
#define ADDR_RCMP2L  0xca
#define ADDR_RCMP2H  0xcb
#define ADDR_TL2     0xcc
#define ADDR_PWM4L   0xcc   /* page1 */
#define ADDR_TH2     0xcd
#define ADDR_PWM5L   0xcd   /* page1 */
#define ADDR_ADCMPL  0xce
#define ADDR_ADCMPH  0xcf
#define ADDR_PSW     0xd0
#define ADDR_PWMPH   0xd1
#define ADDR_PWM0H   0xd2
#define ADDR_PWM1H   0xd3
#define ADDR_PWM2H   0xd4
#define ADDR_PWM3H   0xd5
#define ADDR_PNP     0xd6
#define ADDR_FBD     0xd7
#define ADDR_PWMCON0 0xd8
#define ADDR_PWMPL   0xd9
#define ADDR_PWM0L   0xda
#define ADDR_PWM1L   0xdb
#define ADDR_PWM2L   0xdc
#define ADDR_PWM3L   0xdd
#define ADDR_PIOCON0 0xde
#define ADDR_PWMCON1 0xdf
#define ADDR_ACC     0xe0
#define ADDR_ADCCON1 0xe1
#define ADDR_ADCCON2 0xe2
#define ADDR_ADCDLY  0xe3
#define ADDR_C0L     0xe4
#define ADDR_C0H     0xe5
#define ADDR_C1L     0xe6
#define ADDR_C1H     0xe7
#define ADDR_ADCCON0 0xe8
#define ADDR_PICON   0xe9
#define ADDR_PINEN   0xea
#define ADDR_PIPEN   0xeb
#define ADDR_PIF     0xec
#define ADDR_C2L     0xed
#define ADDR_C2H     0xee
#define ADDR_EIP     0xef
#define ADDR_B       0xf0
#define ADDR_CAPCON3 0xf1
#define ADDR_CAPCON4 0xf2
#define ADDR_SPCR    0xf3
#define ADDR_SPCR2   0xf3 /* page1 */
#define ADDR_SPSR    0xf4
#define ADDR_SPDR    0xf5
#define ADDR_AINDIDS 0xf6
#define ADDR_EIPH    0xf7
#define ADDR_SCON_1  0xf8
#define ADDR_PDTEN   0xf9
#define ADDR_PDTCNT  0xfa
#define ADDR_PMEN    0xfb
#define ADDR_PMD     0xfc
#define ADDR_PORDIS  0xfd
#define ADDR_EIP1    0xfe
#define ADDR_EIPH1   0xff

/* 8051-compatibility layer */

#define ADDR_P0M0 ADDR_P0M2 /* HDR2ASM_SKIP */
#define ADDR_P1M0 ADDR_P1M2 /* HDR2ASM_SKIP */
#define ADDR_P2M0 ADDR_P2M2 /* HDR2ASM_SKIP */
#define ADDR_P3M0 ADDR_P3M2 /* HDR2ASM_SKIP */

/* IRQs */

#define IRQ_IE_0    0   /* External Interrupt 0 */
#define IRQ_TF_0    1   /* Timer 0 overflow */
#define IRQ_IE_1    2   /* External Interrupt 1 */
#define IRQ_TF_1    3   /* Timer 1 overflow */
#define IRQ_RI_TI_0 4   /* Serial port 0 interrupt */
#define IRQ_TF_2    5   /* Timer 2 event */
#define IRQ_SI      6   /* I2C status/timer-out-interrupt */
#define IRQ_PIF     7   /* Pin interrupt */
#define IRQ_BOD     8   /* Brown-out detection interrupt */
#define IRQ_SPI     9   /* SPI interrupt */
#define IRQ_WDTF    10  /* WDT interrupt */
#define IRQ_ADCF    11  /* ADC interrupt */
#define IRQ_CAPF    12  /* Input capture interrupt */
#define IRQ_PWMF    13  /* PWM interrupt */
#define IRQ_FBF     14  /* Fault Brake interrupt */
#define IRQ_RI_TI_1 15  /* Serial port 1 interrupt */
#define IRQ_TF_3    16  /* Timer 3 overflow */
#define IRQ_WKTF    17  /* Self Wake-up Timer interrupt */

#define DEVICE_INTERRUPT_VECTOR_COUNT 18
#define DEVICE_DEFAULT_SYSCLK_HZ      16000000ul

/* selectable systick management */
extern void arch_ack_systick(void);
#define ACK_SYSTICK _arch_ack_systick

#endif
