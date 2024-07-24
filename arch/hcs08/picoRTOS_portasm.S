	.cs08
	.module picoRTOS_portasm

	.include "generated/picoRTOS_device.S"

	.globl _picoRTOS_tick
	.globl _picoRTOS_syscall

	.macro SWAP_STACK
	ldhx _bhx
	pshh
	pshx
	tsx
	aix #2
	sthx _bhx
	pulx
	pulh
	txs
	.endm

	.macro SAVE_CONTEXT
	;; auto-stack frame:
	;; PROGRAM COUNTER LOW
	;; PROGRAM COUNTER HIGH
	;; INDEX REGISTER (LOW BYTE X)
	;; ACCUMULATOR
	;; CONDITION CODE REGISTER
	;; manually-stacked frame:
	pshh ; INDEX REGISTER (HIGH BYTE H)
	SWAP_STACK
	clc
	.endm

	.macro RESTORE_CONTEXT
	SWAP_STACK
	pulh
	.endm

	.macro HX_TO_XA
	txa
	pshh
	pulx
	.endm

	.macro XA_TO_HX
	pshx
	pulh
	tax
	.endm

	.area CSEG (CODE)

	.globl Vswi_Handler
Vswi_Handler:
	SAVE_CONTEXT
	psha ; caller syscall
	ldhx _bhx
	lda 9,x ; priv
	psha
	lda 8,x ; >priv
	psha
	lda 3,s ; syscall
	psha
	;; sp is x:a
	HX_TO_XA
	jsr _picoRTOS_syscall
	ais #4
	XA_TO_HX
	sthx _bhx
	RESTORE_CONTEXT
	rti

	.globl Default_Handler
Default_Handler:
	SAVE_CONTEXT
	;; a contains !vector
	lsla ; multiply by 2
	lsla ; multiply by 4
	;; point to ISR_TABLE
	add #_ISR_TABLE
	tax
	clra
	adc #>_ISR_TABLE
	psha
	pulh
	;; check for nullness
	pshx
	pshh
	ldhx ,x
	cphx #0
	beq isr_fn_is_null
	pulh
	pulx
	;;
	bsr fn_priv
	bra epilogue
fn_priv:
	lda 1,x ; pch
	psha
	lda  ,x ; pcl
	psha
	lda 3,x ; :a
	ldx 2,x ; x:
	rts
epilogue:
	RESTORE_CONTEXT
	rti

isr_fn_is_null:
	;;  isr call is null, use picoRTOS_register_interrupt()
	bgnd
	nop
	bra isr_fn_is_null

	.globl _arch_save_first_context
_arch_save_first_context:
	XA_TO_HX ;; x:a to h:x
	aix #-6
	;; h
	clra
	sta ,x
	;; cc
	sta 1,x
	;; a
	lda 6,s
	sta 2,x
	;; x
	lda 5,s
	sta 3,x
	;; pch
	lda 3,s
	sta 4,x
	;; pcl
	lda 4,s
	sta 5,x
	HX_TO_XA ;; h:x to x:a
	rts

	.globl _arch_start_first_task
_arch_start_first_task:
	;; prepare stack sp
	XA_TO_HX
	sthx _bhx
	RESTORE_CONTEXT
	rti

	.globl _arch_syscall
_arch_syscall:
	swi
	rts

	.area DSEG
	;; leave room for  ___SDCC_hc08_retn
	.area XISEG
	.area XSEG

	.globl _bhx
_bhx:
	.ds 2

	.globl _ISR_TABLE
_ISR_TABLE:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.ds 2 ; fn
	.ds 2 ; priv
	.endm
