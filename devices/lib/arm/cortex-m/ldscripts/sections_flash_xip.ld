SECTIONS
{
        .text :
        {
                __text_start__ = .;
                __vectors_start__ = .;
		__vectors__ = .;
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

                . = ALIGN(4);
                __text_end__ = .;
        } > FLASH

        .ARM.extab :
        {
                *(.ARM.extab* .gnu.linkonce.armextab.*)
        } > FLASH

        __exidx_start = .;
        .ARM.exidx :
        {
                *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        } > FLASH
        __exidx_end = .;

        /* To copy multiple ROM to RAM sections,
         * uncomment .copy.table section and,
         * define __STARTUP_COPY_MULTIPLE in startup_ARMCMx.S */
        /*
        .copy.table :
        {
                . = ALIGN(4);
                __copy_table_start__ = .;
                LONG (__etext)
                LONG (__data_start__)
                LONG (__data_end__ - __data_start__)
                LONG (__etext2)
                LONG (__data2_start__)
                LONG (__data2_end__ - __data2_start__)
                __copy_table_end__ = .;
        } > FLASH
        */

        /* To clear multiple BSS sections,
         * uncomment .zero.table section and,
         * define __STARTUP_CLEAR_BSS_MULTIPLE in startup_ARMCMx.S */
        /*
        .zero.table :
        {
                . = ALIGN(4);
                __zero_table_start__ = .;
                LONG (__bss_start__)
                LONG (__bss_end__ - __bss_start__)
                LONG (__bss2_start__)
                LONG (__bss2_end__ - __bss2_start__)
                __zero_table_end__ = .;
        } > FLASH
        */

        /* Location counter can end up 2byte aligned with narrow Thumb code but
           __etext is assumed by startup code to be the LMA of a section in RAM
           which must be 4byte aligned */
        __etext = ALIGN (4);

        .data : AT (__etext)
        {
                __data_start__ = .;
                *(vtable)
                *(.data*)

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

        /* All these sections are to be user-defined */

        /* This is not exactly necessary on OpenPicoRTOS,
         * at least for the moment
        .heap (COPY):
        {
                __end__ = .;
                PROVIDE(end = .);
                *(.heap*)
                __HeapLimit = .;
        } > RAM
        */

        /* .stack_dummy section doesn't contain any symbols. It is only
         * used for linker to calculate size of stack sections, and assign
         * values to stack symbols later
        .stack_dummy (COPY):
        {
                *(.stack*)
        } > RAM
        */

        /* Set stack top to end of RAM, and stack limit move down by
         * size of stack_dummy section
        __StackTop = ORIGIN(RAM) + LENGTH(RAM);
        __StackLimit = __StackTop - SIZEOF(.stack_dummy);
        PROVIDE(__stack = __StackTop);
        */

        /* Check if data + heap + stack exceeds RAM limit
        ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
        */
}
