/*
 * GD32VF103C8T6 (lite) memory mapping for RAM
 */

MEMORY
{
    boot(rx)    : ORIGIN = 0x1fffb000, LENGTH = 18k
    rom(rx)     : ORIGIN = 0x08000000, LENGTH = 64k
    ram(rwx)    : ORIGIN = 0x20000000, LENGTH = 20k
}

PROVIDE(__relocate_text = 0x1);
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : DEFINED(__stack_size__) ? __stack_size__ : 0x100;

ENTRY(Reset_Handler)
REGION_ALIAS("FLASH", flash);
REGION_ALIAS("RAM", ram);

/* Section Definitions */
INCLUDE rv32imac_ram_sections.ld
