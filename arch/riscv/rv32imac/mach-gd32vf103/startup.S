	#include "../../include/macros.S"
	#include "picoRTOS_device.h"

	/* INIT_MTVT */
	.macro INIT_MTVT base
	la t0, \base
	csrw mtvt, t0
	.endm

	/* INIT_MTVEC */
	.equ MTVEC_STANDARD, 0x0
	.equ MTVEC_ECLIC, 0x3

	.macro INIT_MTVEC base, flags
	la t0, \base
	ori t0, t0, \flags
	csrw mtvec, t0
	.endm

	.section .init, "ax"

	.align 4
	.global Reset_Handler
Reset_Handler:
	csrci mstatus, 0x8	/* disable interrupts */
	/* Jump to logical address first to ensure correct
	 * operation of RAM region  */
	la a0, Reset_Handler
	li a1, 1
	slli a1, a1, 29
	bleu a1, a0, .Reset_Handler_Step2
	srli a1, a1, 2
	bleu a1, a0, .Reset_Handler_Step2
	la a0, .Reset_Handler_Step2
	add a0, a0, a1
	jr a0

.Reset_Handler_Step2:
	la gp, __gp
	la tp, __tp
	la sp, __StackTop

	/* relocate text section if needed */
	la t0, __relocate_text
	beq t0, zero, .ignore_text_relocation

	/* copy .text to ram */
	COPY_TO_RAM __edata, __text_start__, __text_end__
	COPY_TO_RAM __vectors, __vectors_start__, __vectors_end__

.ignore_text_relocation:
	/* vectors */
	INIT_MTVEC GeneralException_Handler, MTVEC_ECLIC
	INIT_MTVT __vectors_start__

	/* copy .data section */
	COPY_TO_RAM __etext, __data_start__, __data_end__
	/* zero .bss section */
	ZERO_RAM __bss_start__, __bss_end__

	call main

	.type _exit, %function
	.global _exit
_exit:
	ebreak
	j _exit

	.section .vectors
__vectors:
	j Reset_Handler
	.word 0
	.word 0
	.word MachineSoftwareInterrupt_Handler
	.word 0
	.word 0
	.word 0
	.word MachineTimerInterrupt_Handler
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.word PlatformDefined_Handler
	.endr

	def_irq_handler MachineSoftwareInterrupt_Handler
