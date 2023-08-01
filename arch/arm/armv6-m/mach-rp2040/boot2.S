	.syntax unified
	.cpu cortex-m0plus

	#include "picoRTOS_device.h"

	/* padded and checksumed version of bs2_default */
	.section .boot2, "ax"
stage2_boot:
	push {lr}
	ldr r3, .literals
	movs r0, #0x21
	str r0, [r3, #4]
	ldr r0, [r3, #8]
	movs r1, #2
	bics r0, r1
	str r0, [r3, #8]
	str r0, [r3, #12]
	str r0, [r3, #16]
	str r0, [r3, #20]
	ldr r3, .literals + 4
	movs r1, #0
	str r1, [r3, #8]
	movs r1, #2
	str r1, [r3, #20]
	movs r1, #1
	movs r2, #0xf0
	str r1, [r3, r2]

program_sregs:
	ldr r1, .literals + 8
	str r1, [r3, #0]
	movs r1, #1
	str r1, [r3, #8]
	movs r0, #0x35
	bl read_flash_sreg
	movs r2, #2
	cmp r0, r2
	beq.n skip_sreg_programming
	movs r1, #6
	str r1, [r3, #96]
	bl wait_ssi_ready
	ldr r1, [r3, #96]
	movs r1, #1
	str r1, [r3, #96]
	movs r0, #0
	str r0, [r3, #96]
	str r2, [r3, #96]
	bl wait_ssi_ready
	ldr r1, [r3, #96]
	ldr r1, [r3, #96]
	ldr r1, [r3, #96]
.loop0:
	movs r0, #5
	bl read_flash_sreg
	movs r1, #1
	tst r0, r1
	bne.n .loop0

skip_sreg_programming:
	movs r1, #0
	str r1, [r3, #8]

dummy_read:
	ldr r1, .literals + 12
	str r1, [r3, #0]
	movs r1, #0
	str r1, [r3, #4]
	ldr r1, .literals + 16
	ldr r0, .literals + 20
	str r1, [r0, #0]
	movs r1, #1
	str r1, [r3, #8]
	movs r1, #0xeb
	str r1, [r3, #96]
	movs r1, #0xa0
	str r1, [r3, #96]
	bl wait_ssi_ready
	movs r1, #0
	str r1, [r3, #8]

configure_ssi:
	ldr r1, .literals + 24
	ldr r0, .literals + 20
	str r1, [r0, #0]
	movs r1, #1
	str r1, [r3, #8]

check_return:
	pop {r0}
	cmp r0, #0
	beq.n vector_into_flash
	bx r0

vector_into_flash:
	ldr r0, .literals + 28
	ldr r1, .literals + 32
	str r0, [r1, #0]
	ldmia r0, {r0, r1}
	msr msp, r0
	bx r1

wait_ssi_ready:
	push {r0, r1, lr}
.loop1:
	ldr r1, [r3, #40]
	movs r0, #4
	tst r1, r0
	beq.n .loop1
	movs r0, #1
	tst r1, r0
	bne.n .loop1
	pop {r0, r1, pc}

read_flash_sreg:
	push {r1, lr}
	str r0, [r3, #96]
	str r0, [r3, #96]
	bl wait_ssi_ready
	ldr r0, [r3, #96]
	ldr r0, [r3, #96]
	pop {r1, pc}

	.align 2
.literals:
	.word ADDR_PADS_QSPI
	.word ADDR_XIP_SSI
	.word 0x00070000
	.word 0x005f0300
	.word 0x00002221
	.word ADDR_XIP_SSI + 0xf4
	.word 0xa0002022
	.word 0x10000100
	.word 0xe000ed08 /* VTOR */

	/* CRC */
	.org stage2_boot + 252, 0
	.word 0x7a4eb274
