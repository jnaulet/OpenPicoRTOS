	/* PPC e200z7 VLE startup */
	#include "picoRTOS_device.h"
	#include "../include/macros.S"

	.equ PCU_CESR, ADDR_PCU + 0x0

	.macro SETUP_PCU x
	e_li r3, \x
	E_LIW r4, PCU_CESR
	e_stw r3, 0 (r4)
	.endm

	.section .init, "ax"

	.long 0x015a0000    /* MPC57xx_ID + VLE */
	.long Reset_Handler /* CPU0 reset vector */

	.align 4
	.global Reset_Handler
Reset_Handler:
	wrteei 0	/* disable interrupts */
	TURN_WATCHDOG_OFF

	/* MMU */
	e_bl arch_init_mmu

	INIT_E2EECC __ram_start__, __ram_end__
	ENABLE_BTB
	CLEAR_RESERVATIONS_ON_EXT_INTERRUPT
	ENABLE_MSR_ME

	INVALIDATE_AND_ENABLE_CACHE ICACHE
	INVALIDATE_AND_ENABLE_CACHE DCACHE
	SETUP_PCU 0xf001		/* bus snooping */

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

	/* IVORs */
	e_bl arch_init_ivors

	/* jump to main */
	e_bl main

__exit:
	se_illegal
	nop
	e_b __exit

	.type arch_init_mmu, %function
	.global arch_init_mmu
arch_init_mmu:
	/* MAS0: TBLSEL (1) & ESEL (Entry select)
	 * MAS1: VALID + IPROT (0xc) & TSIZE
	 * MAS2: EPN (Effective page number) & VLE + WM (write-through)
	 * MAS3: RPN (Real page number) & permissions (0x3f) */
	SET_MMU_TLB 0x10000000, 0xc0000700, 0x00000020, 0x0000003f
	SET_MMU_TLB 0x10010000, 0xc0000480, 0x40000034, 0x4000003f
	SET_MMU_TLB 0x10020000, 0xc0000580, 0xffe0000a, 0xffe0003f
	SET_MMU_TLB 0x10030000, 0xc0000700, 0x20000020, 0x0000003f
	SET_MMU_TLB 0x10040000, 0xc0000580, 0xc3e0000a, 0xc3e0003f
	se_blr

	.type arch_init_ivors, %function
	.global arch_init_ivors
arch_init_ivors:
	INIT_IVOR 400, CriticalInput
	INIT_IVOR 401, MachineCheck
	INIT_IVOR 402, DataStorage
	INIT_IVOR 403, InstructionStorage
	INIT_IVOR 404, ExternalInput
	INIT_IVOR 405, Alignment
	INIT_IVOR 406, Program
	INIT_IVOR 407, FloatingPointUnavailable
	INIT_IVOR 408, SystemCall
	INIT_IVOR 409, APUnavailable
	INIT_IVOR 410, Decrementer
	INIT_IVOR 411, FixedIntervalTimerInterrupt
	INIT_IVOR 412, WatchdogTimerInterrupt
	INIT_IVOR 413, DataTLBError
	INIT_IVOR 414, InstructionTLBError
	INIT_IVOR 415, Debug
	INIT_IVOR 528, SPEUnavailableException
	INIT_IVOR 529, EFPDataException
	INIT_IVOR 530, EFPRoundException
	INIT_IVOR 531, PerformanceMonitorInterrupt
	se_blr

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
	IVOR PerformanceMonitorInterrupt, PerformanceMonitorInterrupt_Handler

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
	def_irq_handler PerformanceMonitorInterrupt_Handler

	/* hijack newlib */
	.global __eabi
__eabi:
	se_blr
