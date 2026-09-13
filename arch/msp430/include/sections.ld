/*
 * SECTION definitions for all MSP430 platforms
 */
SECTIONS
{
	.text :
	{
                . = ALIGN(2);
		__text_start__ = .;
		KEEP(*(.init*))
                KEEP(*(.fini*))
		KEEP(*(.rodata*))
		*(.text*)

		. = ALIGN(2);
		__text_end__ = .;
	} > ROM

	PROVIDE(__etext = .);

	.data :
	{
	        . = ALIGN(2);
                __data_start__ = .;
		KEEP(*(.init_array))
		KEEP(*(.fini_array))
                KEEP(*(.data*))
                KEEP(*(.sdata*))

                . = ALIGN(2);
                __data_end__ = .;
	} > RAM AT> ROM

	.bss (NOLOAD) :
	{
		__bss_start__ = .;
                *(.bss*)
                *(COMMON)

		. = ALIGN(2);
		__bss_end__ = .;
	} > RAM

	.vectors :
	{
		__vectors_start__ = .;
		KEEP(*(.vectors*))
		__vectors_end__ = .;
	} > VECTORS

	/DISCARD/ : { *(.dynsym) }
	/DISCARD/ : { *(.dynstr*) }
	/DISCARD/ : { *(.dynamic*) }
	/DISCARD/ : { *(.plt*) }
	/DISCARD/ : { *(.interp*) }
	/DISCARD/ : { *(.gnu*) }
	/DISCARD/ : { *(.iplt*) }
	/DISCARD/ : { *(.rela*) }
}
