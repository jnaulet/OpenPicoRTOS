/*
 * PIC32MZ-EFM 2048 mapping for ROM
 */

MEMORY
{
    /* kseg0 */
    kseg0_data_mem(rwx)   : ORIGIN = 0x80000000, LENGTH = 512k
    kseg0_program_mem(rx) : ORIGIN = 0x9d000000, LENGTH = 2048k
    /* kseg1 */
    kseg1_sfrs(r)         : ORIGIN = 0xbf800000, LENGTH = 1024k
    kseg1_boot_flash(rx)  : ORIGIN = 0xbfc00000, LENGTH = 63k
    kseg1_config_space(r) : ORIGIN = 0xbfc0ff00, LENGTH = 256
}

PROVIDE(__relocate_text = 0x0);
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : DEFINED(__stack_size__) ? __stack_size__ : 0x100;

ENTRY(Reset_Handler)

SECTIONS
{
    .init :
    {
        __init_start__ = .;
        KEEP(*(.init))

        . = ALIGN(4);
        KEEP(*crti.o(.init))
        KEEP(*crtbegin.o(.init))

        . = ALIGN(4);
        KEEP(*(.fini))
        
        . = ALIGN(4);
        KEEP(*crtend.o(.init))
        KEEP(*crtn.o(.init))

        . = ALIGN(4);
        __init_end__ = .;
    } > kseg1_boot_flash

    .config :
    {
        __config_start__ = .;
        KEEP(*(.config))
        . = ALIGN(4);
        __config_end__ = .;
    } > kseg1_config_space

    .text :
    {
        __text_start__ = .;
        KEEP(*(.vectors))
        *(.stub .text .text.* .gnu.linkonce.t.*)
        *(.mips16.fn.* .mips16.call.*)
        *(.gnu.warning)
        *(.rodata .rodata* .gnu.linkonce.r.*)

        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP(*(.preinit_array))
        __preinit_array_end = .;

        . = ALIGN(4);
        __init_array_start = .;
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        __init_array_end = .;

        . = ALIGN(4);
        __fini_array_start = .;
        KEEP(*(.fini_array))
        KEEP(*(SORT(.fini_array.*)))
        __fini_array_end = .;
        
        . = ALIGN(4);
        *(.eh_frame_hdr)
        KEEP(*(.eh_frame))
        
        . = ALIGN(4);
        __text_end__ = .;
    } > kseg0_program_mem

    __etext = __text_end__;
    __sdata = __etext;

    .data : AT (__sdata)
    {
        __data_start__ = .;
        *(.data .data.* .gnu.linkonce.d.*);
        SORT(CONSTRUCTORS)
        . = ALIGN(4);
        __data_end__ = .;
    } > kseg0_data_mem

    .bss (NOLOAD):
    {
        __bss_start__ = .;
        *(.dynbss)
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > kseg0_data_mem

    /* allow small-access to sdata & sbss */
    . = .;
    _gp = ALIGN(16) + 0x7ff0;

    __gp = _gp;
    __ssdata = __sdata + (__data_end__ - __data_start__);

    .sdata : AT (__ssdata)
    {
        __sdata_start__ = .;
        *(.sdata .sdata.* .gnu.linkonce.s.*);
        . = ALIGN(4);
        __sdata_end__ = .;
    } > kseg0_data_mem

    .sbss (NOLOAD):
    {
        __sbss_start__ = .;
        *(.dynsbss)
        *(.sbss .sbss.*)
        *(.scommon)
        . = ALIGN(4);
        __sbss_end__ = .;
    } > kseg0_data_mem

    .stack1 (NOLOAD):
    {
        . = ALIGN(4);
        __stack_start__ = .;
        . = . + STACK_SIZE;
        . = ALIGN(4);
        __stack_end__ = .;
    } > kseg0_data_mem

    __StackTop = __stack_end__;
    __StackBottom = __stack_start__;

    /DISCARD/ : { *(.rel.dyn) }
}
