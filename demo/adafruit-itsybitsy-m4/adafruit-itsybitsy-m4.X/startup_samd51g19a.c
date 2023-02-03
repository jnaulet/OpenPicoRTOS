/**
 * \file
 *
 * \brief GCC startup file for ATSAMD51G19A
 *
 * Copyright (c) 2022 Microchip Technology Inc.
 *
 * \license_start
 *
 * \page License
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * \license_stop
 *
 */

#include "samd51g19a.h"

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

/* Optional application-provided functions */
extern void __attribute__((weak, long_call)) _on_reset(void);
extern void __attribute__((weak, long_call)) _on_bootstrap(void);

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main(void);
/** \endcond */

void __libc_init_array(void);

/* Reset handler */
void Reset_Handler(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M4 core handlers */
void NonMaskableInt_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void HardFault_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MemoryManagement_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void BusFault_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UsageFault_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SVCall_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DebugMonitor_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SysTick_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Peripherals handlers */
void PM_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MCLK_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void OSCCTRL_XOSC0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void OSCCTRL_XOSC1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void OSCCTRL_DFLL_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void OSCCTRL_DPLL0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void OSCCTRL_DPLL1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void OSC32KCTRL_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SUPC_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SUPC_BODDET_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void WDT_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_2_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_3_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_4_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_5_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_6_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_7_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_8_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_9_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_10_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_11_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_12_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_13_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_14_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_EXTINT_15_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void FREQM_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void NVMCTRL_0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void NVMCTRL_1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DMAC_0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DMAC_1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DMAC_2_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DMAC_3_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DMAC_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_2_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_3_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EVSYS_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PAC_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RAMECC_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM0_0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM0_1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM0_2_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM0_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM1_0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM1_1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM1_2_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM1_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM2_0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM2_1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM2_2_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM2_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM3_0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM3_1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM3_2_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM3_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM4_0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM4_1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM4_2_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM4_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM5_0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM5_1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM5_2_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM5_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USB_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USB_SOF_HSOF_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USB_TRCPT0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USB_TRCPT1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC0_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC0_MC0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC0_MC1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC0_MC2_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC0_MC3_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC0_MC4_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC0_MC5_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC1_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC1_MC0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC1_MC1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC1_MC2_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC1_MC3_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC2_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC2_MC0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC2_MC1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC2_MC2_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC3_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PDEC_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PDEC_MC0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PDEC_MC1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC0_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC0_RESRDY_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC1_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC1_RESRDY_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AC_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DAC_OTHER_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DAC_EMPTY_0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DAC_EMPTY_1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DAC_RESRDY_0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DAC_RESRDY_1_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PCC_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AES_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TRNG_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ICM_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PUKCC_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void QSPI_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SDHC0_Handler( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Exception Table */
__attribute__ ((section(".vectors")))
const DeviceVectors exception_table = {

    /* Configure Initial Stack Pointer, using linker-generated symbols */
    .pvStack                        = (void*)(&_estack),

    .pfnReset_Handler               = (void*)Reset_Handler,
    .pfnNonMaskableInt_Handler      = (void*)NonMaskableInt_Handler,
    .pfnHardFault_Handler           = (void*)HardFault_Handler,
    .pfnMemoryManagement_Handler    = (void*)MemoryManagement_Handler,
    .pfnBusFault_Handler            = (void*)BusFault_Handler,
    .pfnUsageFault_Handler          = (void*)UsageFault_Handler,
    .pvReservedC9                   = (void*)(0UL),     /* Reserved */
    .pvReservedC8                   = (void*)(0UL),     /* Reserved */
    .pvReservedC7                   = (void*)(0UL),     /* Reserved */
    .pvReservedC6                   = (void*)(0UL),     /* Reserved */
    .pfnSVCall_Handler              = (void*)SVCall_Handler,
    .pfnDebugMonitor_Handler        = (void*)DebugMonitor_Handler,
    .pvReservedC3                   = (void*)(0UL),     /* Reserved */
    .pfnPendSV_Handler              = (void*)PendSV_Handler,
    .pfnSysTick_Handler             = (void*)SysTick_Handler,

    /* Configurable interrupts */
    .pfnPM_Handler                  = (void*)PM_Handler,            /* 0  Power Manager */
    .pfnMCLK_Handler                = (void*)MCLK_Handler,          /* 1  Main Clock */
    .pfnOSCCTRL_XOSC0_Handler       = (void*)OSCCTRL_XOSC0_Handler, /* 2  Oscillators Control */
    .pfnOSCCTRL_XOSC1_Handler       = (void*)OSCCTRL_XOSC1_Handler, /* 3  Oscillators Control */
    .pfnOSCCTRL_DFLL_Handler        = (void*)OSCCTRL_DFLL_Handler,  /* 4  Oscillators Control */
    .pfnOSCCTRL_DPLL0_Handler       = (void*)OSCCTRL_DPLL0_Handler, /* 5  Oscillators Control */
    .pfnOSCCTRL_DPLL1_Handler       = (void*)OSCCTRL_DPLL1_Handler, /* 6  Oscillators Control */
    .pfnOSC32KCTRL_Handler          = (void*)OSC32KCTRL_Handler,    /* 7  32kHz Oscillators Control */
    .pfnSUPC_OTHER_Handler          = (void*)SUPC_OTHER_Handler,    /* 8  Supply Controller */
    .pfnSUPC_BODDET_Handler         = (void*)SUPC_BODDET_Handler,   /* 9  Supply Controller */
    .pfnWDT_Handler                 = (void*)WDT_Handler,           /* 10 Watchdog Timer */
    .pfnRTC_Handler                 = (void*)RTC_Handler,           /* 11 Real-Time Counter */
    .pfnEIC_EXTINT_0_Handler        = (void*)EIC_EXTINT_0_Handler,  /* 12 External Interrupt Controller */
    .pfnEIC_EXTINT_1_Handler        = (void*)EIC_EXTINT_1_Handler,  /* 13 External Interrupt Controller */
    .pfnEIC_EXTINT_2_Handler        = (void*)EIC_EXTINT_2_Handler,  /* 14 External Interrupt Controller */
    .pfnEIC_EXTINT_3_Handler        = (void*)EIC_EXTINT_3_Handler,  /* 15 External Interrupt Controller */
    .pfnEIC_EXTINT_4_Handler        = (void*)EIC_EXTINT_4_Handler,  /* 16 External Interrupt Controller */
    .pfnEIC_EXTINT_5_Handler        = (void*)EIC_EXTINT_5_Handler,  /* 17 External Interrupt Controller */
    .pfnEIC_EXTINT_6_Handler        = (void*)EIC_EXTINT_6_Handler,  /* 18 External Interrupt Controller */
    .pfnEIC_EXTINT_7_Handler        = (void*)EIC_EXTINT_7_Handler,  /* 19 External Interrupt Controller */
    .pfnEIC_EXTINT_8_Handler        = (void*)EIC_EXTINT_8_Handler,  /* 20 External Interrupt Controller */
    .pfnEIC_EXTINT_9_Handler        = (void*)EIC_EXTINT_9_Handler,  /* 21 External Interrupt Controller */
    .pfnEIC_EXTINT_10_Handler       = (void*)EIC_EXTINT_10_Handler, /* 22 External Interrupt Controller */
    .pfnEIC_EXTINT_11_Handler       = (void*)EIC_EXTINT_11_Handler, /* 23 External Interrupt Controller */
    .pfnEIC_EXTINT_12_Handler       = (void*)EIC_EXTINT_12_Handler, /* 24 External Interrupt Controller */
    .pfnEIC_EXTINT_13_Handler       = (void*)EIC_EXTINT_13_Handler, /* 25 External Interrupt Controller */
    .pfnEIC_EXTINT_14_Handler       = (void*)EIC_EXTINT_14_Handler, /* 26 External Interrupt Controller */
    .pfnEIC_EXTINT_15_Handler       = (void*)EIC_EXTINT_15_Handler, /* 27 External Interrupt Controller */
    .pfnFREQM_Handler               = (void*)FREQM_Handler,         /* 28 Frequency Meter */
    .pfnNVMCTRL_0_Handler           = (void*)NVMCTRL_0_Handler,     /* 29 Non-Volatile Memory Controller */
    .pfnNVMCTRL_1_Handler           = (void*)NVMCTRL_1_Handler,     /* 30 Non-Volatile Memory Controller */
    .pfnDMAC_0_Handler              = (void*)DMAC_0_Handler,        /* 31 Direct Memory Access Controller */
    .pfnDMAC_1_Handler              = (void*)DMAC_1_Handler,        /* 32 Direct Memory Access Controller */
    .pfnDMAC_2_Handler              = (void*)DMAC_2_Handler,        /* 33 Direct Memory Access Controller */
    .pfnDMAC_3_Handler              = (void*)DMAC_3_Handler,        /* 34 Direct Memory Access Controller */
    .pfnDMAC_OTHER_Handler          = (void*)DMAC_OTHER_Handler,    /* 35 Direct Memory Access Controller */
    .pfnEVSYS_0_Handler             = (void*)EVSYS_0_Handler,       /* 36 Event System Interface */
    .pfnEVSYS_1_Handler             = (void*)EVSYS_1_Handler,       /* 37 Event System Interface */
    .pfnEVSYS_2_Handler             = (void*)EVSYS_2_Handler,       /* 38 Event System Interface */
    .pfnEVSYS_3_Handler             = (void*)EVSYS_3_Handler,       /* 39 Event System Interface */
    .pfnEVSYS_OTHER_Handler         = (void*)EVSYS_OTHER_Handler,   /* 40 Event System Interface */
    .pfnPAC_Handler                 = (void*)PAC_Handler,           /* 41 Peripheral Access Controller */
    .pvReserved42                   = (void*)(0UL),                 /* 42 Reserved */
    .pvReserved43                   = (void*)(0UL),                 /* 43 Reserved */
    .pvReserved44                   = (void*)(0UL),                 /* 44 Reserved */
    .pfnRAMECC_Handler              = (void*)RAMECC_Handler,        /* 45 RAM ECC */
    .pfnSERCOM0_0_Handler           = (void*)SERCOM0_0_Handler,     /* 46 Serial Communication Interface */
    .pfnSERCOM0_1_Handler           = (void*)SERCOM0_1_Handler,     /* 47 Serial Communication Interface */
    .pfnSERCOM0_2_Handler           = (void*)SERCOM0_2_Handler,     /* 48 Serial Communication Interface */
    .pfnSERCOM0_OTHER_Handler       = (void*)SERCOM0_OTHER_Handler, /* 49 Serial Communication Interface */
    .pfnSERCOM1_0_Handler           = (void*)SERCOM1_0_Handler,     /* 50 Serial Communication Interface */
    .pfnSERCOM1_1_Handler           = (void*)SERCOM1_1_Handler,     /* 51 Serial Communication Interface */
    .pfnSERCOM1_2_Handler           = (void*)SERCOM1_2_Handler,     /* 52 Serial Communication Interface */
    .pfnSERCOM1_OTHER_Handler       = (void*)SERCOM1_OTHER_Handler, /* 53 Serial Communication Interface */
    .pfnSERCOM2_0_Handler           = (void*)SERCOM2_0_Handler,     /* 54 Serial Communication Interface */
    .pfnSERCOM2_1_Handler           = (void*)SERCOM2_1_Handler,     /* 55 Serial Communication Interface */
    .pfnSERCOM2_2_Handler           = (void*)SERCOM2_2_Handler,     /* 56 Serial Communication Interface */
    .pfnSERCOM2_OTHER_Handler       = (void*)SERCOM2_OTHER_Handler, /* 57 Serial Communication Interface */
    .pfnSERCOM3_0_Handler           = (void*)SERCOM3_0_Handler,     /* 58 Serial Communication Interface */
    .pfnSERCOM3_1_Handler           = (void*)SERCOM3_1_Handler,     /* 59 Serial Communication Interface */
    .pfnSERCOM3_2_Handler           = (void*)SERCOM3_2_Handler,     /* 60 Serial Communication Interface */
    .pfnSERCOM3_OTHER_Handler       = (void*)SERCOM3_OTHER_Handler, /* 61 Serial Communication Interface */
    .pfnSERCOM4_0_Handler           = (void*)SERCOM4_0_Handler,     /* 62 Serial Communication Interface */
    .pfnSERCOM4_1_Handler           = (void*)SERCOM4_1_Handler,     /* 63 Serial Communication Interface */
    .pfnSERCOM4_2_Handler           = (void*)SERCOM4_2_Handler,     /* 64 Serial Communication Interface */
    .pfnSERCOM4_OTHER_Handler       = (void*)SERCOM4_OTHER_Handler, /* 65 Serial Communication Interface */
    .pfnSERCOM5_0_Handler           = (void*)SERCOM5_0_Handler,     /* 66 Serial Communication Interface */
    .pfnSERCOM5_1_Handler           = (void*)SERCOM5_1_Handler,     /* 67 Serial Communication Interface */
    .pfnSERCOM5_2_Handler           = (void*)SERCOM5_2_Handler,     /* 68 Serial Communication Interface */
    .pfnSERCOM5_OTHER_Handler       = (void*)SERCOM5_OTHER_Handler, /* 69 Serial Communication Interface */
    .pvReserved70                   = (void*)(0UL),                 /* 70 Reserved */
    .pvReserved71                   = (void*)(0UL),                 /* 71 Reserved */
    .pvReserved72                   = (void*)(0UL),                 /* 72 Reserved */
    .pvReserved73                   = (void*)(0UL),                 /* 73 Reserved */
    .pvReserved74                   = (void*)(0UL),                 /* 74 Reserved */
    .pvReserved75                   = (void*)(0UL),                 /* 75 Reserved */
    .pvReserved76                   = (void*)(0UL),                 /* 76 Reserved */
    .pvReserved77                   = (void*)(0UL),                 /* 77 Reserved */
    .pvReserved78                   = (void*)(0UL),                 /* 78 Reserved */
    .pvReserved79                   = (void*)(0UL),                 /* 79 Reserved */
    .pfnUSB_OTHER_Handler           = (void*)USB_OTHER_Handler,     /* 80 Universal Serial Bus */
    .pfnUSB_SOF_HSOF_Handler        = (void*)USB_SOF_HSOF_Handler,  /* 81 Universal Serial Bus */
    .pfnUSB_TRCPT0_Handler          = (void*)USB_TRCPT0_Handler,    /* 82 Universal Serial Bus */
    .pfnUSB_TRCPT1_Handler          = (void*)USB_TRCPT1_Handler,    /* 83 Universal Serial Bus */
    .pvReserved84                   = (void*)(0UL),                 /* 84 Reserved */
    .pfnTCC0_OTHER_Handler          = (void*)TCC0_OTHER_Handler,    /* 85 Timer Counter Control */
    .pfnTCC0_MC0_Handler            = (void*)TCC0_MC0_Handler,      /* 86 Timer Counter Control */
    .pfnTCC0_MC1_Handler            = (void*)TCC0_MC1_Handler,      /* 87 Timer Counter Control */
    .pfnTCC0_MC2_Handler            = (void*)TCC0_MC2_Handler,      /* 88 Timer Counter Control */
    .pfnTCC0_MC3_Handler            = (void*)TCC0_MC3_Handler,      /* 89 Timer Counter Control */
    .pfnTCC0_MC4_Handler            = (void*)TCC0_MC4_Handler,      /* 90 Timer Counter Control */
    .pfnTCC0_MC5_Handler            = (void*)TCC0_MC5_Handler,      /* 91 Timer Counter Control */
    .pfnTCC1_OTHER_Handler          = (void*)TCC1_OTHER_Handler,    /* 92 Timer Counter Control */
    .pfnTCC1_MC0_Handler            = (void*)TCC1_MC0_Handler,      /* 93 Timer Counter Control */
    .pfnTCC1_MC1_Handler            = (void*)TCC1_MC1_Handler,      /* 94 Timer Counter Control */
    .pfnTCC1_MC2_Handler            = (void*)TCC1_MC2_Handler,      /* 95 Timer Counter Control */
    .pfnTCC1_MC3_Handler            = (void*)TCC1_MC3_Handler,      /* 96 Timer Counter Control */
    .pfnTCC2_OTHER_Handler          = (void*)TCC2_OTHER_Handler,    /* 97 Timer Counter Control */
    .pfnTCC2_MC0_Handler            = (void*)TCC2_MC0_Handler,      /* 98 Timer Counter Control */
    .pfnTCC2_MC1_Handler            = (void*)TCC2_MC1_Handler,      /* 99 Timer Counter Control */
    .pfnTCC2_MC2_Handler            = (void*)TCC2_MC2_Handler,      /* 100 Timer Counter Control */
    .pvReserved101                  = (void*)(0UL),                 /* 101 Reserved */
    .pvReserved102                  = (void*)(0UL),                 /* 102 Reserved */
    .pvReserved103                  = (void*)(0UL),                 /* 103 Reserved */
    .pvReserved104                  = (void*)(0UL),                 /* 104 Reserved */
    .pvReserved105                  = (void*)(0UL),                 /* 105 Reserved */
    .pvReserved106                  = (void*)(0UL),                 /* 106 Reserved */
    .pfnTC0_Handler                 = (void*)TC0_Handler,           /* 107 Basic Timer Counter */
    .pfnTC1_Handler                 = (void*)TC1_Handler,           /* 108 Basic Timer Counter */
    .pfnTC2_Handler                 = (void*)TC2_Handler,           /* 109 Basic Timer Counter */
    .pfnTC3_Handler                 = (void*)TC3_Handler,           /* 110 Basic Timer Counter */
    .pvReserved111                  = (void*)(0UL),                 /* 111 Reserved */
    .pvReserved112                  = (void*)(0UL),                 /* 112 Reserved */
    .pvReserved113                  = (void*)(0UL),                 /* 113 Reserved */
    .pvReserved114                  = (void*)(0UL),                 /* 114 Reserved */
    .pfnPDEC_OTHER_Handler          = (void*)PDEC_OTHER_Handler,    /* 115 Quadrature Decodeur */
    .pfnPDEC_MC0_Handler            = (void*)PDEC_MC0_Handler,      /* 116 Quadrature Decodeur */
    .pfnPDEC_MC1_Handler            = (void*)PDEC_MC1_Handler,      /* 117 Quadrature Decodeur */
    .pfnADC0_OTHER_Handler          = (void*)ADC0_OTHER_Handler,    /* 118 Analog Digital Converter */
    .pfnADC0_RESRDY_Handler         = (void*)ADC0_RESRDY_Handler,   /* 119 Analog Digital Converter */
    .pfnADC1_OTHER_Handler          = (void*)ADC1_OTHER_Handler,    /* 120 Analog Digital Converter */
    .pfnADC1_RESRDY_Handler         = (void*)ADC1_RESRDY_Handler,   /* 121 Analog Digital Converter */
    .pfnAC_Handler                  = (void*)AC_Handler,            /* 122 Analog Comparators */
    .pfnDAC_OTHER_Handler           = (void*)DAC_OTHER_Handler,     /* 123 Digital-to-Analog Converter */
    .pfnDAC_EMPTY_0_Handler         = (void*)DAC_EMPTY_0_Handler,   /* 124 Digital-to-Analog Converter */
    .pfnDAC_EMPTY_1_Handler         = (void*)DAC_EMPTY_1_Handler,   /* 125 Digital-to-Analog Converter */
    .pfnDAC_RESRDY_0_Handler        = (void*)DAC_RESRDY_0_Handler,  /* 126 Digital-to-Analog Converter */
    .pfnDAC_RESRDY_1_Handler        = (void*)DAC_RESRDY_1_Handler,  /* 127 Digital-to-Analog Converter */
    .pvReserved128                  = (void*)(0UL),                 /* 128 Reserved */
    .pfnPCC_Handler                 = (void*)PCC_Handler,           /* 129 Parallel Capture Controller */
    .pfnAES_Handler                 = (void*)AES_Handler,           /* 130 Advanced Encryption Standard */
    .pfnTRNG_Handler                = (void*)TRNG_Handler,          /* 131 True Random Generator */
    .pfnICM_Handler                 = (void*)ICM_Handler,           /* 132 Integrity Check Monitor */
    .pfnPUKCC_Handler               = (void*)PUKCC_Handler,         /* 133 PUblic-Key Cryptography Controller */
    .pfnQSPI_Handler                = (void*)QSPI_Handler,          /* 134 Quad SPI interface */
    .pfnSDHC0_Handler               = (void*)SDHC0_Handler          /* 135 SD/MMC Host Controller */
};

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void Reset_Handler(void)
{
    uint32_t *pSrc, *pDest;

    /* Initialize the relocate segment */
    pSrc = &_etext;
    pDest = &_srelocate;

    if (pSrc != pDest)
        for (; pDest < &_erelocate;)
            *pDest++ = *pSrc++;

    /* Clear the zero segment */
    for (pDest = &_szero; pDest < &_ezero;)
        *pDest++ = 0;

    /* Set the vector table base address */
    pSrc = (uint32_t *)&_sfixed;
    SCB->VTOR = ((uint32_t)pSrc & SCB_VTOR_TBLOFF_Msk);

    /* Call the optional application-provided _on_reset() function. */
    if (_on_reset)
        _on_reset();

    /* Initialize the C library */
    __libc_init_array();

    /* Call the optional application-provided _on_bootstrap() function. */
    if (_on_bootstrap)
        _on_bootstrap();

    /* Branch to main function */
    main();

    /* Infinite loop */
    while (1);
}

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
    while (1) {
    }
}
