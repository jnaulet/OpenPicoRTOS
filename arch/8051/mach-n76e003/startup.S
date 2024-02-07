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
	VECTOR #0, Default_Handler		; External Interrupt 0
	VECTOR #1, Default_Handler		; Timer 0 overflow
	VECTOR #2, Default_Handler		; External Interrupt 1
	VECTOR #3, Default_Handler		; Timer 1 overflow
	VECTOR #4, Default_Handler		; Serial port 0 interrupt
	VECTOR #5, Default_Handler		; Timer 2 event
	VECTOR #6, Default_Handler		; I2C status/timer-out-interrupt
	VECTOR #7, Default_Handler		; Pin interrupt
	VECTOR #8, Default_Handler		; Brown-out detection interrupt
	VECTOR #9, Default_Handler		; SPI interrupt
	VECTOR #10, Default_Handler		; WDT interrupt
	VECTOR #11, Default_Handler		; ADC interrupt
	VECTOR #12, Default_Handler		; Input capture interrupt
	VECTOR #13, Default_Handler		; PWM interrupt
	VECTOR #14, Default_Handler		; Fault Brake interrupt
	VECTOR #15, Default_Handler		; Serial port 1 interrupt
	VECTOR #16, Default_Handler		; Timer 3 overflow
	VECTOR #17, SysTick_Handler		; Self Wake-up Timer interrupt

__non_interruptible_startup:
	clr ea
	ljmp __sdcc_gsinit_startup
