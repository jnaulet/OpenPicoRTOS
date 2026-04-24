SECTIONS
{
        .text :
        {
                __text_start__ = .;
                __vectors_start__ = .;
		__vectors = .;
                KEEP(*(.vectors))
                __vectors_end__ = .;

                KEEP(*(.init))
                KEEP(*(.fini))
		
                *(.text*)
		*(.rodata*)
                KEEP(*(.eh_frame*))

                . = ALIGN(4);
                __text_end__ = .;
        } > FLASH

        /* Location counter can end up 2byte aligned with narrow Thumb code but
           __etext is assumed by startup code to be the LMA of a section in RAM
           which must be 4byte aligned */
        __etext = ALIGN (4);

        .data : AT (__etext)
        {
                __data_start__ = .;
		KEEP(*(.init_array))
		KEEP(*(.fini_array))
                KEEP(*(.data*))

                . = ALIGN(4);
                __data_end__ = .;
        } > RAM

        __edata = __etext + (__data_end__ - __data_start__);

        .bss :
        {
                . = ALIGN(4);
                __bss_start__ = .;
                *(.bss*)
                *(COMMON)
                . = ALIGN(4);
                __bss_end__ = .;
        } > RAM

	/DISCARD/ : { *(.dynsym) }
	/DISCARD/ : { *(.dynstr*) }
	/DISCARD/ : { *(.dynamic*) }
	/DISCARD/ : { *(.plt*) }
	/DISCARD/ : { *(.interp*) }
	/DISCARD/ : { *(.gnu*) }
	/DISCARD/ : { *(.ARM*) }
}
