SECTIONS
{
        .init :
        {
		__vectors = .;
		__initial_vectors_start = .;
                KEEP(*(.vectors))
                __initial_vectors_end = .;

                KEEP(*(.init))
                KEEP(*(.fini))

                *(.rodata*)
                KEEP(*(.eh_frame*))

                . = ALIGN(4);
        } > FLASH

        /* Location counter can end up 2byte aligned with narrow Thumb code but
           __etext is assumed by startup code to be the LMA of a section in RAM
           which must be 4byte aligned */
        __etext = ALIGN(4);

        .vectors :
        {
                __vectors_start__ = .;
                . = . + (__initial_vectors_end - __initial_vectors_start);
                . = ALIGN(4);
                __vectors_end__ = .;
        } > RAM

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
        . = ALIGN(8);

        .text : AT (__edata)
        {
                __text_start__ = .;
                *(.text*)
                . = ALIGN(4);
                __text_end__ = .;
        } > RAM

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
