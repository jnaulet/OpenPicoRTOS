;; startup is handled by sdcc, just define
;; vectors here(for the moment)
.globl Default_Handler
.globl SysTick_Handler
.globl __sdcc_gsinit_startup
.globl _vn

.include "macros.S"

.module vectors

.area HOME(CODE)
.globl Reset_Handler
Reset_Handler:
ljmp __non_interruptible_startup

;; vectors
    VECTOR #0, Default_Handler; External Interrupt 0
VECTOR #1, SysTick_Handler; Timer 0
VECTOR #2, Default_Handler; External Interrupt 1
VECTOR #3, Default_Handler; Timer 1
VECTOR #4, Default_Handler; Serial port
    VECTOR #5, Default_Handler; ADC
    VECTOR #6, Default_Handler; LVD
    VECTOR #7, Default_Handler; PCA
    VECTOR #8, Default_Handler; UART2(S2)
VECTOR #9, Default_Handler; SPI

__non_interruptible_startup:
clr ea
ljmp __sdcc_gsinit_startup
