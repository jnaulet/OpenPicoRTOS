/*  Default entry point:  */
ENTRY(_start_cpu0);

/* define stack size */
__STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x00001000;

MEMORY
{
    external_dflash (R) : org = 0x3f400000, len = 0x400000
    external_dram (RW) : org = 0x3f800000, len = 0x400000
    rtc_fast_dram (RW) : org = 0x3ff80000, len = 0x2000
    internal_rom_1 (R) : org = 0x3ff90000, len = 0x10000
    internal_dram_2 (RW) : org = 0x3ffae000, len = 0x32000
    internal_dram_1 (RW) : org = 0x3ffe0000, len = 0x20000
    internal_rom_0_remap (RX) : org = 0x40000000, len = 0x8000
    internal_rom_0 (RW) : org = 0x40008000, len = 0x58000
    internal_iram_0_cache (RX) : org = 0x40070000, len = 0x10000
    internal_iram_0 (RX) : org = 0x40080000, len = 0x20000
    internal_iram_1_0 (RX) : org = 0x400a0000, len = 0x10000
    internal_iram_1_remap (RX) : org = 0x400b0000, len = 0x8000
    internal_iram_1_1 (RX) : org = 0x400b8000, len = 0x8000
    rtc_fast_iram (RX) : org = 0x400c0000, len = 0x2000
    external_iflash (RX) : org = 0x400c2000, len = 0xb3e000
    rtc_slow_ram (RWX) : org = 0x50000000, len = 0x2000
}

SECTIONS
{
    .vector_table :
    {
        *(.vector_table.literal)
        __interrupts_start__ = .;
        KEEP(*(.vector_table))
        __interrupts_end__ = .;
    } > internal_iram_0

    .init :
    {
        *(.init.literal)
        *(.fini.literal)
        KEEP(*(.init))
        KEEP(*(.fini))
    } > internal_iram_0

    .text :
    {
        __code_start__ = .;
        *(.literal)
        *(.literal.*)
        *(.text)
        *(.text.*)
        __code_end__ = .;
        . = ALIGN(16);
    } > internal_iram_0

    .bss (NOLOAD) :
    {
        __bss_start__ = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        __bss_end__ = .;
        . = ALIGN(8);
    } > internal_dram_1

    __bss_size__ = (__bss_end__ - __bss_start__);

    .data :
    {
        __data_start__ = .;
        *(.sdata)
        *(.sdata.*)
        *(.gnu.linkonce.d.*)
        *(.gnu.linkonce.s.*)
        *(.gnu.linkonce.s2.*)
        *(.jcr)
        __data_end__ = .;
        . = ALIGN(4);
    } > internal_dram_1

    .stack (NOLOAD) : ALIGN(4)
    {
        __stack_end = .;
        . += __STACK_SIZE;
        . = ALIGN(4);
        __stack = .;
    } > internal_dram_2

    .rodata :
    {
        __rodata_start__ = .;
        *(.rodata)
        *(.rodata.*)
        __rodata_end__ = .;
        . = ALIGN(4);
    } > internal_dram_2
}
