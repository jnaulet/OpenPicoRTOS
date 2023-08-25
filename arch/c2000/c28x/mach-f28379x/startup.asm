	.cdecls C, NOLIST, "picoRTOS_device.h"
	.include "../../include/macros.asm"

	.sect .init
	.retain

	.if __TI_EABI__
	.asg RamfuncsLoadStart, _RamfuncsLoadStart
	.asg RamfuncsLoadSize, _RamfuncsLoadSize
	.asg RamfuncsRunStart, _RamfuncsRunStart
	.asg RamfuncsRunEnd, _RamfuncsRunEnd
	.endif

	.global _RamfuncsLoadStart
	.global _RamfuncsLoadSize
	.global _RamfuncsRunStart
	.global _RamfuncsRunEnd

	.ref _c_int00

	.global Reset_Handler
Reset_Handler:
	;; disable watchdog for copy (FIXME: debatable)
	TURN_WATCHDOG_OFF

	;; copy code to RAM if needed
	mov AL, #_RamfuncsLoadSize
	addb AL, #-1
	bf .ignore_text_relocation, NC

	COPY_TO_RAM #_RamfuncsLoadStart, #_RamfuncsRunStart, #_RamfuncsRunEnd

.ignore_text_relocation:
	;; copy vectors
	eallow
	COPY_TO_RAM #__vectors, #ADDR_PIEVECTTABLE, #ADDR_PIEVECTTABLE + 0x1c0
	edis

	;; enable ePIE
	mov AL, #1
	mov *(0:ADDR_PIECTRL), AL

	;; don't re-enable watchdog
	;; TURN_WATCHDOG_ON

	;; branch to main through RTS library
	lb _c_int00

_exit:
	estop0
	lb _exit

	.ref Default_Handler
	.ref TIMER2_Handler
	.ref RealTimeOS_Handler

__vectors:
	.long Reset_Handler
	.long 0	; unused
	.long 0	; unused
	.long 0	; unused
	.long 0	; unused
	.long 0	; unused
	.long 0	; unused
	.long 0	; unused
	.long 0	; unused
	.long 0	; unused
	.long 0	; unused
	.long 0	; unused
	.long 0	; unused
	.long TIMER1_Handler		; CPU TIMER1 Interrupt
	.long TIMER2_Handler		; CPU TIMER2 Interrupt
	.long DataLogging_Handler	; CPU Data Logging Interrupt
	.long RealTimeOS_Handler	; CPU Real-Time OS Interrupt
	.long Emulation_Handler	; CPU Emulation Interrupt
	.long NMI_Handler		; Non-Maskable Interrupt
	.long ITRAP_Handler		; Illegal Instruction (ITRAP)
	.long USER1_Handler		; User-Defined Trap
	.long USER2_Handler		; User-Defined Trap
	.long USER3_Handler		; User-Defined Trap
	.long USER4_Handler		; User-Defined Trap
	.long USER5_Handler		; User-Defined Trap
	.long USER6_Handler		; User-Defined Trap
	.long USER7_Handler		; User-Defined Trap
	.long USER8_Handler		; User-Defined Trap
	.long USER9_Handler		; User-Defined Trap
	.long USER10_Handler		; User-Defined Trap
	.long USER11_Handler		; User-Defined Trap
	.long USER12_Handler		; User-Defined Trap
	.loop DEVICE_INTERRUPT_VECTOR_COUNT
	.long Default_Handler
	.endloop

	;; weak vectors
	def_irq_handler TIMER1_Handler
	def_irq_handler DataLogging_Handler
	def_irq_handler Emulation_Handler
	def_irq_handler NMI_Handler
	def_irq_handler ITRAP_Handler
	def_irq_handler USER1_Handler
	def_irq_handler USER2_Handler
	def_irq_handler USER3_Handler
	def_irq_handler USER4_Handler
	def_irq_handler USER5_Handler
	def_irq_handler USER6_Handler
	def_irq_handler USER7_Handler
	def_irq_handler USER8_Handler
	def_irq_handler USER9_Handler
	def_irq_handler USER10_Handler
	def_irq_handler USER11_Handler
	def_irq_handler USER12_Handler

	.end
