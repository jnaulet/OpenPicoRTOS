	.syntax unified
	.cpu cortex-m0plus

	#include "boot2.S"
	#include "picoRTOS_device.h"
	#include "../../include/macros.S"

	.macro ROM_TABLE_LOOKUP code
	movs r0, 0x00000014
	ldrh r0, [r0]
	ldr r1, =\code
	movs r2, 0x00000018
	ldrh r2, [r2]
	blx r2
	.endm

	.macro HOLD_CORE1_IN_BOOTROM
	ldr r0, =(ADDR_SIO + 0x0)
	ldr r0, [r0]
	cmp r0, #0
	beq 1f

	ROM_TABLE_LOOKUP ('W' | ('V' << 8))
	bx r0
1:
	.endm

	.macro RESET_ALL_SPINLOCKS
	movs r0, #0
	movs r1, #32
	ldr r2, =ADDR_SIO + 0x100
1:
	dmb
	stm r2!, {r0}
	subs r1, #1
	bne 1b
	.endm

	.section .init, "ax"

	.thumb_func
	.type Reset_Handler, %function
	.global Reset_Handler
Reset_Handler:
	HOLD_CORE1_IN_BOOTROM

	/* relocate text section if needed */
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

	/* ensure spinlocks are ready */
	RESET_ALL_SPINLOCKS

	/* run main() */
	ldr r1, =main
	blx r1

	.thumb_func
	.type _exit, %function
	.global _exit
_exit:
	bkpt #0
	b _exit
