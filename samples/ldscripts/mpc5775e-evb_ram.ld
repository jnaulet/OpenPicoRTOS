/*
 * MPC5775E memory mapping for RAM
 */

MEMORY
{
    flash(rx) : ORIGIN = 0x00800000, LENGTH = 4096k
    ram(rwx)  : ORIGIN = 0x40000000, LENGTH = 512k
}

PROVIDE(__relocate_text = 0x1);
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : DEFINED(__stack_size__) ? __stack_size__ : 0x100;

ENTRY(Reset_Handler)
REGION_ALIAS("FLASH", flash);
REGION_ALIAS("RAM", ram);
PROVIDE(__interrupt_vector_count = 512);

/* ECC RAM */
PROVIDE(__ram_start__ = 0x40000000);
PROVIDE(__ram_end__ = 0x4007fffc);

/* common sections definition */
INCLUDE ppc-eabivle_ram_sections.ld
