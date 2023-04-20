	/* PPC e22z7 VLE startup */
	#include "picoRTOS_device.h"
	#include "../../lib/ppc/vle/macros.S"

	.section .init, "ax"

	.equ PCU_CESR, ADDR_PCU + 0x0

	.macro SETUP_PCU x
	e_li r3, \x
	E_LIW r4, PCU_CESR
	e_stw r3, 0 (r4)
	.endm

	.align 4
	.global Reset_Handler
Reset_Handler:
	/* first, setup mmu */
	E_LIW r0, device_mmu_setup
	se_mtlr r0
	se_blrl

	TURN_WATCHDOG_OFF
	ENABLE_BTB
	CLEAR_RESERVATIONS_ON_EXT_INTERRUPT
	ENABLE_MSR_ME

	/* TODO: zero ram for ECC */

	INVALIDATE_AND_ENABLE_CACHE ICACHE
	INVALIDATE_AND_ENABLE_CACHE DCACHE
	SETUP_PCU 0xf001		/* bus snooping */

	E_LIW r1, __StackTop		/* stack */
	E_LIW r2, __sdata2_start__	/* sdata2 */
	E_LIW r13, __sdata_start__	/* sdata */

	E_LIW r0, __relocate_text
	se_cmp r0, 0
	se_bne .ignore_text_relocation

	/* copy .text to ram */
	COPY_TO_RAM __etext, __text_start__, __text_end__
	COPY_TO_RAM __vectors, __vectors_start__, __vectors_end__

.ignore_text_relocation:
	/* setup IVORs */
	e_bl device_init_ivpr_and_ivors
	/* copy .data section */
	COPY_TO_RAM __sdata, __data_start__, __data_end__
	/* zero .bss section */
	ZERO_RAM __bss_start__, __bss_end__

	/* jump to main */
	e_bl main

__exit:
	se_illegal
	nop
	e_b __exit

	.type device_mmu_setup, %function
	.global device_mmu_setup
device_mmu_setup:
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

	.type device_init_ivpr_and_ivors, %function
	.global device_init_ivpr_and_ivors
device_init_ivpr_and_ivors:
	/* IVPR */
	e_lis r0, __vectors_start__@ha
	mtivpr r0
	/* IVORS */
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
__vectors:
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
	.long 0
