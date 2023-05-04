	;; Macros for TI C2000 machines

def_irq_handler .macro handler
	.weak handler
	.global handler
handler:
	estop0
	lb handler
	.endm

TURN_WATCHDOG_OFF .macro
	eallow
	movl XAR4, #(ADDR_WD + 0x29)	; WD_WCSR
	mov *XAR4, #0x68
	edis
	.endm

TURN_WATCHDOG_ON .macro
	eallow
	movl XAR4, #(ADDR_WD + 0x29)	; WD_WCSR
	mov *XAR4, #0x28
	edis
	.endm

COPY_TO_RAM .macro base, start, end
	movl XAR4, start
	mov ACC, end - 1
	subl ACC, XAR4
	movl XAR7, base
	rpt @AL || pread *XAR4++, *XAR7
	.endm
