	.syntax unified
	.arch armv7e-m

	#include "picoRTOS_device.h"
	#include "../../lib/arm/cortex-m/macros.S"

	.section .vectors, "ax"
	.global __vectors
__vectors:
	.word __StackTop
	.word Reset_Handler
	.word NMI_Handler
	.word HardFault_Handler
	.word MemManage_Handler
	.word BusFault_Handler
	.word UsageFault_Handler
	.word 0 /* reserved */
	.word 0 /* reserved */
	.word 0 /* reserved */
	.word 0 /* reserved */
	.word SVC_Handler
	.word DebugMon_Handler
	.word 0 /* reserved */
	.word PendSV_Handler
	.word SysTick_Handler
	.rept DEVICE_INTERRUPT_VECTOR_COUNT
	.word Default_Handler
	.endr

	.section .init, "ax"

	.thumb_func
	.type Reset_Handler, %function
	.global Reset_Handler
Reset_Handler:
	/* ignore if __text_end__ < __etext */
	ldr r1, =__relocate_text
	cmp r1, #0
	beq .ignore_text_relocation

	/* copy .text to ram */
	COPY_TO_RAM __edata, __text_start__, __text_end__
	COPY_TO_RAM __vectors, __vectors_start__, __vectors_end__

.ignore_text_relocation:
	/* init vectors & stack */
	INIT_VTOR __vectors_start__
	INIT_STACK __StackTop
	/* copy .data section */
	COPY_TO_RAM __etext, __data_start__, __data_end__
	/* zero .bss section */
	ZERO_RAM __bss_start__, __bss_end__

	/* enable fpu */
	ENABLE_VFP
	/* enable caches */
	ldr r0, =ADDR_CMCC + 0x8
	mov r1, #0x1
	str r1, [r0]

	/* run main() */
	ldr r1, =main
	blx r1

	.thumb_func
	.type _exit, %function
	.global _exit
_exit:
	bkpt #0
	b _exit

	/* Weak handlers */
	def_irq_handler NMI_Handler
	def_irq_handler HardFault_Handler
	def_irq_handler MemManage_Handler
	def_irq_handler BusFault_Handler
	def_irq_handler UsageFault_Handler
	def_irq_handler DebugMon_Handler
	def_irq_handler PendSV_Handler
