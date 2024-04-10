/*
 * CH32V003 memory mapping for ROM
 */

MEMORY
{
    boot(rx)    : ORIGIN = 0x1ffff000, LENGTH = 1920
    rom(rx)     : ORIGIN = 0x00000000, LENGTH = 16k
    ram(rwx)    : ORIGIN = 0x20000000, LENGTH = 2k
}

PROVIDE(__relocate_text = 0x0);
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : DEFINED(__stack_size__) ? __stack_size__ : 0x100;

ENTRY(Reset_Handler)
REGION_ALIAS("FLASH", rom);
REGION_ALIAS("RAM", ram);

/* Section Definitions */
INCLUDE rv32imac_xip_sections.ld
