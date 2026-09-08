	.if __TI_EABI__
	.asg picoRTOS_tick, _picoRTOS_tick
	.asg picoRTOS_syscall, _picoRTOS_syscall
	.asg picoRTOS_irq, _picoRTOS_irq
	.endif

	.ref _picoRTOS_tick
	.ref _picoRTOS_syscall
	.ref _picoRTOS_irq

	.if __TI_EABI__
	.asg arch_save_first_context, _arch_save_first_context
	.asg arch_start_first_task, _arch_start_first_task
	.asg arch_syscall, _arch_syscall
	.asg arch_test_and_set, _arch_test_and_set
	.endif

	.cdecls C, NOLIST, "picoRTOS_device.h"

	.if __TI_EABI__
stack:	 .usect ".bss:stack", 1, 0, 1
	.else
stack:	 .usect ".ebss", 1, 0, 1
	.endif

SWAP_STACK .macro
	movz DP, #stack
	mov AL, @stack
	mov AH, @SP
	mov @stack, AH
	mov @SP, AL
	.endm

SAVE_CONTEXT_NOSWAP .macro
	asp
	push DP:ST1
	push RPC
	push XT
	push AR1H:AR0H
	push XAR2
	push XAR3
	push XAR4
	push XAR5
	push XAR6
	push XAR7
	.if .TMS320C2800_FPU32 | .TMS320C2800_FPU64
	push RB
	mov32 *SP++, STF
	mov32 *SP++, R0L
	mov32 *SP++, R0H
	mov32 *SP++, R1L
	mov32 *SP++, R1H
	mov32 *SP++, R2L
	mov32 *SP++, R2H
	mov32 *SP++, R3L
	mov32 *SP++, R3H
	mov32 *SP++, R4L
	mov32 *SP++, R4H
	mov32 *SP++, R5L
	mov32 *SP++, R5H
	mov32 *SP++, R6L
	mov32 *SP++, R6H
	mov32 *SP++, R7L
	mov32 *SP++, R7H
	.endif
	.endm

SAVE_CONTEXT .macro
	SAVE_CONTEXT_NOSWAP
	SWAP_STACK
	.endm

RESTORE_CONTEXT .macro
	SWAP_STACK
	.if .TMS320C2800_FPU32 | .TMS320C2800_FPU64
	mov32 R7L, *--SP
	mov32 R7H, *--SP
	mov32 R6L, *--SP
	mov32 R6H, *--SP
	mov32 R5L, *--SP
	mov32 R5H, *--SP
	mov32 R4L, *--SP
	mov32 R4H, *--SP
	mov32 R3L, *--SP
	mov32 R3H, *--SP
	mov32 R2L, *--SP
	mov32 R2H, *--SP
	mov32 R1L, *--SP
	mov32 R1H, *--SP
	mov32 R0L, *--SP
	mov32 R0H, *--SP
	mov32 STF, *--SP
	pop RB
	.endif
	pop XAR7
	pop XAR6
	pop XAR5
	pop XAR4
	pop XAR3
	pop XAR2
	pop AR1H:AR0H
	pop XT
	pop RPC
	pop DP:ST1
	nasp
	.endm

	.global TIMER2_Handler
TIMER2_Handler:
	;; tick
	SAVE_CONTEXT
	movz AR4, @stack
	lcr _picoRTOS_tick
	movz DP, #stack
	mov @stack, AR4
	RESTORE_CONTEXT
	iret

	.global _arch_syscall
_arch_syscall:
	;; trigger RTOSINT
	intr RTOSINT
	lretr

	.global RealTimeOS_Handler
RealTimeOS_Handler:
	;; syscall
	SAVE_CONTEXT_NOSWAP
	mov AR6, @SP
	movz AR4, AR6		; sp
	;; fish for arch_syscall params
	.if .TMS320C2800_FPU32 | .TMS320C2800_FPU64
	subb SP, #36
	.endif
	movl XAR5, *-SP[8]	; priv
	mov AR6, *-SP[34]	; syscall
	SWAP_STACK
	mov AL, AR6
	lcr _picoRTOS_syscall
	movz DP, #stack
	mov @stack, AR4
	RESTORE_CONTEXT
	iret

	.global _arch_save_first_context
_arch_save_first_context:
	movl XAR6, ACC	; preserve ACC
	movz DP, #stack
	mov @stack, AR4
	SWAP_STACK
	;; auto-save
	push #0x80	; T:ST0
	push #0
	push #0	; AH:AL
	push #0
	push #0	; PH:PL
	push #0
	push #0	; AR1:AR0
	push #0
	push #0x8a08	; DP:ST1
	push #0
	push #0xa000	; DBGSTAT:IER
	push #0
	push XAR5	; PC (fn)
	push #0	; SP starts at even address
	;; user context
	setc INTM	; disable interrupts
	movl XAR4, XAR6 ; XAR4 (priv)
	movl XAR5, #0
	movl XAR6, #0
	SAVE_CONTEXT
	movz AR4, @stack
	lretr

	.global _arch_start_first_task
_arch_start_first_task:
	movz DP, #stack
	mov @stack, AR4
	RESTORE_CONTEXT
	iret

	.global _arch_test_and_set
_arch_test_and_set:
	mov AL, #1
	tset *AR4, #0
	sb .arch_test_and_set_return, TC
	mov AL, #0
.arch_test_and_set_return:
	lretr

	;; INTERRUPTS

	.global Default_Handler
Default_Handler:
	SAVE_CONTEXT
	movz AR4, @stack
	;; get PIEVECT from PIECTRL
	movl XAR5, #ADDR_PIECTRL
	mov AL, *XAR5
	lsr AL, #1
	mov AR6, AL
	lcr _picoRTOS_irq
	movz DP, #stack
	mov @stack, AR4
	RESTORE_CONTEXT
	iret
