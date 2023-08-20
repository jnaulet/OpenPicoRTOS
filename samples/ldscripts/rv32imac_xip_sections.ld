SECTIONS
{
        .text :
        {
                __text_start__ = .;
                __vectors_start__ = .;
		__vectors = .;
                KEEP(*(.vectors))
                __vectors_end__ = .;
                *(.text*)

                KEEP(*(.init))
                KEEP(*(.fini))

                /* .ctors */
                *crtbegin.o(.ctors)
                *crtbegin?.o(.ctors)
                *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
                *(SORT(.ctors.*))
                *(.ctors)

                /* .dtors */
                *crtbegin.o(.dtors)
                *crtbegin?.o(.dtors)
                *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
                *(SORT(.dtors.*))
                *(.dtors)

                *(.rodata*)
                *(.note.*)

                KEEP(*(.eh_frame*))
                *(.gcc_except_table .gcc_except_table.*)
                *(.gnu_extab*)
                *(.exception_ranges*)
                
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
                *(vtable)
                *(.data*)

                . = ALIGN(8);
                __gp = . + 0x800;
                *(.sdata .sdata.* .gnu.linkonce.s.*);

                . = ALIGN(8);
                __tp = .;
                *(.tdata .tdata.* .gnu.linkonce.td.*)

                . = ALIGN(4);
                /* preinit data */
                PROVIDE_HIDDEN (__preinit_array_start = .);
                KEEP(*(.preinit_array))
                PROVIDE_HIDDEN (__preinit_array_end = .);

                . = ALIGN(4);
                /* init data */
                PROVIDE_HIDDEN (__init_array_start = .);
                KEEP(*(SORT(.init_array.*)))
                KEEP(*(.init_array))
                PROVIDE_HIDDEN (__init_array_end = .);

                . = ALIGN(4);
                /* finit data */
                PROVIDE_HIDDEN (__fini_array_start = .);
                KEEP(*(SORT(.fini_array.*)))
                KEEP(*(.fini_array))
                PROVIDE_HIDDEN (__fini_array_end = .);

                KEEP(*(.jcr*))
                . = ALIGN(4);
                /* All data end */
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

        .stack (NOLOAD):
        {
                . = ALIGN(8);
                __stack_start__ = .;
                . = . + STACK_SIZE;
                . = ALIGN(8);
                __stack_end__ = .;
        } > ram

        __StackTop = __stack_end__;
        __StackBottom = __stack_start__;

        /* All these sections are to be user-defined */
}
