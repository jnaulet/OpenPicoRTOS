	.module picoRTOS_portasm

	.include "picoRTOS_device.inc"

	.globl _picoRTOS_tick
	.globl _picoRTOS_syscall

	.globl _bp
	.globl _vn
	.globl __start__stack

	ar7 = 0x07
	ar6 = 0x06
	ar5 = 0x05
	ar4 = 0x04
	ar3 = 0x03
	ar2 = 0x02
	ar1 = 0x01
	ar0 = 0x00

	.macro SAVE_CONTEXT_STEP1
	push sp
	push ie
	clr ea			; no re-entrance
	push psw
	push a
	push dpl
	push dph
	push b
	push _bp
	push ar0
	push ar1
	push ar2
	push ar3
	push ar4
	push ar5
	push ar6
	push ar7
	.endm

	;; dptr = *(u16*)__start_stack
	.macro DPTR_FROM_SP
	mov r0, #__start__stack
	mov dpl, @r0
	mov r0, #__start__stack + 1
	mov dph, @r0
	.endm

	.macro SAVE_CONTEXT ?loop
	SAVE_CONTEXT_STEP1
	DPTR_FROM_SP
loop:
	;; emtpy stack to dptr++
	pop a
	movx @dptr, a
	inc dptr
	;; until sp is back to "zero"
	mov a, sp
	cjne a, #__start__stack - 1, loop
	.endm

	;; --dptr
	.macro DEC_DPTR ?nborrow
	dec dpl
	mov a, dpl
	cjne a, #0xff, nborrow
	dec dph
nborrow:
	.endm

	;; *sp-- = *--dptr;
	.macro PUSH_xxDPTR
	DEC_DPTR
	movx a, @dptr
	push a
	.endm

	.macro RESTORE_CONTEXT_STEP1 ?loop
	;; reset stack
	mov sp, #__start__stack - 1
	;; fill stack with --dptr
	PUSH_xxDPTR
	mov r6, a		; r6 is SP_L
	PUSH_xxDPTR
	mov r7, a		; r7 is SP_H
loop:
	PUSH_xxDPTR
	mov a, r6
	cjne a, dpl, loop
	mov a, r7
	cjne a, dph, loop
	.endm

	.macro RESTORE_CONTEXT
	RESTORE_CONTEXT_STEP1
	pop ar7
	pop ar6
	pop ar5
	pop ar4
	pop ar3
	pop ar2
	pop ar1
	pop ar0
	pop _bp
	pop b
	pop dph
	pop dpl
	pop a
	pop psw
	pop ie			; restore interrupts
	dec sp
	.endm

	;; *dptr++ = rS;
	.macro MOVX_DPTRxx rS
	mov a, rS
	movx @dptr, a
	inc dptr
	.endm

	;; dptr += num;
	.macro ADD_DPTR num
	mov a, dpl
	add a, num
	mov dpl, a
	mov a, dph
	addc a, #0
	mov dph, a
	.endm

	.area CSEG (CODE)

	.globl Timer2_Handler
Timer2_Handler:
	SAVE_CONTEXT
	anl t2con, #0x7f
	;; sp passed as _bp +1 (dpl:dph:b), ignore b
	mov b, #0
	lcall _picoRTOS_tick
	RESTORE_CONTEXT
	reti

	.globl Default_Handler
Default_Handler:
	SAVE_CONTEXT
	;; point to the right entry
	mov a, _vn
	mov b, #4
	mul ab
	mov dptr, #_ISR_TABLE
	add a, dpl
	mov dpl, a
	mov a, dph
	addc a, #0
	mov dph, a
	;; get fn to r6:r7
	movx a, @dptr
	mov r6, a
	inc dptr
	movx a, @dptr
	mov r7, a
	inc dptr
	;; get priv in r4:r5
	movx a, @dptr
	mov r4, a
	inc dptr
	movx a, @dptr
	mov r5, a
	;; prepare to call fn(priv)
	lcall fn_priv
	sjmp epilogue
fn_priv:
	push ar6
	push ar7
	mov dpl, r4
	mov dph, r5
	ret			; call fn(priv)
epilogue:
	RESTORE_CONTEXT
	reti

	.globl _arch_save_first_context
_arch_save_first_context:
	mov _bp, sp
	pop ar0			; ignore ret_h
	pop ar0			; ignore ret_l
	;; get stacked params
	pop ar4			; fn >> 8
	pop ar3			; fn
	pop ar2			; priv >> 16
	pop ar1			; priv >> 8
	pop ar0			; priv
	;; put stack addr on top
	;; mov dpl, r5 -> already set
	;; mov dph, r6 -> already set
	MOVX_DPTRxx #7		; r7
	MOVX_DPTRxx #6		; r6
	MOVX_DPTRxx #5		; r5
	MOVX_DPTRxx #4		; r4
	MOVX_DPTRxx #3		; r3
	MOVX_DPTRxx #2		; r2
	MOVX_DPTRxx #1		; r1
	MOVX_DPTRxx #0		; r0
	MOVX_DPTRxx _bp		; _bp
	MOVX_DPTRxx r2		; b   : 0xff & (priv >> 16)
	MOVX_DPTRxx r1		; dph : 0xff & (priv >> 8)
	MOVX_DPTRxx r0		; dpl : 0xff & priv
	MOVX_DPTRxx #65		; a
	MOVX_DPTRxx #0		; psw
	mov r0, ie		; ie (auto-enable)
	orl ar0, #INITIAL_IE
	MOVX_DPTRxx r0
	MOVX_DPTRxx #255	; sp
	MOVX_DPTRxx r4		; reti_h : 0xff & (fn >> 8)
	MOVX_DPTRxx r3		; reti_l : 0xff & fn
	;; default stack is empty
	;; task-specific data will be stored here
	MOVX_DPTRxx r6		; sp_h : 0xff & (stack >> 8)
	MOVX_DPTRxx r5		; sp_l : 0xff & stack
	;; restore sp & ret
	mov sp, _bp
	ret

	.globl _arch_start_first_task
_arch_start_first_task:
	;; mov dpl, r5 -> already set
	;; mov dph, r6 -> already set
	;; ignore r7
	RESTORE_CONTEXT
	ret

	.globl _arch_syscall
_arch_syscall:
	SAVE_CONTEXT
	;; param sp is r7:r6:r5
	mov r5, dpl
	mov r6, dph
	mov r7, b
	;; point to beginning of sp
	mov sp, #__start__stack + 1
	pop dph
	pop dpl
	;; fish for dpl (syscall) in r4
	ADD_DPTR #11
	movx a, @dptr
	mov r4, a
	;; restore caller stack
	ADD_DPTR #4
	movx a, @dptr
	mov sp, a
	;; continue normal operation
	add a, #0xfb
	;; priv >> 16
	mov r0, a
	mov a, @r0
	push a
	;; priv >> 8
	inc r0
	mov a, @r0
	push a
	;; priv
	inc r0
	mov a, @r0
	push a
	;; syscall
	push ar4
	;; sp
	mov dpl, r5
	mov dph, r6
	mov b, #0
	;; finally, call
	lcall _picoRTOS_syscall
	RESTORE_CONTEXT
	ret

	.area DSEG (DATA)

	.globl _vn
_vn:
	.ds 1			; vector number

	.area XSEG (XDATA)

	.globl _ISR_TABLE
_ISR_TABLE:
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.ds 2 ; fn
	.ds 3 ; priv
	.endm
