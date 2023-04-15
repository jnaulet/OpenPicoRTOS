#ifndef MIPS32_REGS_H
#define MIPS32_REGS_H

/* general purpose registers */

#define zero $0
#define at   $1
#define v0   $2
#define v1   $3
#define a0   $4
#define a1   $5
#define a2   $6
#define a3   $7
#define t0   $8
#define t1   $9
#define t2   $10
#define t3   $11
#define t4   $12
#define t5   $13
#define t6   $14
#define t7   $15
#define s0   $16
#define s1   $17
#define s2   $18
#define s3   $19
#define s4   $20
#define s5   $21
#define s6   $22
#define s7   $23
#define t8   $24
#define t9   $25
#define k0   $26
#define k1   $27
#define gp   $28
#define sp   $29
#define fp   $30
#define ra   $31

/* CP0 registers */

#define CP0_COUNT    $9, 0
#define CP0_COMPARE $11, 0

#define CP0_STATUS        $12, 0
#define CP0_STATUS_CU3    (1 << 31)
#define CP0_STATUS_CU2    (1 << 30)
#define CP0_STATUS_CU1    (1 << 29)
#define CP0_STATUS_CU0    (1 << 28)
#define CP0_STATUS_RP     (1 << 27)
#define CP0_STATUS_FR     (1 << 26)
#define CP0_STATUS_RE     (1 << 25)
#define CP0_STATUS_MX     (1 << 24)
#define CP0_STATUS_BEV    (1 << 22)
#define CP0_STATUS_TS     (1 << 21)
#define CP0_STATUS_SR     (1 << 20)
#define CP0_STATUS_NMI    (1 << 19)
#define CP0_STATUS_CEE    (1 << 17)
#define CP0_STATUS_IPL_M  0x17fu
#define CP0_STATUS_IPL(x) (((x) & CP0_STATUS_IPL_M) << 10)
#define CP0_STATUS_IM9    (1 << 18)
#define CP0_STATUS_IM8    (1 << 16)
#define CP0_STATUS_IM7    (1 << 15)
#define CP0_STATUS_IM6    (1 << 14)
#define CP0_STATUS_IM5    (1 << 13)
#define CP0_STATUS_IM4    (1 << 12)
#define CP0_STATUS_IM3    (1 << 11)
#define CP0_STATUS_IM2    (1 << 10)
#define CP0_STATUS_IM1    (1 << 9)
#define CP0_STATUS_IM0    (1 << 8)
#define CP0_STATUS_UM     (1 << 4)
#define CP0_STATUS_ERL    (1 << 2)
#define CP0_STATUS_EXL    (1 << 1)
#define CP0_STATUS_IE     (1 << 0)

#define CP0_INTCTL           $12, 1
#define CP0_INTCTL_IPTI_M    0x7u
#define CP0_INTCTL_IPTI(x)   (((x) & CP0_INTCTL_IPTI_M) << 29)
#define CP0_INTCTL_IPPCI_M   0x7u
#define CP0_INTCTL_IPPCI(x)  (((x) & CP0_INTCTL_IPPCI_M) << 26)
#define CP0_INTCTL_IPFDC_M   0x7u
#define CP0_INTCTL_IPFDC(x)  (((x) & CP0_INTCTL_IPFDC_M) << 23)
#define CP0_INTCTL_PF        (1 << 22)
#define CP0_INTCTL_ICE       (1 << 21)
#define CP0_INTCTL_STKDEC_M  0x1fu
#define CP0_INTCTL_STKDEC(x) (((x) & CP0_INTCTL_STKDEC_M) << 16)
#define CP0_INTCTL_CLREXL    (1 << 15)
#define CP0_INTCTL_APE       (1 << 14)
#define CP0_INTCTL_USEKSTK   (1 << 13)
#define CP0_INTCTL_VS_M      0x1fu
#define CP0_INTCTL_VS(x)     (((x) & CP0_INTCTL_VS_M) << 5)

#define CP0_CAUSE            $13, 0
#define CP0_CAUSE_BD         (1 << 31)
#define CP0_CAUSE_TI         (1 << 30)
#define CP0_CAUSE_CE_M       0x3u
#define CP0_CAUSE_CE(x)      (((x) & CP0_CAUSE_CE_M) << 28)
#define CP0_CAUSE_DC         (1 << 27)
#define CP0_CAUSE_PCI        (1 << 26)
#define CP0_CAUSE_IC         (1 << 25)
#define CP0_CAUSE_AP         (1 << 24)
#define CP0_CAUSE_IV         (1 << 23)
#define CP0_CAUSE_WP         (1 << 22)
#define CP0_CAUSE_FDCI       (1 << 21)
#define CP0_CAUSE_RIPL_M     0xffu
#define CP0_CAUSE_RIPL(x)    (((x) & CP0_CAUSE_RIPL_M) << 10)
#define CP0_CAUSE_IP9        (1 << 17)
#define CP0_CAUSE_IP8        (1 << 16)
#define CP0_CAUSE_IP7        (1 << 15)
#define CP0_CAUSE_IP6        (1 << 14)
#define CP0_CAUSE_IP5        (1 << 13)
#define CP0_CAUSE_IP4        (1 << 12)
#define CP0_CAUSE_IP3        (1 << 11)
#define CP0_CAUSE_IP2        (1 << 10)
#define CP0_CAUSE_IP1        (1 << 9)
#define CP0_CAUSE_IP0        (1 << 8)
#define CP0_CAUSE_EXCCODE_M  0x1fu
#define CP0_CAUSE_EXCCODE(x) (((x) & CP0_CAUSE_EXCCODE_M) << 2)

#define CP0_EPC $14, 0

#define CP0_EBASE                   $15, 1
#define CP0_EBASE_EXCEPTION_BASE_M  0x7ffffu
#define CP0_EBASE_EXCEPTION_BASE(x) (((x) & CP0_EBASE_EXCEPTION_BASE_M) << 12)
#define CP0_EBASE_CPUNUM_M          0x3ffu
#define CP0_EBASE_CPUNUM(x)         ((x) & CP0_EBASE_CPUNUM_M)

#define CP0_CONFIG        $16, 0
#define CP0_CONFIG_M      (1 << 31)
#define CP0_CONFIG_K23_M  0x3u
#define CP0_CONFIG_K23(x) (((x) & CP0_CONFIG_K23_M) << 28)
#define CP0_CONFIG_KU_M   0x3u
#define CP0_CONFIG_KU(x)  (((x) & CP0_CONFIG_KU_M) << 25)
#define CP0_CONFIG_ISP    (1 << 24)
#define CP0_CONFIG_DSP    (1 << 23)
#define CP0_CONFIG_UDI    (1 << 22)
#define CP0_CONFIG_SB     (1 << 21)
#define CP0_CONFIG_MDU    (1 << 20)
#define CP0_CONFIG_WC     (1 << 19)
#define CP0_CONFIG_MM_M   0x3u
#define CP0_CONFIG_MM(x)  (((x) & CP0_CONFIG_MM_M) << 17)
#define CP0_CONFIG_BM     (1 << 16)
#define CP0_CONFIG_BE     (1 << 15)
#define CP0_CONFIG_AT_M   0x3u
#define CP0_CONFIG_AT(x)  (((x) & CP0_CONFIG_AT_M) << 13)
#define CP0_CONFIG_AR_M   0x7u
#define CP0_CONFIG_AR(x)  (((x) & CP0_CONFIG_AR_M) << 10)
#define CP0_CONFIG_MT_M   0x7u
#define CP0_CONFIG_MT(x)  (((x) & CP0_CONFIG_MT_M) << 7)
#define CP0_CONFIG_K0_M   0x7u
#define CP0_CONFIG_K0(x)  ((x) & CP0_CONFIG_K0_M)

#define CP0_TAGLO          $28, 0
/* ERRCTL_WST == 0 */
#define CP0_TAGLO_PA_M     0x3fffffu
#define CP0_TAGLO_PA(x)    (((x) & CP0_TAGLO_PA_M) << 10)
#define CP0_TAGLO_V        (1 << 7)
#define CP0_TAGLO_D        (1 << 6)
#define CP0_TAGLO_L        (1 << 5)
#define CP0_TAGLO_P        (1 << 0)
/* ERRCTL_WST == 1 */
#define CP0_TAGLO_WSDP_M   0xfu
#define CP0_TAGLO_WSDP(x)  (((x) & CP0_TAGLO_WSDP_M) << 20)
#define CP0_TAGLO_WSD_M    0xfu
#define CP0_TAGLO_WSD(x)   (((x) & CP0_TAGLO_WSD_M) << 16)
#define CP0_TAGLO_WSLRU_M  0x3fu
#define CP0_TAGLO_WSLRU(x) (((x) & CP0_TAGLO_WSLRU_M) << 10)

#define CP0_KSCRATCH0 $31, 2
#define CP0_KSCRATCH1 $31, 3

#endif
