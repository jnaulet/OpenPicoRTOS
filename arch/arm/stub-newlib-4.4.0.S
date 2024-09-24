	.syntax unified

	/* STUB_NEWLIB */
	.macro STUB_NEWLIB func
	.thumb_func
	.type \func, %function
	.global \func
\func:
	bx lr
	.endm

	/* To ensure we link */
	STUB_NEWLIB _write
	STUB_NEWLIB _sbrk
	STUB_NEWLIB _close
	STUB_NEWLIB _read
	STUB_NEWLIB _lseek
