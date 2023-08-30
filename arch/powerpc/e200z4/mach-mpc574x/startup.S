	/* PPC e200z4 VLE startup */
	#include "picoRTOS_device.h"
	#include "../include/macros.S"

	/* DISABLE_SWT */
	.macro DISABLE_SWT addr
	E_LIW r3, \addr
	e_li r4, 0xc520
	e_stw r4, 0x10 (r3)
	e_li r4, 0xd928
	e_stw r4, 0x10 (r3)
	E_LIW r4, 0xff000102
	e_stw r4, 0 (r3)
	.endm

	/* required for boot */
	.section .boot, "a"

	.long 0x005a0002    /* MPC57xx_ID + CPU0 enabled */
	.long 0	    /* CPU2 reset vector */
	.long 0	    /* reserved */
	.long 0	    /* reserved */
	.long Reset_Handler /* CPU0 reset vector */
	.long 0	    /* CPU1 reset vector */

	.section .init, "ax"

	.align 4
	.global Reset_Handler
Reset_Handler:
	wrteei 0	/* disable interrupts */

	/* SWT watchdog(s) */
	DISABLE_SWT ADDR_SWT0
	DISABLE_SWT ADDR_SWT1
	DISABLE_SWT ADDR_SWT2

	INIT_E2EECC __ram_start__, __ram_end__
	ENABLE_BTB
	CLEAR_RESERVATIONS_ON_EXT_INTERRUPT
	ENABLE_MSR_ME

	INVALIDATE_AND_ENABLE_CACHE ICACHE
	INVALIDATE_AND_ENABLE_CACHE DCACHE

	E_LIW r1, __StackTop	/* stack */
	E_LIW r2, _SDA2_BASE_	/* provided by powerpc-eabivle- toolchain */
	E_LIW r13, _SDA_BASE_	/* provided by powerpc-eabivle- toolchain */

	E_LIW r7, __relocate_text
	se_cmpi r7, 1
	se_bne .ignore_text_relocation

	/* copy .text to ram */
	COPY_TO_RAM __esdata, __text_start__, __text_end__

.ignore_text_relocation:
	/* copy .data section(s) */
	COPY_TO_RAM __etext, __data_start__, __data_end__
	COPY_TO_RAM __edata, __sdata_start__, __sdata_end__
	/* zero .bss section */
	ZERO_RAM __bss_start__, __bss_end__

	/* IVPR */
	E_LIW r0, __vectors_start__
	mtivpr r0

	/* jump to main */
	e_bl main

__exit:
	se_illegal
	nop
	e_b __exit

	.section .vectors, "ax"

	.macro IVOR name, handler
	.align 4
\name:
	e_b \handler
	.endm

	.align 4
	.global __IVORs
__IVORs:
	IVOR CriticalInput, CriticalInput_Handler
	IVOR MachineCheck, MachineCheck_Handler
	IVOR DataStorage, DataStorage_Handler
	IVOR InstructionStorage, InstructionStorage_Handler
	IVOR ExternalInput, ExternalInput_Handler
	IVOR Alignment, Alignment_Handler
	IVOR Program, Program_Handler
	IVOR FloatingPointUnavailable, FloatingPointUnavailable_Handler
	IVOR SystemCall, SystemCall_Handler
	IVOR APUnavailable, APUnavailable_Handler
	IVOR Decrementer, Decrementer_Handler
	IVOR FixedIntervalTimerInterrupt, FixedIntervalTimerInterrupt_Handler
	IVOR WatchdogTimerInterrupt, WatchdogTimerInterrupt_Handler
	IVOR DataTLBError, DataTLBError_Handler
	IVOR InstructionTLBError, InstructionTLBError_Handler
	IVOR Debug, Debug_Handler
	IVOR SPEUnavailableException, SPEUnavailableException_Handler
	IVOR EFPDataException, EFPDataException_Handler
	IVOR EFPRoundException, EFPRoundException_Handler

	/* Default handlers */
	def_irq_handler CriticalInput_Handler
	def_irq_handler MachineCheck_Handler
	def_irq_handler DataStorage_Handler
	def_irq_handler InstructionStorage_Handler
	def_irq_handler Alignment_Handler
	def_irq_handler Program_Handler
	def_irq_handler FloatingPointUnavailable_Handler
	def_irq_handler APUnavailable_Handler
	def_irq_handler Decrementer_Handler
	def_irq_handler FixedIntervalTimerInterrupt_Handler
	def_irq_handler WatchdogTimerInterrupt_Handler
	def_irq_handler DataTLBError_Handler
	def_irq_handler InstructionTLBError_Handler
	def_irq_handler Debug_Handler
	def_irq_handler SPEUnavailableException_Handler
	def_irq_handler EFPDataException_Handler
	def_irq_handler EFPRoundException_Handler

	/* hijack newlib */
	.global __eabi
__eabi:
	se_blr
