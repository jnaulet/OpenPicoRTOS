	.syntax unified
	.arch armv7e-m

	#include "picoRTOS_device.h"
	#include "../../include/macros.S"

	.section .init, "ax"

	.equ RCC_AHB2ENR, 0x580244dc

	.macro TURN_AUX_SRAM_ON
	ldr r11, =RCC_AHB2ENR
	ldr r0, [r11]
	orr r0, r0, #(0x7 << 29)
	str r0, [r11]
	.endm

	.equ FMC_BCRx, 0x52004000

	.macro DISABLE_FMC_BANK1
	ldr r11, =FMC_BCRx
	mov r0, #0x30d2
	str r0, [r11]
	.endm

	.thumb_func
	.type Reset_Handler, %function
	.global Reset_Handler
Reset_Handler:
	/* All SRAM available */
	TURN_AUX_SRAM_ON
	DISABLE_FMC_BANK1

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

	/* data cache */
	INVALIDATE_DCACHE
	ENABLE_D_CACHE
	/* instruction cache */
	INVALIDATE_ICACHE
	ENABLE_I_CACHE

	/* fpu */
	ENABLE_VFP

	/* run main() */
	ldr r1, =main
	blx r1

	.thumb_func
	.type _exit, %function
	.global _exit
_exit:
	bkpt #0
	b _exit
