	;; startup is handled by sdcc, just define
	;; vectors here (for the moment)
	.globl Default_Handler
	.globl Timer2_Handler
	.globl __sdcc_gsinit_startup
	.globl _vn

	.include "../include/macros.S"

	.module vectors

	.area CSEG (CODE)
	.globl Reset_Handler
Reset_Handler:
	ljmp __sdcc_gsinit_startup

	;; vectors
	VECTOR #0, Default_Handler		; INT0# Pin
	VECTOR #1, Default_Handler		; Timer 0 Overflow
	VECTOR #2, Default_Handler		; INT1# Pin
	VECTOR #3, Default_Handler		; Timer 1 Overflow
	VECTOR #4, Default_Handler		; USART0 Rx & Tx
	VECTOR #5, Timer2_Handler		; Timer 2 Overflow
	VECTOR #6, Default_Handler		; WAKEUP / WU2 Pin or USB Resume
	VECTOR #7, Default_Handler		; USART1 Rx & Tx
	VECTOR #8, Default_Handler		; USB
	VECTOR #9, Default_Handler		; I2C Bus
	VECTOR #10, Default_Handler		; GPIF / FIFOs / INT4 Pin
	VECTOR #11, Default_Handler		; INT5# Pin
	VECTOR #12, Default_Handler		; INT6 Pin
