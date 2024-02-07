	;; startup is handled by sdcc, just define
	;; vectors here (for the moment)
	.globl Default_Handler
	.globl SysTick_Handler
	.globl __sdcc_gsinit_startup
	.globl _vn

	.include "macros.S"

	.module vectors

	.area HOME (CODE)
	.globl Reset_Handler
Reset_Handler:
	ljmp __non_interruptible_startup

	;; vectors
	VECTOR #0, Default_Handler		; INT0# Pin
	VECTOR #1, Default_Handler		; Timer 0 Overflow
	VECTOR #2, Default_Handler		; INT1# Pin
	VECTOR #3, Default_Handler		; Timer 1 Overflow
	VECTOR #4, Default_Handler		; USART0 Rx & Tx
	VECTOR #5, SysTick_Handler		; Timer 2 Overflow

__non_interruptible_startup:
	clr ea
	ljmp __sdcc_gsinit_startup
