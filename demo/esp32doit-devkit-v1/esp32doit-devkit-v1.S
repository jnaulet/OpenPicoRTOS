	/* vectors */
	.global WindowVectors
	.global Level2InterruptVector
	.global Level3InterruptVector
	.global Level4InterruptVector
	.global Level5InterruptVector
	.global DebugExceptionVector
	.global NMIExceptionVector
	.global KernelExceptionVector
	.global UserExceptionVector
	.global DoubleExceptionVector

	.extern Window_Handler
	.extern Level2Interrupt_Handler
	.extern Level3Interrupt_Handler
	.extern Level4Interrupt_Handler
	.extern Level5Interrupt_Handler
	.extern DebugException_Handler
	.extern NMIException_Handler
	.extern KernelException_Handler
	.extern UserException_Handler
	.extern DoubleException_Handler

	.section .vector_table, "ax"
	.align 4

	.rept 96
	.long 0
	.endr

	.align 64
WindowVectors:
	call0 Window_Handler

	.align 64
Level2InterruptVector:
	call0 Level2Interrupt_Handler

	.align 64
Level3InterruptVector:
	call0 Level3Interrupt_Handler

	.align 64
Level4InterruptVector:
	call0 Level4Interrupt_Handler

	.align 64
Level5InterruptVector:
	call0 Level5Interrupt_Handler

	.align 64
DebugExceptionVector:
	call0 DebugException_Handler

	.align 64
NMIExceptionVector:
	call0 NMIException_Handler

	.align 64
KernelExceptionVector:
	call0 KernelException_Handler

	.align 64
UserExceptionVector:
	call0 UserException_Handler

	.align 64
DoubleExceptionVector:
	call0 DoubleException_Handler

	.section .init
	.align 4

.Lstack:
	.word __stack
	.weak __stack

.LC1:
	.word __bss_start__
	.word __bss_size__

	.align 4
_stack_init:
	movi a2, .Lstack
	l32i sp, a2, 0
	ret

	.align 4
_bss_init:
	movi a2, .LC1
	l32i a3, a2, 0
	l32i a2, a2, 4
	slli a2, a2, 2
	/* put bss to zero */
	movi a4, 0
	loop a2, .L_bss_init_zero
	s32i a4, a3, 0
	addi a3, a3, 4
.L_bss_init_zero:
	ret

	.global _start_cpu0
	.align 4
_start_cpu0:
	call0 _bss_init
	call0 _stack_init
	/* jump to main */
	movi a4, main
	jx a4

.macro default_handler name
	.weak \name
	.align 4
\name:
	break 1, 1
	j \name
.endm

	.text
default_handler Window_Handler
default_handler Level2Interrupt_Handler
default_handler Level3Interrupt_Handler
default_handler Level4Interrupt_Handler
default_handler Level5Interrupt_Handler
default_handler DebugException_Handler
default_handler NMIException_Handler
default_handler KernelException_Handler
default_handler UserException_Handler
default_handler DoubleException_Handler
