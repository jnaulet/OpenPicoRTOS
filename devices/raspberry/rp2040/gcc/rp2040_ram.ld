/*
 * Memory map for RP2040 (RAM, copy)
 */

MEMORY
{
    rom(rx)        : ORIGIN = 0x10000000, LENGTH = 2048k
    ram(rwx)       : ORIGIN = 0x20000000, LENGTH = 256k
    scratch_x(rwx) : ORIGIN = 0x20040000, LENGTH = 4k
    scratch_y(rwx) : ORIGIN = 0x20041000, LENGTH = 4k
}

PROVIDE(__relocate_text = 0x1);
ENTRY(Reset_Handler)

SECTIONS
{
    .boot2 :
    {
        __boot2_start__ = .;
        KEEP (*(.boot2))
        __boot2_end__ = .;
    } > rom

    .init :
    {
        __init_start__ = .;
        KEEP(*(.vectors .vectors.*))
        *(.rodata .rodata* .gnu.linkonce.r.*)
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        
        . = ALIGN(4);
        KEEP(*(.init))

        . = ALIGN(4);
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))

        . = ALIGN(4);
        KEEP(*(.fini))
        
        . = ALIGN(4);
        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))

        . = ALIGN(4);
        *(.eh_frame*)
        *(.note.gnu.*)
        
        . = ALIGN(4);
        __init_end__ = .;
    } > rom
    
    PROVIDE_HIDDEN (__exidx_start = .);
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > rom
    PROVIDE_HIDDEN (__exidx_end = .);

    . = ALIGN(4);
    __etext = .;

    .text : AT (__etext)
    {
        __text_start__ = .;
        /* leave room to copy vectors */
        __vectors_start__ = .;
        . = . + 168;
        . = ALIGN(4);
        __vectors_end__ = .;

        *(.text .text.* .gnu.linkonce.t.*)
        . = ALIGN(4);
        __text_end__ = .;
    } > ram

    __sdata = __etext + (__text_end__ - __text_start__);

    .data : AT (__sdata)
    {
        __data_start__ = .;
        *(.data .data.*);

        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;
        
        . = ALIGN(4);
        __init_array_start = .;
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        __init_array_end = .;

        . = ALIGN(4);
        __fini_array_start = .;
        *(SORT(.fini_array.*))
        *(.fini_array)
        __fini_array_end = .;

        . = ALIGN(4);
        __data_end__ = .;
    } > ram
    
    .bss (NOLOAD):
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > ram

    .stack1 (COPY):
    {
        *(.stack1*)
    } > scratch_x
    
    .stack0 (COPY):
    {
        *(.stack*)
    } > scratch_y

    __StackTop = ORIGIN(scratch_y) + LENGTH(scratch_y);
    __StackBottom = __StackTop - SIZEOF(.stack0);
    __Stack1Top = ORIGIN(scratch_x) + LENGTH(scratch_x);
    __Stack1Bottom = __Stack1Top - SIZEOF(.stack1);
}
