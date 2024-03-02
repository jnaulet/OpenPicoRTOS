	/* RISC-V macros & definitions
	 * Provides a few useful macros:
	 * COPY_TO_RAM
	 * ZERO_RAM
	*/
	#include "regs.h"

	/* COPY_TO_RAM */
	.macro COPY_TO_RAM base, start, end
	la t0, \base
	la t1, \start
	la t2, \end
	j 2f
1:
	lw a0, (t0)
	sw a0, (t1)
	addi t0, t0, 4
	addi t1, t1, 4
2:
	bne t1, t2, 1b
	.endm

	/* ZERO_RAM */
	.macro ZERO_RAM start, end
	la t0, \start
	la t1, \end
	j 2f
1:
	sw zero, (t0)
	addi t0, t0, 4
2:
	bne t0, t1, 1b
	.endm

	/* def_irq_handler */
	.macro def_irq_handler handler_name
	.align 4
	.weak \handler_name
\handler_name:
	ebreak
	j \handler_name
	.endm
